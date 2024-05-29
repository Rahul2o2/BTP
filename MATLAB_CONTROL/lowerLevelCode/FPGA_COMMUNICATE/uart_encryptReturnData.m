function returnedData = uart_encryptReturnData( fileHandle,fnName,arguments )
% display( mfilename )
returnedData        = [] ; 
stopByte            = 127 ;
parameter_marker    = 126 ;
parameterMarkerByte = parameter_marker ;

switch fnName
     case 'adcRead_BLs'
        fnNameByte      = 94;
%               authored by: Ashwin Balagopal S
%				uart function call: fnNameByte=abcd[0]=94 repeatAvg=abcd[1] bitLines=abcd[2+(0:N-1)] stopByte=abcd[N+1]=127
%				( fileHandle, repeatAvg, bitLines, refs, stringCurr )
            bitLines   = arguments.bitLines ; bitLines = reshape( bitLines, [ 1 numel(bitLines) ] ) ;
            repeatAvg  = arguments.repeatAvg ;
            numArray        = [ fnNameByte repeatAvg bitLines stopByte ] ;
                returnedData = printNumericArray_adcRead_BLsRefs_returnVoltages(fileHandle, numArray, repeatAvg, bitLines, refs_cycle  ) ;
        
end
end