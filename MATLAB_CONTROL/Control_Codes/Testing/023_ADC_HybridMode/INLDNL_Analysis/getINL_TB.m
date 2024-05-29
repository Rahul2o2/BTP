clear;clc;
%load('./ADC_predicted_Dec.mat');
%load('ADC_predicted_Dec_500MHz.mat');
load('ADC_predicted_Dec_500MHz_6BitHybrid.mat');%6-Bit Hybrid Mode Results
N=7;%7 or 6
fit_type='linear';
% fit_type='exp';
% fit_type='SS';
DL_indices=[1:18,27:62,71:88];
%DL_indices=[1];
INL_arr=[];
m=[];c=[];
%DL_indices=1;
RL_indices=[19:26,63:70];
DL_RL_indices=1:92;
for DL_num=DL_RL_indices
        voltages_to_chip=[0:0.001:0.9];
        ADC_act_bin=ADC_predicted_Dec(:,DL_num);
    [ INL, voltage_step, ADC_fit ] = getINL( ADC_act_bin, fit_type, N, voltages_to_chip ) ;
    m=[m;ADC_fit.p1];
    c=[c;ADC_fit.p2];
    close all;
    %INL_arr=[INL_arr,INL];
end
fprintf('\n');
fprintf('%f,',m);
fprintf('\n');
fprintf('%f,',c);
fprintf('\n');