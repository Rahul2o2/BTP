/*
 * perform_RxOnRowCS_FPGA.c
 *
 *  Created on: Jul 21, 2023
 *      Author: ashlo
 */
#include <custom_h.h>
#include "XGpio.h"

scan_out perform_RxOnRowColSel_FPGA(curr_obj currObj,u8 *BL, u16 WLnum )
{
	scan_out scanOut;
// Scan In on WL Scan Chain to select WLnum
	currObj=scanInWLSC_toSelectWLnum( currObj, WLnum ) ;
// load the cell data to the SA counter
    int bitLines_len = 1 ;
	currObj=loadCelltoSACtr_FPGA( currObj, BL, bitLines_len ) ;
// Scan out	the Counter Data
		u8 scanChain=SA_SC;
	scanOut=scanOutVector( currObj, scanChain ) ;
	return scanOut;
}

