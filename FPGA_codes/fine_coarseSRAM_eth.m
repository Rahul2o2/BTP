function returnString = fine_coarseSRAM_eth( client, Fine_cm, Fine_sub, Coarse_cm, Coarse_sub, startStringIn )

result = any(Fine_cm > 15);
if(result)
    error("Overflow in Fine_cm");
end

result = any(Fine_sub > 15);
if(result)
    error("Overflow in Fine_sub");
end

result=any(Coarse_cm < 0 | Coarse_cm > 3);
if(result)
    error("Overflow in Coarse_cm");
end

result=any(Coarse_sub < 0 | Coarse_sub > 3);
if(result)
    error("Overflow in Coarse_cm");
end

    numBytes=length(Fine_sub)+length(Coarse_cm)+length(Coarse_sub)+length(Fine_cm)+1;
    dsend=[106,Fine_cm,Fine_sub,Coarse_cm,Coarse_sub];
    
    write(client,dsend,"uint8");
    data = read(client, numBytes,"uint8");
    
returnString            = startStringIn ;
% display( mfilename )
end

