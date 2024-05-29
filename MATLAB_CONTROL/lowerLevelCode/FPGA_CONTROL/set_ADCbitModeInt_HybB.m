function stringCurr = set_ADCbitModeInt_HybB( fileHandle, internal_hyb_B, ADC_bitMode, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.internal_hyb_B=internal_hyb_B;
    arguments.ADC_bitMode=ADC_bitMode;
    uart_encrypt( fileHandle, fnName, arguments ) ;
%     stringCurr        = startStringIn ;
 display( mfilename );
end