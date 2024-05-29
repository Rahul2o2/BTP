function returnedData = read_received_data_UART( fileHandle )
    returnedData = [] ;
    while( fileHandle.s.NumBytesAvailable < 1 )
    end
    fnOutputByte = read( fileHandle.s, 1 , 'uint8' ) ;
    returnedData = [ returnedData fnOutputByte ] ;
end