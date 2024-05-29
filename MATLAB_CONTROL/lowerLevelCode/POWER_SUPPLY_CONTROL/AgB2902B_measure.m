% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring Ch 1 SUPPLY
%     OutputTarget = 2 for configuring Ch 2 SUPPLY
function measurementResult = AgB2902B_measure( instrumentObj, outputTarget, measureTarget )
% measureTarget = 1 means measuring the voltage
% measureTarget = 0 means measuring the current
display( mfilename )
%% Measuring
    actionObj           = get( instrumentObj, 'Action' ) ;
    Output_chosen       = sprintf( 'OutputChannel%d', outputTarget ) ;
    measurementResult   = invoke( actionObj, 'measure', Output_chosen, measureTarget );
%% Displaying feedback message with measured voltage
switch( measureTarget )
    case 1
        fprintf( '%s Voltage measured to be at %1.4f volts\n', Output_chosen, measurementResult ) ;
    case 0
        fprintf( '%s Current measured to be at %1.6f milliamps\n', Output_chosen, measurementResult ) ;
end
end