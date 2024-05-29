%use this function only to overwrite the 3 write scan chains entirely, not to partially overwrite
function stringCurr = scanInWriteScanChains(fileHandle, data, mask, stringCurr)
display( mfilename )
st                      = dbstack ;
fnName                  = st.name ;
arguments.data          = data ;
arguments.mask          = mask ;
uart_encrypt(fileHandle,fnName,arguments) ;
% returnString            = stringCurr ;
end