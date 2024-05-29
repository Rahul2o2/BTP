function returnString = RBL_IMC_Select_eth( client, RBL, startStringIn )
    
    numBytes=1+1;
    dsend=[111,RBL];
    
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