% This function is meant to be called from /data/ee17d200/cadence_files/verilog/generating_vector_files/10_vecfile_auto_FA/7_vecfile_auto_FA
function fpgaReceivedFile_blockSplit = uart_blockSplit_OLD( fpgaReceivedFile_name )
display( mfilename )
% fileInPath is the fpgaReceivedFile_name of the .txt file in ./fpga_receivedFiles/
fpgaReceivedFile              = [ './fpga_receivedFiles/' fpgaReceivedFile_name '_receive.txt' ] ;
% fpgaReceivedFile            = double( fpgaReceivedFile ) ; % THIS GIVES IT AS ASCII
% temp                        = string( fpgaReceivedFile ) ;
% temp                        = [ temp; repmat( string(newline), 1, numel(temp) ) ];
fpgaReceivedFile_temp_1     = [ './fpga_receivedFiles/' fpgaReceivedFile_name '_receive_temp_1' ] ;
fpgaReceivedFile_temp_2     = [ './fpga_receivedFiles/' fpgaReceivedFile_name '_receive_temp_2' ] ;
% fH                          = fopen( fpgaReceivedFile_temp_1, 'w' );
octalDumpCommand = [ 'cat ' fpgaReceivedFile ' | od -An -vtu1 >  ' fpgaReceivedFile_temp_1 ] ;
%% OLD COMMAND:
% system( octalDumpCommand );
%%
perlCommand                 = [ 'perl -pe ''s/^\s*//g'' ' fpgaReceivedFile_temp_1...remove unnecesary spaces from opening line
                                '|perl -pe ''s/\n/ /g'' '...making it one long line of single-spaced characters
                                '|perl -pe ''s/ 124//g'' '... delete 7C(hex) ie 124(dec)
                                '|perl -pe ''s/(.*?127.*?127)\s*/\1\n/g'' '...
                                '|perl -pe ''s/(\w*)\s*/\1\n/'' '...
                                '|perl -pe ''s/(.*?127)\s*/\1\n/g'' '...
                                ...'|perl -pe ''s/127//g'' '...
                                ' > ' fpgaReceivedFile_temp_2 ...
                                ] ;
%% OLD COMMAND:
% system( perlCommand ) ;
%% NEW COMMAND
bashCommandFile = sprintf( './fpga_receivedFiles/%s.bash', fpgaReceivedFile_name ) ;
fID_commandFile = fopen( bashCommandFile, 'w' );
fprintf( fID_commandFile, '%s\n', '#!/bin/bash' ) ;
fprintf( fID_commandFile, '%s\n', octalDumpCommand ) ;
fprintf( fID_commandFile, '%s', perlCommand ) ;
fclose( fID_commandFile ) ;
system( sprintf( 'git-bash %s', bashCommandFile ) ) ;
%% 
temp                        = fileread( fpgaReceivedFile_temp_2 ) ;
if( temp(end) == newline)
    temp( end ) = [] ;
end
fpgaReceivedFile_blockSplit = splitlines( temp ) ;
fpgaReceivedFile_blockSplit = reshape( fpgaReceivedFile_blockSplit, 3, numel(fpgaReceivedFile_blockSplit) / 3 )' ;
%% OLD COMMAND:
% system( [ 'rm ' fpgaReceivedFile_temp_1 ' ' fpgaReceivedFile_temp_2 ] ) ;
removeCommand = sprintf( 'rm %s %s %s',  fpgaReceivedFile_temp_1, fpgaReceivedFile_temp_2, bashCommandFile ) ;
system( sprintf( 'git-bash -i -c "%s"', removeCommand ) ) ;
end
