nameChangeCommandFileId = fopen( './nameChangeCommandFile.bash', 'w' ) ;
nameChangeCommandString = sprintf( 'a=`echo ${PWD##*/}`\nsed -i "s/testChipSignalling/$a/g" ./*.m' );
    fprintf( nameChangeCommandFileId, '%s\n', '#!/bin/bash' ) ;
    fprintf( nameChangeCommandFileId, '%s'  , nameChangeCommandString ) ;
fclose( nameChangeCommandFileId ) ;
system( 'git-bash ./nameChangeCommandFile.bash' ) ;
system( 'git-bash -i -c "rm ./nameChangeCommandFile.bash"' ) ;