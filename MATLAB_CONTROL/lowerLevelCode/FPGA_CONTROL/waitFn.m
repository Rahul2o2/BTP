function returnString = waitFn( fileHandle, waitTime, startStringIn ) % 0 <= waitTime < 128
display( mfilename )
% Getting the name of the function in fnName variable
st                      = dbstack ;
fnName                  = st.name ;
arguments.waitTime      = waitTime ;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
end