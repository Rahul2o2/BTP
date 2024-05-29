/*
 * populate_scanout.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */

#include<custom_h.h>
scan_out populate_scanout( scan_out scanOut, u8 *vecScanOut, u16 scan_len, u8 scanChain )
{
switch(scanChain)
	{
		case SA_SC:{
			memcpy(scanOut.scanOutBits_SA_SC,vecScanOut,scan_len);
			break;
		}
		case ADC_SC:{
			memcpy(scanOut.scanOutBits_ADC_SC,vecScanOut,scan_len);
			break;
		}
		case AdderTree_SC:{
			memcpy(scanOut.scanOutBits_AdderTree_SC,vecScanOut,scan_len);
			break;
		}
		case SARTIMING_OUT_SC:{
			memcpy(scanOut.scanOutBits_SARTIMING_OUT_SC,vecScanOut,scan_len);
			break;
		}
		case WRITE_2_SC:{
			memcpy(scanOut.scanOutBits_WRITE_2_SC,vecScanOut,scan_len);
			break;
		}
		case WRITE_1_SC:{
			memcpy(scanOut.scanOutBits_WRITE_1_SC,vecScanOut,scan_len);
			break;
		}
		case WRITE_0_SC:
		{
			memcpy(scanOut.scanOutBits_WRITE_0_SC,vecScanOut,scan_len);
			break;
		}
		case IMC_RBL_SC:{
			memcpy(scanOut.scanOutBits_IMC_RBL_SC,vecScanOut,scan_len);
			break;
		}
		case IMC_VOut_SC:{
			memcpy(scanOut.scanOutBits_IMC_VOut_SC,vecScanOut,scan_len);
			break;
		}
		case WWL_SC:{
			memcpy(scanOut.scanOutBits_WWL_SC,vecScanOut,scan_len);
			break;
		}
		case FLOATADC_SC:{
			memcpy(scanOut.scanOutBits_FLOATADC_SC,vecScanOut,scan_len);
			break;
		}
		case SARTIMING_IN_SC:{
			memcpy(scanOut.scanOutBits_SARTIMING_IN_SC,vecScanOut,scan_len);
			break;
		}
		case ControlPath_SC:{
			memcpy(scanOut.scanOutBits_ControlPath_SC,vecScanOut,scan_len);
			break;
		}
	}
	return scanOut;
}
