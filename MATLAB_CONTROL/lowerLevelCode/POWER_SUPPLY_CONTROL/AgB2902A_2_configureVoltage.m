% This Function configures the voltage of the chosen output to the chosen voltage level
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring Ch 1 SUPPLY
%     OutputTarget = 2 for configuring Ch 2 SUPPLY
% Remember, configuring does not necessarily mean enabling the supply
function AgB2902A_2_configureVoltage( instrumentObj, outputTarget, voltageLevel )
display( mfilename )
%% Configuring Supply Voltage
    Output_chosen = sprintf( 'OutputChannel%d', outputTarget ) ; % Got this name from Keysight Command Expert https://onedrive.live.com/view.aspx?resid=23AEDA57AABE2E17%211224&id=documents&wd=target%28PowerSupplyPCControl.one%7C5E1DB5BA-383E-4955-8E93-61BB160A3B4B%2FKeysight%20Command%20Expert%7CF27FF340-B458-4DA6-93CB-E8B25F1A0235%2F%29
    Output_voltage = voltageLevel ;
    Configuration = get( instrumentObj, 'Configuration' ) ;
invoke( Configuration, 'configurevoltagelevel', Output_chosen, Output_voltage );
fprintf( '%s set to: %1.3f Volts \n', Output_chosen, Output_voltage ) ;
end