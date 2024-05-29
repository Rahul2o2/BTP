/*
 * pulse01_fastScan_reset_GPIO_SPL.c
 *
 *  Created on: Aug 24, 2023
 *      Author: ashlo
 */
#include <custom_h.h>
void pulse01_fastScan_reset_GPIO_SPL(curr_obj currObj)
{
	// make reset_fastScanIn=0*
	u32 reset_fastScan=0;
		XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_RESET_FASTSCAN_Inst,1,reset_fastScan);
	u32 SCANIN_DONE=0;
	// 9. Handshaking: wait for SCANIN_DONE=1
	while( SCANIN_DONE==0 )
	{
		u32 dataRead=XGpio_DiscreteRead(&currObj.GPIO_FASTSCAN_SCAN_DONE_Inst, 1);
		SCANIN_DONE=(dataRead<<(31-2))>>31;//SCANIN_DONE_SPL is bit posn 2
	}

	// 10. make reset_fastScanIn=1*
		reset_fastScan=1;
		XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_RESET_FASTSCAN_Inst,1,reset_fastScan);
}






