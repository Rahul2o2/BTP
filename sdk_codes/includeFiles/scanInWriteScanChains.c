/*
 * scanInWriteScanChains.c
 *
 *  Created on: 23-Nov-2021
 *      Author: ashlo
 */


#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInWriteScanChains(curr_obj currObj,u8 *data, int data_len, u8 numPadZeroes_data,u8 *mask, int mask_len, u8 numPadZeroes_mask)
{
//	Scan in WRITE_1_SC-----> BLB => data-Q write on SRAM->> length =97
	u8 scanChain=WRITE_1_SC;//WRITE_1_SC
//	int data_len=sizeof(data)/sizeof(data[0]);
	//int data_len=4;
	currObj=scanInVector(currObj, data, data_len, numPadZeroes_data, scanChain);
	int k;
	u8 inputByte;
//	Scan in WRITE_0_SC-----> BL => data-QB write on SRAM->> length =97
	scanChain=WRITE_0_SC;//WRITE_0_SC
	seven_bits sevenBitsObj;
	u8 compl_bits[6];
	int l;
	for(k=0;k<data_len;k++)
	{
		inputByte = data[k] ;
		sevenBitsObj=get7BitsFrom1Byte( inputByte );
		for( l = 0 ; l < 6 ; l ++ )
			{
				compl_bits[l] = 1-sevenBitsObj.bits[l] ;
			}
		data[k] = ( compl_bits[5] * 32 ) + ( compl_bits[4] * 16 ) + ( compl_bits[3] * 8 ) + ( compl_bits[2] * 4 ) + ( compl_bits[1] * 2 ) + ( compl_bits[0] * 1 ) ;
	}
	currObj=scanInVector(currObj, data, data_len, numPadZeroes_data, scanChain);
//	Scan in WRITE_2_SC-----> Active High Select to write on the particular Dataline or not->> length =97
	scanChain=WRITE_2_SC;//WRITE_2_SC
	currObj=scanInVector(currObj,mask,mask_len,numPadZeroes_mask,scanChain);
	return currObj;
}


