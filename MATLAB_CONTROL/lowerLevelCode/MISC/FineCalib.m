function [data_WL_1025_1084] = FineCalib(Fine_0_CM,Fine_0_SUB,Fine_1_CM,Fine_1_SUB)
%   Coarse Calibration Assignment- Note Values are treated as decimal, use
%   3 for 11, 2 for 10, 1 for 01 and 0 for 00
    %0 Indexed notation
    %1084,1086:CM_DL0
    %1085,1087:CM_DL1
    %1088,1090:SUB_DL0
    %1089,1091:SUB_DL1
    if((Fine_0_CM>15)||(Fine_0_SUB>15)||(Fine_1_CM>15)||(Fine_1_SUB>15)||(Fine_0_CM<0)||(Fine_0_SUB<0)||(Fine_1_CM<0)||(Fine_1_SUB<0))
        error('OVERFLOW ERROR'); 
    end
    
    data_WL_1025_1084=zeros(1,60);

    data_WL_1025_1084(1:2:2*(Fine_0_CM)-1)=1;

    data_WL_1025_1084(2:2:2*Fine_1_CM)=1;

    data_WL_1025_1084(31:2:2*Fine_0_SUB+29)=1;

    data_WL_1025_1084(32:2:2*Fine_1_SUB+30)=1;

end


