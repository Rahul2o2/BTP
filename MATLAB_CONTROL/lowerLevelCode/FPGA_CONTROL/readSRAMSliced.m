function stringCurr = readSRAMSliced( fileHandle, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    uart_encrypt( fileHandle, fnName) ;
%     stringCurr        = startStringIn ;
 display( mfilename );
end