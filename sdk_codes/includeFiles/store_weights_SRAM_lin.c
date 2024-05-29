/*
 * store_weights_SRAM_lin.c
 *
 *  Created on: Aug 24, 2023
 *      Author: ashlo
 */


/*
 * store_weights_SRAM.c
 *
 *  Created on: Jul 27, 2023

 */

#include<utilsinclude.h>
#include<custom_h.h>
// u8 DL_nums_total[WDL_HW]={9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,58,59,60,61,62,63,64,65,66,67,68,69,70,80,81,82,83,84,85,86,88,89,90,91,92,93,94,95,96};
u8 DL_nums_total[72]={9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96};
u8 DLs_ref[2][8]={{27,28,29,30,31,32,33,34},{71,72,73,74,75,76,77,78}};
//u8 IMG_DL_nums[8]={1,2,3,4,5,6,7,8};
curr_obj store_weights_SRAM_lin(uint8_t**** W_BitMat,float**** Ref,float*** RefB,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,curr_obj currObj)
{
	//	curr_obj currObj;
	u8 writeSRAMdata[97*1024];
	long int writeSRAMdata_index=0;
	int DLs_j=0,DL_nums_len=WDL+16+1;
	u8 DL_nums[DL_nums_len];//16 ref DLs and 1 addertree DL

	for(int bitLine=0;bitLine<WBL;bitLine++)
	{
		memset(writeSRAMdata,0,97*1024*sizeof(u8));
		writeSRAMdata_index=0;
		DLs_j=0;
		for(int j=0;j<WDL;j++)//weight DLs
		{
			DL_nums[DLs_j]=DL_nums_total[map_to_72[j]];
			DLs_j++;
			for(int i=0;i<WordLines;i++)
			{
				writeSRAMdata_index=i*97+DL_nums_total[map_to_72[j]];

				writeSRAMdata[writeSRAMdata_index]=W_BitMat[oc+j][c][i][wb+bitLine];
			}
		}
		for(int j=0;j<8;j++)//Reference DLs
		{
			DL_nums[DLs_j]=DLs_ref[0][j];
			DLs_j++;
			DL_nums[DLs_j]=DLs_ref[1][j];
			DLs_j++;
			for(int i=0;i<WordLines;i++)
			{
				for(int LH=0;LH<2;LH++)
				{
					writeSRAMdata_index=i*97+DLs_ref[LH][j];
					writeSRAMdata[writeSRAMdata_index]=(u8)Ref[c][LH][j][i];
				}
			}
		}
		//Addertree DL
		DL_nums[DLs_j]=0;
		DLs_j++;
		for(int i=0;i<WordLines;i++)
		{
			writeSRAMdata_index=i*97;
			writeSRAMdata[writeSRAMdata_index]=(u8)RefB[c][bitLine][i];
		}
		currObj=writeSRAM_1024_FAST_NEW(currObj,bitLine,writeSRAMdata,DL_nums,DL_nums_len);
	}
	return currObj;
}
