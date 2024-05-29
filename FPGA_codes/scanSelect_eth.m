%this function selects the scanchain as specified by the scanChain argument:
%     scanChain = 0 ; % Y<0>
%     scanChain = 1 ; % WL scanChain
%     scanChain = 2 ; % WR<0>
%     scanChain = 3 ; % WR<1>
%     scanChain = 4 ; % WR<2>
%     scanChain = 5 ; % RSC
%     scanChain = 6 ; % FF
%     scanChain = 7 ; % VPPGATE_B
function returnString = scanSelect_eth( client, scanChain, startStringIn )

    numBytes=2;
    dsend=[67,scanChain];
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