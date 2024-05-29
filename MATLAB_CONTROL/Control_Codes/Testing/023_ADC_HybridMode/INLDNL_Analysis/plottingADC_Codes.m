clear;clc;
load('./ADC_predicted_Dec.mat');



IMCVOutvolt_arr=[0:0.001:0.9];
xdata=IMCVOutvolt_arr;
%DL_indices=[1:18,27:62,71:88];
DL_indices=[1];
RL_indices=[19:26,63:70];

DL_ADCs=ADC_predicted_Dec(:,DL_indices+2);
figure;
stem(xdata,DL_ADCs);
title('PREDICTED MACs by CHIP ADC DATA-LINES');
xlabel('Voltages FED IN');
ylabel('ADC Quantised BINS');
hold on;

%%Reference ADCs
% figure;
% RL_ADCs=ADC_predicted_Dec(:,RL_indices+2);
% stem(xdata,RL_ADCs);
% title('PREDICTED MACs by CHIP ADC REF-DATA-LINES');
% xlabel('Voltages FED IN');
% ylabel('ADC Quantised Voltages obtained');
% hold on;
