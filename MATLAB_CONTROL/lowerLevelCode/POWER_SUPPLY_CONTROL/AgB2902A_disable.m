% This Function turns ON the chosen output of the voltage source at whatever voltage it was configured to in AgB2902A_configureVoltage()
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring Ch 1 SUPPLY
%     OutputTarget = 2 for configuring Ch 2 SUPPLY
function AgB2902A_disable( instrumentObj, outputTarget )
display( mfilename )

Output_chosen       = sprintf( 'OutputChannel%d', outputTarget ) ;

configObj           = get(instrumentObj, 'Configuration');
invoke(configObj, 'configureoutputenabled', Output_chosen, 0 );

end