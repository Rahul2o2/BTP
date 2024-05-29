/*
 * scanInVector.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Sri Hari
 */

#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInVector(curr_obj currObj,u8 *vecScanIn,int len_vecScanIn, u8 numPadZeroes_vecScanIn,u8 scanChain)
{
//scanInVectorExpanded in-line
int i;
int k=0;
u8 startArrayIndex;
u8 inputByte ; u8 sevenBits[7];
currObj=scanSelect(currObj,scanChain);
seven_bits sevenBitsObj;
for(i=(len_vecScanIn-1);i>=0;i--)
//filling scanChain:
	// abcd[2]=vecScanIn[0] : last byte to be scanned in
	// As we traverse from LSB to MSB-2 of the byte, flop is filled from the output end to input end
{
	inputByte = vecScanIn[i] ;
	sevenBitsObj=get7BitsFrom1Byte( inputByte );
	for( int i1 = 0 ; i1 < 7 ; i1 ++  )
		sevenBits[i1] = sevenBitsObj.bits[i1] ;
	if(i==(len_vecScanIn-1))
		startArrayIndex=numPadZeroes_vecScanIn;
	else
		startArrayIndex=0;
	for(k=startArrayIndex;k<6;k++)//k traverses vecScanIn[i], which is in left-MSB format, from right to left, i.e. LSB (or) LSB+numPadZeroes_vecScanIn to MSB-2
	{
		{
			u8 signals[3]={CLK_A,CLK_B,SC_DIN};
			u8 values[3]={0,0,sevenBits[k]};//sevenBitsObj.bits[6] is MSB-1 of vecScanIn[i] byte(which is in left-MSB) sevenBitsObj.bits[0] is LSB of vecScanIn[i] byte, we are interested in sending in as scan-in waveform vecScanIn[i] from LSB to (MSB-2) (which corresponds to output flop to input flop scanning) this corresponds to sevenBitsObj.bits[from 0 to 5]
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1]={CLK_A};
			u8 values[1]={1};
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1]={CLK_A};
			u8 values[1]={0};
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1]={CLK_B};
			u8 values[1]={1};
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
	}
}
{
	u8 signals[3]={CLK_A,CLK_B,SC_DIN};
	u8 values[3]={0,0,0};
	int signals_len = sizeof(signals) / sizeof(signals[0]) ;
		currObj=bitsSet(currObj, signals, signals_len, values);
}
//Default selection of scanChain=SA_SC;//scanChain=0
currObj=scanSelect(currObj,SA_SC);
//end of scanInVector in-line
	return currObj;
}
