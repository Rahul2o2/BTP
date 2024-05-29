/*
 * writeSRAM_calib.c
 *
 *  Created on: Jul 31, 2023
 *
 */

// MACROS DEFINED IN custom.h are as below

#include <custom_h.h>
curr_obj writeSRAM_calib(curr_obj currObj, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len)
{
//DL_nums is in ascending order
//writeSRAMdata:97*68- Note that the order must be WL0 97 bit data followed by WL1 all arranged as one array WL0-97,WL1-97 ... for selected BL ;  eg:-DL_nums=[0:5] for writing into first 6 DLs alone DL_nums_len=6
		int i = 0, j=0, k=0 ;

		int rowSel=0, rowSel_start=0 ;
		u8 bitLine=0;
		u8 scanChain ;
		u8  vecScanIn_WL[1160] ;
		int len_vecScanIn_WL=1160;
		u8 scanInZero[1]={0};
		int len_scanInZero=1;
		//u8 scanInOne[1]={1};
		//int len_scanInOne=1;
		int mask_start = 0 ;
		int len_vecScanIn_mask = 97 ;
		int len_vecScanIn_data = 97 ;


		u8 vecScanIn_data[97], vecScanIn_mask[97] ;

		// Initializing vecScanIn_data, vecScanIn_dataComp to all 0
		for( i = 0 ; i < len_vecScanIn_data ; i++ )
		{
			vecScanIn_data[i] = 0 ;
		}

		// Initializing vecScanIn_mask: mask pattern for writing only on relevant DLs.
		for( i = 0 ; i < DL_nums_len ; i++ )
		{
			for( j = mask_start ; j < DL_nums[i] ; j++ )
			{
				vecScanIn_mask[j] = 0 ;
			}

			vecScanIn_mask[j] = 1 ;
			mask_start = DL_nums[i] + 1 ;
		}
		for( j = mask_start ; j < len_vecScanIn_mask ; j++ )
		{
			vecScanIn_mask[j] = 0 ;
		}

		//Initializing Mask
		currObj = scanInWriteScanChains_maskOnly_uncompressed( currObj, vecScanIn_mask, len_vecScanIn_mask ) ;


		int data_1_row_indices[68]={1090,1091,1092,1093,1094,1095,1096,1097,1098,1099,1100,1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1120,1121,1122,1123,1124,1125,1126,1127,1128,1129,1130,1131,1132,1133,1134,1135,1136,1137,1138,1139,1140,1141,1142,1143,1144,1145,1146,1147,1148,1149,1150,1151,1152,1153,1154,1155,1156,1157};
		int len_data_1_row_indices=68;

		// Initializing vecScanIn_WL
		for( i = 0 ; i < len_vecScanIn_WL ; i++ )
		{
			vecScanIn_WL[ i ] = 0 ;
			if(i==data_1_row_indices[0])
			{
				vecScanIn_WL[ i ] = 1 ;
			}
		}
		scanChain = WWL_SC ;
		currObj = scanInVector_uncompressed( currObj, vecScanIn_WL, len_vecScanIn_WL, scanChain ) ;
//		scanChain = WWL_SC ;
//		currObj = scanInVector_uncompressed( currObj, scanInOne, len_scanInOne, scanChain ) ;


		u8 bitLines[1]={bitLine};

		int len_WR_SC=97;

		int logicalWLADDR=0;

		rowSel_start=data_1_row_indices[0];

		for( i = 0 ; i < len_data_1_row_indices ; i++ )
			{
				for( rowSel = rowSel_start ; rowSel < data_1_row_indices[i] ; rowSel++ )
						{
							scanChain = WWL_SC ;
							currObj = scanInVector_uncompressed( currObj, scanInZero, len_scanInZero, scanChain ) ;
						}

				for (k=0 ; k < len_WR_SC ; k++ )
					{
						if(writeSRAMdata[logicalWLADDR*len_WR_SC+k]==1)
						{
							vecScanIn_data[k]=1;

						}
						else
						{
							vecScanIn_data[k]=0;

						}
					}
				currObj = scanInWriteScanChains_dataOnly_uncompressed( currObj, vecScanIn_data, len_vecScanIn_data ) ;
				//WRITE Operation through WriteDrivers Onto Cell
				currObj = loadBitlinetoCell_FPGA( currObj, &bitLines[0], 1 );

				rowSel_start = rowSel ; // as rowSel = data_1_row_indices[i] right now, which is the start of the next scan-in of 11110->
				logicalWLADDR++;
			}

	return currObj;

}


