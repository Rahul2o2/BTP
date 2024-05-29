%% Important Instructions 
% Call this function only after you have connected the Voltage Source using
% USB
function AgE36313A_2_disconnect( instrumentObj )
display( mfilename )
    disconnect( instrumentObj ) ; 
end