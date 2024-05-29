/*
 * set_HYBRIDPIPELINETOPSW_VARS_GPIO.c
 *
 *  Created on: Oct 26, 2023
 *      Author: ashlo
 */

//    assign preADC_Time=GPIO_HYBRIDPIPELINETOPSW_VARS[7:0];
//    assign ADCsample_Time=GPIO_HYBRIDPIPELINETOPSW_VARS[15:8];
//    assign ADCtotal_Time=GPIO_HYBRIDPIPELINETOPSW_VARS[23:16];
//    assign CLKDFF_Time=GPIO_HYBRIDPIPELINETOPSW_VARS[31:24];
#include <custom_h.h>
void set_HYBRIDPIPELINETOPSW_VARS_GPIO(curr_obj currObj, u8 RWL_EN_DELAY,u8 CM_EN_DELAY,u8 ADCSTART_DELAY,u8 SAMPLING_DELAY, u8 EVALUATE_DELAY)
{
	u32 dataWrite;
	dataWrite=0;
		dataWrite+=( ((u32)RWL_EN_DELAY)<<0 )+
		   ( ((u32)CM_EN_DELAY)<<8 )+
		   ( ((u32)ADCSTART_DELAY)<<16 )+
		   ( ((u32)SAMPLING_DELAY)<<24 );
		XGpio_DiscreteWrite(&currObj.GPIO_HYBRIDPIPELINETOPSW_VARS_Inst,1,dataWrite);//GPIO_1 Channel-1

	dataWrite=0;
		dataWrite+=( ((u32)EVALUATE_DELAY)<<0 );
				XGpio_DiscreteWrite(&currObj.GPIO_HYBRIDPIPELINETOPSW_VARS_Inst,2,dataWrite);//GPIO_2 Channel-2
}


//RWL_EN_DELAY,CM_EN_DELAY,ADCSTART_DELAY,SAMPLING_DELAY,EVALUATE_DELAY

