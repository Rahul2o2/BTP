% 
function signal_names = decode_signal_names( signal_numArray, names_GPIO )
% display( mfilename )
    
    signal_names = names_GPIO( signal_numArray+1 ) ;
end