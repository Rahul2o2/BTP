/*
 * bitsGet.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Ashwin Balagopal
 */



#include <custom_h.h>

u8 bitsGet(curr_obj currObj,u8 bank)
{
	u32 dataRead;
	switch(bank)
		{
		case 1:
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_32OUTPUTS_1_Inst, 1);
				break;
			}
		case 2:
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_32INPUTS_2_Inst, 1);
				break;
			}
		case 3:
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_32OUTPUTS_2_Inst, 1);
				break;
			}
		case GPIO_BANKHS_CASENUM://DONE signal given by custom_h defined variable whose value is 4
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_HS_Inst, 1);
				break;
			}
		case GPIO_BANK_HS_Relaxed_CASENUM://DONE signal given by custom_h defined variable whose value is 5
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_HS_Relaxed_Inst, 1);
				break;
			}
		case 100:
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_32OUTPUTS_100_Inst, 1);
				break;
			}
		default://By default read out DONE signal
			{
				dataRead=(u32)XGpio_DiscreteRead(&currObj.GPIO_PCB_HS_Inst, 1);
				break;
			}
		}
	return dataRead;
}
