function printNumericArray(fileHandle, numArray)
% display( mfilename )
    
%%    OLD CODE FOR TERATERM
%     numArrayElements            = numel(numArray) ;
%     numArray                    = reshape(numArray,1,numArrayElements) ;
%     n_124                       = mod( ( 4 - mod( numArrayElements, 4 ) ),4 );
%   
%     numArray                    = [numArray repmat(124,1,n_124)];
%     numArrayElements            = numel( numArray ) ;
%     find92or37                  = ( numArray == 92 )|( numArray == 37 ) ; % 37 = double('%') 92 = double('\')
%     numArray                    = repmat( numArray, 2, 1 ) ;
%     numArray                    = string( numArray ) ;
%     numArray(2,~find92or37)     = "" ;
%     
%     numArray                    = reshape( numArray, 1, numArrayElements * 2 ) ;
%     numArray                    = double( numArray ) ;
%     numArray( isnan( numArray ) )   = [] ;    
%     fprintf( fileHandle, char( numArray ) ) ;

%%     NEW CODE FOR SERIALPORT
%  - fileHandle is actually a struct, fileHandle.s is a serialport object,
%  - fileHandle.fH is a fileHandle for the receive file
    numArrayElements            = numel(numArray) ;
    numSentBytesAtATime         = 16 ; % THIS MUST MATCH WHAT IS THERE IN THE C CODE
    numArray                    = reshape(numArray,1,numArrayElements) ;
    n_124                       = mod( ( numSentBytesAtATime - mod( numArrayElements, numSentBytesAtATime ) ),numSentBytesAtATime );
  
    numArray                    = [numArray repmat(124,1,n_124)];
    
%     returnedData = sendNumericArray( fileHandle, numArray, numSentBytesAtATime ) ;
    returnedData = sendNumericArraySafe( fileHandle, numArray, numSentBytesAtATime ) ;
    
%     returnedDataNumElements         = numel( returnedData ) ;
%     find92or37                      = ( returnedData == 92 )|( returnedData == 37 ) ; % 37 = double('%') 92 = double('\')
%     returnedData                    = repmat( returnedData, 2, 1 ) ;
%     returnedData                    = string( returnedData ) ;
%     returnedData(2,~find92or37)     = "" ;
%     
%     returnedData                    = reshape( returnedData, 1, returnedDataNumElements * 2 ) ;
%     returnedData                    = double( returnedData ) ;
%     returnedData( isnan( returnedData ) )   = [] ;    
%     fprintf( fileHandle.fH, char( returnedData ) ) ;

    fwrite( fileHandle.fH, returnedData ) ;
end