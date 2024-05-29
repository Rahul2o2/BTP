/*
 * bitsGet.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Ashwin Balagopal
 */



#include <custom_h.h>

curr_obj defaultState(curr_obj currObj)
{
//	signals and values obtained from https:https://docs.google.com/spreadsheets/d/1_dIfXRbsM9xImlrx0xJVdJuL3eZa-2ajl-Iui9fRETQ/edit#gid=956893993
	int i;
	for(i=0;i<stringCurr_LEN;i++)
		currObj.stringCurr[i]=currObj.stringCurr_DEFAULT[i];//RWL_EN_EXT

	printString(currObj);
	u8 scanChain = SARTIMING_IN_SC;
	currObj=scanSelect(currObj,scanChain);
	int len_vecScanIn=110;
	u8 vecScanIn_SARTIMING_IN_SC[110]={0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,0};
	currObj = scanInVector_uncompressed( currObj, vecScanIn_SARTIMING_IN_SC, len_vecScanIn, scanChain ) ;

	scanChain = ControlPath_SC;
	currObj=scanSelect(currObj,scanChain);
	len_vecScanIn=17;
	u8 vecScanIn_ControlPath_SC[17]={0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1};
	currObj = scanInVector_uncompressed( currObj, vecScanIn_ControlPath_SC, len_vecScanIn, scanChain ) ;

	//RESET_GLOBALTIMER Pulse 1-0-1
	//bitsSet value=1
	{
	u8 signals[1]={RESET_GLOBALTIMER};
	int signals_len=1;
	u8 values_bytes[1]={1};
	currObj=bitsSet(currObj, signals, signals_len, values_bytes);
	}
	//bitsSet value=0
	{
	u8 signals[1]={RESET_GLOBALTIMER};
	int signals_len=1;
	u8 values_bytes[1]={0};
	currObj=bitsSet(currObj, signals, signals_len, values_bytes);
	}
	//bitsSet value=1
	{
	u8 signals[1]={RESET_GLOBALTIMER};
	int signals_len=1;
	u8 values_bytes[1]={1};
	currObj=bitsSet(currObj, signals, signals_len, values_bytes);
	}

	//Reset DFF
	scanChain=Reset_DFF;
	currObj=scanSelect(currObj,scanChain);
	{
		u8 signals[1]={CLK_B};
		u8 values[1]={1};
		int signals_len = sizeof(signals) / sizeof(signals[0]) ;
			currObj=bitsSet(currObj, signals, signals_len, values);
	}
	{
		u8 signals[1]={CLK_B};
		u8 values[1]={0};
		int signals_len = sizeof(signals) / sizeof(signals[0]) ;
			currObj=bitsSet(currObj, signals, signals_len, values);
	}

	scanChain=SC_DEFAULT;
	currObj=scanSelect(currObj,scanChain);

	// 1. set the values of CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB
			u8 CLK_A_LB=3;
			u8 CLK_A_UB=5;
			u8 CLK_B_LB=9;
			u8 CLK_B_UB=11;
	set_fastScan_Width_GPIO(currObj,CLK_A_LB,CLK_A_UB,CLK_B_LB,CLK_B_UB);
	// 3. // Taking control from C: FASTSCAN_EXT_LOCB = 0*
	// Making sure the fastscanin verilog code drives the scan pins: SCANOUT_INB=0*,
	// default Setting S2P scan settings: numScanInAtOnce=1*
	u8 FASTSCAN_EXT_LOCB=1;
	u8 SCANOUT_INB=0;
	u16 numScanInAtOnce=1;
	set_fastScan_Control_GPIO(currObj,FASTSCAN_EXT_LOCB,SCANOUT_INB,numScanInAtOnce);
	u8 ScanSel_WWL_SC=WWL_SC;
	u8 ScanSel_WRITE_1_SC=WRITE_1_SC;
	u8 ScanSel_WRITE_0_SC=WRITE_0_SC;
	u8 ScanSel_SC_DEFAULT=SC_DEFAULT;
	u8 SCANOUT_SPL=0;
	set_fastScan_Control_SPL_GPIO(currObj, ScanSel_WWL_SC, ScanSel_WRITE_1_SC, ScanSel_WRITE_0_SC, ScanSel_SC_DEFAULT, SCANOUT_SPL);

	return currObj;

}

