function returnString = perform_hybridIMC_TOPSW_FPGA( fileHandle, BL, IMG, startStringIn )
st                      = dbstack ;
fnName                  = st.name ;
arguments.BL            = BL ;
arguments.IMG           = IMG ;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
display( mfilename );
end