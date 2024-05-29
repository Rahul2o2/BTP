/*
 * IMC_TIMING.c
 *
 *  Created on: Aug 14, 2023
 *      Author: ashlo
 */
#include <custom_h.h>
#include<utilsinclude.h>
curr_obj IMC_TIMING(uint16_t XDL, uint16_t XBL,uint16_t WBL,uint16_t**** IMC_Out,curr_obj currObj)
{
	//Time Measurement on ZedBoard
	scan_out scanOut;
	//u8 scanOutArr[8][1][4][92][7];
	u8 Num_Adc=92;
	u8 Bit_Adc=7;
	u8 *****scanOutArr = (u8*****)calloc(XDL,sizeof(u8****));
	        for(uint16_t xd = 0; xd < XDL; xd++){
	        	scanOutArr[xd] = (u8****)calloc(XBL,sizeof(u8***));
	            for(uint16_t xb = 0; xb < XBL; xb++){
	            	scanOutArr[xd][xb] = (u8***)calloc(WBL,sizeof(u8**));
	                for(uint16_t wb = 0; wb < WBL; wb ++){
	                	scanOutArr[xd][xb][wb] = (u8**)calloc(Num_Adc,sizeof(u8*));
	                	for(uint16_t num_adc = 0; num_adc < Num_Adc; num_adc++){
	                		scanOutArr[xd][xb][wb][num_adc] = (u8*)calloc(Bit_Adc,sizeof(u8));
	                	}
	                }
	            }
	        }
	//1 Select IMG_Sel<2>=0 using bitset;

	u8 signals[1]={IMG_Sel_Dec_2};
	{
		u8 values[1] = { 0 } ;
		int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
			currObj=bitsSet(currObj, signals, signals_len, values);
	}
//	currObj=bitsSet(currObj,signals,1,0);

	//2SAR_TIMING_IN_SC->INTERNAL
	u8 vecScanIn[]={0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
	currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn,sizeof(vecScanIn),11);

	//2- High speed Verilog based chip resulting IMC which can compute 4 IMAGES & 4 BITLINES and result in 92*7*4*4  (loadCelltoSACtr_FPGA)
	u8 BL[4]={0,1,2,3};
	currObj = loadCelltoSACtr_FPGA(currObj,BL,sizeof(BL));

	//3 Get out of INTERNAL MODE-> scan-Into SAR_TIMING_IN_SC->HYBRID OR EXTERNA
	currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn,sizeof(vecScanIn),11);


//	for(int i=0;i<4;i++)
	u8 xDL=XDL>4?4:XDL;
	int Imc_ind=0;
	for(int i=0;i<xDL;i++)
	{
//		for(int j=0;j<1;j++)
		for(int j=0;j<XBL;j++)
		{
//			for(int k=0;k<4;k++)
			for(int k=0;k<WBL;k++)
			{
				scanOut=scanOutVector_FAST(currObj,1);
				for(int l=0;l<92;l++)//92 DLs, i.e. 92 ADCs
				{
					if((l>=2&&l<=19)||(l>=28&&l<=63)||(l>=72&&l<=89))
					for(int m=0;m<7;m++)// each ADC is 7b
					{
							scanOutArr[i][j][k][l][m]=scanOut.scanOutBits_ADC_SC[l*7+m];
							IMC_Out[i][Imc_ind][j][k]+=(scanOutArr[i][j][k][l][m])*pow(2,m);
					}

				}
			}
		}
	}

	//1 Select IMG_Sel<2>=1 using bitset;
	{
		u8 values[1] = { 1 } ;
		int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
			currObj=bitsSet(currObj, signals, signals_len, values);
	}
//	currObj=bitsSet(currObj,signals,1,1);

	//2SAR_TIMING_IN_SC->INTERNAL
	currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn,sizeof(vecScanIn),11);

	//2- High speed Verilog based chip resulting IMC which can compute 4 IMAGES & 4 BITLINES and result in 92*7*4*4  (loadCelltoSACtr_FPGA)
	currObj = loadCelltoSACtr_FPGA(currObj,BL,sizeof(BL));

	//3 Get out of INTERNAL MODE-> scan-Into SAR_TIMING_IN_SC->HYBRID OR EXTERNA
	currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn,sizeof(vecScanIn),11);

	if(XDL>4){
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<1;j++)
			{
				for(int k=0;k<4;k++)
				{
					scanOut=scanOutVector_FAST(currObj,1);
					for(int l=0;l<Num_Adc;l++)
					{
						for(int m=0;m<Bit_Adc;m++)
						{
								scanOutArr[i+4][j][k][l][m]=scanOut.scanOutBits_ADC_SC[l*7+m];
								IMC_Out[i+4][Imc_ind][j][k]+=(scanOutArr[i+4][j][k][l][m])*pow(2,m);
						}
					}
				}
			}
		}
	}

	for(uint16_t xd = 0; xd < XDL; xd++){
		for(uint16_t xb = 0; xb < XBL; xb++){
			for(uint16_t wb = 0; wb < WBL; wb ++){
				for(uint16_t num_adc = 0; num_adc < Num_Adc; num_adc++){
					free(scanOutArr[xd][xb][wb][num_adc]);
				}
				free(scanOutArr[xd][xb][wb]);
			}
			free(scanOutArr[xd][xb]);
		}
		free(scanOutArr[xd]);
	}
	free(scanOutArr);

	return currObj;
}

