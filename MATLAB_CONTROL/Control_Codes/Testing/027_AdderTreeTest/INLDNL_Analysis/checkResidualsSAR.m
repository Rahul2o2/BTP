function [ residuals, SAR_bits, SAR_dec ] = checkResidualsSAR( vin_in_mV, VrefH_in_mV, VrefL_in_mV, Nbits )
    SAR_bits = zeros( 1, Nbits ) ;
    residuals = zeros( 1, Nbits ) ;
    power2 = 2.^( Nbits-1 : -1 : 0 ) ;
    for k = 0 : ( Nbits - 1 )
                Bvec = zeros( Nbits, 1 ) ;
                Bvec( 1 : k ) = SAR_bits( 1 : k ) ;
                Bvec( 1 + k ) = 1 ;
            B = power2 * Bvec ;
            Xr = B / 2^Nbits ;
        residuals( 1 + k ) = vin_in_mV - ( ( VrefH_in_mV - VrefL_in_mV )* Xr + VrefL_in_mV )  ;
        SAR_bits( 1 + k ) = ( residuals( 1 + k ) ) > 0 ;
    end
    power_of_two=(2.^( (Nbits-1) :-1 : 0 ))';
    SAR_dec=SAR_bits*power_of_two;
end