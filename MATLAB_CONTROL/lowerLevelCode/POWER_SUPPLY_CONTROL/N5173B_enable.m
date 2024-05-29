% voltage - in volts
function N5173B_enable( instrumentObj )
%%
a=get( instrumentObj, 'rf' ) ;
set( a, 'Output_Enabled', 1 ) ;
end