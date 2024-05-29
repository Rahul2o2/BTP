/*
 * extractBitsFromDWORD.c
 *
 *  Created on: Jun 10, 2023
 *      Author: ashlo
 */

#include <custom_h.h>

u8 extractBitFromDWORD(u32 inputDWORD, u8 k)//k=0 corresponds to LSB and k=31 corresponds to MSB
{
	u32 bitExtract1;
	u8 bitExtract;
	bitExtract1=inputDWORD;
	bitExtract1=bitExtract1>>k;
	bitExtract1=bitExtract1<<31;
	bitExtract1=bitExtract1>>31;

	bitExtract=(u8)bitExtract1;
	return bitExtract;
}



