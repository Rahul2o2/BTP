function FP_Data=bytes_to_int(bytesData,order)
% Initialize an array to store the converted floating-point numbers
numGroups = numel(bytesData) / 4;
FP_Data = zeros(1, numGroups, 'int32');

if order == "LE"
    % Loop through the output array in steps of 4 bytes
    for i = 1:4:numel(bytesData)
        % Extract a group of 4 bytes
        groupBytes = bytesData(i:i+3);
        groupBytes = groupBytes( end : -1 : 1 ) ;
        % Concatenate the 4 bytes into a single uint32 value
        groupValue = typecast(uint8(groupBytes), 'uint32');

        % Convert the uint32 value to an int32 number
        FP_Data((i+3)/4) = typecast(groupValue, 'int32');
    end
end
if order == "BE"
    % Loop through the output array in steps of 4 bytes
    for i = 1:4:numel(bytesData)
        % Extract a group of 4 bytes
        groupBytes = bytesData(i:i+3);

        % Concatenate the 4 bytes into a single uint32 value
        groupValue = typecast(uint8(groupBytes), 'uint32');

        % Convert the uint32 value to an int32 number
        FP_Data((i+3)/4) = typecast(groupValue, 'int32');
    end
end
end