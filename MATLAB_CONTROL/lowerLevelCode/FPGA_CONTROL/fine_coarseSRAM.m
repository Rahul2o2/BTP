function returnString = fine_coarseSRAM( fileHandle, Fine_cm, Fine_sub, Coarse_cm, Coarse_sub, startStringIn )

st                      = dbstack ;
fnName                  = st.name ;

arguments.Fine_cm     = Fine_cm ;
result = any(Fine_cm > 15);
if(result)
    error("Overflow in Fine_cm");
end

arguments.Fine_sub     = Fine_sub ;
result = any(Fine_sub > 15);
if(result)
    error("Overflow in Fine_sub");
end

arguments.Coarse_cm   = Coarse_cm;
result=any(Coarse_cm < 0 | Coarse_cm > 3);
if(result)
    error("Overflow in Coarse_cm");
end

arguments.Coarse_sub   = Coarse_sub;
result=any(Coarse_sub < 0 | Coarse_sub > 3);
if(result)
    error("Overflow in Coarse_cm");
end

uart_encrypt( fileHandle, fnName, arguments ) ;
returnString            = startStringIn ;
% display( mfilename )
end

