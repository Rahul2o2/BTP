function returnString = perform_WxOnRowColSel_FPGA_2_eth( client, WLnum, BL, data, mask, stringCurr )
    numBytes=length(data)+length(mask)+1+1+1+1;
    WLnum_MSD   = floor( WLnum/100  ) ; % eg if WLnum = 1007 then WLnum_MSD = 10
    WLnum_LSD   =   mod( WLnum, 100 ) ; % eg if WLnum = 1007 then WLnum_MSD =  7

    dsend=[91, WLnum_MSD, WLnum_LSD, BL, data, mask];
    
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    data = read(client, numBytes,"uint8");
    
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, data, 'uint8');

    % Close the file
    fclose(fileID);
    
    returnString        = stringCurr ;
end