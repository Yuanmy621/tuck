// ev_eec0e941acb1b8f2f7123ec1805f6bcfe21bba8f6e6326df7e1bd22aee94af8b
    r31 = r31 - 0x70;
    var_10 = r20;
    decomp_var_8 = r19;
    saved_fp = r29;
    decomp_var_m8 = r30;
    r19 = arg2;
    var_30 = 0x0;
    var_28 = &var_30;
    if ([arg2 length] != 0x0) {
            r19 = [r19 stringByReplacingOccurrencesOfString:@"\x08" withString:@" "];
            r20 = [NSRegularExpression regularExpressionWithPattern:@"^( |[0-9])+%( |[0-9])+(\+|\-) " options:0x1 error:0x0];
            r0 = [r19 length];
            [r20 enumerateMatchesInString:r19 options:0x0 range:0x0 usingBlock:r0];
    }
    r19 = var_28[0x6];
    _Block_object_dispose(&var_30, 0x8);
    r0 = r19;
    return r0;
