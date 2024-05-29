
function [ returnString, wlstatus, wscstatus, array_data ] = write_onto_dataLines_eth( client, col_sel, data_DL, DL_nums, wlstatus, wscstatus, array_data, startStringIn )

    
    numBytes=3+length(data_DL)+length(col_sel)+length(DL_nums);
    dsend=[80,data_DL,col_sel,126,DL_nums,127];
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    data = read(client, numBytes,"uint8");
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, data, 'uint8');

    % Close the file
    fclose(fileID);
    
    returnString            = startStringIn ;
end