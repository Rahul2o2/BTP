function stringCurr = set_modeInternalHybridTiming( fileHandle, internal_hyb_B,IMG_TIME_GPIO_DATA,RWL_EN_DELAY,CM_EN_DELAY,ADCSTART_DELAY,SAMPLING_DELAY,EVALUATE_DELAY,FPGA_RESET_REPEAT_UB_hybrid, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.internal_hyb_B=internal_hyb_B;
    arguments.IMG_TIME_GPIO_DATA =IMG_TIME_GPIO_DATA;
    arguments.RWL_EN_DELAY=RWL_EN_DELAY;
    arguments.CM_EN_DELAY=CM_EN_DELAY;
    arguments.ADCSTART_DELAY=ADCSTART_DELAY;
    arguments.SAMPLING_DELAY=SAMPLING_DELAY;
    arguments.EVALUATE_DELAY=EVALUATE_DELAY;
    arguments.FPGA_RESET_REPEAT_UB_hybrid=FPGA_RESET_REPEAT_UB_hybrid;

    uart_encrypt( fileHandle, fnName, arguments ) ;
%     stringCurr        = startStringIn ;
 display( mfilename );
end