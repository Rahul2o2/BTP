function returnString = perform_internalIMC_ImgBankFPGA_eth( client, ImgBank, startStringIn )

    numBytes=1+1;
    dsend=[109, ImgBank];
    
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    data = read(client, 644*4*4,"uint8");
    
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, data, 'uint8');

    % Close the file
    fclose(fileID);
    

returnString            = startStringIn ;

end