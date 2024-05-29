function winPath = linuxPathToWindowsPath( linPath )
display( mfilename )
winPath = linPath ;
winPath( winPath =='/' ) = '\' ;
if( winPath( 1 ) == '.' ) % The assumption here is winPath is something like ./abcd/efgh which becomes abcd\efgh
    winPath = [ winPath( 3 : end ) ] ;
end
if( ~sum( winPath( [ 1  3 ] ) ~= '\\' ) ) % The assumption here is winPath is something like \D\abcd\efgh D:\abcd\efgh
    winPath = [ winPath( 2 ) ':' winPath( 3:end )  ] ;
end
end