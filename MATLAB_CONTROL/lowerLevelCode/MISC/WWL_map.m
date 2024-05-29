% WWL_IN<000:031>,noConn<00:01>,
% WWL_IN<032:063>,noConn<02:03>,
% WWL_IN<064:095>,noConn<04:05>,
% WWL_IN<096:127>,noConn<06:07>,
% WWL_IN<128:159>,noConn<08:09>,
% WWL_IN<160:191>,noConn<10:11>,
% WWL_IN<192:223>,noConn<12:13>,
% WWL_IN<224:255>,noConn<14:15>,
% WWL_IN<256:287>,noConn<16:17>,
% WWL_IN<288:319>,noConn<18:19>,
% WWL_IN<320:351>,noConn<20:21>,
% WWL_IN<352:383>,noConn<22:23>,
% WWL_IN<384:415>,noConn<24:25>,
% WWL_IN<416:447>,noConn<26:27>,
% WWL_IN<448:479>,noConn<28:29>,
% WWL_IN<480:511>,noConn<30:31>,
% WWL_IN<512:543>,noConn<32:33>,
% WWL_IN<544:575>,noConn<34:35>,
% WWL_IN<576:607>,noConn<36:37>,
% WWL_IN<608:639>,noConn<38:39>,
% WWL_IN<640:671>,noConn<40:41>,
% WWL_IN<672:703>,noConn<42:43>,
% WWL_IN<704:735>,noConn<44:45>,
% WWL_IN<736:767>,noConn<46:47>,
% WWL_IN<768:799>,noConn<48:49>,
% WWL_IN<800:831>,noConn<50:51>,
% WWL_IN<832:863>,noConn<52:53>,
% WWL_IN<864:895>,noConn<54:55>,
% WWL_IN<896:927>,noConn<56:57>,
% WWL_IN<928:959>,noConn<58:59>,
% WWL_IN<960:991>,noConn<60:61>,
% WWL_IN<992:1023>,noConn<62:65>
% WWL_IN<1024:1091>,noConn<0:1>

clear;
clc;
% 1090 for just the weight SRAMs
repeatableUnit = [ones(1, 32) zeros(1, 2)];
% 70 for just the calib SRAMs
CalibWWL = [ones(1, 68) zeros(1, 2)];
% totally there are (1090 + 70) 1160 scan flops out of which only (1024 + 68) 1092 are used
valid_WWL = [repmat(repeatableUnit, [1 32]) zeros(1, 2) CalibWWL];
indices = find(valid_WWL == 0); 
% making it 0 indexed
indices = indices - 1;
clearvars CalibWWL repeatableUnit

% data_DL 182 X 6
% Generate random numbers between 0 and 255
randomNumbers = randi([0, 255], 1, 182);
data_DL = dec2bin(randomNumbers) - '0';
dl_row = 0;
dl_row_arr = [];
for i=1:size(data_DL, 1)
    % sevenBitsObj = get7BitsFrom1Byte(data_DL[i]);
    for j=size(data_DL, 2)-2:-1:1
    %     if(sevenBitsObj.bits[j]==1)
	%     {
	% 	    data_DLbitwiseWithUnused[dl_row]=1;
	%     }
	%     else
	%     {
	% 	    data_DLbitwiseWithUnused[dl_row]=0;
	%     }
        dl_row_arr = [ dl_row_arr dl_row];
        if mod(dl_row, 34) == 31 && dl_row ~= 1085 && dl_row < 1089
            dl_row = dl_row + 3;
        elseif dl_row == 1085
            dl_row = dl_row + 5;
        elseif dl_row ~= 1157
	        dl_row = dl_row + 1;
        end
    end
end
