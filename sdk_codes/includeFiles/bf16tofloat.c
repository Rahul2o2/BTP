/*
 * bf16tofloat.c
 *
 *  Created on: Jul 20, 2023
 *      Author: asreddy
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utilsinclude.h"


int32_t concatenate_u8_to_u32(unsigned char u8_1, unsigned char u8_2, unsigned char u8_3, unsigned char u8_4)
{
    int32_t u32_result = 0;
    u32_result |= ((uint32_t)u8_1 << 24);
    u32_result |= ((uint32_t)u8_2 << 16);
    u32_result |= ((uint32_t)u8_3 << 8);
    u32_result |= u8_4;

    return u32_result;
}


float u32_to_float(int32_t u32_value)
{
    // Typecast the u32_value to a pointer of type float
    float *float_ptr = (float *)&u32_value;

    // Dereference the float pointer to obtain the float value
    float float_value = *float_ptr;

    return float_value;
}

float bf16tofloat(unsigned char sentArr1,unsigned char sentArr2,unsigned char sentArr3, unsigned char sentArr4)
{
    float result;
    result=concatenate_u8_to_u32(sentArr1, sentArr2, sentArr3, sentArr4);
    result=u32_to_float(result);
    //printf("\n %f \n",result);

    return result;
}
