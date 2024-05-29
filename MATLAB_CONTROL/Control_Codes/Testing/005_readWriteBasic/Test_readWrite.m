%% 1. AUTHOR INFORMATION
    % NAME: Ashwin Balagopal S
    % EMAIL: ee17d200@smail.iitm.ac.in
%% 2. DESCRIPTION
%     This Code is to test if readWrite works correctly using 0->1 pattern
%% 3 TIMER
    tic
%% 4. PREFACE
%     clc ;
    zeroes = @( x ) zeros( x ) ;
    rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA' ) ;
	rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA\lowerLevelCode' ) ;
	rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM\Character_Files\new_C_code_UART_files\LockdownResearch' ) ;
    rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM_479391\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA' ) ;
	rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM_479391\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA\lowerLevelCode' ) ;
	rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM_479391\Character_Files\new_C_code_UART_files\LockdownResearch' ) ;
    rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM_479391_FAST\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA' ) ;
	rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM_479391_FAST\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA\lowerLevelCode' ) ;
	rmpath( 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM_479391_FAST\Character_Files\new_C_code_UART_files\LockdownResearch' ) ;
    close all ;
    if( ~exist( 'globalVars', 'var' ) )
        globalVars = [] ;
    end
    eval( [ 'clearvars -except globalVars ' sprintf( '%s ', globalVars ) ] ) ;
    zeroes = @(x)zeros(x) ;
%% 5. Filehandling and Serialport Stuff
    openFile    = @( filePath, fileName )   fopen( [  filePath fileName '_receive.txt' ], 'w') ;
    closeFile   = @( fileHandle )           fclose( fileHandle ) ;
    fileName            = [ '/' mfilename ];
    filePath            = './Ashwin_authored/RW_expts/04_23_22_2_testReadWrite' ;
%     fileHandle.s            = serialport( 'COM4', 115200, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1,'FlowControl', 'none' ) ;
%     fileHandle.s            = serialport( 'COM5', 115200, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1,'FlowControl', 'none' ) ;
    fileHandle.s            = serialport( 'COM3', 115200, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1,'FlowControl', 'none' ) ;
    fileHandle.fH           = openFile( './fpgaRcvdFiles', fileName ) ;
%% 6. Defining Voltages for Different Modes of Operation
% Hold Mode
    VDDA_HOLD         = 0.9 ;
    VBIASL_HOLD       = VDDA_HOLD ; 
    MVWLL_HOLD        = 0 ;
    VPP               = VDDA_HOLD ; % This is done by shorting on hardware, on the PCB. VPP is not controlled in this script.
    VFLASH_HOLD       = VDDA_HOLD ; % DISCONNECTED for this script
    VIMCBL_HOLD       = VDDA_HOLD ; % DISCONNECTED for this script
    VBIASP_HOLD       = VDDA_HOLD ;
% ReadWrite Mode
    VDDA_RW         = 0.9 ;
    VBIASL_RW       = VDDA_RW ; 
    MVWLL_RW        = 0 ;
    VPP             = VDDA_RW ; % This is done by shorting on hardware, on the PCB. VPP is not controlled in this script.
    VFLASH_RW       = VDDA_RW ; % DISCONNECTED for this script
    VIMCBL_RW       = VDDA_RW ; % DISCONNECTED for this script
    VBIASP_RW       = VDDA_RW ;
%% Defining the On-Board MUX select signal possibilities
    VPPMUX_SELECT_VPP          = 6 ;
    VPPMUX_SELECT_VDDA         = 4 ;
    VBIASPMUX_SELECT_VBIASP    = 6 ;
    VBIASPMUX_SELECT_VDDA      = 4 ;
%% 8. Power Supply Name Mapping
% For measurement
    measure_voltage = 1 ;
    measure_current = 0 ;
% % AgB2902A
%     AgB2902A_VIMCBL = 1 ;
%         currLim_VIMCBL = 20E-3 ; % in amps
%     AgB2902A_VFLASH = 2 ;
%         currLim_VFLASH = 10E-3 ; % in amps
% % AgB2902A_2
%     AgB2902A_2_VBIASP = 1 ;
%         currLim_VBIASP = 1E-4 ; % in amps
% AgE3613A
    AgE36313A_VDD     = 1 ;
        currLim_VDD       = 300E-3 ; % in amps
    AgE36313A_VBIASL  = 2 ; % DISCONNECTED FOR THIS EXPT
        currLim_VBIASL    = 1E-3 ; % in amps
    AgE36313A_MVWLL   = 3 ;
        currLim_MVWLL     = 20E-3 ; % in amps
%% 6. Go to Global Location
%     Global = 'D:\Xilinx\Vivado_2018_Projs\Srinidhi_Laptop\PCB_TEST_FPGA_ARM\Character_Files\new_C_code_UART_files\10_vecfile_auto_FA\7_vecfile_auto_FA' ;
    cd( '../../../' );
    load( './lowerLevelCode/names.mat' ) ; clear description ;
%% 7. Generate empty stringCurr
    stringCurr          = returnEmptyStringCurr() ;
%% 8. Setting the Board Muxes to Appropriate Select Signals
        pinRequested = VBIASPMUX_SELECT_VDDA ;
    stringCurr = VBIASP_MUX_Ctrl( fileHandle, pinRequested, varsIn, stringCurr ) ;
        pinRequested = VPPMUX_SELECT_VDDA ;
    stringCurr = VPP_MUX_Ctrl( fileHandle, pinRequested, varsIn, stringCurr ) ;
%% 9. CONNECT TO Supplies
    supply_AgE36313A = AgE36313A_connect() ;
%     supply_AgB2902A =  AgB2902A_connect() ;
%     supply_AgB2902A_2 =  AgB2902A_2_connect() ;
%% 10. SET THE SUPPLIES TO THE HOLD MODE VOLTAGE LEVELS
% AgE3613A
    for iteVar = [ 
                   AgE36313A_VDD  AgE36313A_VBIASL AgE36313A_MVWLL ;
                   VDDA_HOLD      VBIASL_HOLD      MVWLL_HOLD      ; % in volts
                  ]
            outputTarget = iteVar( 1 ) ;
            voltageLevel = iteVar( 2 ) ;
        AgE36313A_configureVoltage( supply_AgE36313A, outputTarget, voltageLevel ) ;
    end
% % AgB2902A
%     for iteVar = [ 
%                    AgB2902A_VIMCBL AgB2902A_VFLASH ;
%                    VIMCBL_HOLD     VFLASH_HOLD     ; % in volts
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             voltageLevel = iteVar( 2 ) ;
%         AgB2902A_configureVoltage( supply_AgB2902A, outputTarget, voltageLevel ) ;
%     end
% % AgB2902A_2
%     for iteVar = [ 
%                    AgB2902A_2_VBIASP  ;
%                    VBIASP_HOLD      ; % in volts
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             voltageLevel = iteVar( 2 ) ;
%         AgB2902A_2_configureVoltage( supply_AgB2902A_2, outputTarget, voltageLevel ) ;
%     end
%% 11. SET THE SUPPLIES TO THE REQUIRED CURRENT LIMITS
% AgE3613A
    for iteVar = [ 
                   AgE36313A_VDD  AgE36313A_VBIASL AgE36313A_MVWLL ;
                   currLim_VDD    currLim_VBIASL   currLim_MVWLL   ;
                  ]
            outputTarget = iteVar( 1 ) ;
            currentLimit = iteVar( 2 ) ;
        AgE36313A_configureCurrentLimit( supply_AgE36313A, outputTarget, currentLimit ) ;
    end
% % AgB2902A
%     for iteVar = [ 
%                    AgB2902A_VIMCBL AgB2902A_VFLASH ;
%                    currLim_VIMCBL  currLim_VFLASH  ; % in amps
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             currentLimit = iteVar( 2 ) ;
%         AgB2902A_configureCurrentLimit( supply_AgB2902A, outputTarget, currentLimit ) ;
%     end
% % AgB2902A_2
%     for iteVar = [ 
%                    AgB2902A_2_VBIASP ;
%                    currLim_VBIASP    ; % in amps
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             currentLimit = iteVar( 2 ) ;
%         AgB2902A_2_configureCurrentLimit( supply_AgB2902A_2, outputTarget, currentLimit ) ;
%     end

%% 12. !ENABLE! THE SUPPLIES
% AgE36313A
    for supply = [ 
                   AgE36313A_VDD   AgE36313A_VBIASL AgE36313A_MVWLL ;               
                  ]
            outputTarget = supply ;
        AgE36313A_enable( supply_AgE36313A, outputTarget ) ;
    end
% % AgB2902A
%     for supply = [ 
%                    AgB2902A_VIMCBL AgB2902A_VFLASH ;
%                  ]
%             outputTarget = supply ;
%         AgB2902A_enable( supply_AgB2902A, outputTarget ) ;
%     end
% % AgB2902A_2
%     for supply = [ 
%                    AgB2902A_2_VBIASP ;
%                  ]
%             outputTarget = supply ;
%         AgB2902A_2_enable( supply_AgB2902A_2, outputTarget ) ;
%     end
%%
% ite 1
    BLs_test = [ 0 : 7 ] ;
    WLs_test = [ 0 : 1007 ] ;
%% 14. loadArray %%
%% 14.1. SET THE SUPPLIES TO THE RW MODE VOLTAGE LEVELS
% AgE3613A
    for iteVar = [ 
                   AgE36313A_VDD    AgE36313A_VBIASL    AgE36313A_MVWLL ;
                   VDDA_RW          VBIASL_RW           MVWLL_RW        ; % in volts
                  ]
            outputTarget = iteVar( 1 ) ;
            voltageLevel = iteVar( 2 ) ;
        AgE36313A_configureVoltage( supply_AgE36313A, outputTarget, voltageLevel ) ;
    end
% % AgB2902A
%     for iteVar = [ 
%                    AgB2902A_VIMCBL  AgB2902A_VFLASH ;
%                    VIMCBL_RW        VFLASH_RW       ; % in volts
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             voltageLevel = iteVar( 2 ) ;
%         AgB2902A_configureVoltage( supply_AgB2902A, outputTarget, voltageLevel ) ;
%     end
% % AgB2902A_2
%     for iteVar = [ 
%                    AgB2902A_2_VBIASP    ;
%                    VBIASP_RW            ; % in volts
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             voltageLevel = iteVar( 2 ) ;
%         AgB2902A_2_configureVoltage( supply_AgB2902A_2, outputTarget, voltageLevel ) ;
%     end
%% 14.2. For write_onto_dataLines
    wlstatus            = -1 ;
    wscstatus.data      = zeroes( [ 1 150 ] ) + ( -1 ) ;
    wscstatus.mask      = zeroes( [ 1 150 ] ) + ( -1 ) ;
    array_data          = zeroes( [ 1008 150*8 ] ) + -1 ; % read from L to R across row i(1:1008) it reads WL_(i-1)->( DL_0:149->( BL_0:7 ) ) % -1 means that data isn't initialised
%% 14.3. Now Turning ON MODE_RW with the SA_EN !DISABLED! and PRECHG !DISABLED!
        signals             = [  MODE_RW, SA_EN, PRE_CS_DR_EN ] ;
        values              = [        1,     0,            1 ] ;
    stringCurr          = bitsSet( fileHandle, signals, values, stringCurr ) ;

%% 14.4. Initialising all cells to 0 first    
        colSel          = BLs_test ;
        data_DL         = zeroes( [ 1008 1 ] ) ;
        DL_nums         = 0 : 149 ;
    [ stringCurr, wlstatus, wscstatus, array_data ] = write_onto_dataLines( fileHandle, colSel, data_DL, DL_nums, wlstatus, wscstatus, varsIn, array_data, stringCurr ) ;

%% 14.5. Now Turning OFF MODE_RW with the SA_EN !DISABLED! and PRECHG !ENABLED!
        signals             = [  MODE_RW, SA_EN, PRE_CS_DR_EN ] ;
        values              = [        0,     0,            0 ] ;
    stringCurr          = bitsSet( fileHandle, signals, values, stringCurr ) ;
%% 14. loadArray Done %%
%% 15. readOutArray %%
%% 15.1. Scanning out initial counter data
    %%%Loading Ctr Data to Rsc
            exp_counter_data= [] ;
        stringCurr  = loadCtrtoRsc( fileHandle, exp_counter_data, stringCurr) ;
    %%%Scanning out Rsc
            scanChain   = 5 ;
            vecScanOut  = exp_counter_data ;
        stringCurr  = scanOutVector( fileHandle, vecScanOut, scanChain, stringCurr) ;
%% 15.2. performing the readout
for BL = BLs_test
    for WLnum = WLs_test
        stringCurr = perform_RxOnRowColSel_FPGA_2( fileHandle, WLnum, BL, stringCurr ) ;
%             colSel = BL ;
%             rowSel = WLnum ;
%         [ stringCurr, wlstatus ] = perform_RxOnRowColSel_FPGA( fileHandle, colSel, rowSel, wlstatus, exp_counter_data, varsIn, stringCurr ) ;
    end
end
%% 15. readOutArray done %%
%% 16. loadArray %%
%% 16.1. SET THE SUPPLIES TO THE RW MODE VOLTAGE LEVELS
% AgE3613A
    for iteVar = [ 
                   AgE36313A_VDD    AgE36313A_VBIASL    AgE36313A_MVWLL ;
                   VDDA_RW          VBIASL_RW           MVWLL_RW        ; % in volts
                  ]
            outputTarget = iteVar( 1 ) ;
            voltageLevel = iteVar( 2 ) ;
        AgE36313A_configureVoltage( supply_AgE36313A, outputTarget, voltageLevel ) ;
    end
% % AgB2902A
%     for iteVar = [ 
%                    AgB2902A_VIMCBL  AgB2902A_VFLASH ;
%                    VIMCBL_RW        VFLASH_RW       ; % in volts
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             voltageLevel = iteVar( 2 ) ;
%         AgB2902A_configureVoltage( supply_AgB2902A, outputTarget, voltageLevel ) ;
%     end
% % AgB2902A_2
%     for iteVar = [ 
%                    AgB2902A_2_VBIASP    ;
%                    VBIASP_RW            ; % in volts
%                   ]
%             outputTarget = iteVar( 1 ) ;
%             voltageLevel = iteVar( 2 ) ;
%         AgB2902A_2_configureVoltage( supply_AgB2902A_2, outputTarget, voltageLevel ) ;
%     end
%% 16.2. For write_onto_dataLines
wlstatus            = -1 ;
wscstatus.data      = zeroes( [ 1 150 ] ) + ( -1 ) ;
wscstatus.mask      = zeroes( [ 1 150 ] ) + ( -1 ) ;
array_data          = zeroes( [ 1008 150*8 ] ) + -1 ; % read from L to R across row i(1:1008) it reads WL_(i-1)->( DL_0:149->( BL_0:7 ) ) % -1 means that data isn't initialised
%% 16.3. Now Turning ON MODE_RW with the SA_EN !DISABLED! and PRECHG !DISABLED!
        signals             = [  MODE_RW, SA_EN, PRE_CS_DR_EN ] ;
        values              = [        1,     0,            1 ] ;
    stringCurr          = bitsSet( fileHandle, signals, values, stringCurr ) ;

%% 16.4. Initialising all cells to 1s secondly    
        colSel          = BLs_test ;
        data_DL         = ones( [ 1008 1 ] ) ;
        DL_nums         = 0 : 149 ;
    [ stringCurr, wlstatus, wscstatus, array_data ] = write_onto_dataLines( fileHandle, colSel, data_DL, DL_nums, wlstatus, wscstatus, varsIn, array_data, stringCurr ) ;

%% 16.5. Now Turning OFF MODE_RW with the SA_EN !DISABLED! and PRECHG !ENABLED!
        signals             = [  MODE_RW, SA_EN, PRE_CS_DR_EN ] ;
        values              = [        0,     0,            0 ] ;
    stringCurr          = bitsSet( fileHandle, signals, values, stringCurr ) ;
%% 16. loadArray Done %%
%% 17. readOutArray %%
%% 17.1. performing the readout
for BL = BLs_test
    for WLnum = WLs_test
                stringCurr = perform_RxOnRowColSel_FPGA_2( fileHandle, WLnum, BL, stringCurr ) ;
%             colSel = BL ;
%             rowSel = WLnum ;
%         [ stringCurr, wlstatus ] = perform_RxOnRowColSel_FPGA( fileHandle, colSel, rowSel, wlstatus, exp_counter_data, varsIn, stringCurr ) ;
    end
end
%% 17. readOutArray done %%
%% 18. !DISABLE! THE SUPPLIES
    % AgE36313A
        for supply = [ 
                       AgE36313A_VDD  AgE36313A_VBIASL AgE36313A_MVWLL ;               
                      ]
                outputTarget = supply ;
            AgE36313A_disable( supply_AgE36313A, outputTarget ) ;
        end
%     % AgB2902A
%         for supply = [ 
%                        AgB2902A_VIMCBL AgB2902A_VFLASH ;
%                      ]
%                 outputTarget = supply ;
%             AgB2902A_disable( supply_AgB2902A, outputTarget ) ;
%         end
%     % AgB2902A_2
%         for supply = [ 
%                        AgB2902A_2_VBIASP ;
%                      ]
%                 outputTarget = supply ;
%             AgB2902A_disable( supply_AgB2902A_2, outputTarget ) ;
%         end    
%% 19. TIMER STOP
        exec_time = toc ;    
%% 20. CLEANUP
    fclose( fileHandle.fH ) ;
%     instrreset ;
    delete( supply_AgE36313A ) ; clear supply_AgE36313A ;
%     delete( supply_AgB2902A ) ; clear supply_AgB2902A ;
%     delete( supply_AgB2902A_2 ) ; clear supply_AgB2902A_2 ;
    cd( filePath ) ;
%% 21. saving data
        c = clock ;
        year    = c(1) ;
        month   = c(2) ; 
        day     = c(3) ;
        hour    = c(4) ;
        minute  = c(5) ;
        seconds = floor(c(6)) ;
        filePath = '.' ;
        description = sprintf( 'generated by %s on %s', mfilename, date ) ;
        exec_time = toc ;
%    save( [ filePath sprintf( '/%s_%s_%d_%d_%d', mfilename, date,hour,minute,seconds) ]...
    save( [ filePath sprintf( '/%s_saveData', mfilename ) ]...
                        , 'description'...
                        , 'exec_time'...
                        , 'BLs_test' ...
                        , 'WLs_test' ...
                        ) ; 