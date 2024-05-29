/*
 * get32BitsFrom1DWORD.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */
#include<custom_h.h>
void get32BitsFrom1DWORD(u32 inputDword,u8 *populateArray, u8 len_populateArray){
	// when len_populateArray=32, populateArray[31] is the MSB and populateArray[0] is the LSB
	// i.e. inputDword = populateArray[31]*2^31 + populateArray[30]*2^30 + .... + populateArray[0]*2^0

//	int m=0;//2^m weight index of inputDword is being accessed
	for(int k=0;k<len_populateArray;k++){
		populateArray[k]=(u8)( (inputDword<<(31-k))>>31 );
	}
}
