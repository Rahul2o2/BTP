function RSC_scanOutData = decryptRSC_ScanoutData( returnedData_scanoutRSC )
%% get voltage values, copied from decrypt_and_RSC
    stopByte = 127 ; fillerByte = 124 ;
    stopByteOrFillerByteIndices = find( returnedData_scanoutRSC == stopByte | returnedData_scanoutRSC == fillerByte) ; 
    RSC_scanOutData = returnedData_scanoutRSC( ( stopByteOrFillerByteIndices( end - 1 ) + 1 ) : ( stopByteOrFillerByteIndices( end ) - 1 ) ) ;
end