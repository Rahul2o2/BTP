function returnArray = twoByteSplit( numArray )
% display( mfilename )
    % splits numArray(i) into 2 bytes. At most MSB and MSB-1 in the 1st byte and the remaining 6 data bits in 2nd byte
    % 0 <= numArray(i) <= 255 for this to work
    zeroes              = @( x ) zeros( x ) ;
    numArray            = reshape( numArray, [ numel( numArray ) 1 ] ) ; % reshape to a column vector
    numArray_fi         = fi( numArray, 0, 8, 0 ) ;
    
    numArray_fi_bin     = numArray_fi.bin ;
    numArray_bin_1B     = numArray_fi_bin( :, 1 :   2 ) ;
    numArray_bin_2B     = numArray_fi_bin( :, 3 : end ) ;
    
    numArray_bin_1B     = [ repmat( '0', [ size( numArray_bin_1B, 1 ), ( 8 - size( numArray_bin_1B, 2 ) ) ] ) numArray_bin_1B ] ;
    numArray_bin_2B     = [ repmat( '0', [ size( numArray_bin_2B, 1 ), ( 8 - size( numArray_bin_2B, 2 ) ) ] ) numArray_bin_2B ] ;   
    
    numArray_bin_1B     = numArray_bin_1B -'0' ;
    numArray_bin_2B     = numArray_bin_2B -'0' ;
    
    numArray_1B         = bi2de( numArray_bin_1B, 'left-msb' ) ;
    numArray_2B         = bi2de( numArray_bin_2B, 'left-msb' ) ;
    
    numArray_1B         = reshape( numArray_1B, 1, numel( numArray_1B ) ); % reshape numArray_1B into a row vector
    numArray_2B         = reshape( numArray_2B, 1, numel( numArray_2B ) ); % reshape numArray_2B into a row vector
    
    % interleave numArray_1B and numArray_2B
    
    returnArray         = [ ...
                            numArray_1B ; ...
                            numArray_2B ; ...
                          ] ;
    returnArray         = returnArray( : )' ;
end