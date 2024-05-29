/*
 * scanInWriteScanChains_dataOnly_uncompressed_FAST.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */


#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInWriteScanChains_dataOnly_uncompressed_FAST(curr_obj currObj,u8 *data, int data_len)
{
//	Scan in WRITE_1_SC-----> BLB = data ->> length =97
	u8 scanChain=WRITE_1_SC;//WRITE_1_SC
//	int data_len=sizeof(data)/sizeof(data[0]);
	//int data_len=4;
	currObj=scanInVector_uncompressed_FAST(currObj,data,data_len,scanChain);
	int k;
	//u8 inputByte;
//	Scan in WRITE_0_SC-----> BL= ~data -> length =97
	scanChain=WRITE_0_SC;//WRITE_0_SC
	u8 data_comp[97] ;

	//int l;
	for(k=0;k<data_len;k++)
	{
		//inputByte = data[k] ;
		data_comp[k] = 1 - data[k];
	}
	currObj=scanInVector_uncompressed_FAST(currObj,data_comp,data_len,scanChain);
	return currObj;
}


