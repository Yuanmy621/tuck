// ev_160684e799cb1bc28f4f72fb159c7fe3855d0794c5b57e9f35c578380a717e96
    r31 = r31 - 0x40;
    var_20 = r22;
    decomp_var_18 = r21;
    var_10 = r20;
    decomp_var_8 = r19;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r19 = self;
    if (sub_100042e1c(0x1, 0xa, 0xd, 0x0) != 0x0) {
            var_28 = 0x0;
            r0 = r19[0x1];
            r0 = [r0 launchAndReturnError:&var_28];
            r8 = r0;
            r19[0x32] = r0;
            r0 = var_28;
            if (r0 == 0x0) {
                    r8 = 0x1;
            }
            if ((r8 & 0x1) == 0x0) {
                    [r19 setTaskError:[r0 description]];
                    [var_28 description];
                    sub_100024a7c([NSString stringWithFormat:@"%@", r3]);
            }
    }
    else {
            [r19[0x1] launch];
            r19[0x32] = [r19[0x1] isRunning];
    }
    [r19 removePasswordFileAlmostImmediatly];
    return;
