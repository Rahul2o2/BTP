/*
 * perform_WxOnRowColSel_FPGA_2_new.c
 *
 *  Created on: May 12, 2024
 *      Author: asreddy
 */





#include <custom_h.h>
#include "XGpio.h"

curr_obj perform_WxOnRowColSel_FPGA_2_new(curr_obj currObj, u8 *data, int data_len, u8 *mask, int mask_len, u8 *BL, u16 WLnum )
{
// Scan In on WRITE_2 WRITE_1 WRITE_0 data and mask
	currObj=scanInWriteScanChains_new(currObj, data, data_len, mask, mask_len);


// Scan In on WL Scan Chain to select WLnum
	currObj=scanInWLSC_toSelectWLnum( currObj, WLnum ) ;

// load the data into the cell
	int bitLines_len = 1 ;
	currObj=loadBitlinetoCell_FPGA(currObj,BL, bitLines_len);

	return currObj;
}
