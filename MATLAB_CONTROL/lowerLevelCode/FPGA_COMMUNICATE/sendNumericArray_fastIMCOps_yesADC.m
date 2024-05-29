function [ returnedData, success ] = sendNumericArray_fastIMCOps_yesADC( fileHandle, numArray, numSentBytesAtATime, repeatAvg  )
% display( mfilename )
% fileHandle is actually a struct, fileHandle.s is a serialport object
%% SENDING numArray
%     numSentBytesAtATime = 4 ;
    numArrayElements            = numel(numArray) ; % This is guaranteed to be a multiple of numSentBytesAtATime in the printNumericArray function
    returnedData_echo                = [] ;
    for i = 1 : numSentBytesAtATime : ( numArrayElements - ( numSentBytesAtATime - 1 ) )
        sendBytes = numArray( i : i + ( numSentBytesAtATime - 1 ) ) ;
        write( fileHandle.s, sendBytes, 'uint8' ) ;
        while( fileHandle.s.NumBytesAvailable < numSentBytesAtATime ) 
        end
        retMirrorBytes = read( fileHandle.s, numSentBytesAtATime , 'uint8' ) ;
%         Now to check if sent and returned data are same
        if( isempty( find( sendBytes ~= retMirrorBytes, 1 ) ) )
            returnedData_echo = [ returnedData_echo retMirrorBytes ] ;
        else
            fprintf( 'DATA TRANSMISSION ERROR\n' ) ;
            fprintf( ' SENT DATA : ' ) ;fprintf( '%03d ', sendBytes ) ;fprintf( '\n' ) ;
            fprintf( ' RCVD DATA : ' ) ;fprintf( '%03d ', retMirrorBytes ) ; fprintf( '\n' ) ;
            pause( 1 ) ;
        end
    end
%% NOW TO TRANSMIT FOR SYNCDATA: 125
    write( fileHandle.s, 125, 'uint8' ) ;
%% NOW TO WAIT FOR THE STOPBYTE: 127
    fnOutputByte = -1 ; % fillerValue just to pass the first while loop
    returnedData_output = [] ;
    while fnOutputByte~= 127
        while( fileHandle.s.NumBytesAvailable < 1 ) 
        end
        fnOutputByte = read( fileHandle.s, 1 , 'uint8' ) ;
        returnedData_output = [ returnedData_output fnOutputByte ] ;
    end
%     (4*repeatAvg)*N1*N2 + stopByte
    expect_size = ( 1 + ( 4 * repeatAvg ) ) + 1 ; % 1 byte is the scanout data and rest are for adcread
    if( numel( returnedData_output ) == expect_size )
        success = 1 ;
    else
        success = 0 ;
    end
    returnedData = [ returnedData_echo returnedData_output ] ;
end