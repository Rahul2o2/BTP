/*
 * get_scan_len.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */
#include<custom_h.h>
u16 get_scan_len( u8 scanChain )
{
	u16 scan_len;
	switch(scanChain)
		{
			case SA_SC:
			{
				scan_len=scan_len_SA_SC;
				break;
			}
			case ADC_SC:
			{
				scan_len=scan_len_ADC_SC;
				break;
			}
			case AdderTree_SC:
			{
				scan_len=scan_len_AdderTree_SC;
				break;
			}
			case SARTIMING_OUT_SC:
			{
				scan_len=scan_len_SARTIMING_OUT_SC;
				break;
			}
			case WRITE_2_SC:
			{
				scan_len=scan_len_WRITE_2_SC;
				break;
			}
			case WRITE_1_SC:
			{
				scan_len=scan_len_WRITE_1_SC;
				break;
			}
			case WRITE_0_SC:
			{
				scan_len=scan_len_WRITE_0_SC;
				break;
			}
			case IMC_RBL_SC:
			{
				scan_len=scan_len_IMC_RBL_SC;
				break;
			}
			case IMC_VOut_SC:
			{
				scan_len=scan_len_IMC_VOut_SC;
				break;
			}
			case WWL_SC:
			{
				scan_len=scan_len_WWL_SC;
				break;
			}
			case FLOATADC_SC:
			{
				scan_len=scan_len_FLOATADC_SC;
				break;
			}
			case SARTIMING_IN_SC:
			{
				scan_len=scan_len_SARTIMING_IN_SC;
				break;
			}
			case ControlPath_SC:
			{
				scan_len=scan_len_ControlPath_SC;
				break;
			}
			default:
			{
				scan_len=scan_len_SA_SC;
				break;
			}
		}
	return scan_len;
}

