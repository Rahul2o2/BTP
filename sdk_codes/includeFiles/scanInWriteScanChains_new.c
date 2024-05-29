/*
 * scanInWriteScanChains_new.c
 *
 *  Created on: May 11, 2024
 *      Author: asreddy
 */



#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInWriteScanChains_new(curr_obj currObj,u8 *data, int data_len, u8 *mask, int mask_len)
{
//	Scan in WRITE_1_SC-----> BLB => data-Q write on SRAM->> length =97
	u8 scanChain=WRITE_1_SC;//WRITE_1_SC
//	int data_len=sizeof(data)/sizeof(data[0]);
	//int data_len=4;
	currObj=scanInVector_uncompressed_FAST(currObj, data, data_len, scanChain);
	int k;
	u8 inputByte;
//	Scan in WRITE_0_SC-----> BL => data-QB write on SRAM->> length =97
	scanChain=WRITE_0_SC;//WRITE_0_SC
	for(k=0;k<data_len;k++)
		{
			data[k] = 1-data[k];
		}
	currObj=scanInVector_uncompressed_FAST(currObj, data, data_len, scanChain);
//	Scan in WRITE_2_SC-----> Active High Select to write on the particular Dataline or not->> length =97
	scanChain=WRITE_2_SC;//WRITE_2_SC
	currObj=scanInVector_uncompressed_FAST(currObj, mask, mask_len, scanChain);
	return currObj;
}


