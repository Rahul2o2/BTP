/*
 * scanInVector_uncompressed.c
 *
 *  Created on: Mar 27, 2021
 *      Author: Sri Hari
 */

#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInVector_uncompressed(curr_obj currObj,u8 *vecScanIn, int len_vecScanIn, u8 scanChain)
{
//scanInVectorExpanded in-line
int i;
u8 inputPin;
//u8 len_vecScanIn;

currObj=scanSelect(currObj,scanChain);

inputPin=SC_DIN;

for( i=(len_vecScanIn-1) ; i>=0 ; i--) //changed len_vecScanIn to len_vecScanIn-1
{
		{
			u8 signals[3] = { CLK_A, CLK_B, inputPin } ;
			u8 values[3] = { 0, 0, vecScanIn[i] } ;//sevenBitsObj.bits[6] is MSB-1 of vecScanIn[i] byte(which is in left-MSB) sevenBitsObj.bits[0] is LSB of vecScanIn[i] byte, we are interested in sending in as scan-in waveform vecScanIn[i] from LSB to (MSB-2) (which corresponds to output flop to input flop scanning) this corresponds to sevenBitsObj.bits[from 0 to 5]
			int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
				currObj=bitsSet(currObj, signals, signals_len, values) ;
		}
		{
			u8 signals[1] = { CLK_A } ;
			u8 values[1] = { 1 } ;
			int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1] = { CLK_A } ;
			u8 values[1] = { 0 } ;
			int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1] = { CLK_B } ;
			u8 values[1] = { 1 } ;
			int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
}

{
	u8 signals[3] = { CLK_A, CLK_B, inputPin } ;
	u8 values[3] = { 0, 0, 0 };
	int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
		currObj=bitsSet( currObj, signals, signals_len, values );
}
//Default selection of scanChain=SA_SC;//scanChain=0
currObj=scanSelect(currObj,SA_SC);
//end of scanInVector in-line
	return currObj;
}
