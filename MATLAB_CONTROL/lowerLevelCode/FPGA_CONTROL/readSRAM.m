function stringCurr = readSRAM( fileHandle, bitLine, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.bitLine   = bitLine ;
    uart_encrypt( fileHandle, fnName, arguments ) ;
%     stringCurr        = startStringIn ;
 display( mfilename )
end