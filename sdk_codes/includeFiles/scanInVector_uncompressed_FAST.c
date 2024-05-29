#include <custom_h.h>
curr_obj scanInVector_uncompressed_FAST(curr_obj currObj,u8 *vecScanIn, int len_vecScanIn, u8 scanChain)
{
//scanInVectorExpanded in-line

	u32 dataWrite;
	u16 numScanInAtOnce_HW_setting=256;
	u8 remainder=len_vecScanIn%numScanInAtOnce_HW_setting;
	u8 num_Chunks=(len_vecScanIn-remainder)/numScanInAtOnce_HW_setting;
	if(remainder>0)
		num_Chunks++;

	currObj=scanSelect(currObj,scanChain);
	// must set the following signals from C
		u8 CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB;

	// 1. set the values of CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB
		CLK_A_LB=2;
		CLK_A_UB=3;
		CLK_B_LB=6;
		CLK_B_UB=7;

		set_fastScan_Width_GPIO(currObj,CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB);

	// 2. make reset_fastScanIn=1*
		u32 reset_fastScan=1;
		XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_RESET_FASTSCAN_Inst,1,reset_fastScan);
	// 3. // Taking control from C: FASTSCAN_EXT_LOCB = 0*
		// Making sure the fastscanin verilog code drives the scan pins: SCANOUT_INB=0*,
		// default Setting S2P scan settings: numScanInAtOnce=1*
		u8 FASTSCAN_EXT_LOCB=0;
		u8 SCANOUT_INB=0;
		u16 numScanInAtOnce=1;
		set_fastScan_Control_GPIO(currObj,FASTSCAN_EXT_LOCB,SCANOUT_INB,numScanInAtOnce);


	u8 *vecScanIn_chunk;
	u16 elementsLeft ;
	u8 bank_count=0;
	u8 limit;
	numScanInAtOnce=numScanInAtOnce_HW_setting;
	int i,j,j1,k;
if( len_vecScanIn == 1 )
{
	// in this case set bank 0 channel=0 to the value of vecScanIn[0] and then simply scanIn
	bank_count=0;
	dataWrite = (u32)vecScanIn[0];
	XGpio_DiscreteWrite(&currObj.GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[bank_count],1,dataWrite);// channel 1 is lower bits
	pulse01_fastScan_reset_GPIO(currObj);
}
else
{
	for( i=0 ; i<num_Chunks ; i++)
	{
		elementsLeft = len_vecScanIn-(i*numScanInAtOnce);

		// vecScanIn_chunk[end] should be the first scanned in bit, which is LSB of dataWrite
		// 4. set the value of numScanInAtOnce = min(length of the subarray of vecScanIn, numScanInAtOnce)
		if( elementsLeft >= numScanInAtOnce )
		{
			vecScanIn_chunk = &vecScanIn[elementsLeft-numScanInAtOnce];
			numScanInAtOnce = numScanInAtOnce_HW_setting;
		}
		else
		{
			vecScanIn_chunk = &vecScanIn[0];
			numScanInAtOnce = elementsLeft ;
		}

		bank_count = 0 ;
		for(j=numScanInAtOnce;j>0;j-=64,bank_count++)//j is how many elements are left in vecScanIn_chunk
		{
			// 5.1.construct the GPIO values for P for the numScanInAtOnce subarray of vecScanIn to be scanned in
				dataWrite=0;
				limit=(j>=32?32:j);//max(j,32)
				for( k=0; k<limit ; k++ )//j-32 to j-1
					dataWrite+=( ( (u32) vecScanIn_chunk[j-1-k] )<<k ) ;
			// 5.2. set the GPIOs for dataWrite to the constructed values
				XGpio_DiscreteWrite(&currObj.GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[bank_count],1,dataWrite);// channel 1 is lower bits
			j1=j-32;
			// 5.3. construct the GPIO values for P for the numScanInAtOnce subarray of vecScanIn to be scanned in
				dataWrite=0;
				limit=(j1>=32?32:j1);//max(j1,32)
				for( k=0; k<limit ; k++ )//j1-32 to j1-1
					dataWrite+=( ( (u32) vecScanIn_chunk[j1-1-k] )<<k ) ;
			// 5.4. set the GPIOs for dataWrite to the constructed values
				XGpio_DiscreteWrite(&currObj.GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[bank_count],2,dataWrite);// channel 2 is higher bits
		}


		// 6. // Setting S2P scan settings: numScanInAtOnce=numScanInAtOnce*
			FASTSCAN_EXT_LOCB=0; // maintaining verilog control
			SCANOUT_INB=0; // maintaining fastscanin code control
			numScanInAtOnce=numScanInAtOnce;
			set_fastScan_Control_GPIO(currObj,FASTSCAN_EXT_LOCB,SCANOUT_INB,numScanInAtOnce);

		// 7. pulse01 reset_fastScanIn with handshaking
			pulse01_fastScan_reset_GPIO(currObj);
	}
}

// 8. make reset_fastScanIn=1*
	reset_fastScan=1;
	XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_RESET_FASTSCAN_Inst,1,reset_fastScan);
// Returning control to C: FASTSCAN_EXT_LOCB = 1*
	FASTSCAN_EXT_LOCB=1;
	SCANOUT_INB=0;// this doesn't matter as C now has control
	numScanInAtOnce=1;// returning it to the default setting
	set_fastScan_Control_GPIO(currObj,FASTSCAN_EXT_LOCB,SCANOUT_INB,numScanInAtOnce);
//Default selection of scanChain=SA_SC;//scanChain=0
currObj=scanSelect(currObj,SA_SC);
//end of scanInVector in-line
return currObj;
}
