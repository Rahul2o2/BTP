function [ decryptedData ] = decryptReturnedData( returnedData )
%% get voltage values, copied from decrypt_and_RSC
    stopByte = 127 ; fillerByte = 124 ;
    stopByteOrFillerByteIndices = find( returnedData == stopByte | returnedData == fillerByte) ; 
    decryptedData = returnedData( ( stopByteOrFillerByteIndices( end - 1 ) + 1 ) : ( stopByteOrFillerByteIndices( end ) - 1 ) ) ;
end