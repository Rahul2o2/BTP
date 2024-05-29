function [ adc_outs_VPVN, adc_outs_AUX00, adc_outs_VPVN_orig, adc_outs_AUX00_orig ] = decryptAndMedianVoltages( returnVoltagesEncrypted, repeatAvg )
%% get voltage values, copied from decrypt_and_RSC
    stopByte = 127 ; fillerByte = 124 ;
    stopByteOrFillerByteIndices = find( returnVoltagesEncrypted == stopByte | returnVoltagesEncrypted == fillerByte) ; 
    adc_outs_local = returnVoltagesEncrypted( ( stopByteOrFillerByteIndices( end - 1 ) + 1 ) : ( stopByteOrFillerByteIndices( end ) - 1 ) ) ;
            
            adc_outs_local = reshape( adc_outs_local, [ 2 numel(adc_outs_local)/2 ] )' ; % adc_outs_local( odd, : ) -> VPVN, adc_outs_local( even, : ) -> AUX00
            
            adc_outs_VPVN  = adc_outs_local( 1 :2 : end, : ) ;
            adc_outs_AUX00 = adc_outs_local( 2 : 2 : end, : ) ;
            
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
%             adc_VPVN = [ adc_VPVN ; adc_outs_VPVN ] ;

%             AUX00
            adc_outs_AUX00 = reshape( adc_outs_AUX00', [ numel(adc_outs_AUX00) 1 ] ) ;
            adc_outs_AUX00 = fi( adc_outs_AUX00, 0, 8, 0 ) ;
            adc_outs_AUX00 = permute( permute( adc_outs_AUX00.bin, [ 1 3 2 ] ) - '0', [ 1 3 2 ] ) ;
            adc_outs_AUX00 = adc_outs_AUX00( :, 3 : 8 ) ; % get rid of redundant MSB:MSB-1 as only 6 bits of data per byte is useful
            adc_outs_AUX00 = reshape( adc_outs_AUX00', [ 12 numel(adc_outs_AUX00)/12 ] )' ;
            power2   = ( 2.^( 11 : -1 : 0 ) )' ;
            adc_outs_AUX00 = ( adc_outs_AUX00 * power2 * 0.244 * 0.001 ) ; % + 8 * 0.244 ;
            
            adc_outs_VPVN_orig = adc_outs_VPVN ;
            adc_outs_AUX00_orig = adc_outs_AUX00 ;
%             adc_outs_AUX00 = adc_outs_AUX00 * 0.244 ;
%             adc_outs_AUX00 = ( adc_outs_AUX00 * p1_AUX00 ) + p2_AUX00 ;
%             adc_AUX00 = [ adc_AUX00 ; adc_outs_AUX00 ] ;
    adc_outs_VPVN = median( reshape( adc_outs_VPVN, repeatAvg, [] ) ) ;
    adc_outs_AUX00 = median( reshape( adc_outs_AUX00, repeatAvg, [] ) ) ;
end