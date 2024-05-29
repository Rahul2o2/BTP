function returnString = perform_hybridIMC_BLIMG_FPGA_eth( client, BL, IMG, startStringIn )

    numBytes=1+1+1;
    dsend=[108,BL,IMG];
    
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    data = read(client, 644 ,"uint8");
    
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, data, 'uint8');

    % Close the file
    fclose(fileID);
    

returnString            = startStringIn ;

end