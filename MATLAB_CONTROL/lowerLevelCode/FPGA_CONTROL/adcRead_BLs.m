function [ stringCurr, returnVoltagesEncrypted ] = adcRead_BLs( fileHandle, repeatAvg, bitLines, refs_cycle, stringCurr )
% getting fnName
    st = dbstack;fnName = st.name ;
    arguments.bitLines   = bitLines ;
    arguments.repeatAvg   = repeatAvg ;
    arguments.refs_cycle   = refs_cycle ;
    returnedData = uart_encryptReturnData( fileHandle, fnName, arguments ) ;
    returnVoltagesEncrypted = returnedData ;
%     stringCurr        = startStringIn ;
% display( mfilename )
end