function AgB2902B_enableAuto()
%AGB2902B_ENABLEAUTO Summary of this function goes here
%   Detailed explanation goes here
% % Find a VISA-USB object.
 instrumentObj = instrfind('Type', 'visa-usb', 'RsrcName', 'USB0::0x2A8D::0x9201::MY61391393::0::INSTR','Tag', '');
% 
% % Create the VISA-USB object if it does not exist
% % otherwise use the object that was found.
if isempty(instrumentObj)
    instrumentObj = visa('KEYSIGHT', 'USB0::0x2A8D::0x9201::MY61391393::0::INSTR');
else
    fclose(instrumentObj);
    instrumentObj = instrumentObj(1);
end

% Connect to instrument object, instrumentObj.
fopen(instrumentObj);

%% setting AUTO MODE

fprintf(instrumentObj,":OUTP:ON:AUTO ON");


end

