%     scanChain = 0 ; % Y<0>
%     scanChain = 1 ; % WL scanChain
%     scanChain = 2 ; % WR<0>
%     scanChain = 3 ; % WR<1>
%     scanChain = 4 ; % WR<2>
%     scanChain = 5 ; % RSC
%     scanChain = 6 ; % FF
%     scanChain = 7 ; % VPPGATE_B
function stringCurr = perform_WxOnRowColSel_FPGA_2( fileHandle, WLnum, BL, data, mask, stringCurr )
st                      = dbstack ;
fnName                  = st.name ;
arguments.WLnum         = WLnum ;
arguments.BL            = BL ;
arguments.data          = data ;
arguments.mask          = mask ;
% arguments.scanChain     = scanChain ;
uart_encrypt(fileHandle,fnName,arguments) ;
fprintf('\n%s\tWLnum=%d\tBLnum=%d',mfilename,WLnum,BL);
% display(  )
end