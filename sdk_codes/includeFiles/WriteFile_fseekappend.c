#include <custom_h.h>

FRESULT WriteFile_fseekappend(char *FileName, u32 size, void *SourceAddress)
{
    FIL fil;
    FRESULT rc;
    UINT br;
    u32 file_size;
    rc = f_open(&fil, FileName, FA_WRITE | FA_OPEN_APPEND); // Open in "append" mode
    if (rc) {
        xil_printf(" ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    // Retrieve the current file size
    file_size = f_size(&fil);

    // Move the file pointer to the end of the file
    rc = f_lseek(&fil, file_size);
    if (rc) {
        xil_printf(" ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_write(&fil, (void*) SourceAddress, size, &br);
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
