#!/bin/bash
cat ./fpgaRcvdFiles/_receive.txt | od -An -vtu1 >  ./fpgaRcvdFiles/_receive_temp_1
perl -pe 's/^\s*//g' ./fpgaRcvdFiles/_receive_temp_1|perl -pe 's/\n/ /g' |perl -pe 's/ 124//g' |perl -pe 's/(.*?127.*?127)\s*/\1\n/g' |perl -pe 's/(\w*)\s*/\1\n/' |perl -pe 's/(.*?127)\s*/\1\n/g'  > ./fpgaRcvdFiles/_receive_temp_2