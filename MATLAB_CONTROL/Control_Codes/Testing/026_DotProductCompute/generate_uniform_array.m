function [array,one_indices] = generate_uniform_array(n, x)
    if x > n
        error('Number of ones (x) cannot be greater than the array length (n).');
    end
    
    % Create an array with x ones and (n-x) zeros
    array = [ones(1, x), zeros(1, n-x)];
    
    % Randomly shuffle the array
    %random_indices = randperm(n);
    %random_array = array;%(random_indices);
    one_indices = find(array == 1);
end
