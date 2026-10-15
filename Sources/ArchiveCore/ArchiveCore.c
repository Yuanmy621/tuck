#define _DARWIN_C_SOURCE 1
#include "ArchiveCore.h"
#include "archive.h"
#include "archive_entry.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/attr.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define AC_PATH_CAP 4096
#define AC_BUFFER_SIZE 65536
#define AC_MAX_DEPTH 256
/* Bound retained directory permission metadata independently of expanded file sizes. */
#define AC_MAX_METADATA_BYTES (64u * 1024u * 1024u)

typedef struct {
    ACCallbacks callbacks;
    char *error;
    size_t error_size;
    int status;
    struct archive *archive;
    int64_t input_size;
} ACContext;

static int fail(ACContext *ctx, int status, const char *format, ...) {
    if (ctx->status == AC_SUCCESS) {
        ctx->status = status;
        va_list args;
        va_start(args, format);
        vsnprintf(ctx->error, ctx->error_size, format, args);
        va_end(args);
    }
    return ctx->status;
}

static int check_cancel(ACContext *ctx) {
    if (ctx->callbacks.is_cancelled && ctx->callbacks.is_cancelled(ctx->callbacks.context))
        return fail(ctx, AC_CANCELLED, "操作已取消。");
    return ctx->status;
}

static void progress(ACContext *ctx, const char *name) {
    if (!ctx->callbacks.progress) return;
    double fraction = -1.0;
    if (ctx->input_size > 0 && ctx->archive) {
        int64_t bytes = archive_filter_bytes(ctx->archive, -1);
        fraction = (double)bytes / (double)ctx->input_size;
        if (fraction < 0) fraction = 0;
        if (fraction > 1) fraction = 1;
    }
    ctx->callbacks.progress(ctx->callbacks.context, fraction, name);
}

static int archive_error(ACContext *ctx, struct archive *a, const char *operation) {
    const char *message = archive_error_string(a);
    if (!message) message = "libarchive returned an unspecified error";
    /* libarchive exposes encryption failure as error text rather than a distinct errno. */
    char lower[2048];
    size_t count = strnlen(message, sizeof(lower) - 1);
    for (size_t i = 0; i < count; ++i) lower[i] = (char)tolower((unsigned char)message[i]);
    lower[count] = 0;
    if (strstr(lower, "passphrase") || strstr(lower, "password"))
        return fail(ctx, AC_PASSWORD_REQUIRED, "%s：%s", operation, message);
    return fail(ctx, AC_FAILED, "%s：%s", operation, message);
}

/* Archive names are POSIX paths, irrespective of the host platform. */
static int normalize_path(ACContext *ctx, const char *source, char output[AC_PATH_CAP]) {
    if (!source || !*source) return fail(ctx, AC_FAILED, "压缩包包含空路径。");
    size_t length = strnlen(source, AC_PATH_CAP);
    if (length >= AC_PATH_CAP) return fail(ctx, AC_FAILED, "压缩包路径过长。");
    if (source[0] == '/' || source[0] == '\\' || strchr(source, '\\') ||
        (isalpha((unsigned char)source[0]) && source[1] == ':'))
        return fail(ctx, AC_FAILED, "拒绝不安全的压缩包路径：%s", source);
    size_t used = 0;
    const char *cursor = source;
    while (*cursor) {
        while (*cursor == '/') cursor++;
        const char *start = cursor;
        while (*cursor && *cursor != '/') cursor++;
        size_t size = (size_t)(cursor - start);
        if (!size || (size == 1 && start[0] == '.')) continue;
        if (size == 2 && start[0] == '.' && start[1] == '.')
            return fail(ctx, AC_FAILED, "拒绝路径穿越：%s", source);
        if (used) output[used++] = '/';
        memcpy(output + used, start, size);
        used += size;
    }
    output[used] = 0;
    return AC_SUCCESS;
}

static int safe_symlink(ACContext *ctx, const char *target) {
    char normalized[AC_PATH_CAP];
    if (normalize_path(ctx, target, normalized) != AC_SUCCESS) return ctx->status;
    return AC_SUCCESS;
}

