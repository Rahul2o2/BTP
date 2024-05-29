% voltage - in volts
function N5173B_disable( instrumentObj )
%%
a=get( instrumentObj, 'rf' ) ;
set( a, 'Output_Enabled', 0 ) ;
end