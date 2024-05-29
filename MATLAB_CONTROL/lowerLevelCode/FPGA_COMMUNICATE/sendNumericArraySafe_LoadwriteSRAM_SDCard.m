function returnedData = sendNumericArraySafe_LoadwriteSRAM_SDCard( fileHandle, numArray, numSentBytesAtATime )
% display( mfilename )
% fileHandle is actually a struct, fileHandle.s is a serialport object
%%
maxWaitTime = 30 ; % in seconds
%% SENDING numArray
%     numSentBytesAtATime = 4 ;
    numArrayElements            = numel(numArray) ; % This is guaranteed to be a multiple of numSentBytesAtATime in the printNumericArray function
    returnedData                = [] ;
    tic
    for i = 1 : numSentBytesAtATime : ( numArrayElements - ( numSentBytesAtATime - 1 ) )
        sendBytes = numArray( i : i + ( numSentBytesAtATime - 1 ) ) ;
        write( fileHandle.s, sendBytes, 'uint8' ) ;
        while( fileHandle.s.NumBytesAvailable < numSentBytesAtATime ) 
            timeElapsed = toc ;
            if( timeElapsed > maxWaitTime )
                killAllSuppliesAndPause() ;
            end
        end
        tic
        retMirrorBytes = read( fileHandle.s, numSentBytesAtATime , 'uint8' ) ;
%         Now to check if sent and returned data are same
        if( isempty( find( sendBytes ~= retMirrorBytes, 1 ) ) )
            returnedData = [ returnedData retMirrorBytes ] ;
        else
            fprintf( 'DATA TRANSMISSION ERROR\n' ) ;
            fprintf( ' SENT DATA : ' ) ;fprintf( '%03d ', sendBytes ) ;fprintf( '\n' ) ;
            fprintf( ' RCVD DATA : ' ) ;fprintf( '%03d ', retMirrorBytes ) ; fprintf( '\n' ) ;
            pause( 1 ) ;
        end
    end
%% NOW TO TRANSMIT SYNCDATA: 125
    write( fileHandle.s, 125, 'uint8' ) ;
%% NOW TO WAIT FOR THE STOPBYTE: 127
    fnOutputByte = -1 ; % fillerValue just to pass the first while loop
    tic ;
    while fnOutputByte~= 127
        while( fileHandle.s.NumBytesAvailable < 1 )
            timeElapsed = toc ;
            if( timeElapsed > maxWaitTime )
                killAllSuppliesAndPause() ;
            end
        end
        tic ;
        fnOutputByte = read( fileHandle.s, 1 , 'uint8' ) ;
        returnedData = [ returnedData fnOutputByte ] ;
    end
end