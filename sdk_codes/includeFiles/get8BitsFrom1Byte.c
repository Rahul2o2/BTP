/*
 * get8BitsFrom1Byte.c
 *
 *  Created on: 08-Mar-2021
 *      Author: ashlo
 */
/*
 * bitsHigh.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Ashwin Balagopal S
 */
#include <custom_h.h>

eight_bits get8BitsFrom1Byte(u8 inputByte)
{
	u8 valExtract;
	eight_bits eightBitsObj;
	u8 k;
	for(k=0;k<=7;k++)//inputByte is in  left-MSB format with inputByte[0] being a waste bit. So k starts from 1.
	{
		//getting inputByte[k]
			valExtract=inputByte;
			valExtract=valExtract<<k;
			valExtract=valExtract>>7;
			eightBitsObj.bits[7-k]=valExtract;// k=0, bits[7]=inputByte[0] is the MSB and k=7, bits[0]=inputByte[7] is the LSB
	}
	//sevenBitsObj.bits[6:0]=inputByte[1:7]
//
	return eightBitsObj;
}


