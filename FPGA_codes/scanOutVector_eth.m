%     scanChain = 0 ; % Y<0>
%     scanChain = 1 ; % WL scanChain
%     scanChain = 2 ; % WR<0>
%     scanChain = 3 ; % WR<1>
%     scanChain = 4 ; % WR<2>
%     scanChain = 5 ; % RSC
%     scanChain = 6 ; % FF
%     scanChain = 7 ; % VPPGATE_B
function returnString = scanOutVector_eth( client, vecScanOut, scanChain, startStringIn )
    numBytes=1+1;
    dsend=[69,scanChain];
    write(client,dsend,"uint8");
    echo = read(client, numBytes,"uint8");
    scanLen_bytes = read(client, 2,"uint8");
    scanLen_bytes_rev=fliplr(scanLen_bytes);
    scanLen = typecast(uint8(scanLen_bytes_rev),'uint16');
    vecScanOut = read(client, scanLen,"uint8");
    
    % Open a file for writing
    fileID = fopen('data.bin', 'wb');

    % Write the data to the file
    fwrite(fileID, vecScanOut, 'uint8');

    % Close the file
    fclose(fileID);
    
    returnString            = startStringIn ;
end