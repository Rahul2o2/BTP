% voltage - in volts
function N5173B_setVoltage( instrumentObj, voltage )
%% Calculating P0 from "measured" power level for 0.3V as 2.55 dB
A=0.3;
P=A.^2/2;
%P_dB = 10 * log10( P/P0 );
P0 = (P)*10.^(-2.55/10);
%% Setting the voltage
A=voltage;
P=A.^2/2;
P_dB = 10 * log10( P/P0 );
%%
a=get( instrumentObj, 'rf' ) ;
set( a, 'Power_Level', P_dB ) ;
end