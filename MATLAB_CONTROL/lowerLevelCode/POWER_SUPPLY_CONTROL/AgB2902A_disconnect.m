% This function closes the connection to the supply
function AgB2902A_disconnect( instrumentObj )
display( mfilename )
    disconnect( instrumentObj ) ; 
end