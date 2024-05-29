function uart_encrypt( fileHandle,fnName,arguments )
% display( mfilename )
stopByte            = 127 ;
parameter_marker    = 126 ;
parameterMarkerByte = parameter_marker ;

    switch fnName
        case 'bitsSet'
                fnNameByte  = 64 ;
    %         bitsSet
    %         uart function call: 64 signals parameter_marker( 126 ) values( u8 ) stopByte( 127 )            
                signals     = reshape( arguments.signals, 1, numel( arguments.signals ) ) ;
                values      = reshape( arguments.values, 1, numel( arguments.values ) ) ;
                numArray    = values ;
                values      = returnNumericArray_compressed( numArray ) ;
                numArray    = [ fnNameByte, signals, parameter_marker, values, stopByte ] ;
                    printNumericArray( fileHandle, numArray ) ;
                    %MSN added part starts
                    % writes ( 2 + (no. of signals)) bytes
    %                 numBytes = numel(numArray);
    %                 n_124=mod((4-mod(numBytes,4)),4);
    %                 if(n_124>0)
    %                     printNumericArray( fileHandle,repmat(124,1,n_124) ) ;
    %                 end 
                    %MSN added part ends
        case 'pulse'
            fnNameByte  = 72 ;
%         authored by: Balaji V
%         uart function call: fnNameByte=abcd[0]=72 signals=abcd[1:signals_len] currentValues=abcd[signals_len+1:end-1] stopByte=abcd[end]=127
            signals       = arguments.signals ;
            currentValues = arguments.currentValues;
            numArray      = [ fnNameByte, signals, currentValues, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;

        case 'defaultState'
            fnNameByte  = 73 ;
    %       authored by: Ashwin Balagopal S
    %       uart function call: fnNameByte=abcd[0]=65 stopByte=abcd[end]=127
            numArray = [ fnNameByte, stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;

%         case 'waitFn'
%             fnNameByte      = 84;
%     %               authored by: Ashwin Balagopal
%     %				uart function call: fnNameByte=abcd[0]=84 waitTime=abcd[1] stopByte=abcd[2]=127
%     %				a=adcRead(*returnBytes,repeatAvg);
%             waitTime    = arguments.waitTime ;
%             numArray    = [ fnNameByte, waitTime, stopByte ] ;
%             printNumericArray( fileHandle, numArray ) ;
    
        case 'scanSelect'
            fnNameByte  = 67 ;
    %       authored by: Ashwin Balagopal S
    %       uart function call: fnNameByte=abcd[0]=67 scanChain=abcd[1] stopByte=abcd[2]=127
            scanChain   = arguments.scanChain ;
            numArray    = [ fnNameByte, scanChain, stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
        
         case 'scanInVector'
            fnNameByte  = 68 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=68 scanChain=abcd[1] vecScanIn=abcd[2:end-2] numPadZeroes_vecScanIn=abcd[end-1] stopByte=abcd[end]=127
            scanChain   = arguments.scanChain ;
            vecScanIn   = reshape( arguments.vecScanIn, 1, numel( arguments.vecScanIn ) ) ;
            numArray    = vecScanIn ;
            [ vecScanIn, numPaddedZeroes_vecScanIn ]    ...
                        = returnNumericArray_compressed( numArray ) ;
            numArray    = [ fnNameByte, scanChain, vecScanIn , numPaddedZeroes_vecScanIn, stopByte ] ;
            printNumericArray( fileHandle, numArray  ) ;                  
            
         case 'scanOutVector'
            fnNameByte = 69 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127        
            scanChain = arguments.scanChain ;
            numArray = [ fnNameByte, scanChain, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
        
        case 'bitLineSelect'
            fnNameByte  = 70 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=70 bitLine=abcd[1] stopByte=abcd[3]=127
            bitLine     = arguments.bitLine ;
            numArray    = [ fnNameByte, bitLine, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
        case 'scanInWriteScanChains'
            fnNameByte  = 76 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=76 data=abcd[1:N] numPadZeroes_data=abcd[N+1] mask=abcd[N+2:2N+1] numPadZeroes_mask=abcd[2N+2] stopByte=abcd[2N+3]=127
            data        = reshape( arguments.data, 1, numel( arguments.data )  ) ;
            numArray    = data ;
            [ data, numPadZeroes_data ] ...
                        = returnNumericArray_compressed( numArray );
            mask        = reshape( arguments.mask, 1, numel( arguments.mask )  ) ;
            numArray    = mask ;
            [ mask, numPadZeroes_mask ] ...
                        = returnNumericArray_compressed( numArray );
            numArray    = [ fnNameByte, data, numPadZeroes_data, mask, numPadZeroes_mask, stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
       
        case 'loadBitlinetoCell_FPGA'
            fnNameByte  = 77 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=77 bitLines=abcd[1:end-1] stopByte=abcd[end]=127
            bitLines    = arguments.bitLines ;
            numArray    = [ fnNameByte, bitLines, stopByte ] ;            
            printNumericArray( fileHandle,numArray ) ;

                
        case 'loadCelltoSACtr_FPGA'
            fnNameByte  = 78 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=78 bitLines=abcd[1:end-1] stopByte=abcd[end]=127
            bitLines    = arguments.bitLines ;
            numArray    = [ fnNameByte, bitLines, stopByte ] ;            
            printNumericArray( fileHandle,numArray ) ;
   

        case 'write_onto_dataLines'
            fnNameByte  = 80 ;
%           authored by: Srinidhi Murali
%           uart function call: fnNameByte=abcd[0]=80 data_DL=abcd[1:182](1092 bits compressed into 1092/6= 182 bytes) colSel=abcd[183:183+colSel_len-1] parameterMarkerByte=abcd[183+colSel_len]=126 DL_nums=abcd[183+colSel_len+1:183+colSel_len+1+(2*DL_nums_len)-1 stopByte=abcd[183+colSel_len+1+(2*DL_nums_len)]=127
%           ( fileHandle, colSel, data_DL, DL_nums, wlstatus, wscstatus, varsIn, startStringIn )
            data_DL     = arguments.data_DL  ;
            colSel      = arguments.colSel   ;
            DL_nums     = arguments.DL_nums  ;
            
            
            numArray        =  data_DL ; % this is always 1092 element vector
            [ data_DL, ~ ]  = returnNumericArray_compressed( numArray ) ; % ignoring numPaddedZeroes as 1092/6 = 182
            
            numArray        = DL_nums ;
            DL_nums         = twoByteSplit( numArray ) ; % returns a row vector
            
            colSel          = reshape( colSel, [ 1 numel(colSel) ] ) ; % row vector reshape
            numArray        = [ fnNameByte data_DL colSel parameterMarkerByte DL_nums stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
 
        case 'adcRead'
        fnNameByte      = 83;
%               authored by: Balaji V
%				uart function call: fnNameByte=abcd[0]=83 repeatAvg=abcd[1] stopByte=abcd[2]=127
%				a=adcRead(*returnBytes,repeatAvg);
        repeatAvg=arguments.repeatAvg;
        numArray    = [ fnNameByte, repeatAvg, stopByte ] ;
        printNumericArray( fileHandle, numArray ) ;
    
       case 'perform_RxOnRowColSel_FPGA_2'
          fnNameByte  = 90 ;
%         authored by: Ashwin Balagopal S
%         uart function call: fnNameByte=abcd[0]=90 WLnum_MSD=abcd[1] WLnum_LSD=abcd[2] abcd[3]=BL stopByte=abcd[4]=127
%         ( fileHandle, WLnum, BL, startStringIn )
            WLnum       = arguments.WLnum    ;
            BL          = arguments.BL       ;

            WLnum_MSD   = floor( WLnum/100  ) ; % eg if WLnum = 1007 then WLnum_MSD = 10
            WLnum_LSD   =   mod( WLnum, 100 ) ; % eg if WLnum = 1007 then WLnum_MSD =  7

            numArray        = [ fnNameByte WLnum_MSD WLnum_LSD BL stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
    case 'perform_WxOnRowColSel_FPGA_2'
          fnNameByte  = 91 ;
%         authored by: Ashwin Balagopal S
%         uart function call: fnNameByte=abcd[0]=76 data=abcd[1:N] numPadZeroes_data=abcd[N+1] mask=abcd[N+2:2N+1] numPadZeroes_mask=abcd[2N+2] WLnum_MSD=abcd[end-3] WLnum_LSD=abcd[end-2] abcd[end-1]=BL stopByte=abcd[end]=127
%         ( fileHandle, WLnum, BL, data, mask, stringCurr )
            WLnum       = arguments.WLnum    ;
            BL          = arguments.BL       ;
            data        = reshape( arguments.data, 1, numel( arguments.data )  ) ;
            numArray    = data ;
            [ data, numPadZeroes_data ] ...
                        = returnNumericArray_compressed( numArray );
            mask        = reshape( arguments.mask, 1, numel( arguments.mask )  ) ;
            numArray    = mask ;
            [ mask, numPadZeroes_mask ] ...
                        = returnNumericArray_compressed( numArray );
            WLnum_MSD   = floor( WLnum/100  ) ; % eg if WLnum = 1007 then WLnum_MSD = 10
            WLnum_LSD   =   mod( WLnum, 100 ) ; % eg if WLnum = 1007 then WLnum_MSD =  7
            
            numArray    = [ fnNameByte, data, numPadZeroes_data, mask, numPadZeroes_mask, WLnum_MSD, WLnum_LSD, BL, stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
            
      case 'readSRAM'
            fnNameByte  = 104 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=70 bitLine=abcd[1] stopByte=abcd[3]=127
            bitLine     = arguments.bitLine ;
            numArray    = [ fnNameByte, bitLine, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
      case 'writeSRAM'
            fnNameByte  = 105 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=80 data_DL=abcd[1:182](1092 bits compressed into 1092/6= 182 bytes) colSel=abcd[183:183+colSel_len-1] parameterMarkerByte=abcd[183+colSel_len]=126 DL_nums=abcd[183+colSel_len+1:183+colSel_len+1+(2*DL_nums_len)-1 stopByte=abcd[183+colSel_len+1+(2*DL_nums_len)]=127
%           ( fileHandle, colSel, data_DL, DL_nums, wlstatus, wscstatus, varsIn, startStringIn )
            data_DL     = arguments.data_DL  ;
            colSel      = arguments.colSel   ;
            DL_nums     = arguments.DL_nums  ;
            
            
            %numArray        =  data_DL ; % this is always 1092 X  97 element vector
            
            %[ data_DL, ~ ]  = returnNumericArray_compressed( numArray ) ; % ignoring numPaddedZeroes as 1092/6 = 182
            
            numArray        = DL_nums ;
            DL_nums         = twoByteSplit( numArray ) ; % returns a row vector
            
            colSel          = reshape( colSel, [ 1 numel(colSel) ] ) ; % row vector reshape
            numArray        = [ fnNameByte data_DL colSel parameterMarkerByte DL_nums stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
      case 'fine_coarseSRAM'
            fnNameByte  = 106 ;
    %       authored by: Balaji V
    %       uart function call: fnNameByte=abcd[0]=106 Fine_cm=abcd[1:88] Fine_sub=abcd[89:176] Coarse_cm=abcd[177:264] Coarse_sub=abcd[265:352] stopByte=abcd[353]=127
            Fine_cm   = arguments.Fine_cm ;
            Fine_sub   = arguments.Fine_sub ;
            Coarse_cm = arguments.Coarse_cm;
            Coarse_sub = arguments.Coarse_sub;
            numArray    = [ fnNameByte, Fine_cm, Fine_sub, Coarse_cm, Coarse_sub, stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
      
       case 'IMGSelect'
            fnNameByte  = 107 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=107 IMG=abcd[1] stopByte=abcd[2]=127
            bitLine     = arguments.IMG ;
            numArray    = [ fnNameByte, bitLine, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
      case 'perform_hybridIMC_BLIMG_FPGA'
            fnNameByte = 108 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=108 abcd[1]=BL abcd[2]=IMG stopByte=abcd[4]=127       
            BL  = arguments.BL ;
            IMG = arguments.IMG ;
            numArray = [ fnNameByte, BL, IMG, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
      
        case 'perform_internalIMC_ImgBankFPGA'
            fnNameByte = 109 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=108 abcd[1]=ImgBank stopByte=abcd[4]=127
            ImgBank = arguments.ImgBank ;
            numArray = [ fnNameByte, ImgBank, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
       case 'LoadwriteSRAM_SDCard'
            fnNameByte  = 110 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=70 bitLine=abcd[1] stopByte=abcd[3]=127
            WtSlice     = arguments.WtSlice ;
            numArray    = [ fnNameByte, WtSlice, stopByte ] ;
            printNumericArray_LoadwriteSRAM_SDCard( fileHandle, numArray ) ;
        
        case 'RBL_IMC_Select'
            fnNameByte  = 111 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=70 RBL=abcd[1] stopByte=abcd[3]=127
            RBL         = arguments.RBL ;
            numArray    = [ fnNameByte, RBL, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
        case 'writeSRAM_1024_FAST_NEW'
            fnNameByte  = 112 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=80 data_DL=abcd[1:182](1092 bits compressed into 1092/6= 182 bytes) colSel=abcd[183:183+colSel_len-1] parameterMarkerByte=abcd[183+colSel_len]=126 DL_nums=abcd[183+colSel_len+1:183+colSel_len+1+(2*DL_nums_len)-1 stopByte=abcd[183+colSel_len+1+(2*DL_nums_len)]=127
%           ( fileHandle, colSel, data_DL, DL_nums, wlstatus, wscstatus, varsIn, startStringIn )
            data_DL     = arguments.data_DL  ;
            colSel      = arguments.colSel   ;
            DL_nums     = arguments.DL_nums  ;
            
            
            %numArray        =  data_DL ; % this is always 1092 X  97 element vector
            
            %[ data_DL, ~ ]  = returnNumericArray_compressed( numArray ) ; % ignoring numPaddedZeroes as 1092/6 = 182
            
            numArray        = DL_nums ;
            DL_nums         = twoByteSplit( numArray ) ; % returns a row vector
            
            colSel          = reshape( colSel, [ 1 numel(colSel) ] ) ; % row vector reshape
            numArray        = [ fnNameByte data_DL colSel parameterMarkerByte DL_nums stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
            
         case 'scanInVector_uncompressed_FAST'
            fnNameByte  = 113 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=113 scanChain=abcd[1] vecScanIn=abcd[2:end-1] stopByte=abcd[end]=127
            scanChain   = arguments.scanChain ;
            vecScanIn   = reshape( arguments.vecScanIn, 1, numel( arguments.vecScanIn ) ) ;
            numArray    = [ fnNameByte, scanChain, vecScanIn , stopByte ] ;
            printNumericArray( fileHandle, numArray  ) ; 
            
        case 'scanOutVector_FAST'
            fnNameByte = 114 ;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=114 scanChain=abcd[1] stopByte=abcd[2]=127        
            scanChain = arguments.scanChain ;
            numArray = [ fnNameByte, scanChain, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
         case 'readSRAMSliced'
            fnNameByte  = 115 ;
    %       authored by: Balaji V
    %       uart function call: fnNameByte=abcd[0]=115 stopByte=abcd[end]=127
            numArray = [ fnNameByte, stopByte ] ;
            printNumericArray_readSRAMSliced( fileHandle,numArray ) ;
         case 'perform_hybridIMC_ADCSWEEP_FPGA'
            fnNameByte = 116 ;
%           authored by: Ashwin
%           uart function call: fnNameByte=abcd[0]=116 abcd[1]=BL abcd[2]=IMG stopByte=abcd[4]=127       
            BL  = arguments.BL ;
            IMG = arguments.IMG ;
            numArray = [ fnNameByte, BL, IMG, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
        case 'perform_internalIMC_TOPSW_ImgBankFPGA'
            fnNameByte = 117 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=117 abcd[1]=ImgBank stopByte=abcd[4]=127
            ImgBank = arguments.ImgBank ;
            numArray = [ fnNameByte, ImgBank, stopByte ] ;
            %printNumericArray( fileHandle, numArray ) ;
            printNumericArray_noReturn( fileHandle, numArray ) ;
            
        case 'perform_hybridIMC_TOPSW_FPGA'
            fnNameByte = 118 ;
%           authored by: Balaji V
%           uart function call: fnNameByte=abcd[0]=108 abcd[1]=BL abcd[2]=IMG stopByte=abcd[4]=127       
            BL  = arguments.BL ;
            IMG = arguments.IMG ;
            numArray = [ fnNameByte, BL, IMG, stopByte ] ;
            %printNumericArray( fileHandle, numArray ) ;
            printNumericArray_noReturn( fileHandle, numArray ) ;
            
        case 'store_images_SRAM_MultiBitX'
            fnNameByte  = 119 ;
    %       authored by: Balaji V
    %       uart function call: fnNameByte=abcd[0]=119 IMGnum=abcd[1] mode=abcd[2] X=abcd[3:end-1] stopByte=abcd[end]=127
            X   = reshape(arguments.X,[1 numel(arguments.X)]) ;
            mode   = arguments.mode ;
            IMGnum = arguments.IMGnum;
            numArray    = [ fnNameByte, IMGnum, mode, X, stopByte ] ;
            printNumericArray( fileHandle,numArray ) ;
            
        case 'set_modeInternalHybridTiming'
            fnNameByte  = 120 ;
%           authored by: Balaji V
%           //uart function call: fnNameByte=abcd[0]=120 internal_hyb_B=abcd[1] IMG_TIME_GPIO_DATA=abcd[2]<<8+abcd[3] RWL_EN_DELAY=abcd[4]; CM_EN_DELAY=abcd[5]; ADCSTART_DELAY=abcd[6]; SAMPLING_DELAY=abcd[7]; EVALUATE_DELAY=abcd[8]; FPGA_RESET_REPEAT_UB_hybrid=abcd[9]; FPGA_RESET_REPEAT_UB_internal=abcd[10]; stopByte=abcd[11]=127

            internal_hyb_B                   = arguments.internal_hyb_B  ;
            IMG_TIME_GPIO_DATA               = arguments.IMG_TIME_GPIO_DATA  ;
            RWL_EN_DELAY                     = arguments.RWL_EN_DELAY;
            CM_EN_DELAY                      = arguments.CM_EN_DELAY;
            ADCSTART_DELAY                   = arguments.ADCSTART_DELAY;
            SAMPLING_DELAY                   = arguments.SAMPLING_DELAY;
            EVALUATE_DELAY                   = arguments.EVALUATE_DELAY;
            FPGA_RESET_REPEAT_UB_hybrid      = arguments.FPGA_RESET_REPEAT_UB_hybrid;
            
            
            numArray        = IMG_TIME_GPIO_DATA ;
            IMG_TIME_GPIO_DATA         = twoByteSplit( numArray ) ; % returns a row vector
            
            numArray        = FPGA_RESET_REPEAT_UB_hybrid ;
            FPGA_RESET_REPEAT_UB_hybrid         = twoByteSplit( numArray ) ; % returns a row vector

            
            numArray = [ fnNameByte, internal_hyb_B, IMG_TIME_GPIO_DATA, RWL_EN_DELAY,CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY, EVALUATE_DELAY,FPGA_RESET_REPEAT_UB_hybrid, stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
      
            
        case 'set_ADCbitModeInt_HybB'
            fnNameByte  = 121 ;
%           authored by: Balaji V
%           //uart function call: fnNameByte=abcd[0]=121 internal_hyb_B=abcd[1];ADC_bitMode=abcd[2];  stopByte=abcd[10]=127
            internal_hyb_B                   = arguments.internal_hyb_B  ;
            ADC_bitMode                      = arguments.ADC_bitMode;
            numArray = [ fnNameByte, internal_hyb_B,ADC_bitMode,  stopByte ] ;
            printNumericArray( fileHandle, numArray ) ;
            
    end
end