/*
 * scanInWriteScanChains_maskOnly_uncompressed_FAST.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */

#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInWriteScanChains_maskOnly_uncompressed_FAST(curr_obj currObj,u8 *mask, int mask_len)
{
	u8 scanChain;
	scanChain=WRITE_2_SC;//WRITE_2_SC
	currObj=scanInVector_uncompressed_FAST(currObj,mask,mask_len,scanChain);
	return currObj;
}


