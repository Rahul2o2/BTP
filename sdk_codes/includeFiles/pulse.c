/*
 * bitsGet.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Ashwin Balagopal
 */



#include <custom_h.h>

curr_obj pulse( curr_obj currObj, u8 *signals, u8 *currentValues, int signals_len )
{
//	get current data in signals[] and present Values in currentValues[]
	u8 complementValues[ signals_len ];

	int i;
	for( i = 0 ; i < signals_len ; i++ )
	{
		complementValues[i]=1-currentValues[i];
	}
	//u8 scanChain=5;
	//currObj=scanSelect(currObj,scanChain);
	currObj=bitsSet(currObj, signals, signals_len, complementValues);
	currObj=bitsSet(currObj,signals,signals_len, currentValues);
	return currObj;
}

