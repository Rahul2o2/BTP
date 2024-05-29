/*
 * set_fastScan_Control_GPIO.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */
#include <custom_h.h>
void set_fastScan_Control_GPIO(curr_obj currObj, u8 FASTSCAN_EXT_LOCB, u8 SCANOUT_INB, u16 numScanInAtOnce)
{
	u32 dataWrite;
	dataWrite=0;
	dataWrite+=( ((u32)FASTSCAN_EXT_LOCB)<<0 )+
	   ( ((u32)SCANOUT_INB)<<1 )+
	   ( ((u32)numScanInAtOnce)<<2 );
	XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_GPIO_CONTROL_SETTINGS_Inst,1,dataWrite);
}
void set_fastScan_Control_SPL_GPIO(curr_obj currObj, u8 ScanSel_WWL_SC, u8 ScanSel_WRITE_1_SC, u8 ScanSel_WRITE_0_SC, u8 ScanSel_SC_DEFAULT, u8 SCANOUT_SPL)
{
	u32 dataWrite;
	dataWrite=0;
	dataWrite+=( ((u32)ScanSel_WRITE_1_SC)<<0 )+
	   ( ((u32)ScanSel_WRITE_0_SC)<<4 )+
	   ( ((u32)ScanSel_SC_DEFAULT)<<8 )+
	   ( ((u32)ScanSel_WWL_SC)<<12 )+
	   ( ((u32)SCANOUT_SPL)<<16 );
	XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_GPIO_CONTROL_SETTINGS_Inst,2,dataWrite);
}
