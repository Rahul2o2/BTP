%% Important Instructions 
% Call this function only after you have called AgE36313A_configure() to
% set it to a known, SAFE, voltage level at
% This Function turns OFF the chosen output of the voltage source at
% whatever voltage it was configured to in AgE36313A_configure()
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring YELLOW SUPPLY
%     OutputTarget = 2 for configuring  GREEN SUPPLY
%     OutputTarget = 3 for configuring   BLUE SUPPLY
function AgE36313A_disable( instrumentObj, outputTarget )
display( mfilename )
%% Getting the outputs from the instrument object
    Outputs = get( instrumentObj, 'Outputs' ) ;
%% Setting the channel
    set( Outputs, 'Channel', outputTarget ) ;
%% Enabling output and give it 1 ms to settle
    set( Outputs, 'Enabled', false ) ;
%% Displaying feedback message with measured voltage
    Output_chosen       = sprintf( 'Output%d', outputTarget ) ;
    fprintf( '%s disabled \n', Output_chosen ) ;
end