/*
 * initialise_uart.c
 *
 *  Created on: Sep 6, 2023
 *      Author: ashlo
 */

#include<custom_h.h>
XUartPs initialise_uart(){
        u32 status;
        u32 receivedBytes=0;
        u32 totalFunctionCallBytes=0;
        u32 totalTransmittedBytes=0;
        u32 transmittedBytes=0;
        XUartPs_Config *myUartConfig;//in uartps
        XUartPs myUart;
        XUartPs myUart_fail;

        myUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);//in parameters
        status = XUartPs_CfgInitialize(&myUart,myUartConfig,myUartConfig->BaseAddress);
        if(status != XST_SUCCESS)
        print("UART initialization failed\n\r");
        status= XUartPs_SetBaudRate(&myUart,BAUDRATE);
        if(status != XST_SUCCESS)
        print("Baud rate initialization failed\n\r");
        status = XUartPs_SelfTest(&myUart);
        if (status != XST_SUCCESS)
        	return myUart_fail;
        	//return XST_FAILURE;
        else
        	return myUart;
}

void sendDWordData( uint32_t *returnWords, int numWords, XUartPs myUart ){

	u32 totalTransmittedBytes=0;
	u32 transmittedBytes=0;
//	int p2=1;
//	uint16_t returnBytes[]={oc,c,wb};
//	int fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

	while((totalTransmittedBytes/4) < numWords)
	{
		transmittedBytes = XUartPs_Send(&myUart,&(returnWords[totalTransmittedBytes/4]),4);
		totalTransmittedBytes+=transmittedBytes;
	}
}

void sendWordData( uint16_t *returnWords, int numWords, XUartPs myUart ){

	u32 totalTransmittedBytes=0;
	u32 transmittedBytes=0;
//	int p2=1;
//	uint16_t returnBytes[]={oc,c,wb};
//	int fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

	while((totalTransmittedBytes/2) < numWords)
	{
		transmittedBytes = XUartPs_Send(&myUart,&(returnWords[totalTransmittedBytes/2]),2);
		totalTransmittedBytes+=transmittedBytes;
	}
}

void sendByteData( uint8_t *returnBytes, int numBytes, XUartPs myUart ){

	u32 totalTransmittedBytes=0;
	u32 transmittedBytes=0;
//	int p2=1;
//	uint16_t returnBytes[]={oc,c,wb};
//	int fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

	while(totalTransmittedBytes < numBytes)
	{
		transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
		totalTransmittedBytes+=transmittedBytes;
	}
}
