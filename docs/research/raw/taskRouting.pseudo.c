// ev_47a685cd0aa0455c876ce9631a119e24b992554fbf1db19387c2694a6476c8f4
    r31 = r31 - 0x50;
    var_30 = r24;
    decomp_var_28 = r23;
    var_20 = r22;
    decomp_var_18 = r21;
    var_10 = r20;
    decomp_var_8 = r19;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r19 = self;
    self[0x82] = 0x1;
    r3 = [self getTaskTypeName];
    [progressController newActivity:r19 + 0x40 withReason:r3];
    [r19 setProgressBarHUEColor];
    r20 = r19 + 0xa0;
    if ([*r20 destination] == 0x0) goto loc_10003d858;
    goto loc_10003d754;

loc_10003d858:
    sub_100024a7c([NSString stringWithFormat:@"No destionation set, cancelling the operation"]);
    r19[0x42] = 0x101;
    [r19 finalizeTask];
    [r19 removeTask];
    return;

loc_10003d754:
    r19[0x3b] = 0xffffffff;
    r19[0x109] = 0x0;
    r8 = [r19[0x14] type];
    r0 = r19[0x14];
    if (r8 != 0x1) goto loc_10003d89c;
    goto loc_10003d778;

loc_10003d89c:
    if ([r0 type] != 0x0) goto loc_10003dda4;
    goto loc_10003d8a4;

loc_10003d778:
    if ([r0 extension] == 0x0) {
            sub_100024a7c([NSString stringWithFormat:@"%@", r3]);
            [*r20 setExtension:@"7z"];
    }
    if ([*r20 tarball] != 0x0) {
            if ([*r20 format] != 0x2 && [*r20 split] != 0x0) {
                    [*r20 setNeedsTarballTask:0x1];
            }
    }
    if ([*r20 tarball] == 0x0 || ([*r20 needsTarballTask] & 0x1) != 0x0) goto loc_10003d800;
    goto loc_10003d8d4;

loc_10003dda4:
    [r19 launchTask];
    return;

loc_10003d8a4:
    r21 = [*r20 binary];
    [*r20 setBinary:0xffffffff];
    if ([*r20 formatDetected] != -0x2) goto loc_10003d8e8;
    goto loc_10003d8cc;

loc_10003d800:
    r21 = 0x0;
    r8 = [*r20 format] - 0x2;
    if (r8 > 0xf) goto loc_10003dd40;
    goto loc_10003d818;

loc_10003d8d4:
    [compressionController tar:r20];
    [r19 getEstimatedSize:[r19[0x14] files]];
    goto loc_10003dda4;

loc_10003d8e8:
    r8 = [*r20 package];
    r0 = *r20;
    if (r8 != 0x0) {
            [[r0 package] cleanTemporaryPath];
    }
    else {
            r22 = [[r0 file] copy];
            if ([*r20 formatDetected] == 0x11 && [*r20 firstVolume] != 0x0) {
                    r22 = [[[*r20 file] stringByDeletingPathExtension] stringByDeletingPathExtension];
                    r22 = [r22 stringByAppendingPathExtension:[[*r20 file] pathExtension]];
            }
            r23 = *r20;
            r0 = [r23 destination];
            r3 = r22;
            [r23 setPackage:[KPackage newPackageAtDestinationPath:r0 withSource:r3]];
    }
    if ([*r20 package] == 0x0 || [[*r20 package] packageTemporaryPath] == 0x0) goto loc_10003d9fc;
    goto loc_10003d9ac;

loc_10003d8cc:
    r8 = 0x150;
    goto loc_10003da64;

loc_10003dd40:
    if ([*r20 needsTarballTask] != 0x0) {
            [r19 setCompressionTarballTask];
    }
    if ((r21 & 0x1) == 0x0) {
            [compressionController szip:r20];
            if ([*r20 password] != 0x0) {
                    [r19 showPasswordIcon];
                    r19[0x101] = 0x1;
            }
            if (([*r20 tarball] & 0x1) != 0x0) {
                    [r19 getEstimatedSize:[r19[0x14] files]];
            }
    }
    goto loc_10003dda4;

loc_10003d818:
    index_0 = r8;
    if (index_0 == 0x0) goto loc_10003d830;
    goto .l60;

loc_10003d9fc:
    [r19[0x14] file];
    r0 = r19[0x14];
    [r0 destination];
    r20 = [NSString stringWithFormat:@"Unable to create the extraction package\nSource: %@\nDestination: %@", r3, r4];
    sub_100024ab0([NSString stringWithFormat:@"%@", r3]);
    [r19[0xd] addObject:[r20 copy]];
    r8 = 0x152;
    goto loc_10003da64;

