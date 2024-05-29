function stringCurr = LoadwriteSRAM_SDCard( fileHandle, WtSlice, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.WtSlice   = WtSlice ;
    uart_encrypt( fileHandle, fnName, arguments ) ;
%     stringCurr        = startStringIn ;
 display( mfilename );
end