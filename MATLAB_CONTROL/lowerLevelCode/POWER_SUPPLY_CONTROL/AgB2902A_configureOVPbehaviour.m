% This Function configures the behaviour of the CURRENT SOURCE when the
% output voltage attempts to go over the specified over-voltage limit(specified in AgB2902A_configureVoltageLimit.m)
% 
function AgB2902A_configureOVPbehaviour( instrumentObj, outputTarget, behaviour )
display( mfilename )
%% Configuring Supply Voltage
Output_chosen = sprintf( 'OutputChannel%d', outputTarget ) ; % Got this name from Keysight Command Expert https://onedrive.live.com/view.aspx?resid=23AEDA57AABE2E17%211224&id=documents&wd=target%28PowerSupplyPCControl.one%7C5E1DB5BA-383E-4955-8E93-61BB160A3B4B%2FKeysight%20Command%20Expert%7CF27FF340-B458-4DA6-93CB-E8B25F1A0235%2F%29
% currenLimit = currentLevel ;
instrumentObj.RepCapIdentifier = Output_chosen;
instrumentObj.Output.OVP_Enabled = behaviour;
    behString = { 'Saturate Voltage on voltage limit exceed attempt', 'Turn OFF Supply on voltage limit exceed attempt' } ; % https://onedrive.live.com/view.aspx?resid=23AEDA57AABE2E17%211224&id=documents&wd=target%28KeysightCommandExpert.one%7C74096DE1-81DE-4310-A4DA-8DDB15CC9116%2FLimit%20Crossing%20Behaviour%7C459875F1-FC7D-41F0-BC6E-F553AEAD123D%2F%29
fprintf( '%s Set to OVP behaviour: %s\n', Output_chosen, behString{ 1 + behaviour } ) ;
end