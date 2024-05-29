function linPath = windowsPathToLinuxPath( winPath )
display( mfilename )
% Assumption is that the path starts with the drive letter itself
linPath = winPath ;
linPath( linPath =='\' ) = '/' ;
linPath( linPath ==':' ) = [] ;
linPath = [ '/' linPath ] ;
end