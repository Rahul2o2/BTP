clear;clc;
wtBits=8;
data_val=-1.0156;
data=fi(data_val,1,wtBits,6);
bin2deconv=2.^[1 0 -1 -2 -3 -4 -5 -6];bin2deconv(1)=-bin2deconv(1);
%IMC_RBL_ReadStacks
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v1\MATLAB_CONTROL\Control_Codes\Testing\017_ReadStackAlphaMeasurement\IMC_RBL_ReadStacks.mat');
%RefIMC_RBL_ReadStacks
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v1\MATLAB_CONTROL\Control_Codes\Testing\017_ReadStackAlphaMeasurement\RefIMC_RBL_ReadStacks.mat');
dataCorrectionEN=1;
figEN=1;
DirName='SDCard_BinFiles/dataAlphaCorrected';
[dataStoreMAT_corrected,dataWeights_corrected]=alphaCorrectedWeights(data,wtBits,bin2deconv,IMC_RBL_ReadStacks,RefIMC_RBL_ReadStacks,dataCorrectionEN,figEN,DirName);



wtBits=8;
data_val=-1.0156;
data=fi(data_val,1,wtBits,6);
bin2deconv=2.^[1 0 -1 -2 -3 -4 -5 -6];bin2deconv(1)=-bin2deconv(1);
%IMC_RBL_ReadStacks
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v1\MATLAB_CONTROL\Control_Codes\Testing\017_ReadStackAlphaMeasurement\IMC_RBL_ReadStacks.mat');
%RefIMC_RBL_ReadStacks
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v1\MATLAB_CONTROL\Control_Codes\Testing\017_ReadStackAlphaMeasurement\RefIMC_RBL_ReadStacks.mat');
dataCorrectionEN=0;
figEN=1;
DirName='SDCard_BinFiles/dataAlphaUNCorrected';
[dataStoreMAT_un_corrected,dataWeights_un_corrected]=alphaCorrectedWeights(data,wtBits,bin2deconv,IMC_RBL_ReadStacks,RefIMC_RBL_ReadStacks,dataCorrectionEN,figEN,DirName);

data_bin=data.bin;
data_bin=data_bin-'0';
save(sprintf('%s/data_bin.mat',DirName),'data_bin');