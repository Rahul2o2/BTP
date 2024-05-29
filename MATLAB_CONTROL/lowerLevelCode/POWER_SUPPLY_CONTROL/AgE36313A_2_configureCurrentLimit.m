% This Function configures the current limit of the chosen output to the supplied current limit
% outputTarget is an integer with the following possible values
%     outputTarget = 1 for configuring YELLOW SUPPLY
%     outputTarget = 2 for configuring  GREEN SUPPLY
%     outputTarget = 3 for configuring   BLUE SUPPLY
% Remember, configuring does not necessarily mean enabling the supply
function AgE36313A_2_configureCurrentLimit( instrumentObj, outputTarget, currentLimit )
display( mfilename )
%% Configuring Current Limit
Output_chosen = sprintf( 'Output%d', outputTarget ) ; % Got this name from Keysight Command Expert https://onedrive.live.com/view.aspx?resid=23AEDA57AABE2E17%211224&id=documents&wd=target%28PowerSupplyPCControl.one%7C5E1DB5BA-383E-4955-8E93-61BB160A3B4B%2FKeysight%20Command%20Expert%7CF27FF340-B458-4DA6-93CB-E8B25F1A0235%2F%29
% currenLimit = currentLevel ;
Configuration = get( instrumentObj, 'Configuration' ) ;
    behaviour = 0 ; % THIS MEANS THAT THE POWER SUPPLY WILL RESTRICT THE OUTPUT VOLTAGE SUCH THAT THE OUTPUT CURRENT WILL NOT EXCEED THE currentLimit Value(From Keysight Command Expert)
%     behaviour = 1 ; % THIS MEANS THAT THE POWER SUPPLY WILL DISABLE THE OUTPUT WHEN THE OUTPUT CURRENT IS >= currentLimit Value(From Keysight Command Expert)
invoke( Configuration, 'configurecurrentlimit', Output_chosen, behaviour, currentLimit );
fprintf( '%s set to: %1.3f Amps \n', Output_chosen, currentLimit ) ;
end