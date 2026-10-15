// ev_baf276154cb54e0800e5211570e05bdbe2f903361fc7be42d840926103bc879d
    r3 = arg3;
    r2 = arg2;
    r1 = _cmd;
    r31 = r31 - 0x50;
    var_30 = r24;
    decomp_var_28 = r23;
    var_20 = r22;
    decomp_var_18 = r21;
    var_10 = r20;
    decomp_var_8 = r19;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r21 = r3;
    r20 = r2;
    r19 = self;
    [self logDebugInfo];
    if (r21 != 0x0 && [r19[0x14] type] == 0x0) {
            [formatDetection formatName:[r19[0x14] formatDetected]];
            r21 = [NSString stringWithFormat:@"Format detected: %@", r3];
            loc_100024a08([NSString stringWithFormat:@"%@", r3], r1, r2);
            r22 = r19[0xd];
            r2 = [r21 copy];
            [r22 addObject:r2];
    }
    [[r19[0x14] launchPath] lastPathComponent];
    r21 = [NSString stringWithFormat:@"Binary used: %@", r3];
    loc_100024a08([NSString stringWithFormat:@"%@", r3], r1, r2);
    [r19[0xd] addObject:[r21 copy]];
    [r19[0x14] arguments];
    r21 = [NSString stringWithFormat:@"Arguments: %@", r3];
    loc_100024a08([NSString stringWithFormat:@"%@", r3], r1, r2);
    [r19[0xd] addObject:[r21 copy]];
    if (([r19[0x14] launchPathIsAccessible] & 0x1) != 0x0) {
            if ([r19[0x14] taskPreparationErrorCode] == 0x0) {
                    if ([r19[0x14] type] == 0x0) {
                            [Globals updateTasksList:0x1 Path:[[[r19[0x14] package] packagePath] path]];
                    }
                    [r19 setTerminationObserver];
                    [r19 setPipes];
                    if (r20 != 0x0) {
                            [r19 setStartTime];
                    }
                    [r19[0x14] launch];
                    if (([r19[0x14] launched] & 0x1) != 0x0) {
                            if ([r19[0x14] tarballTask] != 0x0) {
                                    [[r19[0x14] tarballTask] launch];
                            }
                            if (r20 != 0x0) {
                                    [r19 updateRemainingTime];
                            }
                    }
                    else {
                            [r19 checkEndedTask];
                    }
            }
            else {
                    [r19 checkEndedTask];
            }
    }
    else {
            r19[0x80] = 0x0;
            [r19 checkEndedTask];
    }
    return;
