function filePathcdMM = onePrevPath( filePath )
display( mfilename )
% filePathcdMM is the path that you'd get if you do cd .. while starting
% from location: filePath
% filePath must be in linux path format
    filePathcdMM = filePath ; % This is t
    loc = find( filePathcdMM == '/', 1, 'last' ) ;
    filePathcdMM = filePathcdMM( 1 : ( loc - 1 ) ) ;
end