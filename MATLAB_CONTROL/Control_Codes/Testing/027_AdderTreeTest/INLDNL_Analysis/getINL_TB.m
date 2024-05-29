clear;clc;
%load('./ADC_predicted_Dec.mat');
load('ADC_predicted_Dec_500MHz.mat');
N=7;%7 or 6
fit_type='linear';
% fit_type='exp';
% fit_type='SS';
DL_indices=[1:18,27:62,71:88];
DL_num=1;
INL_arr=[];
%for DL_num=DL_indices
    [INL,voltage_step] = getINL( ADC_predicted_Dec, fit_type, N, DL_num ) ;
    %close all;
    %INL_arr=[INL_arr,INL];
% end