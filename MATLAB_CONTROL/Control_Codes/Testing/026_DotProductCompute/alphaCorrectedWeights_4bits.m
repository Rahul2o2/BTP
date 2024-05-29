clear;clc;
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v1\MATLAB_CONTROL\Control_Codes\Testing\017_ReadStackAlphaMeasurement\IMC_RBL_ReadStacks.mat');
load('D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v1\MATLAB_CONTROL\Control_Codes\Testing\017_ReadStackAlphaMeasurement\RefIMC_RBL_ReadStacks.mat')
data=fi(1,0,4,3);


alpha_DLs=(IMC_RBL_ReadStacks)/mean(IMC_RBL_ReadStacks,'all');
weights_DL=data./alpha_DLs;
weights_DL=weights_DL';

alpha_RLs=(RefIMC_RBL_ReadStacks)/mean(RefIMC_RBL_ReadStacks,'all');
weights_RL=data./alpha_RLs;
weights_RL=weights_RL';



dataDLs=reshape(weights_DL,72,1024);dataDLs=dataDLs';
dataRLs=reshape(weights_RL,16,1024);dataRLs=dataRLs';
dataWeights=[dataDLs(:,1:18),dataRLs(:,1:8),dataDLs(:,19:54),dataRLs(:,9:16),dataDLs(:,55:72)];
dataWeights=dataWeights';
dataBL_1D=reshape(dataWeights,[],1);
dataBL_1D_bin=dataBL_1D.bin-'0';
dataWeights=reshape(dataBL_1D_bin,88,1024,4);
dataWeights=permute(dataWeights,[2 1 3]);
save('./SDCard_BinFiles_4Bit/dataWeights.mat','dataWeights');
%% Writing BIN files
dataStoreMAT=zeros(1092,97,4);
for i=1:4
    dataStore=zeros(1092,97);
    fileID = fopen(sprintf('./SDCard_BinFiles_4Bit/%s%d.bin','dW',i-1),'w');
    temp=[dataWeights(:,:,i);zeros(1092-1024,88)];
    dataStore(:,10:97)=temp;
    dataStoreMAT(:,:,i)=dataStore;
    dataStore=dataStore';
    fwrite(fileID,dataStore);
    fclose(fileID);
end
save('./SDCard_BinFiles_4Bit/dataStoreMAT.mat','dataStoreMAT');


%% Check to See if the alpha correction panned out
alpha_RLs=alpha_RLs';
weights_RL_bin=weights_RL.bin-'0';
weights_RL_bin=reshape(weights_RL_bin,16,1024,4);
alpha_RLs=reshape(alpha_RLs,16,1024);

alpha_DLs=alpha_DLs';
weights_DL_bin=weights_DL.bin-'0';
weights_DL_bin=reshape(weights_DL_bin,72,1024,4);
alpha_DLs=reshape(alpha_DLs,72,1024);

alpha_RLs=alpha_RLs';alpha_DLs=alpha_DLs';
alphas=[alpha_DLs(:,1:18),alpha_RLs(:,1:8),alpha_DLs(:,19:54),alpha_RLs(:,9:16),alpha_DLs(:,55:72)];



bin2deconv=2.^[0 -1 -2 -3];bin2deconv_3D=reshape(bin2deconv,1,1,[]);
dataWeights_DecCorrected=dataWeights.*bin2deconv_3D;
dataWeights_DecCorrected=sum(dataWeights_DecCorrected,3);

dataWeights_Dec_onChipCorrected=dataWeights_DecCorrected.*alphas;

dataWeights_Dec_onChipUnCorrected=(data.data).*alphas;


figure;
title('Histogram of Uncorrected Weights');
xlabel('Weight Spread');
ylabel('Counts');
histogram(dataWeights_Dec_onChipUnCorrected);

figure;
title('Histogram of corrected Weights');
xlabel('Weight Spread');
ylabel('Counts');
histogram(dataWeights_Dec_onChipCorrected);
