/*
 * ReadFile.c
 *
 *  Created on: 03-Dec-2021
 *      Author: ashlo
 */
#include <custom_h.h>


FRESULT ReadFile(char *FileName, u32 DestinationAddress)
{
	FIL fil;
	FRESULT rc;
	UINT br;
	u64 file_size;
	rc = f_open(&fil, FileName, FA_READ);
	if (rc) {
		xil_printf(" ERROR : f_open returned %d\r\n", rc);
		return XST_FAILURE;
	}
//	file_size = fil.fsize;
	file_size = f_size(&fil);
	rc = f_lseek(&fil, 0);
	if (rc) {
		xil_printf(" ERROR : f_lseek returned %d\r\n", rc);
		return XST_FAILURE;
	}
	rc = f_read(&fil, (void*) DestinationAddress, file_size, &br);
	if (rc) {
		xil_printf(" ERROR : f_read returned %d\r\n", rc);
		return XST_FAILURE;
	}
	rc = f_close(&fil);
	if (rc) {
		xil_printf(" ERROR : f_close returned %d\r\n", rc);
		return XST_FAILURE;
	}
	Xil_DCacheFlush();
	return XST_SUCCESS;
}

