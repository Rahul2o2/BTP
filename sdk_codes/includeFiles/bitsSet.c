/*
 * bitsSet.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Sri Hari
 */



#include <custom_h.h>

curr_obj bitsSet( curr_obj currObj,u8 *signals,int signals_len,u8 *values )
{
	int i=0;

//	int signals_len=((sizeof(signals))/sizeof(signals[0]));

	u8 signal ;
	u8 value ;
	do{
		signal = signals[i] ;
		value = values[i] ;
		currObj.stringCurr[signal]=value;
		i++;

	}while(i<signals_len);


	printString(currObj);

	return currObj;
}
