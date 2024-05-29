function stringCurr = RBL_IMC_Select( fileHandle, RBL, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.RBL   = RBL ;
    uart_encrypt( fileHandle, fnName, arguments ) ;
%     stringCurr        = startStringIn ;
 display( mfilename );
end