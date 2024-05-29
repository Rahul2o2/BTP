function returnString = perform_internalIMC_ImgBankFPGA( fileHandle, ImgBank, startStringIn )
st                      = dbstack ;
fnName                  = st.name ;
arguments.ImgBank       = ImgBank ;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
display( mfilename );
end