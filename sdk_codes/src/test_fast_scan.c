//
//#include <custom_h.h>
//#include "utilsinclude.h"
//int main()
//{
//	static XSysMon SysMonInst; //a sysmon instance
//	//u8 SeqMode;
//	int xStatus;
//	XSysMon_Config *SysMonConfigPtr;
//	XSysMon *SysMonInstPtr = &SysMonInst;
//	SysMonConfigPtr = XSysMon_LookupConfig(SYSMON_DEVICE_ID);
//	xStatus = XSysMon_CfgInitialize(SysMonInstPtr, SysMonConfigPtr,SysMonConfigPtr->BaseAddress);
//	XSysMon_GetStatus(SysMonInstPtr); // Clear the old status
//// Extra Added for lab pdf adc code ends here
//
//u32 status;
//u32 receivedBytes=0;
//u32 totalFunctionCallBytes=0;
//u32 totalTransmittedBytes=0;
//u32 transmittedBytes=0;
//XUartPs_Config *myUartConfig;//in uartps
//XUartPs myUart;
//
//myUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);//in parameters
//status = XUartPs_CfgInitialize(&myUart,myUartConfig,myUartConfig->BaseAddress);
//if(status != XST_SUCCESS)
//print("UART initialization failed\n\r");
//status= XUartPs_SetBaudRate(&myUart,BAUDRATE);
//if(status != XST_SUCCESS)
//print("Baud rate initialization failed\n\r");
//status = XUartPs_SelfTest(&myUart);
//if (status != XST_SUCCESS)
//return XST_FAILURE;
//
//curr_obj currObj;
//currObj=initialisation_code(currObj);
//
//
//scan_out scanOut;
//scan_out_internal scanoutInternal;
//currObj=defaultState(currObj);
//currObj=defaultState(currObj);
//
//u8 scanChain=WRITE_0_SC;
//u16 scan_len = get_scan_len( scanChain ) ;
//u8 vecScanIn[scan_len_WRITE_0_SC]={ 1,0,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,//1
//									1,0,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,//2
//									1,0,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,//3
//									1};
//// Time measurement for SCAN IN (SLOW)
//XTime tStart_scanIn_slow;
//XTime tStop_scanIn_slow;
//XTime tElapsed_scanIn_slow;
//XTime_GetTime(&tStart_scanIn_slow);
//
//int len_vecScanIn = scan_len ;
//currObj=scanInVector_uncompressed(currObj,vecScanIn, len_vecScanIn, scanChain);
//
//XTime_GetTime(&tStop_scanIn_slow);
//tElapsed_scanIn_slow=tStop_scanIn_slow-tStart_scanIn_slow;
//
//// Time measurement for SCAN OUT (SLOW)
//XTime tStart_scanOut_slow;
//XTime tStop_scanOut_slow;
//XTime tElapsed_scanOut_slow;
//XTime_GetTime(&tStart_scanOut_slow);
//
//scan_out scanOut_slow=scanOutVector(currObj,scanChain);
//
//XTime_GetTime(&tStop_scanOut_slow);
//tElapsed_scanOut_slow=tStop_scanOut_slow-tStart_scanOut_slow;
//
//
//// Time measurement for SCAN IN (FAST)
//XTime tStart_scanIn_fast;
//XTime tStop_scanIn_fast;
//XTime tElapsed_scanIn_fast;
//XTime_GetTime(&tStart_scanIn_fast);
//
//len_vecScanIn = scan_len ;
//currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn, scanChain);
//
//XTime_GetTime(&tStop_scanIn_fast);
//tElapsed_scanIn_fast=tStop_scanIn_fast-tStart_scanIn_fast;
//
//// Time measurement for SCAN OUT (FAST)
//XTime tStart_scanOut_fast;
//XTime tStop_scanOut_fast;
//XTime tElapsed_scanOut_fast;
//XTime_GetTime(&tStart_scanOut_fast);
//
//scan_out scanOut_fast=scanOutVector_FAST(currObj,scanChain);
//
//XTime_GetTime(&tStop_scanOut_fast);
//tElapsed_scanOut_fast=tStop_scanOut_fast-tStart_scanOut_fast;
//
//// Time measurement for SCAN OUT (SLOW)
//
//return 0;
//}
