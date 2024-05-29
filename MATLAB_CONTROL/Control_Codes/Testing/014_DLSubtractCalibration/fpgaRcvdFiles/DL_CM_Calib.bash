#!/bin/bash
cat ./fpgaRcvdFiles/DL_CM_Calib_receive.txt | od -An -vtu1 >  ./fpgaRcvdFiles/DL_CM_Calib_receive_temp_1
perl -pe 's/^\s*//g' ./fpgaRcvdFiles/DL_CM_Calib_receive_temp_1|perl -pe 's/\n/ /g' |perl -pe 's/ 124//g' |perl -pe 's/(.*?127.*?127)\s*/\1\n/g' |perl -pe 's/(\w*)\s*/\1\n/' |perl -pe 's/(.*?127)\s*/\1\n/g'  > ./fpgaRcvdFiles/DL_CM_Calib_receive_temp_2