loc_10003d9ac:
    if (([*r20 usedBinary:0x13] & 0x1) != 0x0 || [*r20 formatDetected] != -0x385 || [[[*r20 file] pathExtension] isEqualToString:@"xip"] == 0x0) goto loc_10003da88;
    goto loc_10003d9e8;

loc_10003da64:
    r19[0x25] = r8;
    r19[0x80] = 0x0;
    [r19 checkEndedTask];
    return;

loc_10003d830:
    [compressionController tar:r20];
    [r19 getEstimatedSize:[r19[0x14] files]];
    goto loc_10003dd3c;

.l60:
    if (index_0 == 0x1) goto loc_10003dd40;
    goto .l61;

loc_10003da88:
    if (([*r20 usedBinary:0x1a] & 0x1) != 0x0 || [*r20 formatDetected] != -0x385) goto loc_10003dc1c;
    goto loc_10003daa8;

loc_10003d9e8:
    [extractionController xip:r20];
    goto loc_10003dab8;

loc_10003dd3c:
    r21 = 0x1;
    goto loc_10003dd40;

.l61:
    if (index_0 == 0x2) goto loc_10003db40;
    goto .l62;

loc_10003dc1c:
    if (([*r20 usedBinary:0xd] & 0x1) == 0x0) goto loc_10003dc80;
    goto loc_10003dc2c;

loc_10003daa8:
    [extractionController xar:r20];
    goto loc_10003dab8;

loc_10003dab8:
    [r19 setProgressIndeterminate:0x1];
    goto loc_10003ded0;

loc_10003db40:
    if ([*r20 split] == 0x0) goto loc_10003dd2c;
    goto loc_10003db4c;

.l62:
    if (index_0 == 0x3) goto loc_10003dadc;
    goto .l63;

loc_10003dc80:
    if ([*r20 formatDetected] != 0x4 || (*(int8_t *)byte_10009c862 & 0x1) == 0x0) goto loc_10003dca0;
    goto .l47;

loc_10003dc2c:
    if (([*r20 usedBinary:0x3] & 0x1) != 0x0 || (*(int8_t *)byte_10009cb8e & 0x1) != 0x0 || [*r20 formatDetected] != 0x11 && [*r20 formatDetected] != 0x12) goto loc_10003ddc0;
    goto loc_10003dc6c;

loc_10003ded0:
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003dd2c:
    [compressionController xz:r20];
    goto loc_10003dd3c;

loc_10003db4c:
    r21 = 0x0;
    goto loc_10003dd40;

loc_10003dadc:
    [compressionController lzip:r20];
    if ([*r20 tarball] != 0x0) {
            [r19 getEstimatedSize:[r19[0x14] files]];
    }
    [r19 lzipMultithreadVolumeSupportWarning];
    goto loc_10003dd3c;

.l63:
    if (index_0 == 0x4) goto loc_10003db8c;
    goto .l64;

loc_10003dca0:
    if ([*r20 formatDetected] == 0x16 || [*r20 formatDetected] == -0x385 || [*r20 formatDetected] == -0x65) goto .l47;
    goto loc_10003dcd0;

.l47:
    [extractionController unar:r20];
    goto loc_10003ded0;

loc_10003ddc0:
    if (([*r20 usedBinary:0x1] & 0x1) == 0x0) goto loc_10003de08;
    goto loc_10003ddd0;

loc_10003dc6c:
    [extractionController unrar:r20];
    goto loc_10003ded0;

loc_10003db8c:
    [compressionController brotli:r20];
    goto loc_10003dd3c;

.l64:
    if (index_0 == 0x5) goto loc_10003dba0;
    goto .l65;

loc_10003dcd0:
    if ([*r20 formatDetected] != 0x0) goto loc_10003dcf8;
    goto loc_10003dcdc;

loc_10003de08:
    if ([*r20 formatDetected] == 0x1 || [*r20 formatDetected] == 0x13 || [*r20 tarball] != 0x0) goto loc_10003de34;

loc_10003ddd0:
    r8 = [*r20 formatDetected];
    r0 = *r20;
    if (r8 != 0x7) goto loc_10003de6c;
    goto loc_10003dde8;

loc_10003dba0:
    [compressionController rar:r20];
    goto loc_10003dbfc;

.l65:
    if (index_0 == 0x6) goto loc_10003db18;
    goto .l66;

loc_10003dcf8:
    if (([*r20 usedBinary:0x16] & 0x1) != 0x0) goto .l49;
    goto loc_10003dd08;

loc_10003dcdc:
    if (([[[*r20 file] pathExtension] isEqualToString:@"cpt"] & 0x1) != 0x0) goto .l48;
    goto .l2;

loc_10003de34:
    [extractionController tar:r20];
    if ([*r20 formatDetected] != 0x1a) {
            [r19 getEstimatedSize:[r19[0x14] files]];
    }
    else {
            [r19 setProgressIndeterminate:0x1];
    }
    goto loc_10003ded0;

