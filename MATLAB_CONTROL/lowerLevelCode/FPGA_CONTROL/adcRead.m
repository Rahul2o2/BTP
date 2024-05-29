function returnString = adcRead( fileHandle, repeatAvg, startStringIn )
% Getting the name of the function in fnName variable
st                      = dbstack ;
fnName                  = st.name ;
arguments.repeatAvg        = repeatAvg;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
% display( mfilename )
end