% Find i, j, k, l such that ( i-1 ) + ( j-1 ) * d1 + ( k-1 ) * ( d1 * d2 ) + ( l-1 ) * ( d1 * d2  * d3 ) = ind - 1
function [ i, j, k, l ] = findInds( arr, ind ) 

    indM1 = ind - 1 ;
    [ d1, d2, d3, d4  ] = size( arr ) ;
%%  Now  ( ind - 1 )  = ( i-1 ) + ( j-1 ) * d1 + ( k-1 ) * ( d1 * d2 ) + ( l-1 ) * ( d1 * d2 * d3 )
    iM1 = mod( indM1, d1 ) ; % if X = K*Y + R; mod( X, Y ) = R
    
    indM1 = ( indM1 - iM1 ) / d1 ;
    
%%  Now  ( ind - 1 )  = ( j-1 ) + ( k-1 ) * ( d2 ) + ( l-1 ) * ( d2 * d3 )
    jM1 = mod( indM1, d2 ) ; % if X = K*Y + R; mod( X, Y ) = R
    
    indM1 = ( indM1 - jM1 ) / d2 ;
%%  Now  ( ind - 1 )  = ( k-1 ) + ( l-1 ) * ( d3 )
    kM1 = mod( indM1, d3 ) ; % if X = K*Y + R; mod( X, Y ) = R
    
    indM1 = ( indM1 - kM1 ) / d3 ;
%%  Now  ( ind - 1 )  = ( l-1 )
    lM1 = indM1 ;
%% Now to add 1 and assign
    i = iM1 + 1 ;
    j = jM1 + 1 ;
    k = kM1 + 1 ;
    l = lM1 + 1 ;
end