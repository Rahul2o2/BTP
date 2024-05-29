//#include "xsysmon.h"
//#include "xstatus.h"
//#include "xil_exception.h"
//#include "xil_printf.h"
//#include "sleep.h"
//#include <stdlib.h>
//#include "xil_types.h"
//#include "xuartps.h"
//#include "xparameters.h"
//#include "sleep.h"
//#include "xgpio.h"
//#include <stdio.h>
//
//#define BAUDRATE 115200
//#define syncdata_val 125
//
//XUartPs initialise_uart();
//void sendWordData( uint16_t *returnWords, int numWords, XUartPs myUart );
//void sendDWordData( float *returnWords, int numWords, XUartPs myUart );
//
////#include<custom_h.h>
//
//int main(){
//
//	u32 status;
//	u32 receivedBytes=0;
//	u32 totalFunctionCallBytes=0;
//	u32 totalTransmittedBytes=0;
//	u32 transmittedBytes=0;
//	XUartPs myUart=initialise_uart();
//
//	//Added by Ashwin Balagopal(Starts)
//	u32 receivedBytes2;
//	int fileSize_return;
//	u8 syncdata;
//	u8 scanChain;
//	u32 totalReceivedBytes2;//***changed
//
//	//** Srinidhi added part starts
//	u32 ttlSentBytes;
//	u32 cSentBytes;
//	u32 totalSentBytes;
//	u32 sentBytes;
//	u32 totalReceivedBytes;
//	//** Srinidhi added part sends
//
//	int flag;
//	{ // TELLING MATLAB TO TURN ON POWER SUPPLIES
//		// UART ADDED CODE STARTS HERE
//		float returnWords[]={0,1,2,3,4};//0 is stride
//		int numDWords=sizeof(returnWords)/sizeof(returnWords[0]);
//		sendDWordData( returnWords, numDWords, myUart ) ;
//		// UART ADDED CODE ENDS HERE
//	  // HANDSHAKING WITH MATLAB
//		do
//		{
//			receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
//		}while(syncdata!=syncdata_val);
//
//		int pause=1;
//	}
//
//
//
//    return 0;
//}
///*
// * initialise_uart.c
// *
// *  Created on: Sep 6, 2023
// *      Author: ashlo
// */
//
//
//XUartPs initialise_uart(){
//        u32 status;
//        u32 receivedBytes=0;
//        u32 totalFunctionCallBytes=0;
//        u32 totalTransmittedBytes=0;
//        u32 transmittedBytes=0;
//        XUartPs_Config *myUartConfig;//in uartps
//        XUartPs myUart;
//        XUartPs myUart_fail;
//
//        myUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);//in parameters
//        status = XUartPs_CfgInitialize(&myUart,myUartConfig,myUartConfig->BaseAddress);
//        if(status != XST_SUCCESS)
//        print("UART initialization failed\n\r");
//        status= XUartPs_SetBaudRate(&myUart,BAUDRATE);
//        if(status != XST_SUCCESS)
//        print("Baud rate initialization failed\n\r");
//        status = XUartPs_SelfTest(&myUart);
//        if (status != XST_SUCCESS)
//        	return myUart_fail;
//        	//return XST_FAILURE;
//        else
//        	return myUart;
//}
//
//void sendDWordData( float *returnWords, int numWords, XUartPs myUart ){
//
//	u32 totalTransmittedBytes=0;
//	u32 transmittedBytes=0;
////	int p2=1;
////	uint16_t returnBytes[]={oc,c,wb};
////	int fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);
//
//	while((totalTransmittedBytes/4) < numWords)
//	{
//		transmittedBytes = XUartPs_Send(&myUart,&(returnWords[totalTransmittedBytes/4]),4);
//		totalTransmittedBytes+=transmittedBytes;
//	}
//}