loc_10003de6c:
    r8 = [r0 formatDetected];
    r0 = *r20;
    if (r8 != 0x10) goto loc_10003dea0;
    goto loc_10003de80;

loc_10003dde8:
    if (([r0 usedBinary:0x5] & 0x1) == 0x0) {
            [extractionController lzip:r20];
    }
    goto loc_10003ded0;

loc_10003dbfc:
    r21 = 0x1;
    if ([*r20 password] != 0x0) {
            r21 = 0x1;
            [r19 showPasswordIcon];
            r19[0x101] = 0x1;
    }
    goto loc_10003dd40;

loc_10003db18:
    [compressionController dmg:r20];
    goto loc_10003db64;

.l66:
    if (index_0 == 0x7) goto loc_10003db54;
    goto .l67;

.l49:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003dd08:
    if (([*r20 usedBinary:0x0] & 0x1) != 0x0) goto .l50;
    goto loc_10003dd18;

.l48:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

.l2:
    if (([*r20 usedBinary:0x16] & 0x1) != 0x0) goto .l27;
    goto .l3;

loc_10003dea0:
    r8 = [r0 formatDetected];
    r0 = *r20;
    if (r8 != 0x14) goto loc_10003df0c;
    goto loc_10003deb4;

loc_10003de80:
    if (([r0 usedBinary:0x19] & 0x1) == 0x0) {
            [extractionController lz4:r20];
    }
    goto loc_10003ded0;

loc_10003db64:
    r21 = 0x1;
    [r19 setProgressIndeterminate:0x1];
    goto loc_10003dd40;

loc_10003db54:
    [compressionController iso:r20];
    goto loc_10003db64;

.l67:
    if (index_0 == 0x8) goto loc_10003db78;
    goto .l68;

.l50:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003dd18:
    if ([*r20 usedBinary:0x17] == 0x0) goto loc_10003dc2c;
    goto loc_10003dd28;

.l27:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

.l3:
    if (([*r20 usedBinary:0x0] & 0x1) != 0x0) goto .l7;
    goto .l4;

loc_10003df0c:
    if ([r0 formatDetected] != 0x18) goto loc_10003df3c;
    goto loc_10003df18;

loc_10003deb4:
    if (([r0 usedBinary:0x11] & 0x1) == 0x0) {
            [extractionController unar:r20];
    }
    goto loc_10003ded0;

loc_10003db78:
    [compressionController zstd:r20];
    goto loc_10003dd3c;

.l68:
    if (index_0 == 0x9) goto loc_10003dbec;
    goto .l69;

loc_10003dd28:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

.l7:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

.l4:
    if ([*r20 usedBinary:0x17] == 0x0) goto .l13;
    goto .l6;

loc_10003df3c:
    if ([*r20 formatDetected] != 0x0 || [[[*r20 file] pathExtension] isEqualToString:@"br"] == 0x0) goto loc_10003df64;

loc_10003df18:
    if (([*r20 usedBinary:0xe] & 0x1) == 0x0) {
            [extractionController brotli:r20];
    }
    goto loc_10003ded0;

loc_10003dbec:
    [compressionController lrzip:r20];
    goto loc_10003dbfc;

.l69:
    if (index_0 == 0xa) goto loc_10003dbb4;
    goto .l70;

.l13:
    if (([*r20 usedBinary:0x3] & 0x1) != 0x0 || (*(int8_t *)byte_10009cb8e & 0x1) != 0x0 || [*r20 formatDetected] != 0x11 && [*r20 formatDetected] != 0x12) goto .l24;
    goto .l11;

.l6:
    [extractionController unar:r20];
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003df64:
    if ([*r20 formatDetected] != 0x19) goto loc_10003df98;
    goto loc_10003df74;

loc_10003dbb4:
    [compressionController yaa:r20];
    r21 = 0x1;
    [r19 setProgressIndeterminate:0x1];
    if ([r19[0x14] password] != 0x0) {
            [r19 showPasswordIcon];
    }
    goto loc_10003dd40;

.l70:
    if (index_0 != 0xb) {
            [compressionController lz4:r20];
    }
    else {
            [compressionController snzip:r20];
    }
    goto loc_10003db64;

.l24:
    if (([*r20 usedBinary:0x1] & 0x1) == 0x0) goto .l5;
    goto .l22;

.l11:
    [extractionController unrar:r20];
    goto .l35;

loc_10003df98:
    if ([*r20 formatDetected] != 0x0 || [[[*r20 file] pathExtension] isEqualToString:@"zst"] == 0x0) goto loc_10003dfc0;

loc_10003df74:
    if (([*r20 usedBinary:0xf] & 0x1) == 0x0) {
            [extractionController zstd:r20];
    }
    goto loc_10003ded0;

.l5:
    if ([*r20 formatDetected] == 0x1) goto .l29;
    goto loc_10003de18;

