function returnString = loadBitlinetoCell_FPGA_eth( client, bitLines, stringCurr )
    numBytes=length(bitLines)+1;
    dsend=[77,bitLines];
    
    write(client,dsend,"uint8");
    data = read(client, numBytes,"uint8");
    returnString        = stringCurr ;
end