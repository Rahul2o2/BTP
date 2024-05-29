% AUTHOR INFORMATION
% NAME: Ashwin Balagopal S
% EMAIL: ee17d200@smail.iitm.ac.in
% Code to decompress a numArray to an array of bits
% THIS CODE IS NOT COMPLETE YET!
function bitArray = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed )
% display( mfilename )
    numArray_compressed = reshape( numArray_compressed, numel( numArray_compressed ), 1 ) ;
    numArray_compressed = fi( numArray_compressed, 0, 8, 0 ) ;
    bitArray_compressed = numArray_compressed.bin ;
    bitArray_compressed = bitArray_compressed( :, 3:end ) ; % IGNORING THE MSB, MSB-1
    bitArray_compressed = bitArray_compressed' ; % TRANSPOSING AS (:) EXPANDS ROW INDICES FIRST
    bitArray_chars      = bitArray_compressed(:) ;
    bitArray            = bitArray_chars - '0' ;
    bitArray(end-numPadZeroes_numArray_compressed+1:end)...
                        = [] ; % REMOVING PADDED ZEROES
end