function killAgE36313A( )
supply_AgE36313A = AgE36313A_connect() ;
% supply_AgB2902A =  AgB2902A_connect() ;
% supply_AgB2902A_2 =  AgB2902A_2_connect() ;
AgE36313A_supplies      = [ 1 2 3 ] ;
%% 17. !DISABLE! THE SUPPLIES
    % AgE36313A
        for supply = AgE36313A_supplies
                outputTarget = supply ;
            AgE36313A_disable( supply_AgE36313A, outputTarget ) ;
        end
%% pause
fprintf( '\nKILLED ALL Channels of AgE36313A\n' )
end