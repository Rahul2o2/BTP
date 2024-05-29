%this function:
%     -generates pulse at time t2. say at t0 signals=0, t1 signals =1, t2 signals=0 
function stringCurr = pulse( fileHandle , signals, currentValues, stringCurr )
display( mfilename )
st                      = dbstack ;
fnName                  = st.name ;
arguments.signals       = signals ;
arguments.currentValues = currentValues;
uart_encrypt(fileHandle,fnName,arguments) ;
% returnString            = stringCurr ;
end