.l22:
    r8 = [*r20 formatDetected];
    r0 = *r20;
    if (r8 != 0x7) goto .l25;
    goto .l23;

.l35:
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003dfc0:
    if ([*r20 formatDetected] == 0x1c || [*r20 formatDetected] == 0x1d) goto loc_10003dfe0;
    goto loc_10003e058;

.l29:
    [extractionController tar:r20];
    if ([*r20 formatDetected] != 0x1a) {
            [r19 getEstimatedSize:[r19[0x14] files]];
    }
    else {
            [r19 setProgressIndeterminate:0x1];
    }
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003de18:
    if ([*r20 formatDetected] == 0x13) goto .l55;
    goto loc_10003de28;

.l25:
    r8 = [r0 formatDetected];
    r0 = *r20;
    if (r8 != 0x10) goto .l19;
    goto .l26;

.l23:
    if (([r0 usedBinary:0x5] & 0x1) == 0x0) {
            [extractionController lzip:r20];
    }
    goto .l35;

loc_10003dfe0:
    if ([systemCheck limitedToMinimumVersion:0xd] == 0x0) goto loc_10003e058;
    goto loc_10003dff4;

loc_10003e058:
    r8 = [*r20 formatDetected];
    r0 = *r20;
    if (r8 != 0x1a) goto loc_10003e090;
    goto loc_10003e070;

.l55:
    [extractionController tar:r20];
    if ([*r20 formatDetected] != 0x1a) {
            [r19 getEstimatedSize:[r19[0x14] files]];
    }
    else {
            [r19 setProgressIndeterminate:0x1];
    }
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

loc_10003de28:
    if ([*r20 tarball] == 0x0) goto .l22;
    goto .l28;

.l19:
    r8 = [r0 formatDetected];
    r0 = *r20;
    if (r8 != 0x14) goto .l10;
    goto .l17;

.l26:
    if (([r0 usedBinary:0x19] & 0x1) == 0x0) {
            [extractionController lz4:r20];
    }
    goto .l35;

loc_10003dff4:
    [r19 setProgressIndeterminate:0x1];
    if ([r19[0x14] formatDetected] != 0x1d) goto loc_10003e034;
    goto loc_10003e010;

loc_10003e090:
    if (([r0 usedBinary:0x15] & 0x1) != 0x0 || [*r20 formatDetected] != 0x1f) goto loc_10003e0ac;
    goto loc_10003e0cc;

loc_10003e070:
    if (([r0 usedBinary:0x12] & 0x1) != 0x0) goto loc_10003ded0;
    goto loc_10003e07c;

.l28:
    [extractionController tar:r20];
    if ([*r20 formatDetected] != 0x1a) {
            [r19 getEstimatedSize:[r19[0x14] files]];
    }
    else {
            [r19 setProgressIndeterminate:0x1];
    }
    if ([*r20 binary] == -0x1) {
            r19[0x80] = 0x0;
            [r19[0x14] setBinary:r21];
            [r19 continueEndedTask];
    }
    else {
            [r19 launchTask];
    }
    return;

.l10:
    if ([r0 formatDetected] != 0x18) goto .l20;
    goto .l9;

.l17:
    if (([r0 usedBinary:0x11] & 0x1) == 0x0) {
            [extractionController unar:r20];
    }
    goto .l35;

loc_10003e034:
    if (([*r20 usedBinary:0x14] & 0x1) == 0x0) {
            [extractionController yaa:r20];
    }
    goto loc_10003ded0;

loc_10003e010:
    if (sub_100042e1c(0x1, 0xc, 0x0, 0x0) == 0x0) goto loc_10003e174;
    goto loc_10003e028;

loc_10003e0ac:
    if (([*r20 usedBinary:0x15] & 0x1) != 0x0 || [*r20 formatDetected] != 0x20) goto loc_10003e0f4;

loc_10003e0cc:
    r19[0x80] = 0x0;
    r3 = [r19[0x14] formatDetected];
    r2 = r20;
    goto loc_10003e0ec;

loc_10003e07c:
    [extractionController lrzip:r20];
    goto loc_10003dab8;

.l20:
    if ([*r20 formatDetected] != 0x0 || [[[*r20 file] pathExtension] isEqualToString:@"br"] == 0x0) goto .l14;

.l9:
    if (([*r20 usedBinary:0xe] & 0x1) == 0x0) {
            [extractionController brotli:r20];
    }
    goto .l35;

loc_10003e174:
    r8 = 0x155;
    goto loc_10003da64;

loc_10003e028:
    if ([*r20 password] == 0x0) goto loc_10003e17c;
    goto loc_10003e034;

loc_10003e0f4:
    if ([*r20 formatDetected] != 0x0 || [[[*r20 file] pathExtension] isEqualToString:@"wpress"] == 0x0 || ([*r20 usedBinary:0x15] & 0x1) != 0x0) goto loc_10003e12c;
    goto loc_10003e1fc;

