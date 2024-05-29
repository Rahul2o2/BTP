/*
 * scanInVector.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Sri Hari
 */

#include <custom_h.h>
#include "XGpio.h"

scan_out scanOutVector(curr_obj currObj,u8 scanChain)
{
//scanOutVector expanded in-line
	scan_out scanOut;
	int i ;
	u8 bank=100;//Since SCQOUT is located in GPIO_PCB_32OUTPUTS_100_Inst
	u32 dataRead;
	u8 inputPin;
	//u8 outputPin;
	u16 scan_len;
	u8 inputByte ;
	u8 scanOutValue ;
	currObj=scanSelect(currObj,scanChain);
	inputPin=SC_DIN;
	//outputPin=SC_QOUT;
	switch(scanChain)
	{
		case SA_SC:
		{
			scan_len=97;
			break;
		}
		case ADC_SC:
		{
			scan_len=644;
			break;
		}
		case AdderTree_SC:
		{
			scan_len=176;
			break;
		}
		case SARTIMING_OUT_SC:
		{
			scan_len=26;
			break;
		}
		case WRITE_2_SC:
		{
			scan_len=97;
			break;
		}
		case WRITE_1_SC:
		{
			scan_len=97;
			break;
		}
		case WRITE_0_SC:
		{
			scan_len=97;
			break;
		}
		case IMC_RBL_SC:
		{
			scan_len=88;
			break;
		}
		case IMC_VOut_SC:
		{
			scan_len=88;
			break;
		}
		case WWL_SC:
		{
			scan_len=1160;
			break;
		}
		case FLOATADC_SC:
		{
			scan_len=92;
			break;
		}
		case SARTIMING_IN_SC:
		{
			scan_len=110;
			break;
		}
		case ControlPath_SC:
		{
			scan_len=17;
			break;
		}
		default:
		{
			scan_len=97;
			break;
		}
	}

	//seven_bits sevenBitsObj ; u8 sevenBits[7] ;
	{
		{
			u8 signals[1]={inputPin};
			u8 values[1];
			values[0] = 0 ;//this is what will overwrite the scanChain

			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
	}


	for(i=(scan_len-1);i>=0;i--) // each iteration pops an element from the scan-chain
	{
		dataRead=bitsGet(currObj,bank);// reading status of FPGA output pins(FMC) bank=5
		inputByte =(u8) dataRead ;
		//sevenBitsObj=get7BitsFrom1Byte( inputByte );//this will ensure sevenBitsObj.bits[6:0]={2'b 00, WR_SC_OUT_2_data,WR_SC_OUT_1_data,WR_SC_OUT_0_data,R_SC_OUT_data,WL_SC_OUT_data};
		//for( i1 = 0 ; i1 < 7 ; i1 ++ )
		//	sevenBits[i1] = sevenBitsObj.bits[i1] ;
		/*
		 * custom_h.h has the following #defines
			#define SA_SC	0
			#define ADC_SC	1
			#define AdderTree_SC	2
			#define SARTIMING_OUT_SC	3
			#define WRITE_2_SC	4
			#define WRITE_1_SC	5
			#define WRITE_0_SC	6
			#define IMC_RBL_SC	7
			#define IMC_VOut_SC	8
			#define WWL_SC	9
			#define FLOATADC_SC	10
			#define SARTIMING_IN_SC	11
			#define ControlPath_SC	12
			#define Reset_DFF	13
			#define SC_DEFAULT	SA_SC
		 */
		//scanOutValue = sevenBits[outputPin] ;
		scanOutValue = inputByte;
		switch(scanChain)
		{
			case SA_SC:
			{
				scanOut.scanOutBits_SA_SC[i] = scanOutValue ;
				break;
			}
			case ADC_SC:
			{
				scanOut.scanOutBits_ADC_SC[i] = scanOutValue ;
				break;
			}
			case AdderTree_SC:
			{
				scanOut.scanOutBits_AdderTree_SC[i] = scanOutValue ;
				break;
			}
			case SARTIMING_OUT_SC:
			{
				scanOut.scanOutBits_SARTIMING_OUT_SC[i] = scanOutValue ;
				break;
			}
			case WRITE_2_SC:
			{
				scanOut.scanOutBits_WRITE_2_SC[i] = scanOutValue ;
				break;
			}
			case WRITE_1_SC:
			{
				scanOut.scanOutBits_WRITE_1_SC[i] = scanOutValue ;
				break;
			}
			case WRITE_0_SC:
			{
				scanOut.scanOutBits_WRITE_0_SC[i] = scanOutValue ;
				break;
			}
			case IMC_RBL_SC:
			{
				scanOut.scanOutBits_IMC_RBL_SC[i] = scanOutValue ;
				break;
			}
			case IMC_VOut_SC:
			{
				scanOut.scanOutBits_IMC_VOut_SC[i] = scanOutValue ;
				break;
			}
			case WWL_SC:
			{
				scanOut.scanOutBits_WWL_SC[i] = scanOutValue ;
				break;
			}
			case FLOATADC_SC:
			{
				scanOut.scanOutBits_FLOATADC_SC[i] = scanOutValue ;
				break;
			}
			case SARTIMING_IN_SC:
			{
				scanOut.scanOutBits_SARTIMING_IN_SC[i] = scanOutValue ;
				break;
			}
			case ControlPath_SC:
			{
				scanOut.scanOutBits_ControlPath_SC[i] = scanOutValue ;
				break;
			}

		}

		// pop one element from the scan-chain
		{
			u8 signals[2]={CLK_A,CLK_B};
			u8 values[2]={0,0};
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1]={CLK_A};
			u8 values[1]={1};
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1]={CLK_A};
			u8 values[1]={0};
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
				currObj=bitsSet(currObj, signals, signals_len, values);
		}
		{
			u8 signals[1]={CLK_B};
			u8 values[1]={1};
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
//	Restoring default scan select
	scanChain=SA_SC;
	currObj=scanSelect(currObj,scanChain);
	scanOut.currObj=currObj;
//end of scanOutVector expanded in-line
	return scanOut;
}
