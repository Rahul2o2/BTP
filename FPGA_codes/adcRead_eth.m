function returnString = adcRead_eth( client, repeatAvg, startStringIn )
    
    numBytes=length(repeatAvg)+1;
    dsend=[83,repeatAvg];
    
    write(client,dsend,"uint8");
    data = read(client, numBytes,"uint8");
    returnString            = startStringIn ;
end