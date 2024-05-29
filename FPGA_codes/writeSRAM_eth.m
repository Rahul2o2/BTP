function returnString = writeSRAM_eth( client, colsel, dataSRAM,DL_nums, startStringIn )

   % numBytes=length(dataAllOnes)+length(DL_nums)+1+1+1;

   dsend=[105,dataSRAM,colsel,DL_nums,127];
    i=1;
    n_chunks=68;
   while(i<1445*n_chunks)
           write(client,dsend(i:i+1445),"uint8");
           data = read(client, 1,"uint8");
           i=i+1445;
   end   
           write(client,dsend(i:end),"uint8");
           data = read(client, 1,"uint8");
    
    returnString            = startStringIn ;
end