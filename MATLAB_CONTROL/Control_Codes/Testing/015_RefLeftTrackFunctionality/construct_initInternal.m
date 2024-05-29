clear;clc;load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\012_CurrentMirrorCalibration\fine_Calib_array.mat');
fine_CM_Calib(1,1:18)=fine_Calib_array(1,1:18);
fine_CM_Calib(1,27:62)=fine_Calib_array(1,19:54);
fine_CM_Calib(1,71:88)=fine_Calib_array(1,55:72);
fine_SUB_Calib=7*ones(1,88);
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\014_DLSubtractCalibration\fine_Calib_array.mat');
fine_SUB_Calib(1,1:18)=fine_Calib_array(1,1:18);
fine_SUB_Calib(1,27:62)=fine_Calib_array(1,19:54);
fine_SUB_Calib(1,71:88)=fine_Calib_array(1,55:72);
Coarse_cm=3*ones(1,88);
Coarse_sub=3*ones(1,88);

ADC_out=[2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,51,52,53,54,55,56,57,58,59,60,61,62,63,73,74,75,76,77,78,79,81,82,83,84,85,86,87,88,89]+1;
vecScanIn=ones( [ 1, 92 ] ) ;
vecScanIn(ADC_out)=0;
