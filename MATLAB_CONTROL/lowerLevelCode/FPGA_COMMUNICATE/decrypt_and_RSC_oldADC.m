% AUTHOR INFORMATION
% NAME: Ashwin Balagopal S
% EMAIL: ee17d200@smail.iitm.ac.in
% Code to decrypt a received file
% To be called from driver-code directory: 
%     /data/ee17d200/cadence_files/verilog/generating_vector_files/10_vecfile_auto_FA/7_vecfile_auto_FA
function [ read_scanouts_collect, fpgaReceivedFile_decrypted, adc_VPVN, adc_AUX00 ] = decrypt_and_RSC_oldADC( fpgaReceivedFile_name )
display( mfilename )
% fpgaReceivedFile_name       = 'WRx_All_Cells_Check' ;
    fpgaReceivedFile_decrypted  = uart_decrypt( fpgaReceivedFile_name ) ;
    r_sc_out_rows               = [] ;
    adc_VPVN                    = [] ;
    adc_AUX00                   = [] ;
    load( './lowerLevelCode/adc_corrn_old.mat', 'adc_corrn' ) ;
    p1_VPVN = adc_corrn.p1 ;
    p2_VPVN = adc_corrn.p2 ;
    for i = 1 : size( fpgaReceivedFile_decrypted, 1 )
        if( strcmp( 'scanOutVector', fpgaReceivedFile_decrypted{i,1} ) )
            if( strcmp( 'R_SC', fpgaReceivedFile_decrypted{i,2}.scanChain ) )
    %             read_scanouts_collect = [ read_scanouts_collect ; fpgaReceivedFile_decrypted{i,3} ] ;
                r_sc_out_rows   = [ r_sc_out_rows i ] ;
            end
        end
        if( strcmp( 'adcRead', fpgaReceivedFile_decrypted{i,1} ) )
            adc_outs_local = fpgaReceivedFile_decrypted{i,3} ;
            
            adc_outs_local = reshape( adc_outs_local, [ 2 numel(adc_outs_local)/2 ] )' ;
            
            adc_outs_VPVN  = adc_outs_local ;
            
%             VPVN
            adc_outs_VPVN = reshape( adc_outs_VPVN', [ numel(adc_outs_VPVN) 1 ] ) ;
            adc_outs_VPVN = fi( adc_outs_VPVN, 0, 8, 0 ) ;
            adc_outs_VPVN = permute( permute( adc_outs_VPVN.bin, [ 1 3 2 ] ) - '0', [ 1 3 2 ] ) ;
            adc_outs_VPVN = adc_outs_VPVN( :, 3 : 8 ) ; % get rid of redundant MSB:MSB-1 as only 6 bits of data per byte is useful
            adc_outs_VPVN = reshape( adc_outs_VPVN', [ 12 numel(adc_outs_VPVN)/12 ] )' ;
            power2   = ( 2.^( 11 : -1 : 0 ) )' ;
            adc_outs_VPVN = ( adc_outs_VPVN * power2 ) ;
            adc_outs_VPVN = adc_outs_VPVN * 0.244 ;
            adc_outs_VPVN = ( adc_outs_VPVN * p1_VPVN ) + p2_VPVN ;
            adc_VPVN = [ adc_VPVN ; adc_outs_VPVN ] ;
        end
    end
    num_read_scanouts           = numel( r_sc_out_rows ) ;
    read_scanouts_collect       = zeros( num_read_scanouts, 150 ) ;
    for i = 1 : num_read_scanouts
        read_scanouts_collect(i,:) = fpgaReceivedFile_decrypted{ r_sc_out_rows(i), 3 } ;
    end
end