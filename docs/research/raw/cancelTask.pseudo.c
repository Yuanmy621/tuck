// ev_26ff6293fc49f9e446cb9e6196bd12e4981d638dc3c54ef322780dbe05898863
    r2 = arg2;
    if (r2 != 0x0) {
            r31 = r31 - 0x50;
            var_20 = r20;
            decomp_var_18 = r19;
            var_10 = r29;
            decomp_var_8 = r30;
            r19 = r2;
            if ([r2 isRunning] != 0x0) {
                    [r19 terminate];
                    r0 = dispatch_get_global_queue(0x0, 0x0);
                    var_48 = *__NSConcreteStackBlock;
                    dispatch_async(r0, &var_48);
            }
    }
    return;
