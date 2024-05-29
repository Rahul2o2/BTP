/*
 * SD_Init.c
 *
 *  Created on: 03-Dec-2021
 *      Author: ashlo
 */
#include <custom_h.h>

int SD_Init()
{
	FRESULT rc;
	TCHAR *Path = "0:/";
	rc = f_mount(&fatfs,Path,0);
	if (rc) {
		xil_printf(" ERROR : f_mount returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

