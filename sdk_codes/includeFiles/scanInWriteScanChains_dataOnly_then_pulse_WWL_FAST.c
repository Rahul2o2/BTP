/*
 * scanInWriteScanChains_dataOnly_then_pulse_WWL_FAST.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */


#include <custom_h.h>
#include "XGpio.h"

curr_obj scanInWriteScanChains_dataOnly_then_pulse_WWL_FAST(curr_obj currObj,u8 *vecScanIn, int len_vecScanIn)
{
//scanInVectorExpanded in-line

	u32 dataWrite;

	u8 bank_count=0;
	u8 limit;
	int j,j1,k;

	bank_count = 0 ;
	// for(j=numScanInAtOnce;j>0;j-=64,bank_count++)//j is how many elements are left in vecScanIn_chunk
	for(j=len_vecScanIn;j>0;j-=64,bank_count++)//j is how many elements are left in vecScanIn_chunk
	{
		// 5.1.construct the GPIO values for P for the numScanInAtOnce subarray of vecScanIn to be scanned in
			dataWrite=0;
			limit=(j>=32?32:j);//max(j,32)
			for( k=0; k<limit ; k++ )//j-32 to j-1
				dataWrite+=( ( (u32) vecScanIn[j-1-k] )<<k ) ;
		// 5.2. set the GPIOs for dataWrite to the constructed values
			XGpio_DiscreteWrite(&currObj.GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[bank_count],1,dataWrite);// channel 1 is lower bits
		j1=j-32;
		// 5.3. construct the GPIO values for P for the numScanInAtOnce subarray of vecScanIn to be scanned in
			dataWrite=0;
			limit=(j1>=32?32:j1);//max(j1,32)
			for( k=0; k<limit ; k++ )//j1-32 to j1-1
				dataWrite+=( ( (u32) vecScanIn[j1-1-k] )<<k ) ;
		// 5.4. set the GPIOs for dataWrite to the constructed values
			XGpio_DiscreteWrite(&currObj.GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[bank_count],2,dataWrite);// channel 2 is higher bits
	}

	// 7. pulse01 reset_fastScanIn with handshaking
		pulse01_fastScan_reset_GPIO_SPL(currObj);

return currObj;
}
