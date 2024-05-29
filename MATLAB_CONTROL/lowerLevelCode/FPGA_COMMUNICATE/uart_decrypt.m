% This function is meant to be called from /data/ee17d200/cadence_files/verilog/generating_vector_files/10_vecfile_auto_FA/7_vecfile_auto_FA
function [ fpgaReceivedFile_decrypted, fpgaReceivedFile_blockSplit ] = uart_decrypt( fpgaReceivedFile_name )
%     addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\FPGA_COMMUNICATE")
%     addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\FPGA_CONTROL")
%     addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\MISC")
%     addpath("D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\lowerLevelCode\POWER_SUPPLY_CONTROL")
% display( mfilename )
% fileInPath is the fpgaReceivedFile_name of the .txt file in ./fpga_receivedFiles/
fpgaReceivedFile_blockSplit = uart_blockSplit( fpgaReceivedFile_name ) ;
load( 'names.mat','names_GPIO','scanChainNames');
% fpgaReceivedFile_blockSplit is an N x 3 string array where N is the number of rows and also the total number of function calls in fpgaReceivedFile_name
%     fpgaReceivedFile_blockSplit(:,1) is the F
%     fpgaReceivedFile_blockSplit(:,2) is the A
%     fpgaReceivedFile_blockSplit(:,3) is the outputData for that (F,A)
numFAcalls                  = size( fpgaReceivedFile_blockSplit, 1) ;
fpgaReceivedFile_decrypted  = cell( numFAcalls, 3);
for i=1:numFAcalls
    eval( [ ' fnNumber = ' fpgaReceivedFile_blockSplit{i,1} ';']);
    switch fnNumber
        case ...'bitsSet'
                ...fnNameByte  = 
                64 ...;
    %         bitsSet
    %         uart function call: 64 signals parameter_marker( 126 ) values( u8 ) stopByte( 127 )            
