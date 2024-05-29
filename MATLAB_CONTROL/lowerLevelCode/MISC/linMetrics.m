function [mse, slope, intercept] = linMetrics(xData,yData)
%LINMETRICS Summary of this function goes here
%   Detailed explanation goes here
% Step 1: Define your dataset (replace xData and yData with your data)
%xData = [5E-06:10E-06:110E-06];
%yData = adc_VPVN_725mV;

% Step 2: Fit a straight line to the data (1st order polynomial)
%coefficients = polyfit(xData, yData, 1);
slope =(yData(end)-yData(1))/((xData(end)-xData(1)));
intercept = yData(1)-xData(1)*slope;
%slope = coefficients(1);
%intercept = coefficients(2);

% Step 3: Calculate the predicted y values
yPredicted = slope * xData + intercept;

% Step 4: Calculate the mean squared error (MSE)
mse = mean((yData - yPredicted).^2,"all");
%sum_mse=sum(mse,'all');


% Step 5: Calculate the linearity
%linearity = (mse / var(yData));


% Display the results
fprintf("\n Slope: %f \n", slope);
fprintf("\n Intercept: %f \n", intercept);
%fprintf("\n Mean Squared Error (MSE): %f \n", mse);
%disp(['Linearity: ',num2str(linearity)  ]);

%mse = mse;
%linearity = linearity;
end

