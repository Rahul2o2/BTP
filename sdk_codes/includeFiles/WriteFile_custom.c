/*
 * WriteFile.c
 *
 *  Created on: Jul 25, 2023
 *
 */
#include <custom_h.h>


FRESULT WriteFile_custom(char *FileName, u32 size, void *SourceAddress)
{
    FIL fil;
    FRESULT rc;
    UINT br;
    u32 file_size;
    rc = f_open(&fil, FileName, FA_WRITE | FA_CREATE_ALWAYS);
    if (rc) {
        xil_printf(" ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
//  file_size = fil.fsize;
    file_size = size;
    rc = f_lseek(&fil, 0);
    if (rc) {
        xil_printf(" ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_write(&fil, (void*) SourceAddress, file_size, &br);
    if (rc) {
        xil_printf(" ERROR : f_write returned %d\r\n", rc);
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


int WriteFile_custom2(char *FileName, u32 size, void *SourceAddress, FIL fil, FRESULT rc, UINT br)
{
//    FIL fil;
//    FRESULT rc;
//    UINT br;
//    u32 file_size;
//    rc = f_open(&fil, FileName, FA_WRITE | FA_CREATE_ALWAYS);
//    if (rc) {
//        xil_printf(" ERROR : f_open returned %d\r\n", rc);
//        return XST_FAILURE;
//    }
//  file_size = fil.fsize;
//    rc = f_lseek(&fil, 0);
//    if (rc) {
//        xil_printf(" ERROR : f_lseek returned %d\r\n", rc);
//        return XST_FAILURE;
//    }
    rc = f_write(&fil, (void*) SourceAddress, size, &br);
//    if (rc) {
//        xil_printf(" ERROR : f_write returned %d\r\n", rc);
//        return XST_FAILURE;
//    }
//    rc = f_close(&fil);
//    if (rc) {
//        xil_printf(" ERROR : f_close returned %d\r\n", rc);
//        return XST_FAILURE;
//    }
    Xil_DCacheFlush();
    return XST_SUCCESS;
}