%                 signals     = reshape( arguments.signals, 1, numel( arguments.signals ) ) ;
%                 values      = reshape( arguments.values, 1, numel( arguments.values ) ) ;
%                 numArray    = values ;
%                 values      = returnNumericArray_compressed( numArray ) ;
%                 numArray    = [ fnNameByte, signals, parameter_marker, values, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                  fnName                              = 'bitsSet' ;
                  eval( [ ' args                      = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData                = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.signals                   = decode_signal_names( args( 1:end-3 ), names_GPIO ) ;
                  
                  numArray_compressed                 = args( end-1 ) ;
                  numExtraSignals                     = mod( numel( arguments.signals ), 6 ) ;
                  numPadZeroes_numArray_compressed    = mod( 6 - numExtraSignals, 6 ) ;
                  arguments.values                    = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                                                                        
                  fpgaReceivedFile_decrypted{i,1}     = fnName ;
                  fpgaReceivedFile_decrypted{i,2}     = arguments ;
                  fpgaReceivedFile_decrypted{i,3}     = outputData(1:end-1) ;
                  clear fnName arguments outputData args

        case ...'bitsHigh'
                ...fnNameByte  = 
                65 ...;
    %         uart function call: 65 signals stopByte( 127 )
%                 signals     = arguments.signals ;
%                 numArray    = [ fnNameByte, signals, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                  fnName                              = 'bitsHigh' ;
                  eval( [ ' args                      = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                  eval( [ ' outputData                = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.signals                   = decode_signal_names( args( 1:end-1 ), names_GPIO ) ;
                  fpgaReceivedFile_decrypted{i,1}     = fnName ;
                  fpgaReceivedFile_decrypted{i,2}     = arguments ;
                  fpgaReceivedFile_decrypted{i,3}     = outputData(1:end-1) ;
                  clear fnName arguments outputData args

        case ...'bitsLow'
                ...fnNameByte  =
                66 ...;
    %         uart function call: 66 signals stopByte( 127  )
%                 signals     = arguments.signals ;
%                 numArray    = [ fnNameByte, signals, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                  fnName                              = 'bitsLow' ;
                  eval( [ ' args                      = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                  eval( [ ' outputData                = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.signals                   = decode_signal_names( args( 1:end-1 ), names_GPIO ) ;
                  fpgaReceivedFile_decrypted{i,1}     = fnName ;
                  fpgaReceivedFile_decrypted{i,2}     = arguments ;
                  fpgaReceivedFile_decrypted{i,3}     = outputData(1:end-1) ;
                  clear fnName arguments outputData args

        case ...'scanSelect'
                ...fnNameByte  = 
                67 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=67 scanChain=abcd[1] stopByte=abcd[2]=127
%                 scanChain   = arguments.scanChain ;
%                 numArray    = [ fnNameByte, scanChain, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                eval( [ ' args                      = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                eval( [ ' outputData                = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                fnName                              = 'scanSelect' ;
                arguments.scanChain = scanChainNames{ args(1)+1 } ;
%                 switch( args(1) )
%                       case 0
%                           arguments.scanChain       =  'SA_SC';
%                       case 1
%                           arguments.scanChain       =  'ADC_SC';
%                       case 2
%                           arguments.scanChain       =  'AdderTree_SC';
%                       case 3
%                           arguments.scanChain       =  'SARTIMING_OUT_SC';
%                       case 4
%                           arguments.scanChain       =  'WRITE_2_SC';
%                       case 5
%                           arguments.scanChain       =  'WRITE_1_SC';
%                       case 6
%                           arguments.scanChain       =  'WRITE_0_SC';
%                       case 7
%                           arguments.scanChain       =  'IMC_RBL_SC';
%                       case 8
%                           arguments.scanChain       =  'IMC_VOut_SC';
%                       case 9
%                           arguments.scanChain       =  'WWL_SC';
%                       case 10
%                           arguments.scanChain       =  'FLOATADC_SC';
%                       case 11
%                           arguments.scanChain       =  'SARTIMING_IN_SC';
%                       case 12
%                           arguments.scanChain       =  'ControlPath_SC';
%                       case 13
%                           arguments.scanChain       =  'Reset_DFF';
%                 end                
                fpgaReceivedFile_decrypted{i,1}     = fnName ;
                fpgaReceivedFile_decrypted{i,2}     = arguments ;
                fpgaReceivedFile_decrypted{i,3}     = outputData(1:end-1) ;
                clear fnName arguments outputData args

        case ...'scanInVector'
                ...fnNameByte  = 
                68 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=68 scanChain=abcd[1] vecScanIn=abcd[2:end-2] numPadZeroes_vecScanIn=abcd[end-1] stopByte=abcd[end]=127
%                 scanChain   = arguments.scanChain ;
%                 vecScanIn   = reshape( arguments.vecScanIn, 1, numel( arguments.vecScanIn ) ) ;
%                 numArray    = vecScanIn ;
%                 [ vecScanIn, numPaddedZeroes_vecScanIn ]    ...
%                             = returnNumericArray_compressed( numArray ) ;
%                 numArray    = [ fnNameByte, scanChain, vecScanIn , numPaddedZeroes_vecScanIn, stopByte ] ;
%                     printNumericArray( fileHandle, numArray  ) ;  
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'scanInVector' ;
                  arguments.scanChain = scanChainNames{ args(1)+1 } ;
%                   switch( args(1) )
%                       case 1
%                           arguments.scanChain       =  'WL_SC';
%                       case 2
%                           arguments.scanChain       =  'WR_SC_0';
%                       case 3
%                           arguments.scanChain       =  'WR_SC_1';
%                       case 4
%                           arguments.scanChain       =  'WR_SC_2';
%                       case 5
%                           arguments.scanChain       =  'R_SC';
%                   end

                  numArray_compressed               = args( 2:end-2 ) ;
                  numPadZeroes_numArray_compressed  = args( end-1 ) ;
                  arguments.vecScanIn               = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
                  clear scanOut arguments outputData fnDecrypt


%             case 'scanInAnyVec_OutputDontCare' % same as scanInVector
%                 fnNameByte  = 68 ;
%     %         authored by: Ashwin Balagopal S
%     %         uart function call: fnNameByte=abcd[0]=68 scanChain=abcd[1] vecScanIn=abcd[2:end-2] numPadZeroes_vecScanIn=abcd[end-1] stopByte=abcd[end]=127
%                 scanChain   = arguments.scanChain ;
%                 vecScanIn   = reshape( arguments.vecScanIn, 1, numel( arguments.vecScanIn ) ) ;
%                 numArray    = vecScanIn ;
%                 [ vecScanIn, numPaddedZeroes_vecScanIn ]    ...
%                             = returnNumericArray_compressed( numArray ) ;
%                 numArray    = [ fnNameByte, scanChain, vecScanIn , numPaddedZeroes_vecScanIn, stopByte ] ;
%                     printNumericArray( fileHandle, numArray  ) ;


            case ...'scanOutVector'
                ...fnNameByte = 
                69 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127        
%                 scanChain = arguments.scanChain ;
%                 numArray = [ fnNameByte, scanChain, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'scanOutVector' ;
%                   switch( args(1) )
%                       case 1
%                           arguments.scanChain       =  'WL_SC';
%                       case 2
%                           arguments.scanChain       =  'WR_SC_0';
%                       case 3
%                           arguments.scanChain       =  'WR_SC_1';
%                       case 4
%                           arguments.scanChain       =  'WR_SC_2';
%                       case 5
%                           arguments.scanChain       =  'R_SC';
%                   end
                  arguments.scanChain = scanChainNames{ args(1)+1 } ;
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
                  clear scanOut arguments outputData fnDecrypt

            case ...'bitLineSelect'
                ...fnNameByte  = 
                70 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=70 bitLine=abcd[1] stopByte=abcd[3]=127
%                 bitLine     = arguments.bitLine ;
%                 numArray    = [ fnNameByte, bitLine, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                  fnName                                = 'bitLineSelect' ;
                  eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                  eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.bitLine                     = args( end-1 ) ; 
                  fpgaReceivedFile_decrypted{i,1}       = fnName ;
                  fpgaReceivedFile_decrypted{i,2}       = arguments ;
                  fpgaReceivedFile_decrypted{i,3}       = outputData(1:end-1) ;
                  clear fnName arguments outputData args


            case ...'loadCtrToRsc'
                ...fnNameByte  = 
                71 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=71 stopByte=abcd[1]=127
%                 numArray    = [ fnNameByte, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                  fnName                                = 'loadCtrToRsc' ;
                  eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                  eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments                             = [] ; 
                  fpgaReceivedFile_decrypted{i,1}       = fnName ;
                  fpgaReceivedFile_decrypted{i,2}       = arguments ;
                  fpgaReceivedFile_decrypted{i,3}       = outputData(1:end-1) ;
                  clear fnName arguments outputData args

            case ...'pulse'
                ...fnNameByte  = 
                72 ...;
    %         authored by: Balaji V, Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=72 signals=abcd[1:signals_len] currentValues=abcd[signals_len+1:end-1] stopByte=abcd[end]=127
%                 signals     = arguments.signals ;
%                 numArray    = [ fnNameByte, signals, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                  fnName                                = 'pulse' ;
                  eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                  eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.signals                     = decode_signal_names( args( 1:(end-1)/2 ), names_GPIO ) ;
                  arguments.currentValues               = args( (end-1)/2+1:end-1 ) ;
                  fpgaReceivedFile_decrypted{i,1}       = fnName ;
                  fpgaReceivedFile_decrypted{i,2}       = arguments ;
                  fpgaReceivedFile_decrypted{i,3}       = outputData( 1:end-1 ) ;
                  clear fnName arguments outputData args

            case ...'defaultState'
                ...fnNameByte  = 
                73 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=73 stopByte=abcd[end]=127
%                 numArray = [ fnNameByte, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                 fnName                                = 'defaultState' ;
                 eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                 eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                 arguments                             = [] ; 
                 fpgaReceivedFile_decrypted{i,1}       = fnName ;
                 fpgaReceivedFile_decrypted{i,2}       = arguments ;
                 fpgaReceivedFile_decrypted{i,3}       = outputData( 1:end-1 ) ;
                 clear fnName arguments outputData args

            case ...'scanInWriteScanChains_dataOnly'
                ...fnNameByte  = 
                74 ...;
%                 authored by: Ashwin Balagopal S
%                 uart function call: fnNameByte=abcd[0]=74 data=abcd[1:end-2]  numPadZeroes_data=abcd[end-1] stopByte=abcd[end]=127
%                 data        = reshape( arguments.data, 1, numel( arguments.data )  ) ;
%                 numArray    = data ;
%                 [ data, numPadZeroes_data ] ...
%                             = returnNumericArray_compressed( numArray );
%                 numArray    = [ fnNameByte, data, numPadZeroes_data, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                fnName                                = 'scanInWriteScanChains_dataOnly' ;
                eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                numArray_compressed                   = args( 1 : end-2 ) ;
                numPadZeroes_numArray_compressed      = args( end-1 ) ;
                arguments.data                        = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                arguments.numPadZeroes_data           = args( end-1 ) ;
                fpgaReceivedFile_decrypted{i,1}       = fnName ;
                fpgaReceivedFile_decrypted{i,2}       = arguments ;
                fpgaReceivedFile_decrypted{i,3}       = outputData( 1 : end-1 ) ;
                clear fnName arguments outputData args


            case ...'scanInWriteScanChains_maskOnly'
                ...fnNameByte  = 
                75 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=75 mask=abcd[1:end-2] numPadZeroes_mask=abcd[end-1] stopByte=abcd[end]=127
%                 mask        = reshape( arguments.mask, 1, numel( arguments.mask )  ) ;
%                 numArray    = mask ;
%                 [ mask, numPadZeroes_mask ] ...
%                             = returnNumericArray_compressed( numArray );
%                 numArray    = [ fnNameByte, mask, numPadZeroes_mask, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                fnName                                = 'scanInWriteScanChains_maskOnly' ;
                eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                numArray_compressed                   = args( 1 : end-2 ) ;
                numPadZeroes_numArray_compressed      = args( end-1 ) ;
                arguments.mask                        = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                fpgaReceivedFile_decrypted{i,1}       = fnName ;
                fpgaReceivedFile_decrypted{i,2}       = arguments ;
                fpgaReceivedFile_decrypted{i,3}       = outputData( 1 : end-1 ) ;
                clear fnName arguments outputData args

            case ...'scanInWriteScanChains'
                ...fnNameByte  = 
                76 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=76 data=abcd[1:N] numPadZeroes_data=abcd[N+1] mask=abcd[N+2:2N+1] numPadZeroes_mask=abcd[2N+2] stopByte=abcd[2N+3]=127
%                 data        = reshape( arguments.data, 1, numel( arguments.data )  ) ;
%                 numArray    = data ;
%                 [ data, numPadZeroes_data ] ...
%                             = returnNumericArray_compressed( numArray );
%                 mask        = reshape( arguments.mask, 1, numel( arguments.mask )  ) ;
%                 numArray    = mask ;
%                 [ mask, numPadZeroes_mask ] ...
%                             = returnNumericArray_compressed( numArray );
%                 numArray    = [ fnNameByte, data, numPadZeroes_data, mask, numPadZeroes_mask, stopByte ] ;
%                     printNumericArray( fileHandle,numArray ) ;
                fnName                                = 'scanInWriteScanChains' ;
                eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                N                                     = (numel(args) - 3 ) / 2 ;
                
                numArray_compressed                   = args( 1:N ) ;
                numPadZeroes_numArray_compressed      = args( N+1 ) ;                
                arguments.data                        = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                numArray_compressed                   = args( N+2:(2*N+1) ) ;
                numPadZeroes_numArray_compressed      = args( 2*N+2 ) ;
                arguments.mask                        = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                fpgaReceivedFile_decrypted{i,1}       = fnName ;
                fpgaReceivedFile_decrypted{i,2}       = arguments ;
                fpgaReceivedFile_decrypted{i,3}       = outputData(1:end-1) ;
                clear fnName arguments outputData args N


            case ...'loadBitlinetoCell_FPGA'
                ...fnNameByte  = 
                77 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=77 bitLines=abcd[1:end-1] stopByte=abcd[end]=127
%                 bitLines    = arguments.bitLines ;
%                 numArray    = [ fnNameByte, bitLines, stopByte ] ;            
%                     printNumericArray( fileHandle,numArray ) ;
                fnName                                = 'loadBitlinetoCell_FPGA' ;
                eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                arguments.bitLines                    =  args(1:end-1) ; 
                fpgaReceivedFile_decrypted{i,1}       = fnName ;
                fpgaReceivedFile_decrypted{i,2}       = arguments ;
                fpgaReceivedFile_decrypted{i,3}       = outputData(1:end-1) ;
                clear fnName arguments outputData args


            case ...'loadCelltoSACtr_FPGA'
                ...fnNameByte  = 
                78 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=78 bitLines=abcd[1:end-1] stopByte=abcd[end]=127
%                 bitLines    = arguments.bitLines ;
%                 numArray    = [ fnNameByte, bitLines, stopByte ] ;            
%                     printNumericArray( fileHandle,numArray ) ;
                fnName                                = 'loadCelltoSACtr_FPGA' ;
                eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                arguments.bitLines                    =  args(1:end-1) ; 
                fpgaReceivedFile_decrypted{i,1}       = fnName ;
                fpgaReceivedFile_decrypted{i,2}       = arguments ;
                fpgaReceivedFile_decrypted{i,3}       = outputData(1:end-1) ;
                clear fnName arguments outputData args


            case ...'scanInOutVector'
                ...fnNameByte  = 
                79 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=79 scanChain=abcd[1] vecScanIn=abcd[2:end-2] numPaddedZeroes_vecScanIn=abcd[end-1] stopByte=abcd[end]=127
%                 scanChain   = arguments.scanChain ;
%                 vecScanIn   = arguments.vecScanIn ;
%                 numArray    =  vecScanIn ;
%                 [ vecScanIn, numPaddedZeroes_vecScanIn ]    ...
%                             = returnNumericArray_compressed( numArray ) ;
%                 numArray    = [ fnNameByte, scanChain, vecScanIn, numPaddedZeroes_vecScanIn, stopByte ] ;            
%                     printNumericArray( fileHandle,numArray ) ;
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'scanInOutVector' ;
                  switch( args(1) )
                      case 1
                          arguments.scanChain       =  'WL_SC';
                      case 2
                          arguments.scanChain       =  'WR_SC_0';
                      case 3
                          arguments.scanChain       =  'WR_SC_1';
                      case 4
                          arguments.scanChain       =  'WR_SC_2';
                      case 5
                          arguments.scanChain       =  'R_SC';
                  end
                  numArray_compressed               = args( 2:end-2 ) ;
                  numPadZeroes_numArray_compressed  = args( end-1 ) ;
                  arguments.vecScanIn               = decompress_numArray( numArray_compressed, numPadZeroes_numArray_compressed ) ;
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
                  clear scanOut arguments outputData fnDecrypt
                  
              case ...'adcRead'
                ...fnNameByte  = 
                    83 ...;
%               authored by: Balaji V
%				uart function call: fnNameByte=abcd[0]=69 repeatAvg=abcd[1] stopByte=abcd[2]=127
%				a=adcRead(*returnBytes,repeatAvg);
%         repeatAvg=arguments.repeatAvg;
%         numArray    = [ fnNameByte, repeatAvg, stopByte ] ;
%         printNumericArray( fileHandle, numArray ) ;
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'adcRead' ;
%                   arguments                         = args ;
                  arguments.repeatAvg               = args( 1 ) ; % end is 127
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
                  %% Added by Balaji on 07/10/21
              case ...'SarADCLogic'
                ...fnNameByte  = 
                    87 ...;
%               authored by: Balaji V
%         //uart function call: fnNameByte=abcd[0]=87 refSelStart=abcd[1] nBit = abcd[2] repeatAvg = abcd[3] stopByte=abcd[4]=127
%        refSelStart=arguments.refSelStart;
%        nBit=arguments.nBit;
%        repeatAvg=arguments.repeatAvg;
%        numArray    = [ fnNameByte, refSelStart, nBit, repeatAvg, stopByte ] ;
%        printNumericArray( fileHandle, numArray ) ;
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'SarADCLogic' ;
%                   arguments                         = args ;
                  arguments.refSelStart               = args( 1 ) ; % end is 127
                  arguments.nBit                      = args( 2 ) ; % end is 127
                  arguments.repeatAvg                 = args( 3 ) ; % end is 127
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
                  
              case ...'scanInWLSC_toSelectWLnum'
                    ...fnNameByte  = 
                    88 ...;
%         authored by: Ashwin Balagopal S
%         uart function call: fnNameByte=abcd[0]=88 WLnum_MSD=abcd[1] WLnum_LSD=abcd[2] stopByte=abcd[3]=127
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'SarADCLogic' ;
%                   arguments                         = args ;
                  arguments.WLnum_MSD                 = args( 1 ) ; 
                  arguments.WLnum_LSD                 = args( 2 ) ; 
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
            case ...'scanInRSC_toSelectDLnum'
                  ...fnNameByte  = 
                  89 ...;
%         authored by: Ashwin Balagopal S
%         uart function call: fnNameByte=abcd[0]=88 DLnum=abcd[1] stopByte=abcd[2]=127
%         ( fileHandle, DLnum, startStringIn )
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'scanInRSC_toSelectDLnum' ;
%                   arguments                         = args ;
                  arguments.DLnum                 = args( 1 ) ; 
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
            case ...'perform_RxOnRowColSel_FPGA_2'
                  ...fnNameByte  = 
                  90 ...;
%         authored by: Ashwin Balagopal S
%         uart function call: fnNameByte=abcd[0]=90 WLnum_MSD=abcd[1] WLnum_LSD=abcd[2] abcd[3]=BL stopByte=abcd[4]=127
%         ( fileHandle, WLnum, BL, startStringIn )
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'perform_RxOnRowColSel_FPGA_2' ;
%                   arguments                         = args ;
                  arguments.WLnum_MSD                 = args( 1 ) ; 
                  arguments.WLnum_LSD                 = args( 2 ) ; 
                  arguments.BL                        = args( 3 ) ; 
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
          case ...'perform_WxOnRowColSel_FPGA_2'
                ...fnNameByte  = 
                91 ...;
%         authored by: Ashwin Balagopal S
%         uart function call: fnNameByte=abcd[0]=76 data=abcd[1:N] numPadZeroes_data=abcd[N+1] mask=abcd[N+2:2N+1] numPadZeroes_mask=abcd[2N+2] WLnum_MSD=abcd[end-3] WLnum_LSD=abcd[end-2] abcd[end-1]=BL stopByte=abcd[end]=127
%         ( fileHandle, WLnum, BL, data, mask, stringCurr )
          eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
          eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
          fnName                            = 'perform_WxOnRowColSel_FPGA_2' ;
            arguments.WLnum    = args(1);
            %% below is incomplete look at scanInWriteScanChains and complete later
%             arguments.BL       = args(2);
%             arguments.data     = args(3+(0:149));
%             arguments.numPadZeroes_data = args(3+149+1);
%             arguments.mask     = args(3+149+2+(0:149));
%             arguments.numPadZeroes_data = args(3+149+2+149+1);
%             arguments.WLnum_MSD   = args(3+149+2+149+1+1);
%             arguments.WLnum_LSD   = args(3+149+2+149+1+1+1);
%             arguments.BL   = args(3+149+2+149+1+1+1);  
            %% above is incomplete
              fpgaReceivedFile_decrypted{i,1}   = fnName ;
              fpgaReceivedFile_decrypted{i,2}   = arguments ;
              fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
              clear scanOut arguments outputData fnDecrypt
			  
		  case ...'fastIMCOps'
				...fnNameByte      = 
				92 ...;
%               authored by: Ashwin Balagopal S
%				uart function call: fnNameByte=abcd[0]=92 bitLine=abcd[1] ref=abcd[2] numComparisions=abcd[3] repeatAvg=abcd[4]=127 stopByte=abcd[5]=127
%				( fileHandle, bitLine, ref, numComparisions, repeatAvg, stringCurr )
          eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
          eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
          fnName                            = 'fastIMCOps' ;
			arguments.bitLine = args(1);
            arguments.ref = args(2) ;
            arguments.numComparisions = args(3);
			arguments.repeatAvg= args(4);
            
			fpgaReceivedFile_decrypted{i,1}   = fnName ;
			fpgaReceivedFile_decrypted{i,2}   = arguments ;
			fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
			clear scanOut arguments outputData fnDecrypt
        case ...'adcRead_BLs'
                ...fnNameByte  = 
                    94 ...;
%               authored by: Ashwin Balagopal S
%				uart function call: fnNameByte=abcd[0]=94 repeatAvg=abcd[1] bitLines=abcd[2+(0:N-1)] stopByte=abcd[N+1]=127
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'adcRead_BLs' ;
%                   arguments                         = args ;
                  arguments.repeatAvg               = args( 1 ) ; % end is 127
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
      case ...'scanInRSC_toSelectDLnums'
              ...fnNameByte  = 
                 101 ...;
%               authored by: Ashwin Balagopal S
%               uart function call: fnNameByte=abcd[0]=101 DLnums=abcd[1:end-1] stopByte=abcd[end]=127
%               ( fileHandle, DLnum, startStringIn )
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'scanInRSC_toSelectDLnums' ;
                  arguments.DLnums                  = args( 1 ) ; % end is 127
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
      case ...'adcRead_BLsRefs_waitTime'
                ...fnNameByte  = 
                    103 ...;
%               authored by: Ashwin Balagopal S
%				uart function call: fnNameByte=abcd[0]=103 repeatAvg=abcd[1] bitLines=abcd[2+(0:N1-1)] parameter_marker=abcd[2+N1]=126 refs=abcd[2+N1+1+(0:N2)] parameter_marker=abcd[2+N1+1+(0:N2)+1]=126 waitTime=abcd[2+N1+1+(0:N2)+1+1] stopByte=abcd[N+1]=127
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  fnName                            = 'adcRead_BLsRefs_waitTime' ;
%                   arguments                         = args ;
                  arguments.repeatAvg               = args( 1 ) ; % end is 127
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;% end is 127
                  clear scanOut arguments outputData fnDecrypt
                  
       case ...'readSRAM'
                ...fnNameByte = 
                104 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127        
%                 scanChain = arguments.scanChain ;
%                 numArray = [ fnNameByte, scanChain, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                  eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
                  eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.bitLine                     = args( end-1 ) ; 
                  fnName                            = 'readSRAM' ;
                  fpgaReceivedFile_decrypted{i,1}   = fnName ;
                  fpgaReceivedFile_decrypted{i,2}   = arguments ;
                  fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
                  clear scanOut arguments outputData fnDecrypt
        
        case ...'IMGSelect'
                ...fnNameByte  = 
                107 ...;
    %         authored by: Ashwin Balagopal S
    %         uart function call: fnNameByte=abcd[0]=70 bitLine=abcd[1] stopByte=abcd[3]=127
%                 bitLine     = arguments.bitLine ;
%                 numArray    = [ fnNameByte, bitLine, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
                  fnName                                = 'IMGSelect' ;
                  eval( [ ' args                        = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ; 
                  eval( [ ' outputData                  = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
                  arguments.IMG                         = args( end-1 ) ; 
                  fpgaReceivedFile_decrypted{i,1}       = fnName ;
                  fpgaReceivedFile_decrypted{i,2}       = arguments ;
                  fpgaReceivedFile_decrypted{i,3}       = outputData(1:end-1) ;
                  clear fnName arguments outputData args
                  
        case ...'perform_hybridIMC_BLIMG_FPGA'
            ...fnNameByte = 
            108 ...;
%         authored by: Balaji V
%         uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127        
%                 scanChain = arguments.scanChain ;
%                 numArray = [ fnNameByte, scanChain, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
              eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
              eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
              fnName                            = 'perform_hybridIMC_BLIMG_FPGA' ;
              %arguments.scanChain = scanChainNames{ args(1)+1 } ;
              fpgaReceivedFile_decrypted{i,1}   = fnName ;
              %fpgaReceivedFile_decrypted{i,2}   = arguments ;
              fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
              clear scanOut arguments outputData fnDecrypt

        case ...'perform_internalIMC_ImgBankFPGA'
            ...fnNameByte = 
            109 ...;
%         authored by: Balaji V
%         uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127        
%                 scanChain = arguments.scanChain ;
%                 numArray = [ fnNameByte, scanChain, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
              eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
              eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
              fnName                            = 'perform_internalIMC_ImgBankFPGA' ;
              %arguments.scanChain = scanChainNames{ args(1)+1 } ;
              fpgaReceivedFile_decrypted{i,1}   = fnName ;
%              fpgaReceivedFile_decrypted{i,2}   = arguments ;
              fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
              clear scanOut arguments outputData fnDecrypt
              
        case ...'scanInVector_uncompressed_FAST'
            ...fnNameByte  = 
            113 ...;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=113 scanChain=abcd[1] vecScanIn=abcd[2:end-1] stopByte=abcd[end]=127
%             scanChain   = arguments.scanChain ;
%             vecScanIn   = reshape( arguments.vecScanIn, 1, numel( arguments.vecScanIn ) ) ;
%             numArray    = [ fnNameByte, scanChain, vecScanIn , stopByte ] ;
%             printNumericArray( fileHandle, numArray  ) ; 
              eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
              eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
              fnName                            = 'scanInVector_uncompressed_FAST' ;
              arguments.scanChain = scanChainNames{ args(1)+1 } ;
              fpgaReceivedFile_decrypted{i,1}   = fnName ;
              fpgaReceivedFile_decrypted{i,2}   = arguments ;
              fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
              clear scanOut arguments outputData fnDecrypt
        case ...'scanOutVector_FAST'
            ...fnNameByte = 
            114 ...;
%           authored by: Ashwin Balagopal S
%           uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127        
%             scanChain = arguments.scanChain ;
%             numArray = [ fnNameByte, scanChain, stopByte ] ;
%             printNumericArray( fileHandle, numArray ) ;
              eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
              eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
              fnName                            = 'scanOutVector_FAST' ;
              arguments.scanChain = scanChainNames{ args(1)+1 } ;
              fpgaReceivedFile_decrypted{i,1}   = fnName ;
              fpgaReceivedFile_decrypted{i,2}   = arguments ;
              fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
              clear scanOut arguments outputData fnDecrypt
              
      case ...'perform_hybridIMC_ADCSWEEP_FPGA'
            ...fnNameByte = 
            116 ...;
%         authored by: Ashwin
%         uart function call: fnNameByte=abcd[0]=116 scanChain=abcd[1] stopByte=abcd[2]=127        
%                 scanChain = arguments.scanChain ;
%                 numArray = [ fnNameByte, scanChain, stopByte ] ;
%                     printNumericArray( fileHandle, numArray ) ;
              eval( [ ' args                    = [' fpgaReceivedFile_blockSplit{i,2} '] ;' ] ) ;
              eval( [ ' outputData              = [' fpgaReceivedFile_blockSplit{i,3} '] ;' ] ) ;
              fnName                            = 'perform_hybridIMC_ADCSWEEP_FPGA' ;
              %arguments.scanChain = scanChainNames{ args(1)+1 } ;
              fpgaReceivedFile_decrypted{i,1}   = fnName ;
              %fpgaReceivedFile_decrypted{i,2}   = arguments ;
              fpgaReceivedFile_decrypted{i,3}   = outputData(1:end-1) ;
              clear scanOut arguments outputData fnDecrypt
 
    end
end
end
