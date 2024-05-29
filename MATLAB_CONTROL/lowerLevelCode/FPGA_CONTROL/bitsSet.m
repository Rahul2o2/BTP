function returnString = bitsSet( fileHandle, signals, values, startStringIn )

% Getting the name of the function in fnName variable
st                      = dbstack ;
fnName                  = st.name ;
values( signals > 68 )  = [] ;
arguments.values        = values ;
signals( signals > 68 ) = [] ; 
arguments.signals       = signals ;
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
% display( mfilename )
end