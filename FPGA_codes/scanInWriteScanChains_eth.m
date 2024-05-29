%use this function only to overwrite the 3 write scan chains entirely, not to partially overwrite
function stringCurr = scanInWriteScanChains_eth(client, data, mask, stringCurr)
    numBytes=1+length(data)+length(mask);
    dsend=[76,data,mask];
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    data = read(client, numBytes,"uint8");
   
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, data, 'uint8');

    % Close the file
    fclose(fileID);
    
    returnString            = stringCurr ;

end