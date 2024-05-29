% AUTHOR INFORMATION
% NAME: Ashwin Balagopal S
% EMAIL: ee17d200@smail.iitm.ac.in
% Code to decrypt a received file
% To be called from driver-code directory: 
%     /data/ee17d200/cadence_files/verilog/generating_vector_files/10_vecfile_auto_FA/7_vecfile_auto_FA
%Added by Balaji -- SAR_Bin_Num
function [    fpgaReceivedFile_decrypted ...
            , adc_VPVN ...
            , adc_AUX00 ...
            ,    SA_SC_collect    ...
            ,    ADC_SC_collect  ...
            ,    AdderTree_SC_collect  ...
            ,    SARTIMING_OUT_SC_collect  ...
            ,    WRITE_2_SC_collect  ...
            ,    WRITE_1_SC_collect  ...
            ,    WRITE_0_SC_collect  ...
            ,    IMC_RBL_SC_collect  ...
            ,    IMC_VOut_SC_collect  ...
            ,    WWL_SC_collect  ...
            ,    FLOATADC_SC_collect  ...
            ,    SARTIMING_IN_SC_collect  ...
            ,    ControlPath_SC_collect  ...
            ,    Reset_DFF_collect  ...
            ,    SAR_Bin_Num ...
            ,    read_SRAM_collect_Cell...
            ,    perform_hybridIMC_BLIMG_FPGA_collect...
            ,    perform_internalIMC_ImgBankFPGA_collect ...
            ,    perform_hybridIMC_ADCSWEEP_FPGA_collect] = decrypt_and_RSC( fpgaReceivedFile_name )
% display( mfilename )
% addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\FPGA_COMMUNICATE")
% addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\FPGA_CONTROL")
% addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\MISC")
% addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\POWER_SUPPLY_CONTROL")
load( 'names.mat','scanChainNames');
% fpgaReceivedFile_name       = 'WRx_All_Cells_Check' ;
    [ fpgaReceivedFile_decrypted, fpgaReceivedFile_blockSplit ]  = uart_decrypt( fpgaReceivedFile_name ) ;
    SA_SC_rows   =[];
    ADC_SC_rows =[];
    AdderTree_SC_rows =[];
    SARTIMING_OUT_SC_rows =[];
    WRITE_2_SC_rows =[];
    WRITE_1_SC_rows =[];
    WRITE_0_SC_rows =[];
    IMC_RBL_SC_rows =[];
    IMC_VOut_SC_rows =[];
    WWL_SC_rows =[];
    FLOATADC_SC_rows =[];
    SARTIMING_IN_SC_rows =[];
    ControlPath_SC_rows =[];
    Reset_DFF_rows = [] ;
    read_SRAM_rows =[];
    perform_hybridIMC_BLIMG_FPGA_rows =[];
    perform_hybridIMC_ADCSWEEP_FPGA_rows =[];
    perform_internalIMC_ImgBankFPGA_rows = [];
    
    
    SA_SC_collect   =[];
    ADC_SC_collect =[];
    AdderTree_SC_collect =[];
    SARTIMING_OUT_SC_collect =[];
    WRITE_2_SC_collect =[];
    WRITE_1_SC_collect =[];
    WRITE_0_SC_collect =[];
    IMC_RBL_SC_collect =[];
    IMC_VOut_SC_collect =[];
    WWL_SC_collect =[];
    FLOATADC_SC_collect =[];
    SARTIMING_IN_SC_collect =[];
    ControlPath_SC_collect =[];
    Reset_DFF_collect = [] ;
    read_SRAM_collect = [];
    perform_hybridIMC_BLIMG_FPGA_collect=[];
    perform_hybridIMC_ADCSWEEP_FPGA_collect=[];
    perform_internalIMC_ImgBankFPGA_collect=[];

      adc_VPVN=[];
      adc_AUX00=[];
    SAR_Bin_Num                 = [] ;
