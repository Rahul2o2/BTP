/*
 * init_Internal.c
 *
 *  Created on: Aug 31, 2023
 *      Author: ashlo
 */

#include <custom_h.h>
curr_obj init_XADC(curr_obj currObj)
{

	//1. ADC scan Chain Initialised with all Zeroes
	{
		u8 scanChain=ADC_SC;
		int len_vecScanIn=scan_len_ADC_SC;
		u8 vecScanIn[scan_len_ADC_SC];
		memset(vecScanIn,0,scan_len_ADC_SC*sizeof(u8));
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//2. ControlPath_SC ENABLED CONFIGURATION- CM-1X,Ref-1X
	{
		u8 scanChain=ControlPath_SC;
		int len_vecScanIn=scan_len_ControlPath_SC;
		u8 vecScanIn[scan_len_ControlPath_SC]={0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1};
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//3. FLOATADC_SC
	{
		u8 scanChain=FLOATADC_SC;
		int len_vecScanIn=scan_len_FLOATADC_SC;
		u8 vecScanIn[scan_len_FLOATADC_SC];
		memset(vecScanIn,1,scan_len_FLOATADC_SC*sizeof(u8));
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

	//6. SRAM_calib Populate
	{
		u8 Fine_cm[88]=       {6,6,6,3,9,7,7,3,6,4,6,7,9,7,6,7,7,6,7,7,7,7,7,7,7,7,3,8,6,7,6,9,7,7,4,6,5,6,6,5,8,8,7,5,9,8,9,5,12,8,8,6,6,6,8,10,7,7,9,5,9,6,7,7,7,7,7,7,7,7,7,7,8,6,7,5,9,6,5,5,8,10,7,7,9,8,6,9};
		u8 Coarse_cm[88] =    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
		u8 Fine_sub[88]=      {6,7,8,7,9,8,6,7,7,7,8,6,7,6,7,6,7,5,7,7,7,7,7,7,7,7,7,6,7,6,7,6,6,8,8,6,5,7,5,7,8,7,6,8,8,9,5,7,8,6,8,8,7,9,8,7,8,7,8,8,8,5,7,7,7,7,7,7,7,7,7,8,7,6,8,7,10,6,8,8,6,6,6,5,7,6,6,6};
		u8 Coarse_sub[88]=    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
		currObj=fine_coarseSRAM(currObj,Fine_cm,Coarse_cm,Fine_sub,Coarse_sub);
	}

//Internal mode in SARTIMING_IN_SC scan chain has been moved to perform_IMC_Internal_pyTorch as it must happen at every function call of perform_IMC_Internal_pyTorch
//	//0. Internal mode in SARTIMING_IN_SC scan chain
//		{
//			u8 scanChain=SARTIMING_IN_SC;
//			int len_vecScanIn=scan_len_SARTIMING_IN_SC;
//			//6-Bit ADC Set in Internal Mode
//			u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//			currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
//		}
	return currObj;
}
