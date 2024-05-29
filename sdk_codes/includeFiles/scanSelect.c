/*
 * scanSelect.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Ashwin Balagopal
 */
// MACROS DEFINED IN custom.h are as below
//%this function selects the scanchain as specified by the scanChain argument:
//%     scanChain = 0 ; % SA_SC
//%     scanChain = 1 ; % ADC_SC
//%     scanChain = 2 ; % AdderTree_SC
//%     scanChain = 3 ; % SARTIMING_OUT_SC
//%     scanChain = 4 ; % WRITE_2_SC
//%     scanChain = 5 ; % WRITE_1_SC
//%     scanChain = 6 ; % WRITE_0_SC
//%     scanChain = 7 ; % IMC_RBL_SC
//%     scanChain = 8 ; % IMC_VOut_SC
//%     scanChain = 9 ; % WWL_SC
//%     scanChain = 10 ; % FLOATADC_SC
//%     scanChain = 11 ; % SARTIMING_IN_SC
//%     scanChain = 12 ; % ControlPath_SC
//%     scanChain = 13 ; % Reset_DFF
#include <custom_h.h>
curr_obj scanSelect(curr_obj currObj,u8 scanChain)
{
	u8 signals[4]={ScanSel_3,ScanSel_2,ScanSel_1,ScanSel_0};
	u8 values[4];
	//values[0] : value__ScanSel_3; values[1] : value__ScanSel_2; values[2] : value__ScanSel_1; values[3] : value__ScanSel_0
	switch (scanChain)
	{
		case SA_SC:
		{
				values[0]=0;
				values[1]=0;
				values[2]=0;
				values[3]=0;
				break;
		}
		case ADC_SC:
		{
				values[0]=0;
				values[1]=0;
				values[2]=0;
				values[3]=1;
				break;
		}
		case AdderTree_SC:
		{
				values[0]=0;
				values[1]=0;
				values[2]=1;
				values[3]=0;
		}		break;
		case SARTIMING_OUT_SC:
		{
				values[0]=0;
				values[1]=0;
				values[2]=1;
				values[3]=1;
				break;
		}
		case WRITE_2_SC:
		{
				values[0]=0;
				values[1]=1;
				values[2]=0;
				values[3]=0;
				break;
		}
		case WRITE_1_SC:
		{
				values[0]=0;
				values[1]=1;
				values[2]=0;
				values[3]=1;
				break;
		}
		case WRITE_0_SC:
		{
				values[0]=0;
				values[1]=1;
				values[2]=1;
				values[3]=0;
				break;
		}
		case IMC_RBL_SC:
		{
				values[0]=0;
				values[1]=1;
				values[2]=1;
				values[3]=1;
				break;
		}
		case IMC_VOut_SC:
		{
				values[0]=1;
				values[1]=0;
				values[2]=0;
				values[3]=0;
				break;
		}
		case WWL_SC:
		{
				values[0]=1;
				values[1]=0;
				values[2]=0;
				values[3]=1;
				break;
		}
		case FLOATADC_SC:
		{
				values[0]=1;
				values[1]=0;
				values[2]=1;
				values[3]=0;
				break;
		}
		case SARTIMING_IN_SC:
		{
				values[0]=1;
				values[1]=0;
				values[2]=1;
				values[3]=1;
				break;
		}
		case ControlPath_SC:
		{
				values[0]=1;
				values[1]=1;
				values[2]=0;
				values[3]=0;
				break;
		}
		case Reset_DFF:
		{
				values[0]=1;
				values[1]=1;
				values[2]=0;
				values[3]=1;
				break;
		}
	}

	{
		int signals_len = sizeof(signals) / sizeof(signals[0]) ;
			currObj=bitsSet(currObj, signals, signals_len, values);
	}

	return currObj;


}
