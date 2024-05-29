clear;clc;
load('ADC_predicted_Dec.mat');



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

INL_arr=[];DNL_arr=[];
for iter=1:size(ADC_predicted_Dec,2)
    ADC_x=ADC_predicted_Dec(:,iter);
    [INL,DNL]=INL_DNL_fn(7,ADC_x);
    INL_arr=[INL_arr,INL];DNL_arr=[DNL_arr,DNL];
end
DL_ADCs_INL=INL_arr(:,DL_indices+2);
figure;
stem([0,xdata],DL_ADCs_INL);
title('INL PREDICTED MACs by CHIP ADC DATA-LINES');
xlabel('Voltages FED IN');
ylabel('INL ');

DL_ADCs_DNL=DNL_arr(:,DL_indices+2);
figure;
stem([0,xdata],DL_ADCs_DNL);
title('DNL PREDICTED MACs by CHIP ADC DATA-LINES');
xlabel('Voltages FED IN');
ylabel('DNL ');


%%Reference ADCs
% figure;
% RL_ADCs=ADC_predicted_Dec(:,RL_indices+2);
% stem(xdata,RL_ADCs);
% title('PREDICTED MACs by CHIP ADC REF-DATA-LINES');
% xlabel('Voltages FED IN');
% ylabel('ADC Quantised Voltages obtained');
% hold on;
