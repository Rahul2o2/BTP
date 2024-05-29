/*
 * bitLineSelect.c
 *
 *  Created on: Jun 11, 2023
 *      Author: Ashwin Balagopal S, Balaji V
 */


#include <custom_h.h>
curr_obj IMGSelect(curr_obj currObj,u8 IMG)
{
	u8 signals[3]={IMG_Sel_Dec_2,IMG_Sel_Dec_1,IMG_Sel_Dec_0};
		u8 values[3];
			switch (IMG)
			{
				case 0:
				{
						values[0]=0;
						values[1]=0;
						values[2]=0;
						break;
				}
				case 1:
				{
						values[0]=0;
						values[1]=0;
						values[2]=1;
						break;
				}
				case 2:
				{
						values[0]=0;
						values[1]=1;
						values[2]=0;
				}		break;
				case 3:
				{
						values[0]=0;
						values[1]=1;
						values[2]=1;
						break;
				}
				case 4:
				{
						values[0]=1;
						values[1]=0;
						values[2]=0;
						break;
				}
				case 5:
				{
						values[0]=1;
						values[1]=0;
						values[2]=1;
						break;
				}
				case 6:
				{
						values[0]=1;
						values[1]=1;
						values[2]=0;
				}		break;
				case 7:
				{
						values[0]=1;
						values[1]=1;
						values[2]=1;
						break;
				}
			}
			int signals_len = sizeof(signals) / sizeof(signals[0]) ;
			currObj=bitsSet( currObj, signals, signals_len, values );
			return currObj;
}
//  Ashwin Newly Modified code, issue: gets infinitely stuck in the for loop
//	u8 signals[2]={CS_1,CS_0};
//	u8 values[2];
//
//	u32 BL = (u32)bitLine;
//	u8 k;
//	for(k=1;k>=0;k--)
//		values[k] = extractBitFromDWORD(BL, k) ;
//
//	int signals_len = sizeof(signals) / sizeof(signals[0]) ;
//	currObj=bitsSet( currObj, signals, signals_len, values );
//
//	return currObj;
