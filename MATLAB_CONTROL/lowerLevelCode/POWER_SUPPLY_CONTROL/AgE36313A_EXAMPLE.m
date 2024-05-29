%% PREFACE
clear ;
clc ;
VX      = 1 ;
V5V     = 2 ;
VBIASP  = 3 ;
%% CONNECT TO AgE36313A
instrumentObj = AgE36313A_connect() ;
%% SET THE SUPPLIES TO THE REQUIRED VOLTAGE LEVELS
    outputTarget = VX ;
    voltageLevel = 0.2 ;
AgE36313A_configure( instrumentObj, outputTarget, voltageLevel ) ;
    outputTarget = V5V ;
    voltageLevel = 0.4 ;
AgE36313A_configure( instrumentObj, outputTarget, voltageLevel ) ;
    outputTarget = VBIASP ;
    voltageLevel = 0.6 ;
AgE36313A_configure( instrumentObj, outputTarget, voltageLevel ) ;
%% ENABLE THE SUPPLIES AT THE SET VOLTAGE LEVELS
    outputTarget = VX ;
AgE36313A_enable( instrumentObj, outputTarget ) ;
    outputTarget = V5V ;
AgE36313A_enable( instrumentObj, outputTarget ) ;
    outputTarget = VBIASP ;
AgE36313A_enable( instrumentObj, outputTarget ) ;
%% CHANGE SUPPLY VOLTAGES WHILE ENABLED
    outputTarget = VX ;
    voltageLevel = 0.4 ;
AgE36313A_configure( instrumentObj, outputTarget, voltageLevel ) ;
    outputTarget = V5V ;
    voltageLevel = 0.6 ;
AgE36313A_configure( instrumentObj, outputTarget, voltageLevel ) ;
    outputTarget = VBIASP ;
    voltageLevel = 0.8 ;
AgE36313A_configure( instrumentObj, outputTarget, voltageLevel ) ;
%% DISABLE THE SUPPLIES
    outputTarget = VX ;
AgE36313A_disable( instrumentObj, outputTarget ) ;
    outputTarget = V5V ;
AgE36313A_disable( instrumentObj, outputTarget ) ;
    outputTarget = VBIASP ;
AgE36313A_disable( instrumentObj, outputTarget ) ;
%% DISCONNECT FROM THE INSTRUMENT
AgE36313A_disconnect( instrumentObj ) ;
%% CLEANUP
delete( instrumentObj ) ;