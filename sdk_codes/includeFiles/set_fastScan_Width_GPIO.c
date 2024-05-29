/*
 * set_fastScan_Width_GPIO.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */

#include <custom_h.h>
void set_fastScan_Width_GPIO(curr_obj currObj, u8 CLK_A_LB,u8 CLK_A_UB,u8 CLK_B_LB,u8 CLK_B_UB)
{
	u32 dataWrite;
	dataWrite=0;
		dataWrite+=( ((u32)CLK_A_LB)<<0 )+
		   ( ((u32)CLK_A_UB)<<8 )+
		   ( ((u32)CLK_B_LB)<<16 )+
		   ( ((u32)CLK_B_UB)<<24 );
		XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_GPIO_WIDTH_SETTINGS_Inst,1,dataWrite);
}


