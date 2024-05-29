%% Important Instructions 
% Call this function only after you have done the below 4 steps:
%1% makemid( 'AgE36xx', 'AgE36313A.mdd' ) ; % Got this from https://www.keysight.com/in/en/lib/software-detail/driver/e36xx-e36xxx-dc-power-supply-ivi-and-matlab-instrument-drivers-1669314.html
%2% INSTRU_ID = 'USB0::0x2A8D::0x1202::MY61002743::0::INSTR' ; % Got this by running keysight connection expert after connecting the power supply to the PC
%3% instrumentObj = icdevice( 'AgE36313A.mdd', INSTRU_ID ) ;
% Connect driver instance
%4% connect( instrumentObj ) ;
% This Function configures the voltage of the chosen output to the chosen
% voltage level
% OutputTarget is an integer with the following possible values
%     OutputTarget = 1 for configuring YELLOW SUPPLY
%     OutputTarget = 2 for configuring  GREEN SUPPLY
%     OutputTarget = 3 for configuring   BLUE SUPPLY
% Remember, configuring a voltage does not necessarily mean enabling the supply
function AgE36313A_2_configure( instrumentObj, outputTarget, voltageLevel )
AgE36313A_2_configureVoltage( instrumentObj, outputTarget, voltageLevel )
end