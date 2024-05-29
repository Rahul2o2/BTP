function R_square = get_R_square( actual_data, predicted_data )
data_mean = mean( actual_data(:) ) ;
res = actual_data(:) - predicted_data(:) ;
deviation_mean = actual_data(:) - data_mean ;

SSR = sum( res.^2 ) ;
SST = sum( deviation_mean.^2 ) ;

R_square = 1 - ( SSR / SST ) ;
end