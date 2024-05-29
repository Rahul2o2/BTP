function stringCurr = IMGSelect( fileHandle, IMG, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.IMG   = IMG ;
    uart_encrypt( fileHandle, fnName, arguments ) ;
%     stringCurr        = startStringIn ;
 display( mfilename );
end