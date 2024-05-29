%this function selects the scanchain as specified by the scanChain argument:
%     scanChain = 0 ; % Y<0>
%     scanChain = 1 ; % WL scanChain
%     scanChain = 2 ; % WR<0>
%     scanChain = 3 ; % WR<1>
%     scanChain = 4 ; % WR<2>
%     scanChain = 5 ; % RSC
%     scanChain = 6 ; % FF
%     scanChain = 7 ; % VPPGATE_B
function returnString = scanSelect( fileHandle, scanChain, startStringIn )

st                      = dbstack ;
fnName                  = st.name ;
arguments.scanChain     = scanChain ;
uart_encrypt( fileHandle, fnName, arguments ) ;
returnString            = startStringIn ;
% display( mfilename )
end