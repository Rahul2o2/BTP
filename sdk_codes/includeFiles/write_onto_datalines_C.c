/*
 * write_onto_datalines.c
 *
 *  Created on: Mar 26, 2021
 *      Author: Sri Hari
 */


#include <custom_h.h>
#include "XGpio.h"

curr_obj write_onto_datalines_C( curr_obj currObj,u8 *data_DL, u8 *colSel, int len_colSel, u8 *DL_nums, int len_DL_nums)
{
	int i,j;
	int len_data_DL = 1092 ;
	//data_DL:  1092
	//int len_colSel = 1 or 2 or 3 or 4;
	//u8 DL_nums[97];DL_nums=0:96
	//int len_DL_nums = k;

	//START of Additional functionality to morph the 1092 bits into 1160 bits with additional unconnected bits
	//seven_bits sevenBitsObj;
	int dl_row=0, dl_rowLogical=0;
	u8 data_DLbitwiseWithUnused[1160];
	int len_data_DLbitwiseWithUnused=1160;
	//INITIALISE UNUSED SCAN BITS TO ZERO HERE
	int unusedBits[68]={32,33,66,67,100,101,134,135,168,169,202,203,236,237,270,271,304,305,338,339,372,373,406,407,440,441,474,475,508,509,542,543,576,577,610,611,644,645,678,679,712,713,746,747,780,781,814,815,848,849,882,883,916,917,950,951,984,985,1018,1019,1052,1053,1086,1087,1088,1089,1158,1159};
	int len_unusedBits=68;
	for (i=0;i<len_unusedBits;i++)
	{
		data_DLbitwiseWithUnused[unusedBits[i]]=0;//MAKING ALL UNUSED BITS AS ZERO
	}
	//POPULATING USEFUL DATA

	for(i=0;i<len_data_DL;i++)
	{
		//sevenBitsObj = get7BitsFrom1Byte(data_DL[i]);

		for(j=5;j>=0;j--)
		{
			if(data_DL[dl_rowLogical]==1)
			{
				data_DLbitwiseWithUnused[dl_row]=1;
			}
			else
			{
				data_DLbitwiseWithUnused[dl_row]=0;
			}
			// if dlRow is in the unusedbits then do dlrow++
			if (dl_row%34 == 31 && dl_row != 1085 && dl_row < 1089)
			{
				dl_row += 3;
			}
			else if (dl_row == 1085)
			{
			   dl_row += 5;
			}
			else if (dl_row != 1157)
			{
				dl_row++;
			}
			dl_rowLogical++;
		}

	}
		//END of Additional functionality to morph the 1092 bits into 1160 bits with additional unconnected bits
		//seven_bits sevenBitsObj;
		i=0,j=0,dl_row=0;
		int data_1_i=0,data_0_i=0;
		int data_1_row_indices[1160];
		int data_0_row_indices[1160];

		for(i=0;i<len_data_DLbitwiseWithUnused;i++)
		{
			if(data_DLbitwiseWithUnused[i]==1)
			{
				data_1_row_indices[data_1_i] = dl_row;
				data_1_i++;
			}
			else
			{
				data_0_row_indices[data_0_i] = dl_row;
				data_0_i++;
			}
			dl_row++;
		}

		int len_data_1_row_indices = data_1_i;
		int len_data_0_row_indices = data_0_i;



	currObj = write_onto_datalines(currObj,data_1_row_indices,len_data_1_row_indices,data_0_row_indices,len_data_0_row_indices,colSel,len_colSel,DL_nums,len_DL_nums);

	return currObj;

	}









