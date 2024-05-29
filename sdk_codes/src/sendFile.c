///*
// * resnet20.c
// *
// *  Created on: 30-Sep-2023
// *      Author: iitmbscdegree
// */
//#include "utilsinclude.h"
//#include <custom_h.h>
//int main(){
//
//	s32 status;
//	XUartPs_Config *myUartConfig;//in uartps
//	XUartPs myUart;
//
//	myUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);//in parameters
//	status = XUartPs_CfgInitialize(&myUart,myUartConfig,myUartConfig->BaseAddress);
//	if(status != XST_SUCCESS)
//	print("UART initialization failed\n\r");
//	status= XUartPs_SetBaudRate(&myUart,BAUDRATE);
//	if(status != XST_SUCCESS)
//	print("Baud rate initialization failed\n\r");
//	status = XUartPs_SelfTest(&myUart);
//	if (status != XST_SUCCESS)
//	return XST_FAILURE;
//
//	curr_obj currObj;
//	currObj=initialisation_code(currObj);
//	int Status;
//	Status = SD_Init(&fatfs);
//
//	u32 totalReceivedBytes2;//***changed
//
//	//** Srinidhi added part starts
//	u32 ttlSentBytes;
//	u32 cSentBytes;
//	u32 totalSentBytes;
//	u32 sentBytes;
//	u32 totalReceivedBytes;
//
//	//** Srinidhi added part sends
//
//
//	{
//		//Read the .bin
//		long int nbytes=64*1000*sizeof(float);
//		char* imageBuffer_xdim = (char*)calloc(nbytes,sizeof(char));
//		char fileName_xm[] = "OUTR.BIN";
//		Status = ReadFile( fileName_xm,(u32)imageBuffer_xdim);
//		int numBytes_sendAtOnce=4*sizeof(float);
//		for(long int nl = 0; nl<nbytes; nl+=numBytes_sendAtOnce){
//			sendByteData( &imageBuffer_xdim[nl], numBytes_sendAtOnce, myUart ) ;
//			usleep(100);
//		}
//	}
//
//	Status=SD_Eject(&fatfs); // SEARCH FOR THIS, ENDS HERE
//    return 0;
//}
//
//
