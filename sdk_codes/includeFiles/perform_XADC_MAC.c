/*
 * perform_hybridIMC_BLIMG_FPGA.c
 *
 *  Created on: Aug 11, 2022
 *      Author: Balaji Vijayakumar
 */

#include <custom_h.h>

read_out_XADCvolt perform_XADC_MAC(curr_obj currObj, u8 ImgBank)
{
	u8 scanChain;
//	u8 signals[1];
//	u8 currentValues[1];
//	u8 values[1];
//	scan_out scanOut;
	int signals_len;
	read_out_XADCvolt read_out_XADCv;
	memset(read_out_XADCv.scanOutVolts_XADC,0,scan_len_FLOATADC_SC*4*4*sizeof(read_out_XADCv.scanOutVolts_XADC[0]));
	//1. Enable DATA_Resis, RefResis, RWL_EN_EXT
	{

		u8 signals_preVerilogSET[5]={DataResis_EN,RefResis_EN,ADCSTART_EXT,RESET_GLOBALTIMER,RWL_EN_EXT};//ADCSTART_EXT RBL_PRE_EN_B
		u8 values_preVerilogSET[5]={1,1,0,0,1};
		signals_len = 5 ;
		currObj=bitsSet( currObj, signals_preVerilogSET, signals_len, values_preVerilogSET );
	}
	//2. Enable DataCM_EN_EXT  Remove Pre-charge on RBL
	{
		u8 signals_preVerilogSET[5]={DataCM_EN_EXT,DataSUB_EN_EXT,RefCM_EN_EXT,RefSUB_EN_EXT,RBL_PRE_EN_B};
		u8 values_preVerilogSET[5]={1,1,1,1,1};
		signals_len = 5 ;
		currObj=bitsSet( currObj, signals_preVerilogSET, signals_len, values_preVerilogSET );
	}
	//3. Perform IMG BL based CIM and record it on XADC
	u8 i,j;
	u8 IMG;
	int k;
//	int len_XADCVolts=scan_len_ADC_SC/7;
	u16 repeatAvg;
	repeatAvg=1;
	XADCvolt XADCv;
	for (i=0;i<4;i++)
	{
		IMG=((u8)ImgBank<<2)+i;
		currObj=IMGSelect(currObj,IMG);
		for( j=0 ; j<4 ; j++ )
		{
			//4. Select ADC Scan Chain
			scanChain=ADC_SC;
			currObj=scanSelect(currObj,scanChain);

			//currObj=bitLineSelect(currObj,j);
			currObj=RBL_IMC_Select(currObj, j);

//			//5. IMC_RBL_SC[0]=1
//			{
//				u8 scanChain=IMC_RBL_SC;
//				int len_vecScanIn=scan_len_IMC_RBL_SC;
//				u8 vecScanIn[scan_len_IMC_RBL_SC];
//				memset(vecScanIn,0,scan_len_IMC_RBL_SC*sizeof(u8));
//				vecScanIn[0]=1;
//				currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
//			}

			//5. IMC_VOut_SC[0]=1
			{
				u8 scanChain=IMC_VOut_SC;
				int len_vecScanIn=scan_len_IMC_VOut_SC;
				u8 vecScanIn[scan_len_IMC_VOut_SC];
				memset(vecScanIn,0,scan_len_IMC_VOut_SC*sizeof(u8));
				vecScanIn[0]=1;
				currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
			}

			//6. Copy the ADC scanOut Bits
			for (k=0;k<(len_XADCVolts-2);k++)
			{
				usleep(1);
				repeatAvg=10;
				XADCv =adcRead_uncompressed(repeatAvg);

				// OLD CODE
				 read_out_XADCv.scanOutVolts_XADC[k + len_XADCVolts * (j + i * 4)]=XADCv.adcOut_VPVN[0];//scanOut.scanOutBits_ADC_SC[k];

				// NEW CODE STARTS HERE
//				float voltage_avg=0;
//				for(int i1=0;i1<repeatAvg;i1++)
//				{
//					voltage_avg+=XADCv.adcOut_VPVN[i1];
//				}
//				voltage_avg/=repeatAvg;
//				read_out_XADCv.scanOutVolts_XADC[k + len_XADCVolts * (j + i * 4)]=voltage_avg;//scanOut.scanOutBits_ADC_SC[k];
				// NEW CODE ENDS HERE

				if(k>=2)
				{
					//5. IMC_VOut_SC[0]=1
					{
						u8 scanChain=IMC_VOut_SC;
						int len_vecScanIn=1;
						u8 vecScanIn[1]={0};
						currObj=scanInVector_uncompressed(currObj,vecScanIn, len_vecScanIn,scanChain);
					}
				}

			}
		}
	}

	//7. Disable DataCM_EN_EXT,RWL_EN_EXT;  Enable Pre-charge on RBL
	{

		u8 signals_preVerilogSET[6]={DataCM_EN_EXT,DataSUB_EN_EXT,RefCM_EN_EXT,RefSUB_EN_EXT,RBL_PRE_EN_B,RWL_EN_EXT};
		u8 values_preVerilogSET[6]={0,0,0,0,0,0};
		signals_len = 6 ;
		currObj=bitsSet( currObj, signals_preVerilogSET, signals_len, values_preVerilogSET );
	}

	return read_out_XADCv;
}
