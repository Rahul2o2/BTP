/*
 * pulseFPGARESET_handshake.c
 *
 *  Created on: 29-Oct-2021
 *      Author: ashlo
 */

#include <custom_h.h>

void pulse01FPGA_RESETRelaxed_handshake(curr_obj currObj)
{
	{
			u8 signals[1]={ FPGA_RESET } ;
			u8  values[1]={     0      } ;
			int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
		currObj=bitsSet(currObj, signals, signals_len, values) ;
	}
	u8 doneBit=0;
	while( doneBit==0 )
	{
			u8 bank=GPIO_BANK_HS_Relaxed_CASENUM;
		doneBit=bitsGet(currObj,bank);
	}
	{
			u8 signals[1]={ FPGA_RESET } ;
			u8  values[1]={     1      } ;
			int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
		currObj=bitsSet(currObj, signals, signals_len, values) ;
	}
}
