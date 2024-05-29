function returnString = perform_RxOnRowColSel_FPGA_2_eth( client, WLnum, BL, stringCurr )
    numBytes=1+1+1+1;
    WLnum_MSD   = floor( WLnum/100  ) ; % eg if WLnum = 1007 then WLnum_MSD = 10
    WLnum_LSD   =   mod( WLnum, 100 ) ; % eg if WLnum = 1007 then WLnum_LSD =  7

    dsend=[90, WLnum_MSD, WLnum_LSD, BL];
    
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    data = read(client, 97,"uint8");
    
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, data, 'uint8');

    % Close the file
    fclose(fileID);
    
    returnString        = stringCurr ;
end