loc_10003e0ec:
    [extractionController kwet:r2 withFormat:r3];
    goto loc_10003ded0;

.l14:
    if ([*r20 formatDetected] != 0x19) goto .l18;
    goto .l16;

loc_10003e17c:
    [r19 needsPasswordActions];
    return;

loc_10003e12c:
    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
            if ([*r20 formatDetected] == 0x21) {
                    if ([*r20 formatDetected] == 0x21) {
                            r19[0x80] = 0x0;
                    }
                    [extractionController snzip:r20];
            }
            else {
                    if ([*r20 formatDetected] != 0x0) {
                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                            if ([*r20 formatDetected] == 0x1b) {
                                                    r19[0x80] = 0x0;
                                            }
                                            [extractionController szip:r20];
                                    }
                            }
                    }
                    else {
                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                            }
                                            else {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            r19[0x80] = 0x0;
                                                    }
                                                    [extractionController snzip:r20];
                                            }
                                    }
                                    else {
                                            if ([*r20 formatDetected] == 0x21) {
                                                    r19[0x80] = 0x0;
                                            }
                                            [extractionController snzip:r20];
                                    }
                            }
                            else {
                                    if ([*r20 formatDetected] == 0x21) {
                                            r19[0x80] = 0x0;
                                    }
                                    [extractionController snzip:r20];
                            }
                    }
            }
    }
    else {
            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                            if ([*r20 formatDetected] == 0x1b) {
                                    r19[0x80] = 0x0;
                            }
                            [extractionController szip:r20];
                    }
            }
    }
    goto loc_10003ded0;

loc_10003e1fc:
    r19[0x80] = 0x0;
    r2 = r20;
    r3 = 0x1e;
    goto loc_10003e0ec;

.l18:
    if ([*r20 formatDetected] != 0x0 || [[[*r20 file] pathExtension] isEqualToString:@"zst"] == 0x0) goto .l15;

.l16:
    if (([*r20 usedBinary:0xf] & 0x1) == 0x0) {
            [extractionController zstd:r20];
    }
    goto .l35;

.l15:
    if ([*r20 formatDetected] == 0x1c || [*r20 formatDetected] == 0x1d) goto .l12;
    goto .l32;

.l12:
    if ([systemCheck limitedToMinimumVersion:0xd] == 0x0) goto .l21;
    goto .l30;

