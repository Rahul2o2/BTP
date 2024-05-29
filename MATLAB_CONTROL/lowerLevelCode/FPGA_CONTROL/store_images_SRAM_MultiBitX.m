function returnString = store_images_SRAM_MultiBitX(fileHandle,X,mode,IMGnum,startStringIn)
st                      = dbstack ;
fnName                  = st.name ;
arguments.X     = X ;
arguments.mode     = mode ;
arguments.IMGnum    =IMGnum;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
% display( mfilename )
end