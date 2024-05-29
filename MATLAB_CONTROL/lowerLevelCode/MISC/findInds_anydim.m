% Find i, j, k, l such that ( i-1 ) + ( j-1 ) * d1 + ( k-1 ) * ( d1 * d2 ) + ( l-1 ) * ( d1 * d2  * d3 ) = ind - 1
function iout = findInds_anydim( arr, ind ) 

ioutM1 = [] ;
if( ind <= numel(arr) )
%%
    indM1 = ind - 1 ;
    d = size( arr ) ;
    [ d1, d2, d3, d4  ] = size( arr ) ;
%%  
    for n = 1 : (numel( d ))
        ioutM1(end+1) = mod( indM1, d(n) ) ;
        indM1 = ( indM1 - ioutM1(end) ) / d(n) ;
    end
end
iout =ioutM1+1;
end