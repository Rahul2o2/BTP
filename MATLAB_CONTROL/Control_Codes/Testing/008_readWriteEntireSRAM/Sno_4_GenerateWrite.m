load('dataArray.mat');
DirName='WeightSave';
for i=1:4
    if i==1
        data_DL = dataBL0;
    elseif i==2
        data_DL =dataBL1;
    elseif i==3
        data_DL = dataBL2;
    elseif i==4
        data_DL = dataBL3;
    end
    fileID = fopen(sprintf('./%s/%s%d.bin',DirName,'dW',i-1),'w');
    data_DL=data_DL';
    fwrite(fileID,data_DL);
    fclose(fileID);
end