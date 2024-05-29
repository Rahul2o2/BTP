% frequency - in hertz
function N5173B_setFrequency( instrumentObj, frequency )
%%
a=get( instrumentObj, 'rf' ) ;
set( a, 'Frequency', frequency ) ;
end