% How to use this function
% y_act - y values of EXPECTED  Xfer chars
% y_apx - y values of ERRONEOUS Xfer chars

% EXAMPLE USE
% x = 0 : 0.1 : 10 ;
% y_act = x+1 ;
% y_apx = y_act + randn(size(y_act)) ;
% R_square = checkRsq( y_act, y_apx ) ;
function R_square = checkRsq( y_act, y_fit )

mu_y_act = mean(y_act(:));

SST=sum((y_act-mu_y_act).^2);%% variance of y_act
SSR=sum((y_fit-y_act).^2);%% variance of error in y_act prediction

R_square = 1-(SSR/SST);

end