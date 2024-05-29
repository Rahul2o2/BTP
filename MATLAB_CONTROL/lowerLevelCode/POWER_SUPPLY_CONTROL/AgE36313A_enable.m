%% Important Instructions 
% Call this function only after you have called AgE36313A_configure() to
% set it to a known, SAFE, voltage level at
% This Function turns ON the chosen output of the voltage source at
% whatever voltage it was configured to in AgE36313A_configure()
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring YELLOW SUPPLY
%     OutputTarget = 2 for configuring  GREEN SUPPLY
%     OutputTarget = 3 for configuring   BLUE SUPPLY
function AgE36313A_enable( instrumentObj, outputTarget )
display( mfilename )
%% Getting the outputs from the instrument object
    Outputs = get( instrumentObj, 'Outputs' ) ;
%% Setting the channel
    set( Outputs, 'Channel', outputTarget ) ;
%% Enabling output and give it 1 s to settle
    set( Outputs, 'Enabled', true ) ;
    pause( 1 ) ;
%% Measuring output voltage of supply
    actionObj           = get( instrumentObj, 'Action' ) ;
    Output_chosen       = sprintf( 'Output%d', outputTarget ) ;
    Voltage_measured    = invoke( actionObj, 'measure', Output_chosen, 1 );
%% Displaying feedback message with measured voltage
    fprintf( '%s enabled, measured to be at %1.3f volts\n', Output_chosen, Voltage_measured ) ;
end