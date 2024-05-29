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
function returnString = scanInVector_uncompressed_FAST( fileHandle, vecScanIn, scanChain, startStringIn )
st                      = dbstack ;
fnName                  = st.name ;
arguments.vecScanIn     = vecScanIn ;
arguments.scanChain     = scanChain ;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
% display( mfilename )
end