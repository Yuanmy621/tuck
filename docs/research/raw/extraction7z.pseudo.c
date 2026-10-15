// ev_c7334ba0c1aac7163798f81919897e314775e328b32a769073a6c038d9c20876
    r31 = r31 - 0x140;
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
    r8 = *___stack_chk_guard;
    r8 = *r8;
    var_58 = r8;
    r19 = *arg2;
    if (sub_100042e1c(0x1, 0xa, 0xd, 0x0) != 0x0) {
            r8 = 0x1;
    }
    r24 = r8 & *(int8_t *)byte_10009c854;
    r0 = r19;
    if (r24 == 0x1) {
            r3 = 0x0;
            [r0 setBinary:0x16];
            [r19 setLaunchPath:[[NSBundle mainBundle] pathForAuxiliaryExecutable:[Globals binaryKekaName:[r19 binary]]]];
            if (*(int8_t *)byte_10009c856 == 0x1) {
                    r3 = @"DECOMPOSE_UNICODE_TO_MULTIBYTE";
                    [r19 addToEnvironment:[NSDictionary dictionaryWithObject:@"" forKey:@"DECOMPOSE_UNICODE_TO_MULTIBYTE"]];
            }
    }
    else {
            r3 = 0x0;
            [r0 setBinary:0x0];
            [r19 setLaunchPath:[[NSBundle mainBundle] pathForAuxiliaryExecutable:@"keka7z"]];
    }
    if (*(int8_t *)byte_10009cc11 == 0x1) {
            r3 = @"IGNORE_NAME_GZ";
            [r19 addToEnvironment:[NSDictionary dictionaryWithObject:@"" forKey:@"IGNORE_NAME_GZ"]];
    }
    [r19 setCurrentDirectoryPath:[[[r19 package] packageTemporaryPath] path]];
    r0 = [NSMutableArray array];
    r20 = r0;
    [r0 addObject:@"x"];
    [r20 addObject:[r19 file]];
    if (r24 != 0x0) {
            [r20 addObject:@"-snld20"];
    }
    [r20 addObject:@"-aou"];
    [r20 addObject:@"-xr!__MACOSX"];
    if (*(int8_t *)byte_10009cb9a == 0x1) {
            var_120 = q0;
            var_110 = q0;
            r0 = [Globals defaultExclusions];
            r21 = r0;
            r0 = [r0 countByEnumeratingWithState:&var_120 objects:&decomp_var_d8 count:0x10];
            if (r0 != 0x0) {
                    r23 = r0;
                    r25 = *var_110;
                    do {
                            r27 = 0x0;
                            do {
                                    if (*var_110 != r25) {
                                            objc_enumerationMutation(r21);
                                    }
                                    [r20 addObject:[NSString stringWithFormat:@"-xr!%@", r3]];
                                    r27 = r27 + 0x1;
                            } while (r23 != r27);
                            r0 = [r21 countByEnumeratingWithState:&var_120 objects:&decomp_var_d8 count:0x10];
                            r23 = r0;
                    } while (r0 != 0x0);
            }
    }
    if ([r19 formatDetected] != 0x1b && [r19 maximumThreads] >= 0x1) {
            [r20 addObject:[Globals setThreadCount7Z:[r19 maximumThreads]]];
    }
    if (r24 != 0x0) {
            [r20 addObject:@"-bsp1"];
    }
    [r20 addObject:@"-keka"];
    [r19 setArguments:r20];
    if (**___stack_chk_guard != var_58) {
            __stack_chk_fail();
    }
    return;
