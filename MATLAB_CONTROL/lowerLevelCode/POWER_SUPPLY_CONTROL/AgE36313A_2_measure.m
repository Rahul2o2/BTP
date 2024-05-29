% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring YELLOW SUPPLY
%     OutputTarget = 2 for configuring  GREEN SUPPLY
%     OutputTarget = 3 for configuring   BLUE SUPPLY
function measurementResult = AgE36313A_2_measure( instrumentObj, outputTarget, measureTarget )
% measureTarget = 1 means measuring the voltage
% measureTarget = 0 means measuring the current
display( mfilename )
%% Measuring
    actionObj           = get( instrumentObj, 'Action' ) ;
    Output_chosen       = sprintf( 'Output%d', outputTarget ) ;
    measurementResult   = invoke( actionObj, 'measure', Output_chosen, measureTarget );
%% Displaying feedback message with measured voltage
switch( measureTarget )
    case 1
        fprintf( '%s Voltage measured to be at %1.4f volts\n', Output_chosen, measurementResult ) ;
    case 0
        fprintf( '%s Current measured to be at %1.4f amps\n', Output_chosen, measurementResult ) ;
end
end