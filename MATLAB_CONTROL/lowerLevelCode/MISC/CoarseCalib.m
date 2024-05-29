function [data_WL_1084_1091] = CoarseCalib(coarse_0_CM,coarse_0_SUB,coarse_1_CM,coarse_1_SUB)
%   Coarse Calibration Assignment- Note Values are treated as decimal, use
%   3 for 11, 2 for 10, 1 for 01 and 0 for 00
    %0 Indexed notation
    %1084,1086:CM_DL0
    %1085,1087:CM_DL1
    %1088,1090:SUB_DL0
    %1089,1091:SUB_DL1
    
if((coarse_0_CM>3)||(coarse_1_CM>3)||(coarse_0_SUB>3)||(coarse_1_SUB>3)||(coarse_0_CM<0)||(coarse_1_CM<0)||(coarse_0_SUB<0)||(coarse_1_SUB<0))
    error('OVERFLOW ERROR'); 
end

data_WL_1084_1091=zeros(1,8);

coarse_0_CM_bin=de2bi(coarse_0_CM,2);
data_WL_1084_1091(1,1)=coarse_0_CM_bin(1);
data_WL_1084_1091(1,3)=coarse_0_CM_bin(2);


coarse_0_SUB_bin=de2bi(coarse_0_SUB,2);
data_WL_1084_1091(1,5)=coarse_0_SUB_bin(1);
data_WL_1084_1091(1,7)=coarse_0_SUB_bin(2);


coarse_1_CM_bin=de2bi(coarse_1_CM,2);
data_WL_1084_1091(1,2)=coarse_1_CM_bin(1);
data_WL_1084_1091(1,4)=coarse_1_CM_bin(2);


coarse_1_SUB_bin=de2bi(coarse_1_SUB,2);
data_WL_1084_1091(1,6)=coarse_1_SUB_bin(1);
data_WL_1084_1091(1,8)=coarse_1_SUB_bin(2);



end


