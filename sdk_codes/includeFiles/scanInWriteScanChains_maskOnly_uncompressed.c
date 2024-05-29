/*
 * scanInVector.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Sri Hari
 */

#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInWriteScanChains_maskOnly_uncompressed(curr_obj currObj,u8 *mask, int mask_len)
{
	u8 scanChain;
	scanChain=WRITE_2_SC;//WRITE_2_SC
	currObj=scanInVector_uncompressed(currObj,mask,mask_len,scanChain);
	return currObj;
}
