// ev_09b4f433a4f9b7a3e5eea0ccab525e1992f39d9d0d7b5f8a212acbb135798aaa
    var_10 = r20;
    decomp_var_8 = r19;
    r31 = r31 + 0xffffffffffffffe0;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r19 = self;
    if (([self[0x14] writesToStandardOutput] & 0x1) == 0x0) {
            r0 = [NSPipe pipe];
            r19[0x15] = r0;
            [r19[0x14] setStandardOutput:r0];
    }
    r0 = [NSPipe pipe];
    r19[0x16] = r0;
    [r19[0x14] setStandardError:r0];
    if ([r19[0x14] tarballTask] != 0x0) {
            r2 = [[r19[0x14] tarballTask] standardOutput];
    }
    else {
            r0 = [NSPipe pipe];
            r2 = r0;
            r19[0x17] = r0;
    }
    [r19[0x14] setStandardInput:r2];
    [r19 setReadHandleInBackgroundAndNotify];
    return;
