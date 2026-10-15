// ev_75621052b6388c6c40356c35a11ffea2e44fd594acdcfc331766e62eeccb409f
    r1 = _cmd;
    r31 = r31 - 0x60;
    var_40 = r26;
    decomp_var_38 = r25;
    var_30 = r24;
    decomp_var_28 = r23;
    var_20 = r22;
    decomp_var_18 = r21;
    var_10 = r20;
    decomp_var_8 = r19;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r20 = arg2;
    r19 = self;
    r2 = self[0x1a];
    if (r2 != 0x0) {
            if ([r20 writeToURL:r2 atomically:0x1 encoding:0x4 error:0x0] != 0x0) {
                    [r19[0x1a] path];
                    loc_1000246e8([NSString stringWithFormat:@"Saved password file at: %@", r3], r1, r2);
                    r0 = r19[0x1a];
            }
            else {
                    r0 = 0x0;
            }
    }
    else {
            while (true) {
                    r21 = CFUUIDCreate(0x0);
                    r1 = r21;
                    r22 = CFUUIDCreateString(0x0, r1);
                    r2 = [[sandboxHelpers getContainerTempPath] stringByAppendingString:r22];
                    r19[0x1a] = [NSURL fileURLWithPath:r2];
                    CFRelease(r21);
                    r0 = [r22 release];
                    if (r19[0x1a] == 0x0) {
                            temp_0 = 0x1;
                            break;
                    }
                    r21 = [NSFileManager defaultManager];
                    r2 = [r19[0x1a] path];
                    if (([r21 fileExistsAtPath:r2] & 0x1) != 0x0) {
                            continue;
                    }
                    r0 = r19[0x1a];
                    if (r0 != 0x0) {
                            r0 = [r0 retain];
                            r2 = r19[0x1a];
                            temp_0 = 0x0;
                    }
                    else {
                            temp_0 = 0x2;
                    }
                    break;
            }
            if (temp_0 != 0x0) {
                    if (temp_0 == 0x1) {
                            r0 = 0x0;
                    }
            }
            else {
                    if ([r20 writeToURL:r2 atomically:0x1 encoding:0x4 error:0x0] != 0x0) {
                            [r19[0x1a] path];
                            loc_1000246e8([NSString stringWithFormat:@"Saved password file at: %@", r3], r1, r2);
                            r0 = r19[0x1a];
                    }
                    else {
                            r0 = 0x0;
                    }
            }
    }
    return r0;
