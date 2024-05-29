/*
 * init_Internal.c
 *
 *  Created on: Aug 31, 2023
 *      Author: ashlo
 */

#include <custom_h.h>
curr_obj init_internal_2X(curr_obj currObj)
{

	//1. ADC scan Chain Initialised with all Zeroes
	{
		u8 scanChain=ADC_SC;
		int len_vecScanIn=scan_len_ADC_SC;
		u8 vecScanIn[scan_len_ADC_SC];
		memset(vecScanIn,0,scan_len_ADC_SC*sizeof(u8));
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//2. ControlPath_SC ENABLED CONFIGURATION- CM-2X, SUB, Ref-1X, Left Track
	{
		u8 scanChain=ControlPath_SC;
		int len_vecScanIn=scan_len_ControlPath_SC;
		u8 vecScanIn[scan_len_ControlPath_SC]={0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1};
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//3. FLOATADC_SC
	{
		u8 scanChain=FLOATADC_SC;
		int len_vecScanIn=scan_len_FLOATADC_SC;
		u8 vecScanIn[scan_len_FLOATADC_SC];
		memset(vecScanIn,1,scan_len_FLOATADC_SC*sizeof(u8));
		for(int i=0;i<WDL_HW;i++)
		{
			vecScanIn[ADC_outs_total[map_to_72[i]]]=0;
		}
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//4. IMC_RBL_SC
	{
		u8 scanChain=IMC_RBL_SC;
		int len_vecScanIn=scan_len_IMC_RBL_SC;
		u8 vecScanIn[scan_len_IMC_RBL_SC];
		memset(vecScanIn,0,scan_len_IMC_RBL_SC*sizeof(u8));
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//5. IMC_VOut_SC
	{
		u8 scanChain=IMC_VOut_SC;
		int len_vecScanIn=scan_len_IMC_VOut_SC;
		u8 vecScanIn[scan_len_IMC_VOut_SC];
		memset(vecScanIn,0,scan_len_IMC_VOut_SC*sizeof(u8));
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//6. SRAM_calib Populate for 2X SUB Left Track
	{
//Slopes of CM and SUB adjusted to mean slope pattern
//		u8 Fine_cm[88]=       {3,8,6,9,8,7,7,7,7,9,6,8,7,7,6,5,5,6,0,0,0,0,0,0,0,0,5,13,6,8,7,7,9,8,4,4,6,6,7,4,7,7,6,6,6,7,5,6,2,11,9,7,3,6,4,6,7,9,6,8,6,7,0,0,0,0,0,0,0,0,11,7,7,5,9,6,5,10,8,8,2,8,4,5,7,8,8,6};
//		u8 Coarse_cm[88] =    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
//		u8 Fine_sub[88]=      {6,7,8,7,9,8,6,7,7,7,8,6,7,6,7,6,7,5,15,15,15,15,15,15,15,15,7,6,7,6,7,6,6,8,8,6,5,7,5,7,8,7,6,8,8,9,5,7,8,6,8,8,7,9,8,7,8,7,8,8,8,5,0,0,0,0,0,0,0,0,7,8,7,6,8,7,10,6,8,8,6,6,6,5,7,6,6,6};
//		u8 Coarse_sub[88]=    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};

// Slopes of CM adjusted to mean slope and SUB adjusted to m2=1 pattern
// FROM D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\028_SystemMAC_DryRuns\CURRENTMIRRORCalibPattern\OUT_2X.txt
		u8 Fine_cm[88]=       {3,8,6,9,8,7,7,7,7,9,6,8,7,7,6,5,5,6,0,0,0,0,0,0,0,0,5,13,6,8,7,7,9,8,4,4,6,6,7,4,7,7,6,6,6,7,5,6,2,11,9,7,3,6,4,6,7,9,6,8,6,7,0,0,0,0,0,0,0,0,11,7,7,5,9,6,5,10,8,8,2,8,4,5,7,8,8,6};
		u8 Fine_sub[88]=      {4,5,8,7,6,5,6,7,6,7,8,6,4,7,6,6,6,5,15,15,15,15,15,15,15,15,4,6,6,6,7,5,5,5,5,7,5,7,5,6,7,6,6,7,7,9,4,4,7,5,7,7,6,8,5,6,7,4,5,5,7,5,0,0,0,0,0,0,0,0,7,7,7,6,5,6,7,6,7,7,6,6,5,5,6,5,3,6};
		u8 Coarse_cm[88] =    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
		u8 Coarse_sub[88]=    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

		currObj=fine_coarseSRAM(currObj,Fine_cm,Coarse_cm,Fine_sub,Coarse_sub);
	}

	//7. Internal mode in SARTIMING_IN_SC scan chain
	{
		u8 scanChain=SARTIMING_IN_SC;
		int len_vecScanIn=scan_len_SARTIMING_IN_SC;
		//6-Bit ADC Set in Internal Mode
		u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//8. Adder Tree Enable
	{
	u8 signals[1]={ADCSTART_EXT};
	int signals_len=1;
	u8 values_bytes[1]={1};
	currObj=bitsSet(currObj, signals, signals_len, values_bytes);
	}

	return currObj;
}
