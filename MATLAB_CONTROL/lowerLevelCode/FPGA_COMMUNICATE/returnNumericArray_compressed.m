% Author Information
% NAME:     Ashwin Balagopal S
% EMAIL:    ee17d200@smail.iitm.ac.in
function [ numArray, numPadZeroes ] = returnNumericArray_compressed( numArray )
% display( mfilename )
%     We will print characters
%     8 bits( 1 byte ) in a character: b7 b6 b5 b4 b3 b2 b1 b0. b7 is MSB. b[7:6]=2'b 00. b[5:0] are usable for data.
%     This function assumes numArray is an array of binary elements that needs to be compressed as above
    numArrayElements    = numel( numArray ) ;
    numArray            = reshape( numArray, 1, numArrayElements ) ; % reshape into row vector
    remainder           = mod( numArrayElements, 6 ) ;
    numPadZeroes        = ( 6 - remainder ) * double( remainder > 0 ) ; % these many zeroes must be padded to complete the 8 bits of the last printed character
    numArray            = [ numArray zeros( 1, numPadZeroes ) ] ; % padding the zeroes tail-ending numArray

    numArrayElements    =    numel( numArray ) ;
    numArray            =  reshape( numArray, 6, [] )' ; % reshape to get 6 columns and numArrayElements/6 rows
    numArrayX           =     size( numArray, 1 ) ;

    numArray            = [ zeros( numArrayX, 2 ) numArray ] ; % zeros corresponding to b[7:6]
    
    numArray            = bi2de( numArray,'left-msb' ) ; 
    
    numArray            = reshape( numArray, 1, numArrayX ) ; % to return a row vector
end