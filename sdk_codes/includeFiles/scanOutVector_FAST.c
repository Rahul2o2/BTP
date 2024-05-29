#include <custom_h.h>
#include "XGpio.h"

scan_out scanOutVector_FAST(curr_obj currObj,u8 scanChain)
{
	// Select the appropriate scan chain
		currObj=scanSelect(currObj,scanChain);

	// Get the length of the scan chain
		u16 scan_len=get_scan_len( scanChain );

	// Size of synthesised S2P's buffer: P_SCANOUT
		u16 numScanInAtOnce_HW_setting=256;
	// Necessary variables
		u8 FASTSCAN_EXT_LOCB,SCANOUT_INB;
	// How many chunks will the chip scan chain be scanned out as
		u8 remainder=scan_len%numScanInAtOnce_HW_setting;
		u8 num_Chunks=(scan_len-remainder)/numScanInAtOnce_HW_setting;
		if(remainder>0)
			num_Chunks++;
	// Each vecScanOut_chunk represents a P_SCANOUT that is serial loaded from SC_QOUT and available to be read as parallel bits through GPIO banks
	// The LSB of P_SCANOUT is connected to SC_QOUT

	// variables used in loop (starts)
		u32 dataRead_MSDWORD,dataRead_LSDWORD;
		u16 numbitsLeft_inPSCANOUT, num_bits_in_bank, numUsefulBits_inChnl1,numUsefulBits_inChnl2;
		int i,j,j1,k;
		u8 bank_count;
		u8 *vecScanOut_chunk;
		u8 vecScanOut[scan_len];
	// variables used in loop (ends)
	// 1. set the values of CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB
		u8 CLK_A_LB=3;
		u8 CLK_A_UB=5;
		u8 CLK_B_LB=9;
		u8 CLK_B_UB=11;

		set_fastScan_Width_GPIO(currObj,CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB);
	// make reset_fastScanIn=1* before starting this (for safety)
		u32 reset_fastScan=1;
		XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_RESET_FASTSCAN_Inst,1,reset_fastScan);


	// How many bits in the scan chain and how many can be scanned out to the S2P
		u16 numbitsLeft_inChipScanChain=scan_len;
		u16 numScanInAtOnce = (u16) min( numScanInAtOnce_HW_setting, numbitsLeft_inChipScanChain ) ;
	// This loop populates vecScanOut numScanInAtOnce bits per iteration, for num_Chunks iterations
	for( i=0 ; i<num_Chunks ; i++)
	{
		// Taking control from C: FASTSCAN_EXT_LOCB = 0*
		// Making sure the fastscanout verilog code drives the scan pins: SCANOUT_INB=1*,
		// Setting S2P scan settings: numScanInAtOnce=numScanInAtOnce*
			   FASTSCAN_EXT_LOCB=0;
			   SCANOUT_INB=1;
			   numScanInAtOnce=numScanInAtOnce;
			set_fastScan_Control_GPIO(currObj,FASTSCAN_EXT_LOCB,SCANOUT_INB,numScanInAtOnce);
		// scanning out the first set of bits (with handshaking)
			pulse01_fastScan_reset_GPIO(currObj);
		// Pointing vecScanOut_chunk to the right index in vecScanOut
			vecScanOut_chunk = &vecScanOut[numbitsLeft_inChipScanChain-numScanInAtOnce];
		// each bank is a dual channel GPIO i.e. 64 bits
			bank_count = 0 ;// starting with GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[0]
		// This loop populates vecScanOut_chunk 64 bits per iteration
			for(j=0;j<numScanInAtOnce;j+=64,bank_count++)// j is how many bits we've populated in vecScanOut_chunk
			{
				// if numScanInAtOnce = 33, j=0, bank_count=0 then numUsefulBits_inChnl1 = 32 numUsefulBits_inChnl2 =  1
				// if numScanInAtOnce = 65, j=0, bank_count=0 then numUsefulBits_inChnl1 = 32 numUsefulBits_inChnl2 = 32
				// if numScanInAtOnce = 65, j=64, bank_count=0 then numUsefulBits_inChnl1 =  1 numUsefulBits_inChnl2 =  0
				numbitsLeft_inPSCANOUT = numScanInAtOnce - j ;
				num_bits_in_bank = (u16) min( numbitsLeft_inPSCANOUT, 64 ) ;
				numUsefulBits_inChnl1 = (u16) min( num_bits_in_bank, 32 ) ;
				numUsefulBits_inChnl2 = num_bits_in_bank - numUsefulBits_inChnl1 ;
				if( numUsefulBits_inChnl2 > 0 ){
					dataRead_MSDWORD=XGpio_DiscreteRead(&currObj.GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK_Inst[bank_count], 2);
					get32BitsFrom1DWORD(dataRead_MSDWORD,&vecScanOut_chunk[j+32],numUsefulBits_inChnl2);
				}
				if( numUsefulBits_inChnl1 > 0 ){
					dataRead_LSDWORD=XGpio_DiscreteRead(&currObj.GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK_Inst[bank_count], 1);
					get32BitsFrom1DWORD(dataRead_LSDWORD,&vecScanOut_chunk[j],numUsefulBits_inChnl1);
				}
			}
		// Accounting for the bits scanned out of chip
			numbitsLeft_inChipScanChain-=numScanInAtOnce;
		// Updating the #bits to be scanned out in the next iteration
			numScanInAtOnce = (u16) min( numScanInAtOnce_HW_setting, numbitsLeft_inChipScanChain ) ;
	}
	// 8. make reset_fastScanIn=1* (for safety)
		reset_fastScan=1;
		XGpio_DiscreteWrite(&currObj.GPIO_FASTSCAN_RESET_FASTSCAN_Inst,1,reset_fastScan);
	// Returning control to C: FASTSCAN_EXT_LOCB = 1*
		FASTSCAN_EXT_LOCB=1;
		SCANOUT_INB=0;// this doesn't matter as C now has control
		numScanInAtOnce=1;// returning it to the default setting
		set_fastScan_Control_GPIO(currObj,FASTSCAN_EXT_LOCB,SCANOUT_INB,numScanInAtOnce);
	//
	u8 inputPin;
	inputPin=SC_DIN;
	{
		{
			u8 signals[1]={inputPin};
			u8 values[1];
			values[0] = 0 ;//this is what will overwrite the scanChain

			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
	}
	{
		u8 signals[3]={CLK_A,CLK_B,inputPin};
		u8 values[3]={0,0,0};
		int signals_len = sizeof(signals) / sizeof(signals[0]) ;
			currObj=bitsSet(currObj, signals, signals_len, values);
	}
	scan_out scanOut ;
	scanOut.currObj=currObj;
// Populate the appropriate attribute in scanOut with vecScanOut
	scanOut = populate_scanout( scanOut, vecScanOut, scan_len, scanChain ) ;
	currObj=scanOut.currObj;
//	Restoring default scan select
	scanChain=SA_SC;
	currObj=scanSelect(currObj,scanChain);
	scanOut.currObj=currObj;
//end of scanOutVector expanded in-line
	return scanOut;
}
