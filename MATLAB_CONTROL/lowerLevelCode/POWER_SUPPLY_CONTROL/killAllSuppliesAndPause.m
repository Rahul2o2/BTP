function killAllSuppliesAndPause( )
supply_AgE36313A = AgE36313A_connect() ;
% supply_AgB2902A =  AgB2902A_connect() ;
% supply_AgB2902A_2 =  AgB2902A_2_connect() ;
AgE36313A_supplies      = [ 1 2 3 ] ;
AgB2902A_supplies       = [ 1 2 ] ;
AgB2902A_2_supplies     = [ 1 2 ] ;
%% 17. !DISABLE! THE SUPPLIES
    % AgE36313A
        for supply = AgE36313A_supplies
                outputTarget = supply ;
            AgE36313A_disable( supply_AgE36313A, outputTarget ) ;
        end
    % AgB2902A
        for supply = AgB2902A_supplies
                outputTarget = supply ;
            AgB2902A_disable( supply_AgB2902A, outputTarget ) ;
        end
    % AgB2902A_2
        for supply = AgB2902A_2_supplies
                outputTarget = supply ;
            AgB2902A_disable( supply_AgB2902A_2, outputTarget ) ;
        end
%% pause
fprintf( '\nKILLED ALL SUPPLIES AND PAUSED\n' )
while(true)
end
end