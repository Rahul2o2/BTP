/*
 * readBanks.c
 *
 *  Created on: Jun 10, 2023
 *      Author: ashlo
 */

#include <custom_h.h>
#include "xgpio.h"

curr_obj read_FMC_PINS(curr_obj currObj)
{
	u32 dataRead;

	dataRead=XGpio_DiscreteRead(&currObj.GPIO_PCB_32OUTPUTS_Inst, 1);
	u8 i;
	u8 bitShift;
	for(i=31;i>=0;i--)
		{
			bitShift=i;
			currObj.stringCurr[i]=extractBitFromDWORD(dataRead, bitShift);
		}

	dataRead=XGpio_DiscreteRead(&currObj.GPIO_PCB_32OUTPUTS_1_Inst, 1);
	for(i=63;i>=32;i--)
		{
			bitShift=i-32;
			currObj.stringCurr[i]=extractBitFromDWORD(dataRead, bitShift);
		}
	dataRead=XGpio_DiscreteRead(&currObj.GPIO_PCB_32OUTPUTS_2_Inst, 1);
	for(i=67;i>=64;i--)
		{
			bitShift=i-64;
			currObj.stringCurr[i]=extractBitFromDWORD(dataRead, bitShift);
		}

	return currObj;
}