.l32:
    r8 = [*r20 formatDetected];
    r0 = *r20;
    if (r8 == 0x1a) {
            if (([r0 usedBinary:0x12] & 0x1) == 0x0) {
                    [extractionController lrzip:r20];
                    [r19 setProgressIndeterminate:0x1];
            }
            if ([*r20 binary] == -0x1) {
                    r19[0x80] = 0x0;
                    [r19[0x14] setBinary:r21];
                    [r19 continueEndedTask];
            }
            else {
                    [r19 launchTask];
            }
    }
    else {
            if (([r0 usedBinary:0x15] & 0x1) == 0x0) {
                    if ([*r20 formatDetected] != 0x1f) {
                            if (([*r20 usedBinary:0x15] & 0x1) == 0x0) {
                                    if ([*r20 formatDetected] == 0x20) {
                                            r19[0x80] = 0x0;
                                            r3 = [r19[0x14] formatDetected];
                                            r2 = r20;
                                            [extractionController kwet:r2 withFormat:r3];
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                                    else {
                                            if ([*r20 formatDetected] == 0x0) {
                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                                            if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                                            r19[0x80] = 0x0;
                                                                                                                                    }
                                                                                                                                    [extractionController szip:r20];
                                                                                                                            }
                                                                                                                    }
                                                                                                            }
                                                                                                            else {
                                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController snzip:r20];
                                                                                                            }
                                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                                    [r19 continueEndedTask];
                                                                                                            }
                                                                                                            else {
                                                                                                                    [r19 launchTask];
                                                                                                            }
                                                                                                    }
                                                                                                    else {
                                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController snzip:r20];
                                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                                    [r19 continueEndedTask];
                                                                                                            }
                                                                                                            else {
                                                                                                                    [r19 launchTask];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    r19[0x80] = 0x0;
                                                                    r2 = r20;
                                                                    [extractionController kwet:r2 withFormat:0x1e];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] != 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                                    r19[0x80] = 0x0;
                                                                                                                            }
                                                                                                                            [extractionController szip:r20];
                                                                                                                    }
                                                                                                            }
                                                                                                    }
                                                                                                    else {
                                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController snzip:r20];
                                                                                                    }
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                    }
                            }
                            else {
                                    if ([*r20 formatDetected] == 0x0) {
                                            if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                                    if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] != 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                                    r19[0x80] = 0x0;
                                                                                                                            }
                                                                                                                            [extractionController szip:r20];
                                                                                                                    }
                                                                                                            }
                                                                                                    }
                                                                                                    else {
                                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController snzip:r20];
                                                                                                    }
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            r19[0x80] = 0x0;
                                                            r2 = r20;
                                                            [extractionController kwet:r2 withFormat:0x1e];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                            }
                    }
                    else {
                            r19[0x80] = 0x0;
                            r3 = [r19[0x14] formatDetected];
                            r2 = r20;
                            [extractionController kwet:r2 withFormat:r3];
                            if ([*r20 binary] == -0x1) {
                                    r19[0x80] = 0x0;
                                    [r19[0x14] setBinary:r21];
                                    [r19 continueEndedTask];
                            }
                            else {
                                    [r19 launchTask];
                            }
                    }
            }
            else {
                    if (([*r20 usedBinary:0x15] & 0x1) == 0x0) {
                            if ([*r20 formatDetected] == 0x20) {
                                    r19[0x80] = 0x0;
                                    r3 = [r19[0x14] formatDetected];
                                    r2 = r20;
                                    [extractionController kwet:r2 withFormat:r3];
                                    if ([*r20 binary] == -0x1) {
                                            r19[0x80] = 0x0;
                                            [r19[0x14] setBinary:r21];
                                            [r19 continueEndedTask];
                                    }
                                    else {
                                            [r19 launchTask];
                                    }
                            }
                            else {
                                    if ([*r20 formatDetected] == 0x0) {
                                            if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                                    if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] != 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                                    r19[0x80] = 0x0;
                                                                                                                            }
                                                                                                                            [extractionController szip:r20];
                                                                                                                    }
                                                                                                            }
                                                                                                    }
                                                                                                    else {
                                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController snzip:r20];
                                                                                                    }
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            r19[0x80] = 0x0;
                                                            r2 = r20;
                                                            [extractionController kwet:r2 withFormat:0x1e];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                            }
                    }
                    else {
                            if ([*r20 formatDetected] == 0x0) {
                                    if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                            if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                            else {
                                                    r19[0x80] = 0x0;
                                                    r2 = r20;
                                                    [extractionController kwet:r2 withFormat:0x1e];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                            }
                            else {
                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                            if ([*r20 formatDetected] == 0x21) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            r19[0x80] = 0x0;
                                                    }
                                                    [extractionController snzip:r20];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                            else {
                                                    if ([*r20 formatDetected] != 0x0) {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController szip:r20];
                                                    }
                                            }
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                            }
                    }
            }
    }
    return;

.l21:
    r8 = [*r20 formatDetected];
    r0 = *r20;
    if (r8 != 0x1a) goto .l31;
    goto .l8;

.l30:
    [r19 setProgressIndeterminate:0x1];
    if ([r19[0x14] formatDetected] == 0x1d) {
            if (sub_100042e1c(0x1, 0xc, 0x0, 0x0) != 0x0) {
                    if ([*r20 password] != 0x0) {
                            if (([*r20 usedBinary:0x14] & 0x1) == 0x0) {
                                    [extractionController yaa:r20];
                            }
                            if ([*r20 binary] == -0x1) {
                                    r19[0x80] = 0x0;
                                    [r19[0x14] setBinary:r21];
                                    [r19 continueEndedTask];
                            }
                            else {
                                    [r19 launchTask];
                            }
                    }
                    else {
                            [r19 needsPasswordActions];
                    }
            }
            else {
                    r19[0x25] = 0x155;
                    r19[0x80] = 0x0;
                    [r19 checkEndedTask];
            }
    }
    else {
            if (([*r20 usedBinary:0x14] & 0x1) == 0x0) {
                    [extractionController yaa:r20];
            }
            if ([*r20 binary] == -0x1) {
                    r19[0x80] = 0x0;
                    [r19[0x14] setBinary:r21];
                    [r19 continueEndedTask];
            }
            else {
                    [r19 launchTask];
            }
    }
    return;