/* Parent traversal is descriptor relative and refuses all symlink directories. */
static int open_parent(ACContext *ctx, int root, const char *path, char leaf[AC_PATH_CAP]) {
    char copy[AC_PATH_CAP];
    strlcpy(copy, path, sizeof(copy));
    char *last = strrchr(copy, '/');
    strlcpy(leaf, last ? last + 1 : copy, AC_PATH_CAP);
    if (last) *last = 0;
    int fd = dup(root);
    if (fd < 0) { fail(ctx, AC_FAILED, "无法打开目标目录：%s", strerror(errno)); return -1; }
    if (!last) return fd;
    char *save = NULL;
    for (char *piece = strtok_r(copy, "/", &save); piece; piece = strtok_r(NULL, "/", &save)) {
        if (check_cancel(ctx)) { close(fd); return -1; }
        if (mkdirat(fd, piece, 0700) < 0 && errno != EEXIST) {
            fail(ctx, AC_FAILED, "无法创建目录 %s：%s", path, strerror(errno)); close(fd); return -1;
        }
        int next = openat(fd, piece, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC);
        if (next < 0) {
            fail(ctx, AC_FAILED, "拒绝非目录或符号链接路径 %s：%s", path, strerror(errno)); close(fd); return -1;
        }
        close(fd);
        fd = next;
    }
    return fd;
}

typedef struct DirectoryMode {
    char *path;
    mode_t mode;
    size_t sequence;
    struct DirectoryMode *next;
} DirectoryMode;

static unsigned path_depth(const char *path) {
    unsigned depth = 0;
    for (; *path; ++path) if (*path == '/') depth++;
    return depth;
}

static int compare_directory_depth(const void *left, const void *right) {
    const DirectoryMode *a = *(const DirectoryMode *const *)left;
    const DirectoryMode *b = *(const DirectoryMode *const *)right;
    unsigned da = path_depth(a->path), db = path_depth(b->path);
    if (da != db) return da < db ? 1 : -1;
    return a->sequence < b->sequence ? -1 : a->sequence > b->sequence ? 1 : 0;
}

static int restore_directories(ACContext *ctx, int root, DirectoryMode *list) {
    size_t count = 0;
    for (DirectoryMode *item = list; item; item = item->next) count++;
    DirectoryMode **ordered = calloc(count ? count : 1, sizeof(*ordered));
    if (!ordered) return fail(ctx, AC_FAILED, "无法恢复目录权限：内存不足。");
    size_t index = 0;
    for (DirectoryMode *item = list; item; item = item->next) ordered[index++] = item;
    qsort(ordered, count, sizeof(*ordered), compare_directory_depth);
    for (size_t i = 0; i < count; ++i) {
        DirectoryMode *item = ordered[i];
        char leaf[AC_PATH_CAP];
        int parent = open_parent(ctx, root, item->path, leaf);
        if (parent < 0) { free(ordered); return ctx->status; }
        struct stat st;
        if (fstatat(parent, leaf, &st, AT_SYMLINK_NOFOLLOW) < 0 || !S_ISDIR(st.st_mode)) {
            close(parent); free(ordered); return fail(ctx, AC_FAILED, "目录权限恢复时身份不符：%s", item->path);
        }
        /* These entries are owned directories; extraction has finished and never replaces them.
         * fchmodat also handles repeated directory headers whose prior mode removed read access. */
        if (fchmodat(parent, leaf, item->mode & 0777, 0) < 0) {
            int saved = errno;
            close(parent); free(ordered); return fail(ctx, AC_FAILED, "无法设置目录权限：%s", strerror(saved));
        }
        close(parent);
    }
    free(ordered);
    return AC_SUCCESS;
}