%     load( './lowerLevelCode/adc_corrn.mat', 'p1_VPVN', 'p2_VPVN', 'p1_AUX00', 'p2_AUX00' ) ;
    for i = 1 : size( fpgaReceivedFile_decrypted, 1 )
        if( strcmp( 'scanOutVector', fpgaReceivedFile_decrypted{i,1} ) || strcmp( 'scanOutVector_FAST', fpgaReceivedFile_decrypted{i,1} ) )

            if( strcmp( 'SA_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                SA_SC_rows   = [ SA_SC_rows i ] ;
            elseif( strcmp( 'ADC_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                ADC_SC_rows = [ ADC_SC_rows i ] ;
            elseif( strcmp( 'AdderTree_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                AdderTree_SC_rows = [ AdderTree_SC_rows i ] ;
            elseif( strcmp( 'SARTIMING_OUT_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                SARTIMING_OUT_SC_rows = [ SARTIMING_OUT_SC_rows i ] ;
            elseif( strcmp( 'WRITE_2_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                WRITE_2_SC_rows = [ WRITE_2_SC_rows i ] ;
            elseif( strcmp( 'WRITE_1_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                WRITE_1_SC_rows = [ WRITE_1_SC_rows i ] ;
            elseif( strcmp( 'WRITE_0_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                WRITE_0_SC_rows = [ WRITE_0_SC_rows i ] ;
            elseif( strcmp( 'IMC_RBL_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                IMC_RBL_SC_rows = [ IMC_RBL_SC_rows i ] ;
            elseif( strcmp( 'IMC_VOut_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                IMC_VOut_SC_rows = [ IMC_VOut_SC_rows i ] ;
            elseif( strcmp( 'WWL_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                WWL_SC_rows = [ WWL_SC_rows i ] ;
            elseif( strcmp( 'FLOATADC_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                FLOATADC_SC_rows = [ FLOATADC_SC_rows i ] ;
            elseif( strcmp( 'SARTIMING_IN_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                SARTIMING_IN_SC_rows = [ SARTIMING_IN_SC_rows i ] ;
            elseif( strcmp( 'ControlPath_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
                ControlPath_SC_rows = [ ControlPath_SC_rows i ] ;
            else%must be the Reset_DFF
                Reset_DFF_rows = [ Reset_DFF_rows i ] ;
            end
        end
        if( strcmp( 'readSRAM', fpgaReceivedFile_decrypted{i,1} ) )
            read_SRAM_rows = [ read_SRAM_rows i ] ;
        end
        if( strcmp( 'perform_hybridIMC_BLIMG_FPGA', fpgaReceivedFile_decrypted{i,1} ) )
            perform_hybridIMC_BLIMG_FPGA_rows = [ perform_hybridIMC_BLIMG_FPGA_rows  i ] ;
        end
        if( strcmp( 'perform_hybridIMC_ADCSWEEP_FPGA', fpgaReceivedFile_decrypted{i,1} ) )
            perform_hybridIMC_ADCSWEEP_FPGA_rows = [ perform_hybridIMC_ADCSWEEP_FPGA_rows  i ] ;
        end
        if( strcmp( 'perform_internalIMC_ImgBankFPGA', fpgaReceivedFile_decrypted{i,1} ) )
            perform_internalIMC_ImgBankFPGA_rows = [ perform_internalIMC_ImgBankFPGA_rows  i ] ;
        end
        if( strcmp( 'perform_RxOnRowColSel_FPGA_2', fpgaReceivedFile_decrypted{i,1} ) )
    %             read_scanouts_collect = [ read_scanouts_collect ; fpgaReceivedFile_decrypted{i,3} ] ;
                SA_SC_rows   = [ SA_SC_rows i ] ;
        end
        if( strcmp( 'adcRead_BLs', fpgaReceivedFile_decrypted{i,1} ) || strcmp( 'adcRead', fpgaReceivedFile_decrypted{i,1} ) || strcmp( 'fastIMCOps', fpgaReceivedFile_decrypted{i,1} ) || strcmp( 'adcRead_BLsRefs_waitTime', fpgaReceivedFile_decrypted{i,1} ) )
            adc_outs_local = fpgaReceivedFile_decrypted{i,3} ;
            
            adc_outs_local = reshape( adc_outs_local, [ 2 numel(adc_outs_local)/2 ] )' ; % adc_outs_local( odd, : ) -> VPVN, adc_outs_local( even, : ) -> AUX00
            
            adc_outs_VPVN  = adc_outs_local( 1:2:end, : ) ;
            adc_outs_AUX00 = adc_outs_local( 2:2:end, : ) ;
            
%             VPVN
            adc_outs_VPVN = reshape( adc_outs_VPVN', [ numel(adc_outs_VPVN) 1 ] ) ;
            adc_outs_VPVN = fi( adc_outs_VPVN, 0, 8, 0 ) ;
            adc_outs_VPVN = permute( permute( adc_outs_VPVN.bin, [ 1 3 2 ] ) - '0', [ 1 3 2 ] ) ;
            adc_outs_VPVN = adc_outs_VPVN( :, 3 : 8 ) ; % get rid of redundant MSB:MSB-1 as only 6 bits of data per byte is useful
            adc_outs_VPVN = reshape( adc_outs_VPVN', [ 12 numel(adc_outs_VPVN)/12 ] )' ;
            power2   = ( 2.^( 11 : -1 : 0 ) )' ;
            adc_outs_VPVN = ( adc_outs_VPVN * power2 * 0.244 * 0.001 ) ; % + 8 * 0.244 ;
%             adc_outs_VPVN = adc_outs_VPVN * 0.244 ;
%             adc_outs_VPVN = ( adc_outs_VPVN * p1_VPVN ) + p2_VPVN ;
            adc_VPVN = [ adc_VPVN ; adc_outs_VPVN ] ;

%             AUX00
            adc_outs_AUX00 = reshape( adc_outs_AUX00', [ numel(adc_outs_AUX00) 1 ] ) ;
            adc_outs_AUX00 = fi( adc_outs_AUX00, 0, 8, 0 ) ;
            adc_outs_AUX00 = permute( permute( adc_outs_AUX00.bin, [ 1 3 2 ] ) - '0', [ 1 3 2 ] ) ;
            adc_outs_AUX00 = adc_outs_AUX00( :, 3 : 8 ) ; % get rid of redundant MSB:MSB-1 as only 6 bits of data per byte is useful
            adc_outs_AUX00 = reshape( adc_outs_AUX00', [ 12 numel(adc_outs_AUX00)/12 ] )' ;
            power2   = ( 2.^( 11 : -1 : 0 ) )' ;
            adc_outs_AUX00 = ( adc_outs_AUX00 * power2 * 0.244 * 0.001 ) ; % + 8 * 0.244 ;
%             adc_outs_AUX00 = adc_outs_AUX00 * 0.244 ;
%             adc_outs_AUX00 = ( adc_outs_AUX00 * p1_AUX00 ) + p2_AUX00 ;
            adc_AUX00 = [ adc_AUX00 ; adc_outs_AUX00 ] ;
        end
        %% Added By Balaji for SAR ADC Logic
        if( strcmp( 'SarADCLogic', fpgaReceivedFile_decrypted{i,1} ) )
%             SAR_Bin_Num = fpgaReceivedFile_decrypted{i,3} ;
            SAR_Bin_Num = [SAR_Bin_Num ; fpgaReceivedFile_decrypted{i,3} ];
        end
    end
    
    num_read_SRAM_rows    = numel( read_SRAM_rows ) ;
    read_SRAM_collect     = zeros( num_read_SRAM_rows, 97*1092 ) ;
    read_SRAM_collect_Cell={};
    temp=[];
    for i = 1 : num_read_SRAM_rows
        read_SRAM_collect(i,:) = fpgaReceivedFile_decrypted{ read_SRAM_rows(i), 3 } ;
        temp=reshape(read_SRAM_collect(i,:),97,1092);
        temp=permute(temp,[2 1]);
        read_SRAM_collect_Cell(i)=mat2cell(temp,1092,97);
    end
    
    num_SA_SC           = numel( SA_SC_rows ) ;
    SA_SC_collect       = zeros( num_SA_SC, 97 ) ;
    for i = 1 : num_SA_SC
        SA_SC_collect(i,:) = fpgaReceivedFile_decrypted{ SA_SC_rows(i), 3 } ;
    end
    
    %NOTE THAT 644 is used as we have hardcoded hybrid mode to perform and
    %scan out the ADC_SC only once decided by the choosen IMG and BL
    num_perform_hybridIMC_BLIMG_FPGA_SC           = numel( perform_hybridIMC_BLIMG_FPGA_rows ) ;
    perform_hybridIMC_BLIMG_FPGA_collect          = zeros( num_perform_hybridIMC_BLIMG_FPGA_SC, 644 ) ;
    for i = 1 : num_perform_hybridIMC_BLIMG_FPGA_SC
        perform_hybridIMC_BLIMG_FPGA_collect(i,:) = fpgaReceivedFile_decrypted{ perform_hybridIMC_BLIMG_FPGA_rows(i), 3 } ;
    end
    
    %NOTE THAT 644 is used as we have hardcoded hybrid mode to perform and
    %scan out the ADC_SC only once decided by the choosen IMG and BL
    num_perform_hybridIMC_ADCSWEEP_FPGA_SC           = numel( perform_hybridIMC_ADCSWEEP_FPGA_rows ) ;
    perform_hybridIMC_ADCSWEEP_FPGA_collect          = zeros( num_perform_hybridIMC_ADCSWEEP_FPGA_SC, 644 ) ;
    for i = 1 : num_perform_hybridIMC_ADCSWEEP_FPGA_SC
        perform_hybridIMC_ADCSWEEP_FPGA_collect(i,:) = fpgaReceivedFile_decrypted{ perform_hybridIMC_ADCSWEEP_FPGA_rows(i), 3 } ;
    end
    
    %NOTE THAT 644*4*4 is used as we have hardcoded internal mode to perform and
    %scan out the ADC_SC 16 times 4 BLs 4 IMGS
    num_perform_internalIMC_ImgBankFPGA_rows           = numel( perform_internalIMC_ImgBankFPGA_rows ) ;
    perform_internalIMC_ImgBankFPGA_collect          = zeros( num_perform_internalIMC_ImgBankFPGA_rows, 644*4*4 ) ;
    for i = 1 : num_perform_internalIMC_ImgBankFPGA_rows
        perform_internalIMC_ImgBankFPGA_collect(i,:) = fpgaReceivedFile_decrypted{ perform_internalIMC_ImgBankFPGA_rows(i), 3 } ;
    end
    
    
    
    num_ADC_SC           = numel( ADC_SC_rows ) ;
    ADC_SC_collect       = zeros( num_ADC_SC, 644 ) ;
    for i = 1 : num_ADC_SC
        ADC_SC_collect(i,:) = fpgaReceivedFile_decrypted{ ADC_SC_rows(i), 3 } ;
    end
    
    num_AdderTree_SC           = numel( AdderTree_SC_rows ) ;
    AdderTree_SC_collect       = zeros( num_AdderTree_SC, 176 ) ;
    for i = 1 : num_AdderTree_SC
        AdderTree_SC_collect(i,:) = fpgaReceivedFile_decrypted{ AdderTree_SC_rows(i), 3 } ;
    end
    
    num_SARTIMING_OUT_SC           = numel( SARTIMING_OUT_SC_rows ) ;
    SARTIMING_OUT_SC_collect       = zeros( num_SARTIMING_OUT_SC, 26 ) ;
    for i = 1 : num_SARTIMING_OUT_SC
        SARTIMING_OUT_SC_collect(i,:) = fpgaReceivedFile_decrypted{ SARTIMING_OUT_SC_rows(i), 3 } ;
    end
    
    num_WRITE_2_SC           = numel( WRITE_2_SC_rows ) ;
    WRITE_2_SC_collect       = zeros( num_WRITE_2_SC, 97 ) ;
    for i = 1 : num_WRITE_2_SC
        WRITE_2_SC_collect(i,:) = fpgaReceivedFile_decrypted{ WRITE_2_SC_rows(i), 3 } ;
    end
    
    num_WRITE_1_SC           = numel( WRITE_1_SC_rows ) ;
    WRITE_1_SC_collect       = zeros( num_WRITE_1_SC, 97 ) ;
    for i = 1 : num_WRITE_1_SC
        WRITE_1_SC_collect(i,:) = fpgaReceivedFile_decrypted{ WRITE_1_SC_rows(i), 3 } ;
    end
    
    num_WRITE_0_SC           = numel( WRITE_0_SC_rows ) ;
    WRITE_0_SC_collect       = zeros( num_WRITE_0_SC, 97 ) ;
    for i = 1 : num_WRITE_0_SC
        WRITE_0_SC_collect(i,:) = fpgaReceivedFile_decrypted{ WRITE_0_SC_rows(i), 3 } ;
    end
    
    num_IMC_RBL_SC           = numel( IMC_RBL_SC_rows ) ;
    IMC_RBL_SC_collect       = zeros( num_IMC_RBL_SC, 88 ) ;
    for i = 1 : num_IMC_RBL_SC
        IMC_RBL_SC_collect(i,:) = fpgaReceivedFile_decrypted{ IMC_RBL_SC_rows(i), 3 } ;
    end
    
    num_IMC_VOut_SC           = numel( IMC_VOut_SC_rows ) ;
    IMC_VOut_SC_collect       = zeros( num_IMC_VOut_SC, 88 ) ;
    for i = 1 : num_IMC_VOut_SC
        IMC_VOut_SC_collect(i,:) = fpgaReceivedFile_decrypted{ IMC_VOut_SC_rows(i), 3 } ;
    end
    
    num_WWL_SC           = numel( WWL_SC_rows ) ;
    WWL_SC_collect       = zeros( num_WWL_SC, 1160 ) ;
    for i = 1 : num_WWL_SC
        WWL_SC_collect(i,:) = fpgaReceivedFile_decrypted{ WWL_SC_rows(i), 3 } ;
    end
    
    num_FLOATADC_SC           = numel( FLOATADC_SC_rows ) ;
    FLOATADC_SC_collect       = zeros( num_FLOATADC_SC, 92 ) ;
    for i = 1 : num_FLOATADC_SC
        FLOATADC_SC_collect(i,:) = fpgaReceivedFile_decrypted{ FLOATADC_SC_rows(i), 3 } ;
    end
    
    num_SARTIMING_IN_SC           = numel( SARTIMING_IN_SC_rows ) ;
    SARTIMING_IN_SC_collect       = zeros( num_SARTIMING_IN_SC, 110 ) ;
    for i = 1 : num_SARTIMING_IN_SC
        SARTIMING_IN_SC_collect(i,:) = fpgaReceivedFile_decrypted{ SARTIMING_IN_SC_rows(i), 3 } ;
    end
    
    num_ControlPath_SC           = numel( ControlPath_SC_rows ) ;
    ControlPath_SC_collect       = zeros( num_ControlPath_SC, 17 ) ;
    for i = 1 : num_ControlPath_SC
        ControlPath_SC_collect(i,:) = fpgaReceivedFile_decrypted{ ControlPath_SC_rows(i), 3 } ;
    end
    
    num_Reset_DFF           = numel( Reset_DFF_rows ) ;
    Reset_DFF_collect       = zeros( num_Reset_DFF, 1 ) ;
    for i = 1 : num_Reset_DFF
        Reset_DFF_collect(i,:) = fpgaReceivedFile_decrypted{ Reset_DFF_rows(i), 3 } ;
    end
%     num_WRSC0_scanouts           = numel( WR_SC_0_rows ) ;
%     WRSC0_scanouts_collect       = zeros( num_WRSC0_scanouts, 150 ) ;
%     for i = 1 : num_WRSC0_scanouts
%         WRSC0_scanouts_collect(i,:) = fpgaReceivedFile_decrypted{ WR_SC_0_rows(i), 3 } ;
%     end
%     
%     num_WRSC1_scanouts           = numel( WR_SC_1_rows ) ;
%     WRSC1_scanouts_collect       = zeros( num_WRSC1_scanouts, 150 ) ;
%     for i = 1 : num_WRSC1_scanouts
%         WRSC1_scanouts_collect(i,:) = fpgaReceivedFile_decrypted{ WR_SC_1_rows(i), 3 } ;
%     end
%     
%     num_WRSC2_scanouts           = numel( WR_SC_2_rows ) ;
%     WRSC2_scanouts_collect       = zeros( num_WRSC2_scanouts, 150 ) ;
%     for i = 1 : num_WRSC2_scanouts
%         WRSC2_scanouts_collect(i,:) = fpgaReceivedFile_decrypted{ WR_SC_2_rows(i), 3 } ;
%     end
%     
%     WR_SC_scanouts.WRSC0 = WRSC0_scanouts_collect ;
%     WR_SC_scanouts.WRSC1 = WRSC1_scanouts_collect ;
%     WR_SC_scanouts.WRSC2 = WRSC2_scanouts_collect ;
%     
%     if( ~isempty( WL_SC_rows ) ) 
%         num_WLSC_scanouts           = numel( WL_SC_rows ) ;
%         WLSC_scanouts_collect       = zeros( num_WLSC_scanouts, numel( fpgaReceivedFile_decrypted{ WL_SC_rows(1), 3 } ) ) ;
%         for i = 1 : num_WLSC_scanouts
%             WLSC_scanouts_collect(i,:) = fpgaReceivedFile_decrypted{ WL_SC_rows(i), 3 } ;
%         end
%     end
end