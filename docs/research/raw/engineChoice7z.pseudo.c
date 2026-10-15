// ev_b7ff0fbb3e136abce0ee3c534a1c2e9ea7f5d7dee7ea186b9b670cb71acf9736
    var_30 = r24;
    decomp_var_28 = r23;
    r31 = r31 + 0xffffffffffffffc0;
    var_20 = r22;
    decomp_var_18 = r21;
    var_10 = r20;
    decomp_var_8 = r19;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r19 = *arg2;
    if (sub_100042e1c(0x1, 0xa, 0xd, 0x0) != 0x0) {
            r8 = 0x1;
    }
    r20 = r8 & *(int8_t *)byte_10009c854;
    r0 = r19;
    if (r20 == 0x1) {
            [r0 setBinary:0x16];
            [r19 setLaunchPath:[[NSBundle mainBundle] pathForAuxiliaryExecutable:[Globals binaryKekaName:[r19 binary]]]];
            if (*(int8_t *)byte_10009c855 == 0x1) {
                    [r19 addToEnvironment:[NSDictionary dictionaryWithObject:@"" forKey:@"COMPOSE_MULTIBYTE_TO_UNICODE"]];
            }
    }
    else {
            [r0 setBinary:0x0];
            [r19 setLaunchPath:[[NSBundle mainBundle] pathForAuxiliaryExecutable:@"keka7z"]];
    }
    [r19 addToEnvironment:[NSDictionary dictionaryWithObject:[sandboxHelpers getContainerTempPath] forKey:@"TEMP_PATH_7Z"]];
    r0 = r20;
    return r0;
