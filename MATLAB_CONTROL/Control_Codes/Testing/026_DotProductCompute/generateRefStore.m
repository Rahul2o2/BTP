function [subset_array] = generateRefStore(array,target_sum)
%generates a subset array that can develop an appropriate subtracting sum

    array_length = 1024;
    %target_sum = 120;
    subset_stop = 1;  % Initialize the starting index of the subset
    %array=dataCols(1,:);
    % Find the subset
    for n = 1:array_length
    subset_sum = sum(array(1,1:n));  % Calculate the sum of elements from index 1 to n
    if subset_sum >= target_sum
        subset_stop = n;  % Set the starting index of the subset
        break;
    end
    end

    % Create the subset array
    subset_array = [array(1,1:subset_stop),zeros(1,1024-subset_stop)];

end

