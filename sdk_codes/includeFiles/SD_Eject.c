/*
 * SD_Eject.c
 *
 *  Created on: 03-Dec-2021
 *      Author: ashlo
 */
#include <custom_h.h>

int SD_Eject()
{
	FRESULT rc;
	TCHAR *Path = "0:/";
	rc = f_mount(0,Path,1);
	if (rc) {
		xil_printf(" ERROR : f_mount returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}


