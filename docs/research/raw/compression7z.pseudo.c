// ev_e9396a6c714f6f224b54043fd0ecf1578e18b377f55b14ddcac1fa44933bc013
    var_50 = r28;
    decomp_var_48 = r27;
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
    r31 = r31 + 0xffffffffffffffa0 - 0x260;
    r22 = arg2;
    r23 = self;
    var_60 = **___stack_chk_guard;
    r21 = [self szipBinary:arg2];
    r19 = *r22;
    r0 = [NSMutableArray array];
    r20 = r0;
    [r0 addObject:@"a"];
    r8 = [r19 format] - 0x1;
    if (r8 <= 0xd) {
            r2 = *(0x10007c2c0 + r8 * 0x8);
    }
    else {
            r2 = @"-t7z";
    }
    [r20 addObject:r2];
    if (r21 != 0x0) {
            [r20 addObject:@"-snh"];
            [r20 addObject:@"-snl"];
            if ([r19 legacySupport] != 0x0 && [r19 format] == 0xc) {
                    [r20 addObject:@"-myv=1900"];
            }
            [r20 addObject:@"-ssp"];
    }
    [r20 addObject:@"-ssc"];
    if ([r19 split] != 0x0) {
            [r20 addObject:[r23 setSplit:[r19 split]]];
    }
    if ([r19 format] != 0xe) {
            if ([r19 maximumThreads] >= 0x1) {
                    [r20 addObject:[Globals setThreadCount7Z:[r19 maximumThreads]]];
            }
            [r20 addObject:[r23 setMethod:[r19 method]]];
            if ([r19 password] != 0x0) {
                    [r20 addObject:@"-p"];
                    if ([r19 encryption] != 0x0 && [r19 format] == 0xc) {
                            [r20 addObject:@"-mhe"];
                    }
                    if ([r19 format] == 0x1 && [r19 encryptionAES256] != 0x0) {
                            [r20 addObject:@"-mem=AES256"];
                    }
            }
    }
    if ([r19 format] == 0xc) {
            if ([r19 solid] == 0x0) {
                    r2 = @"-ms=off";
            }
            else {
                    r2 = @"-ms=on";
            }
            [r20 addObject:r2];
    }
    if ([r19 sfx] != 0x0) {
            [r23 setSFXModule:@"7z.sfx" forTask:r22 andArguments:r20];
            if ([r19 split] != 0x0) {
                    [r19 setDestination:[[[r19 destination] stringByDeletingPathExtension] stringByAppendingPathExtension:[Globals formatExtension:[r19 format]]]];
            }
    }
    if ([r19 exclude] != 0x0) {
            var_220 = q0;
            var_210 = q0;
            r0 = [Globals defaultExclusions];
            r22 = r0;
            r0 = [r0 countByEnumeratingWithState:&var_220 objects:&decomp_var_e0 count:0x10];
            if (r0 != 0x0) {
                    r24 = r0;
                    r25 = *var_210;
                    do {
                            r27 = 0x0;
                            do {
                                    if (*var_210 != r25) {
                                            objc_enumerationMutation(r22);
                                    }
                                    [r20 addObject:[NSString stringWithFormat:@"-xr!%@", r3]];
                                    r27 = r27 + 0x1;
                            } while (r24 != r27);
                            r0 = [r22 countByEnumeratingWithState:&var_220 objects:&decomp_var_e0 count:0x10];
                            r24 = r0;
                    } while (r0 != 0x0);
            }
    }
    if (r21 != 0x0) {
            [r20 addObject:@"-bsp1"];
    }
    if (([r19 contentsOfFolder] & 0x1) == 0x0) {
            [r20 addObject:@"-spd"];
    }
    if ([r19 needsTarballTask] != 0x0) {
            [r20 addObject:@"-si"];
    }
    [r20 addObject:@"-keka"];
    [r20 addObject:[r19 destination]];
    if (([r19 needsTarballTask] & 0x1) == 0x0) {
            if ([r19 contentsOfFolder] != 0x0) {
                    [r19 setCurrentDirectoryPath:[r19 file]];
                    [r20 addObject:@"*"];
            }
            else {
                    if ([r19 commonPath] != 0x0) {
                            [r19 setCurrentDirectoryPath:[r19 commonPath]];
                            var_260 = q0;
                            var_250 = q0;
                            r0 = [r19 files];
                            r21 = r0;
                            r0 = [r0 countByEnumeratingWithState:&var_260 objects:&decomp_var_160 count:0x10];
                            if (r0 != 0x0) {
                                    r23 = r0;
                                    r26 = *var_250;
                                    do {
                                            r28 = 0x0;
                                            do {
                                                    if (*var_250 != r26) {
                                                            objc_enumerationMutation(r21);
                                                    }
                                                    [*(var_258 + r28 * 0x8) substringFromIndex:[[r19 commonPath] length]];
                                                    [r20 addObject:[NSString stringWithFormat:@"./%@", r3]];
                                                    r28 = r28 + 0x1;
                                            } while (r23 != r28);
                                            r0 = [r21 countByEnumeratingWithState:&var_260 objects:&decomp_var_160 count:0x10];
                                            r23 = r0;
                                    } while (r0 != 0x0);
                            }
                    }
                    else {
                            var_2A0 = q0;
                            var_290 = q0;
                            r0 = [r19 files];
                            r21 = r0;
                            r0 = [r0 countByEnumeratingWithState:&var_2A0 objects:&decomp_var_1e0 count:0x10];
                            if (r0 != 0x0) {
                                    r23 = r0;
                                    r24 = *var_290;
                                    do {
                                            r26 = 0x0;
                                            do {
                                                    if (*var_290 != r24) {
                                                            objc_enumerationMutation(r21);
                                                    }
                                                    [r20 addObject:[NSString stringWithFormat:@"%@", r3]];
                                                    r26 = r26 + 0x1;
                                            } while (r23 != r26);
                                            r0 = [r21 countByEnumeratingWithState:&var_2A0 objects:&decomp_var_1e0 count:0x10];
                                            r23 = r0;
                                    } while (r0 != 0x0);
                            }
                    }
            }
    }
    [r19 setArguments:r20];
    if (**___stack_chk_guard != var_60) {
            __stack_chk_fail();
    }
    return;
