%%%%%%%%%%%%%%%%%%%%%%%%%
function len=scan_len( sc )
scan_len_var=[ 
     97   ... SA_SC
    ,644  ... ADC_SC
    ,176  ... AdderTree_SC
    ,26   ... SARTIMING_OUT_SC
    ,97   ... WRITE_2_SC
    ,97   ... WRITE_1_SC
    ,97   ... WRITE_0_SC
    ,88   ... IMC_RBL_SC
    ,88   ... IMC_VOut_SC
    ,1160 ... WWL_SC
    ,92   ... FLOATADC_SC
    ,110  ... SARTIMING_IN_SC
    ,17   ... ControlPath_SC
] ;
len=scan_len_var( 1 + sc ) ;
end