.l31:
    if (([r0 usedBinary:0x15] & 0x1) == 0x0) {
            if ([*r20 formatDetected] != 0x1f) {
                    if (([*r20 usedBinary:0x15] & 0x1) == 0x0) {
                            if ([*r20 formatDetected] == 0x20) {
                                    r19[0x80] = 0x0;
                                    r3 = [r19[0x14] formatDetected];
                                    r2 = r20;
                                    [extractionController kwet:r2 withFormat:r3];
                                    if ([*r20 binary] == -0x1) {
                                            r19[0x80] = 0x0;
                                            [r19[0x14] setBinary:r21];
                                            [r19 continueEndedTask];
                                    }
                                    else {
                                            [r19 launchTask];
                                    }
                            }
                            else {
                                    if ([*r20 formatDetected] == 0x0) {
                                            if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                                    if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] != 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                                    r19[0x80] = 0x0;
                                                                                                                            }
                                                                                                                            [extractionController szip:r20];
                                                                                                                    }
                                                                                                            }
                                                                                                    }
                                                                                                    else {
                                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController snzip:r20];
                                                                                                    }
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                                    if ([*r20 binary] == -0x1) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                            [r19[0x14] setBinary:r21];
                                                                                                            [r19 continueEndedTask];
                                                                                                    }
                                                                                                    else {
                                                                                                            [r19 launchTask];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            r19[0x80] = 0x0;
                                                            r2 = r20;
                                                            [extractionController kwet:r2 withFormat:0x1e];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                            }
                    }
                    else {
                            if ([*r20 formatDetected] == 0x0) {
                                    if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                            if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                            else {
                                                    r19[0x80] = 0x0;
                                                    r2 = r20;
                                                    [extractionController kwet:r2 withFormat:0x1e];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                            }
                            else {
                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                            if ([*r20 formatDetected] == 0x21) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            r19[0x80] = 0x0;
                                                    }
                                                    [extractionController snzip:r20];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                            else {
                                                    if ([*r20 formatDetected] != 0x0) {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController szip:r20];
                                                    }
                                            }
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                            }
                    }
            }
            else {
                    r19[0x80] = 0x0;
                    r3 = [r19[0x14] formatDetected];
                    r2 = r20;
                    [extractionController kwet:r2 withFormat:r3];
                    if ([*r20 binary] == -0x1) {
                            r19[0x80] = 0x0;
                            [r19[0x14] setBinary:r21];
                            [r19 continueEndedTask];
                    }
                    else {
                            [r19 launchTask];
                    }
            }
    }
    else {
            if (([*r20 usedBinary:0x15] & 0x1) == 0x0) {
                    if ([*r20 formatDetected] == 0x20) {
                            r19[0x80] = 0x0;
                            r3 = [r19[0x14] formatDetected];
                            r2 = r20;
                            [extractionController kwet:r2 withFormat:r3];
                            if ([*r20 binary] == -0x1) {
                                    r19[0x80] = 0x0;
                                    [r19[0x14] setBinary:r21];
                                    [r19 continueEndedTask];
                            }
                            else {
                                    [r19 launchTask];
                            }
                    }
                    else {
                            if ([*r20 formatDetected] == 0x0) {
                                    if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                            if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] != 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                                            r19[0x80] = 0x0;
                                                                                                                    }
                                                                                                                    [extractionController szip:r20];
                                                                                                            }
                                                                                                    }
                                                                                            }
                                                                                            else {
                                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController snzip:r20];
                                                                                            }
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                            if ([*r20 binary] == -0x1) {
                                                                                                    r19[0x80] = 0x0;
                                                                                                    [r19[0x14] setBinary:r21];
                                                                                                    [r19 continueEndedTask];
                                                                                            }
                                                                                            else {
                                                                                                    [r19 launchTask];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                            else {
                                                    r19[0x80] = 0x0;
                                                    r2 = r20;
                                                    [extractionController kwet:r2 withFormat:0x1e];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                            }
                            else {
                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                            if ([*r20 formatDetected] == 0x21) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            r19[0x80] = 0x0;
                                                    }
                                                    [extractionController snzip:r20];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                            else {
                                                    if ([*r20 formatDetected] != 0x0) {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController szip:r20];
                                                    }
                                            }
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                            }
                    }
            }
            else {
                    if ([*r20 formatDetected] == 0x0) {
                            if ([[[*r20 file] pathExtension] isEqualToString:@"wpress"] != 0x0) {
                                    if (([*r20 usedBinary:0x15] & 0x1) != 0x0) {
                                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] != 0x0) {
                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController szip:r20];
                                                                            }
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                                    r19[0x80] = 0x0;
                                                                                                            }
                                                                                                            [extractionController szip:r20];
                                                                                                    }
                                                                                            }
                                                                                    }
                                                                                    else {
                                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController snzip:r20];
                                                                                    }
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                                    if ([*r20 binary] == -0x1) {
                                                                                            r19[0x80] = 0x0;
                                                                                            [r19[0x14] setBinary:r21];
                                                                                            [r19 continueEndedTask];
                                                                                    }
                                                                                    else {
                                                                                            [r19 launchTask];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                            else {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                    }
                                    else {
                                            r19[0x80] = 0x0;
                                            r2 = r20;
                                            [extractionController kwet:r2 withFormat:0x1e];
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                            }
                            else {
                                    if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                            if ([*r20 formatDetected] == 0x21) {
                                                    if ([*r20 formatDetected] == 0x21) {
                                                            r19[0x80] = 0x0;
                                                    }
                                                    [extractionController snzip:r20];
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                            else {
                                                    if ([*r20 formatDetected] != 0x0) {
                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController szip:r20];
                                                                    }
                                                            }
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                                    else {
                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                            if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                                                            r19[0x80] = 0x0;
                                                                                                    }
                                                                                                    [extractionController szip:r20];
                                                                                            }
                                                                                    }
                                                                            }
                                                                            else {
                                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                                            r19[0x80] = 0x0;
                                                                                    }
                                                                                    [extractionController snzip:r20];
                                                                            }
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                            if ([*r20 binary] == -0x1) {
                                                                                    r19[0x80] = 0x0;
                                                                                    [r19[0x14] setBinary:r21];
                                                                                    [r19 continueEndedTask];
                                                                            }
                                                                            else {
                                                                                    [r19 launchTask];
                                                                            }
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                                    else {
                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController szip:r20];
                                                    }
                                            }
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                            }
                    }
                    else {
                            if (([*r20 usedBinary:0x18] & 0x1) == 0x0) {
                                    if ([*r20 formatDetected] == 0x21) {
                                            if ([*r20 formatDetected] == 0x21) {
                                                    r19[0x80] = 0x0;
                                            }
                                            [extractionController snzip:r20];
                                            if ([*r20 binary] == -0x1) {
                                                    r19[0x80] = 0x0;
                                                    [r19[0x14] setBinary:r21];
                                                    [r19 continueEndedTask];
                                            }
                                            else {
                                                    [r19 launchTask];
                                            }
                                    }
                                    else {
                                            if ([*r20 formatDetected] != 0x0) {
                                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                    if ([*r20 formatDetected] == 0x1b) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController szip:r20];
                                                            }
                                                    }
                                                    if ([*r20 binary] == -0x1) {
                                                            r19[0x80] = 0x0;
                                                            [r19[0x14] setBinary:r21];
                                                            [r19 continueEndedTask];
                                                    }
                                                    else {
                                                            [r19 launchTask];
                                                    }
                                            }
                                            else {
                                                    if (([[[*r20 file] pathExtension] isEqualToString:@"snappy"] & 0x1) == 0x0) {
                                                            if (([[[*r20 file] pathExtension] isEqualToString:@"iwa"] & 0x1) == 0x0) {
                                                                    if ([[[*r20 file] pathExtension] isEqualToString:@"raw"] == 0x0) {
                                                                            if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                                                                    if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                                                            if ([*r20 formatDetected] == 0x1b) {
                                                                                                    r19[0x80] = 0x0;
                                                                                            }
                                                                                            [extractionController szip:r20];
                                                                                    }
                                                                            }
                                                                    }
                                                                    else {
                                                                            if ([*r20 formatDetected] == 0x21) {
                                                                                    r19[0x80] = 0x0;
                                                                            }
                                                                            [extractionController snzip:r20];
                                                                    }
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                            else {
                                                                    if ([*r20 formatDetected] == 0x21) {
                                                                            r19[0x80] = 0x0;
                                                                    }
                                                                    [extractionController snzip:r20];
                                                                    if ([*r20 binary] == -0x1) {
                                                                            r19[0x80] = 0x0;
                                                                            [r19[0x14] setBinary:r21];
                                                                            [r19 continueEndedTask];
                                                                    }
                                                                    else {
                                                                            [r19 launchTask];
                                                                    }
                                                            }
                                                    }
                                                    else {
                                                            if ([*r20 formatDetected] == 0x21) {
                                                                    r19[0x80] = 0x0;
                                                            }
                                                            [extractionController snzip:r20];
                                                            if ([*r20 binary] == -0x1) {
                                                                    r19[0x80] = 0x0;
                                                                    [r19[0x14] setBinary:r21];
                                                                    [r19 continueEndedTask];
                                                            }
                                                            else {
                                                                    [r19 launchTask];
                                                            }
                                                    }
                                            }
                                    }
                            }
                            else {
                                    if (([*r20 usedBinary:0x16] & 0x1) == 0x0 && ([*r20 usedBinary:0x0] & 0x1) == 0x0) {
                                            if (([*r20 usedBinary:0x17] & 0x1) == 0x0) {
                                                    if ([*r20 formatDetected] == 0x1b) {
                                                            r19[0x80] = 0x0;
                                                    }
                                                    [extractionController szip:r20];
                                            }
                                    }
                                    if ([*r20 binary] == -0x1) {
                                            r19[0x80] = 0x0;
                                            [r19[0x14] setBinary:r21];
                                            [r19 continueEndedTask];
                                    }
                                    else {
                                            [r19 launchTask];
                                    }
                            }
                    }
            }
    }
    return;

.l8:
    if (([r0 usedBinary:0x12] & 0x1) == 0x0) {
            [extractionController lrzip:r20];
            [r19 setProgressIndeterminate:0x1];
    }
    goto .l35;
