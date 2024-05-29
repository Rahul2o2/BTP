% This Function configures the CURRENT of the chosen CURRENT SOURCE to the chosen CURRENT level
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring Ch 1 SUPPLY
%     OutputTarget = 2 for configuring Ch 2 SUPPLY
% Remember, configuring does not necessarily mean enabling the supply
function AgB2902A_configureCurrent_noOut( instrumentObj, outputTarget, currentLevel )
% display( mfilename )
%% Configuring Supply Voltage
Output_chosen = sprintf( 'OutputChannel%d', outputTarget ) ; % Got this name from Keysight Command Expert https://onedrive.live.com/view.aspx?resid=23AEDA57AABE2E17%211224&id=documents&wd=target%28PowerSupplyPCControl.one%7C5E1DB5BA-383E-4955-8E93-61BB160A3B4B%2FKeysight%20Command%20Expert%7CF27FF340-B458-4DA6-93CB-E8B25F1A0235%2F%29
instrumentObj.RepCapIdentifier = Output_chosen ;
instrumentObj.Instrumentspecificoutputcurrent.Level = currentLevel ;
% fprintf( '%s set to: %1.9f Amps=%1.6f milli-Amps= %1.3f micro-Amps\n', Output_chosen, currentLevel, currentLevel*1E3, currentLevel*1E6 ) ;
end