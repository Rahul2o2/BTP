function Uint16_Data=bytes_to_uint16(bytesData,order)
% Initialize an array to store the converted floating-point numbers
numGroups = numel(bytesData) / 2;
Uint16_Data = zeros(1, numGroups, 'uint16');

if order == "LE"
    % Loop through the output array in steps of 2 bytes
    for i = 1:2:numel(bytesData)
        % Extract a group of 2 bytes
        groupBytes = bytesData(i:i+1);
        groupBytes = groupBytes( end : -1 : 1 ) ;
        % Concatenate the 2 bytes into a single uint16 value
        groupValue = typecast(uint8(groupBytes), 'uint16');

        % Convert the uint16 value to an int32 number
        Uint16_Data((i+1)/2) = typecast(groupValue, 'uint16');
    end
end
if order == "BE"
    % Loop through the output array in steps of 2 bytes
    for i = 1:2:numel(bytesData)
        % Extract a group of 2 bytes
        groupBytes = bytesData(i:i+1);

        % Concatenate the 2 bytes into a single uint16 value
        groupValue = typecast(uint8(groupBytes), 'uint16');

        % Convert the uint16 value to an int32 number
        Uint16_Data((i+1)/2) = typecast(groupValue, 'uint16');
    end
end
end