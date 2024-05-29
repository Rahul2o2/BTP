%this function:
%     -assigns default state: all 0s, SCAN SEL 101
%	example:- stringCurr=defaultState(fileHandle,stringCurr);
function returnString=defaultState( fileHandle, startStringIn  )
display( mfilename )
st                      = dbstack ;
fnName                  = st.name ;
arguments               = [] ; % case for this in uart_encrypt actually doesn't use arguments but need it for calling uart_encrypt
uart_encrypt(fileHandle,fnName,arguments) ;
returnString            = startStringIn ;
end
