% This Function turns ON the chosen output of the voltage source at whatever voltage it was configured to in AgB2902A_2_configureVoltage()
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring Ch 1 SUPPLY
%     OutputTarget = 2 for configuring Ch 2 SUPPLY
%% ISSUE: IF I MEASURE THE OUTPUTCHANNEL2, OUTPUTCHANNEL1 GETS ENABLED
function AgB2902A_2_enable( instrumentObj, outputTarget )
display( mfilename )

Output_chosen       = sprintf( 'OutputChannel%d', outputTarget ) ;

% configObj           = get(instrumentObj, 'Configuration');
% invoke(configObj, 'configureoutputenabled', Output_chosen, 1 );

instrumentObj.RepCapIdentifier = Output_chosen;
instrumentObj.Instrumentspecificoutput.Enabled = 1;

if( strcmp( Output_chosen, 'OutputChannel1' ) )
%% Measuring output voltage of supply
    actionObj           = get( instrumentObj, 'Action' ) ;
    Voltage_measured    = invoke( actionObj, 'measure', Output_chosen, 1 );
%% Displaying feedback message with measured voltage
    fprintf( '%s enabled, measured to be at %1.3f volts\n', Output_chosen, Voltage_measured ) ;
else
%% Displaying feedback message without measured voltage
fprintf( '%s enabled\n', Output_chosen ) ;
end
end