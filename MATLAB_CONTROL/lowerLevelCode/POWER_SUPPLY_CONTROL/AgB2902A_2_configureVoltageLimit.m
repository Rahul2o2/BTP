% This Function configures the voltage limit of the chosen current source the chosen voltage limit
% What happens when the output voltage crosses the limit depends on the setting of AgB2902A_2_configureOVPLimitbehaviour for CURRENT SOURCE
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring Ch 1 SUPPLY
%     OutputTarget = 2 for configuring Ch 2 SUPPLY
function AgB2902A_2_configureVoltageLimit( instrumentObj, outputTarget, voltageLimit )
display( mfilename )
%% Configuring Supply Voltage
Output_chosen = sprintf( 'OutputChannel%d', outputTarget ) ; % Got this name from Keysight Command Expert https://onedrive.live.com/view.aspx?resid=23AEDA57AABE2E17%211224&id=documents&wd=target%28PowerSupplyPCControl.one%7C5E1DB5BA-383E-4955-8E93-61BB160A3B4B%2FKeysight%20Command%20Expert%7CF27FF340-B458-4DA6-93CB-E8B25F1A0235%2F%29
% currenLimit = currentLevel ;
instrumentObj.RepCapIdentifier = Output_chosen;
instrumentObj.Output.OVP_Limit = voltageLimit;
AgB2902A_configureOVPLimitbehaviour( instrumentObj, outputTarget, voltageLimit )
fprintf( '%s LIMITED to: %1.3f Volts \n', Output_chosen, voltageLimit ) ;
end