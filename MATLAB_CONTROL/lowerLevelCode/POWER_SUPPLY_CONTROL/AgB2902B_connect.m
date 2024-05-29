%% Important Instructions 
% Call this function only after you have connected the Voltage Source using USB
function instrumentObj = AgB2902B_connect()
display( mfilename )
% From the Keysight IO Connection Expert Software got these details:
serial_number = 'MY61391393' ;
%% GETTING THE INSTRUMENT VISA ADDRESS
a = instrhwinfo( 'visa', 'agilent' ) ; % This command lists all the 
% find the object constructor with the serial number as this guy
for i = 1 : numel( a.ObjectConstructorName )
    if( contains( a.ObjectConstructorName{ i }, serial_number ) )
        eval( sprintf('a = %s ;',a.ObjectConstructorName{ i } ) )
        break
    end
end

makemid( 'AgB29xx', 'AgB2902B.mdd' ) ; % Got this from https://www.keysight.com/in/en/lib/software-detail/driver/b2900a-precision-source-measure-unit-ivi-and-matlab-instrument-drivers-2056761.html
% Got to know about 'AgB29xx' from Running: IviInfo = instrhwinfo('ivi');IviInfo.Modules ( from here https://www.mathworks.com/help/instrument/set-output-voltage-and-make-measurements-from-a-keysight-age3633a-dc-power-supply-using-the-ivi-c-driver.html )
INSTRU_ID = a.RsrcName ; % Can also get this info by running keysight connection expert after connecting the voltage source to the PC
%% USING icdevice to create an instrument object
instrumentObj = icdevice( 'AgB2902B.mdd', INSTRU_ID ) ;

%% Connecting to instrument
% THIS WILL THROW AN ERROR AND STOP PROGRAM EXECUTION IF YOU HAVEN'T
% CONNECTED THE USB CABLE FROM THE POWER SUPPLY TO YOUR PC. IF YOU HAVE
% DONE SO, IT WILL NOT THROW AN ERROR.
connect( instrumentObj ) ; 
end