/*
 * store_images_SRAM.c
 *
 *  Created on: Jul 27, 2023
 *
 */

#include<utilsinclude.h>
#include<custom_h.h>
//u8 IMG_DL_nums[8]={1,2,3,4,5,6,7,8};
curr_obj store_images_SRAM_2bit(uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t c, uint16_t WordLines, uint16_t xb, uint16_t XBL,u8 mode_xbit, curr_obj currObj)
{
//	curr_obj currObj;
	u8 writeSRAMdata[9*1024];

	int DL_nums_len=8;
//	u8 DL_nums[8]={1,2,3,4,5,6,7,8};
//	scan_out scanOut;
//	u8 scanOutArr[8][1][4][92][7];
	//u16 scanOutDecimal[8

	for(int bitLine=0;bitLine<4;bitLine++)
	{
		/*for(int wsi=0;wsi<97*1024;wsi++)
		{
			writeSRAMdata[wsi]=0;
		}
		*/
//		XTime tStart_make_writeSram;
//		XTime tStop_make_writeSram;
//		XTime tElapsed_make_writeSram;
//		XTime_GetTime(&tStart_make_writeSram);
		memset(writeSRAMdata,0,9*1024*sizeof(writeSRAMdata[0]));
		if(bitLine<XDL)
		{
			for(int i=0;i<WordLines;i++)
			{
				if(mode_xbit==1)
				{
					if(X_BitMat[b+bitLine][s][c][i][xb])
					{
						writeSRAMdata[9*i+1]=1;
					}
					else
					{

						writeSRAMdata[9*i+7]=1;
					}
				}
				else
				{
					if((X_BitMat[b+bitLine][s][c][i][xb+1])&&(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+1]=1; // Datalines 1, 2 correspond to DACVolts<3> and xb+1,xb = 1,1
					}
					else if((X_BitMat[b+bitLine][s][c][i][xb+1])&&!(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+3]=1; // Datalines 3, 4 correspond to DACVolts<2> and xb+1,xb = 1,0
					}
					else if(!(X_BitMat[b+bitLine][s][c][i][xb+1])&&(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+5]=1; // Datalines 5, 6 correspond to DACVolts<1> and xb+1,xb = 0,1
					}
					else if(!(X_BitMat[b+bitLine][s][c][i][xb+1])&&!(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+7]=1; // Datalines 7, 8 correspond to DACVolts<0> and xb+1,xb = 0,0
					}
				}

			}
		}
		if((bitLine+4)<XDL)
		{
			for(int i=0;i<WordLines;i++)
			{
				if(mode_xbit==1)
				{
					if(X_BitMat[b+4+bitLine][s][c][i][xb])
					{
						writeSRAMdata[9*i+2]=1;
					}
					else
					{
						writeSRAMdata[9*i+8]=1;
					}
				}
				else
				{
					if((X_BitMat[b+bitLine][s][c][i][xb+1])&&(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+2]=1; // Datalines 1, 2 correspond to DACVolts<3> and xb+1,xb = 1,1
					}
					else if((X_BitMat[b+bitLine][s][c][i][xb+1])&&!(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+4]=1; // Datalines 3, 4 correspond to DACVolts<2> and xb+1,xb = 1,0
					}
					else if(!(X_BitMat[b+bitLine][s][c][i][xb+1])&&(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+6]=1; // Datalines 5, 6 correspond to DACVolts<1> and xb+1,xb = 0,1
					}
					else if(!(X_BitMat[b+bitLine][s][c][i][xb+1])&&!(X_BitMat[b+bitLine][s][c][i][xb]))
					{
						writeSRAMdata[9*i+8]=1; // Datalines 7, 8 correspond to DACVolts<0> and xb+1,xb = 0,0
					}
				}
			}
		}
		for(int i=WordLines;i<1024;i++)
		{
			writeSRAMdata[9*i+7]=1;
			writeSRAMdata[9*i+8]=1;
		}
//		XTime_GetTime(&tStop_make_writeSram);
//		tElapsed_make_writeSram=tStop_make_writeSram-tStart_make_writeSram;
//		double tElapsed_make_writeSram_sec=((double)tElapsed_make_writeSram)/((double)COUNTS_PER_SECOND);


//		XTime tStart_writeSram;
//		XTime tStop_writeSram;
//		XTime tElapsed_writeSram;
//		XTime_GetTime(&tStart_writeSram);
//		currObj=writeSRAM_1024(currObj,bitLine,writeSRAMdata,DL_nums,DL_nums_len);
//
//		XTime_GetTime(&tStop_writeSram);
//		tElapsed_writeSram=tStop_writeSram-tStart_writeSram;
//		double tElapsed_writeSram_sec=((double)tElapsed_writeSram)/((double)COUNTS_PER_SECOND);
//
//		XTime tStart_writeSram_FAST;
//		XTime tStop_writeSram_FAST;
//		XTime tElapsed_writeSram_FAST;
//		XTime_GetTime(&tStart_writeSram_FAST);
//		currObj=writeSRAM_1024_Images_FAST(currObj,bitLine,writeSRAMdata,DL_nums,DL_nums_len);
		currObj=writeSRAM_1024_Images_FAST_NEW(currObj,bitLine,writeSRAMdata,IMG_DL_nums,DL_nums_len);

//		XTime_GetTime(&tStop_writeSram_FAST);
//		tElapsed_writeSram_FAST=tStop_writeSram_FAST-tStart_writeSram_FAST;
//		double tElapsed_writeSram_FAST_sec=((double)tElapsed_writeSram_FAST)/((double)COUNTS_PER_SECOND);
//		int pauseVal=1;
	}



	return currObj;
}