int ac_extract_archive(const char *input, const char *output_directory, const char *password,
                       ACCallbacks callbacks, char *error, size_t error_size) {
    ACContext ctx = { .callbacks = callbacks, .error = error, .error_size = error_size };
    if (error_size) *error = 0;
    if (check_cancel(&ctx)) return ctx.status;
    int root = open(output_directory, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC);
    if (root < 0) return fail(&ctx, AC_FAILED, "无法打开解压目录：%s", strerror(errno));
    int source = open(input, O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
    if (source < 0) { close(root); return fail(&ctx, AC_FAILED, "无法打开压缩包：%s", strerror(errno)); }
    struct stat source_stat;
    if (fstat(source, &source_stat) < 0 || !S_ISREG(source_stat.st_mode)) {
        close(root); close(source); return fail(&ctx, AC_FAILED, "压缩包必须是普通文件。");
    }
    ctx.input_size = source_stat.st_size;
    struct archive *a = archive_read_new();
    if (!a) { close(root); close(source); return fail(&ctx, AC_FAILED, "无法分配解压器。"); }
    ctx.archive = a;
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    archive_read_support_format_raw(a);
    if (password && *password && archive_read_add_passphrase(a, password) < ARCHIVE_OK)
        archive_error(&ctx, a, "无法使用密码");
    if (!ctx.status && archive_read_open_fd(a, source, AC_BUFFER_SIZE) < ARCHIVE_OK)
        archive_error(&ctx, a, "无法读取压缩包");
    DirectoryMode *directories = NULL;
    size_t directory_metadata_bytes = 0;
    size_t directory_sequence = 0;
    char buffer[AC_BUFFER_SIZE];
    struct archive_entry *entry = NULL;
    while (!ctx.status) {
        if (check_cancel(&ctx)) break;
        int result = archive_read_next_header(a, &entry);
        if (result == ARCHIVE_EOF) break;
        if (result < ARCHIVE_OK) { archive_error(&ctx, a, "无法读取压缩包条目"); break; }
        char raw_name[AC_PATH_CAP];
        if (archive_format(a) == ARCHIVE_FORMAT_RAW) {
            int compressed = 0;
            for (int filter = 0; filter < archive_filter_count(a); ++filter)
                if (archive_filter_code(a, filter) != ARCHIVE_FILTER_NONE) compressed = 1;
            if (!compressed) { fail(&ctx, AC_FAILED, "无法识别压缩格式；文件可能损坏或不是压缩包。"); break; }
            const char *base = strrchr(input, '/');
            base = base ? base + 1 : input;
            if (strlcpy(raw_name, base, sizeof(raw_name)) >= sizeof(raw_name)) { fail(&ctx, AC_FAILED, "源文件名过长。"); break; }
            char *extension = strrchr(raw_name, '.');
            if (extension && (!strcasecmp(extension, ".gz") || !strcasecmp(extension, ".gzip") || !strcasecmp(extension, ".bz") || !strcasecmp(extension, ".bz2") || !strcasecmp(extension, ".bzip2") || !strcasecmp(extension, ".xz"))) *extension = 0;
            if (!*raw_name) strlcpy(raw_name, "解压文件", sizeof(raw_name));
        }
        const char *raw = archive_format(a) == ARCHIVE_FORMAT_RAW ? raw_name : archive_entry_pathname_utf8(entry);
        if (!raw) raw = archive_entry_pathname(entry);
        char path[AC_PATH_CAP];
        if (normalize_path(&ctx, raw, path)) break;
        mode_t type = archive_entry_filetype(entry);
        if (!*path) {
            if (type == AE_IFDIR) { archive_read_data_skip(a); continue; }
            fail(&ctx, AC_FAILED, "压缩包文件路径为空。"); break;
        }
        progress(&ctx, path);
        char leaf[AC_PATH_CAP];
        int parent = open_parent(&ctx, root, path, leaf);
        if (parent < 0) break;
        const char *hardlink = archive_entry_hardlink_utf8(entry);
        if (!hardlink) hardlink = archive_entry_hardlink(entry);
        if (hardlink) {
            char target[AC_PATH_CAP], target_leaf[AC_PATH_CAP];
            if (!normalize_path(&ctx, hardlink, target) && *target) {
                int target_parent = open_parent(&ctx, root, target, target_leaf);
                struct stat st;
                if (target_parent >= 0) {
                    if (fstatat(target_parent, target_leaf, &st, AT_SYMLINK_NOFOLLOW) < 0 || !S_ISREG(st.st_mode))
                        fail(&ctx, AC_FAILED, "硬链接目标必须是已解压的普通文件：%s", target);
                    else if (linkat(target_parent, target_leaf, parent, leaf, 0) < 0)
                        fail(&ctx, AC_FAILED, "无法创建硬链接 %s：%s", path, strerror(errno));
                    close(target_parent);
                }
            } else if (!ctx.status) fail(&ctx, AC_FAILED, "硬链接目标为空。");
        } else if (type == AE_IFDIR) {
            if (mkdirat(parent, leaf, 0700) < 0 && errno != EEXIST)
                fail(&ctx, AC_FAILED, "无法创建目录 %s：%s", path, strerror(errno));
            int fd = ctx.status ? -1 : openat(parent, leaf, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC);
            if (fd < 0 && !ctx.status) fail(&ctx, AC_FAILED, "拒绝非目录或链接冲突：%s", path);
            if (fd >= 0) close(fd);
            if (!ctx.status) {
                size_t retained = sizeof(DirectoryMode) + strlen(path) + 1 + sizeof(DirectoryMode *);
                if (retained > AC_MAX_METADATA_BYTES - directory_metadata_bytes) {
                    fail(&ctx, AC_FAILED, "目录权限元数据超过 64 MiB 安全预算。");
                    close(parent);
                    break;
                }
                directory_metadata_bytes += retained;
                DirectoryMode *item = calloc(1, sizeof(*item));
                if (!item || !(item->path = strdup(path))) {
                    free(item); fail(&ctx, AC_FAILED, "目录过多，内存不足。");
                } else {
                    item->mode = archive_entry_perm(entry);
                    item->sequence = directory_sequence++;
                    item->next = directories;
                    directories = item;
                }
            }
        } else if (type == AE_IFLNK) {
            const char *target = archive_entry_symlink_utf8(entry);
            if (!target) target = archive_entry_symlink(entry);
            if (!safe_symlink(&ctx, target) && symlinkat(target, parent, leaf) < 0)
                fail(&ctx, AC_FAILED, "无法创建符号链接 %s：%s", path, strerror(errno));
        } else if (type == AE_IFREG) {
            int fd = openat(parent, leaf, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600);
            if (fd < 0) fail(&ctx, AC_FAILED, "无法创建文件（可能重名）%s：%s", path, strerror(errno));
            while (fd >= 0 && !ctx.status) {
                if (check_cancel(&ctx)) break;
                la_ssize_t bytes = archive_read_data(a, buffer, sizeof(buffer));
                if (bytes < 0) { archive_error(&ctx, a, "解压失败"); break; }
                if (!bytes) break;
                size_t used = 0;
                while (used < (size_t)bytes) {
                    ssize_t written = write(fd, buffer + used, (size_t)bytes - used);
                    if (written < 0 && errno == EINTR) continue;
                    if (written <= 0) { fail(&ctx, AC_FAILED, "无法写入文件 %s：%s", path, strerror(errno)); break; }
                    used += (size_t)written;
                }
                progress(&ctx, path);
            }
            if (fd >= 0) {
                if (!ctx.status && fchmod(fd, archive_entry_perm(entry) & 0777) < 0)
                    fail(&ctx, AC_FAILED, "无法设置文件权限 %s：%s", path, strerror(errno));
                if (close(fd) < 0 && !ctx.status) fail(&ctx, AC_FAILED, "关闭文件失败：%s", strerror(errno));
            }
        } else fail(&ctx, AC_FAILED, "不支持解压设备、管道或特殊文件：%s", path);
        close(parent);
        if (!ctx.status && archive_read_data_skip(a) < ARCHIVE_OK) archive_error(&ctx, a, "无法跳过条目数据");
    }
    if (!ctx.status && check_cancel(&ctx) == AC_SUCCESS) restore_directories(&ctx, root, directories);
    while (directories) { DirectoryMode *next = directories->next; free(directories->path); free(directories); directories = next; }
    if (archive_read_close(a) < ARCHIVE_OK && !ctx.status) archive_error(&ctx, a, "关闭压缩包失败");
    archive_read_free(a);
    close(source);
    close(root);
    return ctx.status;
}

static int append_entry(ACContext *ctx, int parent, const char *name, const char *archive_path, unsigned depth) {
    if (check_cancel(ctx)) return ctx->status;
    if (depth > AC_MAX_DEPTH) return fail(ctx, AC_FAILED, "目录嵌套超过 %d 层。", AC_MAX_DEPTH);
    struct stat st;
    if (fstatat(parent, name, &st, AT_SYMLINK_NOFOLLOW) < 0)
        return fail(ctx, AC_FAILED, "无法读取 %s：%s", archive_path, strerror(errno));
    if (!S_ISREG(st.st_mode) && !S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode))
        return fail(ctx, AC_FAILED, "不支持压缩设备、管道或特殊文件：%s", archive_path);
    struct archive_entry *entry = archive_entry_new();
    if (!entry) return fail(ctx, AC_FAILED, "无法分配压缩条目。");
    archive_entry_copy_stat(entry, &st);
    archive_entry_set_uid(entry, 0);
    archive_entry_set_gid(entry, 0);
    archive_entry_set_perm(entry, st.st_mode & 0777);
    archive_entry_set_pathname_utf8(entry, archive_path);
    char link_target[AC_PATH_CAP];
    if (S_ISLNK(st.st_mode)) {
        ssize_t size = readlinkat(parent, name, link_target, sizeof(link_target) - 1);
        if (size < 0 || size >= (ssize_t)sizeof(link_target) - 1)
            fail(ctx, AC_FAILED, "无法读取符号链接：%s", archive_path);
        else {
            link_target[size] = 0;
            if (!safe_symlink(ctx, link_target)) archive_entry_set_symlink_utf8(entry, link_target);
        }
        archive_entry_set_size(entry, 0);
    }
    if (S_ISDIR(st.st_mode)) archive_entry_set_size(entry, 0);
    int file = -1;
    if (!ctx->status && (S_ISREG(st.st_mode) || S_ISDIR(st.st_mode))) {
        int flags = O_RDONLY | O_NOFOLLOW | O_CLOEXEC;
        if (S_ISDIR(st.st_mode)) flags |= O_DIRECTORY;
        file = openat(parent, name, flags);
        struct stat opened;
        if (file < 0) fail(ctx, AC_FAILED, "无法打开 %s：%s", archive_path, strerror(errno));
        else if (fstat(file, &opened) < 0 || opened.st_dev != st.st_dev || opened.st_ino != st.st_ino || opened.st_mode != st.st_mode)
            fail(ctx, AC_FAILED, "压缩期间源文件发生变化：%s", archive_path);
    }
    progress(ctx, archive_path);
    if (!ctx->status && archive_write_header(ctx->archive, entry) < ARCHIVE_OK)
        archive_error(ctx, ctx->archive, "无法写入压缩包条目");
    archive_entry_free(entry);
    if (!ctx->status && S_ISREG(st.st_mode)) {
        char buffer[AC_BUFFER_SIZE];
        int64_t remaining = st.st_size;
        while (remaining > 0 && !ctx->status) {
            if (check_cancel(ctx)) break;
            size_t wanted = remaining < AC_BUFFER_SIZE ? (size_t)remaining : AC_BUFFER_SIZE;
            ssize_t bytes = read(file, buffer, wanted);
            if (bytes < 0 && errno == EINTR) continue;
            if (bytes <= 0) { fail(ctx, AC_FAILED, "源文件读取失败或发生变化：%s", archive_path); break; }
            size_t used = 0;
            while (used < (size_t)bytes) {
                la_ssize_t written = archive_write_data(ctx->archive, buffer + used, (size_t)bytes - used);
                if (written <= 0) { archive_error(ctx, ctx->archive, "写入压缩数据失败"); break; }
                used += (size_t)written;
            }
            remaining -= bytes;
            progress(ctx, archive_path);
        }
    }
    if (!ctx->status && archive_write_finish_entry(ctx->archive) < ARCHIVE_OK)
        archive_error(ctx, ctx->archive, "完成压缩条目失败");
    if (!ctx->status && S_ISDIR(st.st_mode)) {
        DIR *directory = fdopendir(file);
        if (!directory) fail(ctx, AC_FAILED, "无法读取目录：%s", archive_path);
        else {
            file = -1;
            while (!ctx->status) {
                errno = 0;
                struct dirent *child = readdir(directory);
                if (!child) { if (errno) fail(ctx, AC_FAILED, "读取目录失败：%s", strerror(errno)); break; }
                if (!strcmp(child->d_name, ".") || !strcmp(child->d_name, "..")) continue;
                char child_path[AC_PATH_CAP];
                int size = snprintf(child_path, sizeof(child_path), "%s/%s", archive_path, child->d_name);
                if (size < 0 || size >= (int)sizeof(child_path)) { fail(ctx, AC_FAILED, "源文件路径过长。"); break; }
                append_entry(ctx, dirfd(directory), child->d_name, child_path, depth + 1);
            }
            closedir(directory);
        }
    }
    if (file >= 0) close(file);
    return ctx->status;
}

