%     scanChain = 0 ; % Y<0>
%     scanChain = 1 ; % WL scanChain
%     scanChain = 2 ; % WR<0>
%     scanChain = 3 ; % WR<1>
%     scanChain = 4 ; % WR<2>
%     scanChain = 5 ; % RSC
%     scanChain = 6 ; % FF
%     scanChain = 7 ; % VPPGATE_B

% 	~WR_SC_0 drives  BL -> length =150
% 	~WR_SC_1 drives BLB -> length =150
%   Thus essentially data on WR_SC_0 serves as the BLB bit and data on WR_SC_1 serves as BL bit
function returnString = scanInVector_eth( client, vecScanIn, scanChain, startStringIn )
    numBytes=1+1+length(vecScanIn);
    dsend=[68,scanChain,vecScanIn];
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