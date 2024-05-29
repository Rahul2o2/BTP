/*
 * write_onto_datalines.c
 *
 *  Created on: Mar 26, 2021
 *      Author: Sri Hari
 */


#include <custom_h.h>
#include "XGpio.h"

curr_obj write_onto_datalines( curr_obj currObj,int *data_1_row_indices,int len_data_1_row_indices,int *data_0_row_indices,int len_data_0_row_indices,u8 *colSel,int colSel_len, u8 *DL_nums, int DL_nums_len)
{

	int i = 0, j = 0 ;
	int mask_start = 0 ;

	u8 scanChain ;

	int rowSel, rowSel_start ;
	int len_vecScanIn_mask = 97 ;
	int len_vecScanIn_data = 97 ;
	int len_vecScanIn_WL = 1160 ;

	u8 vecScanIn_1[97], vecScanIn_0[97], vecScanIn_mask[97], vecScanIn_WL[1160] ;
	u8 scanInZero[1]={0};
	int len_scanInZero=1;
	u8 scanInOne[1]={1};
	int len_scanInOne=1;
	// initialising vecScanIn_0, vecScanIn_1
	for( i = 0 ; i < len_vecScanIn_data ; i++ )
	{
		vecScanIn_1[i] = 1 ;
		vecScanIn_0[i] = 0 ;

	}

	// initialising vecScanIn_mask: mask pattern for writing only on relevant DLs.
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

	// initialising vecScanIn_WL
	for( i = 0 ; i < len_vecScanIn_WL ; i++ )
	{
		vecScanIn_WL[ i ] = 0 ;
	}

	currObj = scanInWriteScanChains_maskOnly_uncompressed( currObj, vecScanIn_mask, len_vecScanIn_mask ) ;

	// write 1s on required cells next
	if( len_data_1_row_indices > 0 )
		{
		currObj = scanInWriteScanChains_dataOnly_uncompressed( currObj, vecScanIn_1, len_vecScanIn_data ) ;

		scanChain = WWL_SC ;
		currObj = scanInVector_uncompressed( currObj, vecScanIn_WL, len_vecScanIn_WL, scanChain ) ;
		scanChain = WWL_SC ;

		currObj = scanInVector_uncompressed( currObj, scanInOne, len_scanInOne, scanChain ) ;//Balaji Added: Seed 1

		rowSel = 0 ;
		rowSel_start = 0 ;
		for( i = 0 ; i < len_data_1_row_indices ; i++ )
			{
				for( rowSel = rowSel_start ; rowSel < data_1_row_indices[i] ; rowSel++ )
						{
							scanChain = WWL_SC ;
							currObj = scanInVector_uncompressed( currObj, scanInZero, len_scanInZero, scanChain ) ;
						}

				// write onto cell
					for( j = 0 ; j < colSel_len ; j++ )
							{
								currObj = loadBitlinetoCell_FPGA( currObj, &colSel[j], colSel_len );
							}

				rowSel_start = rowSel ; // as rowSel = data_1_row_indices[i] right now, which is the start of the next scan-in of 11110->
			}
		}

	// write 0s on required cells next
	if( len_data_0_row_indices > 0 )
		{
		currObj = scanInWriteScanChains_dataOnly_uncompressed( currObj, vecScanIn_0, len_vecScanIn_data ) ;

		scanChain = WWL_SC;
		currObj = scanInVector_uncompressed( currObj, vecScanIn_WL, len_vecScanIn_WL, scanChain ) ;
		scanChain = WWL_SC ;
		currObj = scanInVector_uncompressed( currObj, scanInOne, len_scanInOne, scanChain ) ;//Balaji Added: Seed 1

		rowSel = 0 ;
		rowSel_start = 0 ;

		for( i = 0 ; i < len_data_0_row_indices ; i++ )
			{
				for( rowSel = rowSel_start ; rowSel < data_0_row_indices[i] ; rowSel++ )
				{
					scanChain = WWL_SC ;
					currObj = scanInVector_uncompressed( currObj, scanInZero, len_scanInZero, scanChain ) ;
				}

				// write onto cell
					for( j = 0 ; j < colSel_len ; j++ )
						{
							currObj = loadBitlinetoCell_FPGA( currObj, &colSel[j], 1 ) ;
						}

				rowSel_start = rowSel ; // as rowSel = data_0_row_indices[i] right now, which is the start of the next scan-in of 11110->

			}

		}
	//Scanning in all 0's into WWL scan chain: Default Setting
//	scanChain = WWL_SC;
//	currObj = scanInVector_uncompressed( currObj, vecScanIn_WL, len_vecScanIn_WL, scanChain );

	return currObj;

	}









