#ifndef MACARCHIVE_CORE_H
#define MACARCHIVE_CORE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Callback context is owned by the synchronous caller and must outlive each call. */
typedef int (*ACCancelCallback)(void *context);
typedef void (*ACProgressCallback)(void *context, double fraction, const char *message);
typedef struct {
    void *context;
    ACCancelCallback is_cancelled;
    ACProgressCallback progress;
} ACCallbacks;
enum { AC_SUCCESS = 0, AC_PASSWORD_REQUIRED = 1, AC_CANCELLED = 2, AC_FAILED = 3 };
int ac_create_archive(const char *const *inputs, size_t count, const char *output,
                      int format, const char *password, ACCallbacks callbacks,
                      char *error, size_t error_size);
int ac_extract_archive(const char *input, const char *output_directory,
                       const char *password, ACCallbacks callbacks,
                       char *error, size_t error_size);
int ac_remove_staging(const char *path, char *error, size_t error_size);
int ac_finalize_exclusive(const char *staging, const char *destination,
                          char *error, size_t error_size);
#ifdef __cplusplus
}
#endif
#endif
