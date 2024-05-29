load('xdata.mat');
load('DL_ADCs.mat');
load('RL_ADCs.mat');

figure;
stem(xdata,DL_ADCs);
title('PREDICTED MACs by CHIP ADC DATA-LINES');
xlabel('Voltages FED IN');
ylabel('ADC Quantised Voltages obtained');
hold on;



figure;
stem(xdata,RL_ADCs);
title('PREDICTED MACs by CHIP ADC REF-DATA-LINES');
xlabel('Voltages FED IN');
ylabel('ADC Quantised Voltages obtained');
hold on;