int ac_create_archive(const char *const *inputs, size_t count, const char *output, int format,
                      const char *password, ACCallbacks callbacks, char *error, size_t error_size) {
    ACContext ctx = { .callbacks = callbacks, .error = error, .error_size = error_size };
    if (error_size) *error = 0;
    if (check_cancel(&ctx)) return ctx.status;
    if (!count) return fail(&ctx, AC_FAILED, "请选择要压缩的文件。");
    if (password && *password) return fail(&ctx, AC_FAILED, "此版本不支持创建加密压缩包；解压支持系统 libarchive 可读取的加密格式。");
    int fd = open(output, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600);
    if (fd < 0) return fail(&ctx, AC_FAILED, "无法创建压缩包：%s", strerror(errno));
    struct archive *a = archive_write_new();
    ctx.archive = a;
    if (!a) fail(&ctx, AC_FAILED, "无法分配压缩器。");
    else if (format == 0) {
        if (archive_write_set_format_zip(a) < ARCHIVE_OK || archive_write_set_options(a, "zip:compression=deflate,zip:hdrcharset=UTF-8") < ARCHIVE_OK)
            archive_error(&ctx, a, "ZIP 压缩不可用");
    } else if (format == 1) {
        if (archive_write_set_format_pax_restricted(a) < ARCHIVE_OK || archive_write_set_options(a, "hdrcharset=UTF-8") < ARCHIVE_OK || archive_write_add_filter_gzip(a) < ARCHIVE_OK)
            archive_error(&ctx, a, "TAR.GZ 压缩不可用");
    } else fail(&ctx, AC_FAILED, "未知压缩格式。");
    if (!ctx.status && archive_write_open_fd(a, fd) < ARCHIVE_OK) archive_error(&ctx, a, "无法写入压缩包");
    char **root_names = calloc(count, sizeof(*root_names));
    if (!root_names) fail(&ctx, AC_FAILED, "内存不足。");
    for (size_t i = 0; i < count && !ctx.status; ++i) {
        char input[AC_PATH_CAP];
        if (strlcpy(input, inputs[i], sizeof(input)) >= sizeof(input)) { fail(&ctx, AC_FAILED, "源路径过长。"); break; }
        size_t length = strlen(input);
        while (length > 1 && input[length - 1] == '/') input[--length] = 0;
        char *slash = strrchr(input, '/');
        const char *name = slash ? slash + 1 : input;
        char normalized[AC_PATH_CAP];
        if (normalize_path(&ctx, name, normalized) || !*normalized) { if (!ctx.status) fail(&ctx, AC_FAILED, "不能压缩文件系统根目录。"); break; }
        for (size_t j = 0; j < i; ++j)
            if (!strcmp(root_names[j], normalized)) { fail(&ctx, AC_FAILED, "所选文件重名，请先分别压缩：%s", normalized); break; }
        if (ctx.status) break;
        root_names[i] = strdup(normalized);
        if (!root_names[i]) { fail(&ctx, AC_FAILED, "内存不足。"); break; }
        char parent_path[AC_PATH_CAP];
        if (slash) {
            size_t parent_length = (size_t)(slash - input);
            if (!parent_length) strlcpy(parent_path, "/", sizeof(parent_path));
            else { memcpy(parent_path, input, parent_length); parent_path[parent_length] = 0; }
        } else strlcpy(parent_path, ".", sizeof(parent_path));
        int parent = open(parent_path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
        if (parent < 0) { fail(&ctx, AC_FAILED, "无法打开源目录：%s", strerror(errno)); break; }
        append_entry(&ctx, parent, name, normalized, 0);
        close(parent);
    }
    if (root_names) { for (size_t i = 0; i < count; ++i) free(root_names[i]); free(root_names); }
    if (a) {
        if (archive_write_close(a) < ARCHIVE_OK && !ctx.status) archive_error(&ctx, a, "完成压缩包失败");
        archive_write_free(a);
    }
    if (close(fd) < 0 && !ctx.status) fail(&ctx, AC_FAILED, "关闭压缩包失败：%s", strerror(errno));
    check_cancel(&ctx);
    return ctx.status;
}

int ac_finalize_exclusive(const char *staging, const char *destination, char *error, size_t error_size) {
    if (renamex_np(staging, destination, RENAME_EXCL) == 0) return 0;
    int code = errno;
    snprintf(error, error_size, "无法保存结果：%s", strerror(code));
    return code == EEXIST || code == ENOTEMPTY ? 1 : 2;
}

/* Only called for a private, newly created staging directory owned by this operation. */
static int remove_directory_contents(int directory) {
    int duplicate = dup(directory);
    if (duplicate < 0) return -1;
    DIR *stream = fdopendir(duplicate);
    if (!stream) { close(duplicate); return -1; }
    int result = 0, saved = 0;
    while (1) {
        errno = 0;
        struct dirent *entry = readdir(stream);
        if (!entry) { if (errno) { result = -1; saved = errno; } break; }
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        struct stat st;
        if (fstatat(directory, entry->d_name, &st, AT_SYMLINK_NOFOLLOW) < 0) { result = -1; saved = errno; break; }
        if (S_ISDIR(st.st_mode)) {
            /* No extraction worker remains; the descriptor-relative entry is ours. */
            if (fchmodat(directory, entry->d_name, 0700, 0) < 0) { result = -1; saved = errno; break; }
            int child = openat(directory, entry->d_name, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC);
            if (child < 0) { result = -1; saved = errno; break; }
            result = remove_directory_contents(child);
            saved = result < 0 ? errno : 0;
            close(child);
            if (result < 0) break;
            if (unlinkat(directory, entry->d_name, AT_REMOVEDIR) < 0) { result = -1; saved = errno; break; }
        } else if (unlinkat(directory, entry->d_name, 0) < 0) { result = -1; saved = errno; break; }
    }
    closedir(stream);
    if (saved) errno = saved;
    return result;
}

int ac_remove_staging(const char *path, char *error, size_t error_size) {
    int root = open(path, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC);
    if (root < 0) {
        if (errno == ENOENT) return 0;
        snprintf(error, error_size, "无法打开临时目录：%s", strerror(errno));
        return -1;
    }
    int result = remove_directory_contents(root);
    int saved = errno;
    close(root);
    if (result == 0 && rmdir(path) == 0) return 0;
    if (result == 0) saved = errno;
    snprintf(error, error_size, "无法删除临时目录：%s", strerror(saved));
    return -1;
}
