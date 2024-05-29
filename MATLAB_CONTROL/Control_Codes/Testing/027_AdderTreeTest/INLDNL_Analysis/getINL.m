
function [INL,voltage_step]=getINL( ADC_predicted_Dec, fit_type, N, DL_num )
voltages_to_chip=[0:0.001:0.9];
voltages=voltages_to_chip;
%DL_indices=[1:18,27:62,71:88];
DL_indices=DL_num;
RL_indices=[19:26,63:70];

actual_bins=ADC_predicted_Dec(:,DL_indices+2);

%% TO CHANGE THE ADC BIT COUNT
if( N==7)
    M=4;
end
if( N==6 )
    M=7;
end
%%
VrefH_in_mV = 900 ;
VrefL_in_mV = 0 ;
Nbits = N ;
volt_LSB = ( VrefH_in_mV - VrefL_in_mV )/(2^Nbits) * 1E-3 ;
ADC_ideal_bin=[];
for vin_in_mV = voltages_to_chip*1E3
[ ~, ~,SAR_dec_ ] = checkResidualsSAR( vin_in_mV, VrefH_in_mV, VrefL_in_mV, Nbits ) ;
ADC_ideal_bin(end+1)=SAR_dec_;
end
clearvars SAR_dec_
%%
ADC_act_bin=ADC_predicted_Dec(:,DL_indices+2)';
%%
Nbits=N;
% fit_type='linear';
% fit_type='exp';
% fit_type='SS';
ADC_fit=returnFit(Nbits,fit_type,ADC_act_bin, ADC_ideal_bin);
% [ADC_fit, ~] = createFit_ideal_7bADC(ADC_act, ADC_ideal_bin) ;
% [ADC_fit, ~] = createFit_linear_7bADC(ADC_act, ADC_ideal_bin) ;
%%
ADC_mapped_bin = ADC_map_fn(ADC_act_bin,ADC_fit) ;
%% FIND STEPS in actual data
% Definition: The curve is said to have stepped out of a bin at voltage 'v' if the
% average of the ADC_mapped_bin corresp to 'v' to 'v+v_step*(M-1)' is (bin + 1)

% M=7;%for N=6


voltage_step_=-Inf;
voltage_step=[];
for bin = 0 : (2^N-2)
    voltage_step_ = step_finder( voltages_to_chip, ADC_mapped_bin, bin, M, voltage_step_ ) ;
    voltage_step(end+1)=voltage_step_;
end
clearvars voltage_step_ M
%% STEM PLOTTING THE ADC MAPPED XFER
close all;
fh1 = figure();
fh1.WindowState = 'maximized';
hold on
    stem(voltages_to_chip,ADC_mapped_bin);
    plot(voltages_to_chip,ADC_ideal_bin,'-r');
hold off
legend('MAPPED BIN','IDEAL BIN')
title('PREDICTED MACs by CHIP ADC DATA-LINES AFTER SLOPE AND OFFSET CORRECTION');
xlabel('Voltages FED IN (V)');
ylabel('ADC Quantised BINS');
%% CALCULATING INL
voltage_step_ideal = step_finder_ideal( voltages_to_chip, ADC_ideal_bin ) ;
delta_voltage_step = voltage_step - voltage_step_ideal ;
INL = delta_voltage_step/volt_LSB;
INL_mean=mean(INL);
INL_std=std(INL,0);
%% CALCULATING THE INL SWEEP PLOT VECTORS
volt_sweep_delta = ( voltages_to_chip(2) - voltages_to_chip(1) ) / 10 ;
volt_sweep = 0 : volt_sweep_delta : max( voltages_to_chip )  ;
% EXP2
ADC_ideal_bin2=[];
for vin_in_mV = volt_sweep*1E3
[ ~, ~,SAR_dec_ ] = checkResidualsSAR( vin_in_mV, VrefH_in_mV, VrefL_in_mV, Nbits ) ;
ADC_ideal_bin2(end+1)=SAR_dec_;
end
clearvars SAR_dec_
% ACT
ADC_step_mapped_bin = step_mapped_bin( volt_sweep, voltage_step ) ;
%% PLOTTING INL SWEEP
fh2 = figure();
fh2.WindowState = 'maximized';
%  PLOT 1: INL CURVES
subplot(2,1,1);
hold on
    plot(volt_sweep,ADC_step_mapped_bin,'-b');
    plot(volt_sweep,ADC_ideal_bin2,'-r');
hold off
legend('ACTUAL Xfer','IDEAL Xfer','Location','best');
title('INL CURVES');
xlabel('Voltage (V)');
ylabel('BIN');
%  PLOT 2: INL STEM
subplot(2,1,2);
stem(voltage_step,INL);
legend('INL STEM','Location','best');
title(sprintf('INL STEM: \\mu=%.2f, \\sigma=%.2f)',INL_mean,INL_std));
xlabel('Voltage (V)');
ylabel('INL (#LSB)');
end
%% HELPER FUNCTIONS
%%
function mapped_bin = ADC_map_fn( actual_bin, ADC_fit )
    mapped_bin = min( max( 0, ADC_fit( actual_bin ) ), 127 ) ;
    mapped_bin = mapped_bin - mod( mapped_bin, 1 ) ;
end
%%
function voltage_step = step_finder( voltages_to_chip, ADC_mapped_bin, bin, M, voltage_step_ )
    local_window_mean=[];
    ADC_mapped_bin( voltages_to_chip <= voltage_step_ ) = [] ;
    voltages_to_chip( voltages_to_chip <= voltage_step_ ) = [] ;
    for n = 1 :( numel( voltages_to_chip ) - M + 1 )
        local_window_mean(end+1) = mean( ADC_mapped_bin( n : (n+M-1) ) ) ;
    end
    [ val, ind ] = min( abs( local_window_mean - (bin+1) ) ) ;
    voltage_step = voltages_to_chip( ind ) ;
    voltage_step = voltage_step(1);
    if isempty( voltage_step )
        a=1;
    end
end
%%
function voltage_step_ideal = step_finder_ideal( voltages_to_chip, ADC_ideal_bin )
    ADC_ideal_bin_delta = ADC_ideal_bin(2:end)-ADC_ideal_bin(1:(end-1));
    voltage_step_ideal_ind = find( ADC_ideal_bin_delta ) ;
    voltage_step_ideal = voltages_to_chip( voltage_step_ideal_ind ) ;
end
%%
function ADC_step_mapped_bin = step_mapped_bin( voltage_in, voltage_step )
    ADC_step_mapped_bin=[];
    for i = 1 : numel( voltage_in )
        ADC_step_mapped_bin_=find( voltage_step > voltage_in( i ) , 1 ) - 1 ;
        if isempty(ADC_step_mapped_bin_)
            ADC_step_mapped_bin_ = NaN ;
        end
        ADC_step_mapped_bin(end+1)=ADC_step_mapped_bin_(1);
    end
end