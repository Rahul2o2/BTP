function returnString = bitsSet_eth( client, signals, values, startStringIn )
    
    numBytes=length(signals)+length(values)+1;
    dsend=[64,signals,values];
    
    write(client,dsend,"uint8");
    data = read(client, numBytes,"uint8");
    
    returnString            = startStringIn ;
end