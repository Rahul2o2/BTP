%% PREFACE
clear ;
clc ;
V1      = 1 ; % Corresponds to Supply labelled as "Ch 1" on the instrument
V2      = 2 ; % Corresponds to Supply labelled as "Ch 2" on the instrument
%% CONNECT TO AgB2902A
instrumentObj = AgB2902A_connect() ;
%% SET THE SUPPLIES TO THE REQUIRED VOLTAGE LEVELS
    outputTarget = V1 ;
    voltageLevel = 0.2 ;
AgB2902A_configureVoltage( instrumentObj, outputTarget, voltageLevel ) ;
    outputTarget = V2 ;
    voltageLevel = 0.4 ;
AgB2902A_configureVoltage( instrumentObj, outputTarget, voltageLevel ) ;
%% SET THE SUPPLIES TO THE REQUIRED CURRENT LIMITS
    outputTarget = V1 ;
    currentLimit = 0.1 ;
AgB2902A_configureCurrentLimit( instrumentObj, outputTarget, currentLimit ) ;
    outputTarget = V2 ;
    currentLimit = 0.5 ;
AgB2902A_configureCurrentLimit( instrumentObj, outputTarget, currentLimit ) ;
%% ENABLE THE SUPPLIES AT THE SET VOLTAGE LEVELS
    outputTarget = V1 ;
AgB2902A_enable( instrumentObj, outputTarget ) ;
    outputTarget = V2 ;
AgB2902A_enable( instrumentObj, outputTarget ) ;
%% CHANGE SUPPLY VOLTAGES WHILE ENABLED
    outputTarget = V1 ;
    voltageLevel = 0.4 ;
AgB2902A_configureVoltage( instrumentObj, outputTarget, voltageLevel ) ;
    outputTarget = V2 ;
    voltageLevel = 0.6 ;
AgB2902A_configureVoltage( instrumentObj, outputTarget, voltageLevel ) ;
%% CHANGE SUPPLY CURRENT LIMITS WHILE ENABLED
    outputTarget = V1 ;
    currentLimit = 0.3 ;
AgB2902A_configureCurrentLimit( instrumentObj, outputTarget, currentLimit ) ;
    outputTarget = V2 ;
    currentLimit = 0.7 ;
AgB2902A_configureCurrentLimit( instrumentObj, outputTarget, currentLimit ) ;
%% MEASURE VOLTAGE OF SUPPLY
    outputTarget  = V1 ;
    measureTarget = 1  ;
measurementResult = AgB2902A_measure( instrumentObj, outputTarget, measureTarget ) ;
    outputTarget  = V2 ;
    measureTarget = 1  ;
measurementResult = AgB2902A_measure( instrumentObj, outputTarget, measureTarget ) ;
%% MEASURE CURRENT OF SUPPLY
    outputTarget  = V1 ;
    measureTarget = 0  ;
measurementResult = AgB2902A_measure( instrumentObj, outputTarget, measureTarget ) ;
    outputTarget  = V2 ;
    measureTarget = 0  ;
measurementResult = AgB2902A_measure( instrumentObj, outputTarget, measureTarget ) ;
%% DISABLE THE SUPPLIES
    outputTarget = V1 ;
AgB2902A_disable( instrumentObj, outputTarget ) ;
    outputTarget = V2 ;
AgB2902A_disable( instrumentObj, outputTarget ) ;
%% DISCONNECT FROM THE INSTRUMENT
AgB2902A_disconnect( instrumentObj ) ;
%% CLEANUP
delete( instrumentObj ) ;