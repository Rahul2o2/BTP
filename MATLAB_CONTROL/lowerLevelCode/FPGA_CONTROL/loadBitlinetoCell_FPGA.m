function returnString = loadBitlinetoCell_FPGA( fileHandle, bitLines, stringCurr )
display( mfilename )
% getting fnName
    st = dbstack;fnName = st.name ;
% encrypting fn,args
    arguments.bitLines   = bitLines ;
    uart_encrypt( fileHandle, fnName, arguments ) ;
    returnString        = stringCurr ;
end