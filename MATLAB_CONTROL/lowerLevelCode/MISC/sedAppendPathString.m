% This function is to convert a windows path to proper sed append format
function returnedPathString = sedAppendPathString( pathStringIn )
display( mfilename )
% returnedPathString = pathStringIn ;
a = fopen( './tempFileOut', 'w' ) ;
fprintf( a, '%s', pathStringIn ) ;
fclose( a ) ;
findReplaceCommand = 'git-bash -i -c "sed -i ''s/\\\/\\\\\\\\\\\/g'' ./tempFileOut"' ;
system( findReplaceCommand ) ;
returnedPathString = fileread( './tempFileOut' ) ;
deleteTempFileCommand = 'git-bash -i -c "rm ./tempFileOut"' ;
system( deleteTempFileCommand ) ;
end