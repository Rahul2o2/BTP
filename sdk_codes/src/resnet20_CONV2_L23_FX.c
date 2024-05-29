///*
// * resnet20_CONV2_L23_FX.c
// *
// *  Created on: Oct 19, 2023
// *      Author: ashlo
// */
//
//#include "utilsinclude.h"
//#include <custom_h.h>
//int main(){
//    static XSysMon SysMonInst; //a sysmon instance
//	//u8 SeqMode;
//	int xStatus;
//	XSysMon_Config *SysMonConfigPtr;
//	XSysMon *SysMonInstPtr = &SysMonInst;
//	SysMonConfigPtr = XSysMon_LookupConfig(SYSMON_DEVICE_ID);
//	xStatus = XSysMon_CfgInitialize(SysMonInstPtr, SysMonConfigPtr,SysMonConfigPtr->BaseAddress);
//	XSysMon_GetStatus(SysMonInstPtr); // Clear the old status
//// Extra Added for lab pdf adc code ends here
//
//	u32 status;
//	u32 receivedBytes=0;
//	u32 totalFunctionCallBytes=0;
//	u32 totalTransmittedBytes=0;
//	u32 transmittedBytes=0;
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
//
//	scan_out scanOut;
//	scan_out_internal scanoutInternal;
//	int signals_len;
//	int k=0,i=0,marker_pos=0;
//	//Added by Ashwin Balagopal(Starts)
//	u32 receivedBytes2;
//	int fileSize_return;
//	u8 syncdata;
//	u8 scanChain;
//	//u8 abcd[200000];
//	//u8 abcd[20];
//	//u8 counter_data ;
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
//	int scan_len;
//	u8 bitLine;
//	//int l1;
//	u8 numPadZeroes_vecScanIn;
//	int flag;
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	{ // TELLING MATLAB TO TURN ON POWER SUPPLIES
//		// UART ADDED CODE STARTS HERE
//		uint16_t returnWords[]={power_supply_start_word};//0 is stride
//		int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
//		sendWordData( returnWords, numWords, myUart ) ;
//		// UART ADDED CODE ENDS HERE
//	  // HANDSHAKING WITH MATLAB
//		do
//		{
//			receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
//		}while(syncdata!=syncdata_val);
//	}
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//
//
//	//	 u8 slopeSetting=1;
//	//	 u8 slopeSetting=2;
//		 u8 slopeSetting=4;
//	currObj=init_internal_slopeSetting_ICQ(currObj, slopeSetting);
//
//
//	int Status;
//	Status = SD_Init(&fatfs);
//
// 	 #pragma region
//	float a = 1e-5;
//    //Image Matrix = X matrix
//	bool IMC = true;
//    uint16_t X_Batch = 0; //Batch size for Images
//    uint16_t X_Channels = 0; //No of channels in each image
//    uint16_t X_Height = 0; //Height of the each image
//    uint16_t X_Width = 0; //Width of the input image
//
//
//    //Read the .bin and update the input image dimensions.
//    char* imageBuffer_xdim = (char*)calloc(16,sizeof(char));
//    char fileName_xm[] = "XDim.bin";
//    Status = ReadFile( fileName_xm,(u32)imageBuffer_xdim);
//    X_Batch=(uint16_t)bf16tofloat(imageBuffer_xdim[0],imageBuffer_xdim[1],imageBuffer_xdim[2],imageBuffer_xdim[3]);
//	X_Channels=(uint16_t)bf16tofloat(imageBuffer_xdim[4],imageBuffer_xdim[5],imageBuffer_xdim[6],imageBuffer_xdim[7]);
//	X_Height=(uint16_t)bf16tofloat(imageBuffer_xdim[8],imageBuffer_xdim[9],imageBuffer_xdim[10],imageBuffer_xdim[11]);
//	X_Width=(uint16_t)bf16tofloat(imageBuffer_xdim[12],imageBuffer_xdim[13],imageBuffer_xdim[14],imageBuffer_xdim[15]);
//	printf("\nImage Dimensions : %d %d %d %d", X_Batch, X_Channels, X_Height, X_Width);
//
//	uint16_t X_Batch2 = 0; //Batch size for Images
//	uint16_t X_Channels2 = 0; //No of channels in each image
//	uint16_t X_Height2 = 0; //Height of the each image
//	uint16_t X_Width2 = 0; //Width of the input image
//	//Read the .bin and update the input image dimensions.
//	char* imageBuffer_xdim2 = (char*)calloc(16,sizeof(char));
//	char fileName_xm2[] = "XDi2.bin";
//	Status = ReadFile( fileName_xm2,(u32)imageBuffer_xdim2);
//	X_Batch2=(uint16_t)bf16tofloat(imageBuffer_xdim2[0],imageBuffer_xdim2[1],imageBuffer_xdim2[2],imageBuffer_xdim2[3]);
//	X_Channels2=(uint16_t)bf16tofloat(imageBuffer_xdim2[4],imageBuffer_xdim2[5],imageBuffer_xdim2[6],imageBuffer_xdim2[7]);
//	X_Height2=(uint16_t)bf16tofloat(imageBuffer_xdim2[8],imageBuffer_xdim2[9],imageBuffer_xdim2[10],imageBuffer_xdim2[11]);
//	X_Width2=(uint16_t)bf16tofloat(imageBuffer_xdim2[12],imageBuffer_xdim2[13],imageBuffer_xdim2[14],imageBuffer_xdim2[15]);
//	printf("\nImage Dimensions : %d %d %d %d", X_Batch2, X_Channels2, X_Height2, X_Width2);
//
//
//	//X_Batch = 1; //Make it one as of now.
//
//    //Read the .bin to get NL -> Number of Layers.
//    char* NL_Buffer = (char*)calloc(4,sizeof(char));
//    char fileName_nl[] = "WNL.bin";
//    Status = ReadFile( fileName_nl,(u32)NL_Buffer);
//    uint16_t Total_Layers = (uint16_t)bf16tofloat(NL_Buffer[0],NL_Buffer[1],NL_Buffer[2],NL_Buffer[3]);
//    printf("\nNumber of Total Layers: %d",Total_Layers);
//
//    //Kernel Matrix = W matrix. This will be per layer.
//    uint16_t* W_Batch = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Batch size for kernels
//    uint16_t* W_Channels = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //No of channels in each kernel
//    uint16_t* W_Height = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Height of the each kernel
//    uint16_t* W_Width = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Width of the input kernel
//    uint16_t W_Padding = 0; //Padding  (Required because same unfolding function as image is used) (Do not change)
//    uint16_t W_Stride = 1;  //Stride (Required because same unfolding function as image is used) (Do not change)
//
//    char *imageBuffer_wdim = (char *)calloc(16*Total_Layers, sizeof(char));
//	char fileName_wm[]="WDim.bin";
//	Status = ReadFile( fileName_wm,(u32)imageBuffer_wdim);
//
//    for(uint16_t nl = 0; nl<Total_Layers; nl++){
//        W_Batch[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 0],imageBuffer_wdim[16*nl + 1],imageBuffer_wdim[16*nl + 2],imageBuffer_wdim[16*nl + 3]);
//        W_Channels[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 4],imageBuffer_wdim[16*nl + 5],imageBuffer_wdim[16*nl + 6],imageBuffer_wdim[16*nl + 7]);
//        W_Height[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 8],imageBuffer_wdim[16*nl + 9],imageBuffer_wdim[16*nl + 10],imageBuffer_wdim[16*nl + 11]);
//        W_Width[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 12],imageBuffer_wdim[16*nl + 13],imageBuffer_wdim[16*nl + 14],imageBuffer_wdim[16*nl + 15]);
//        printf("\nWeight Dimensions for layer %d : %d %d %d %d", nl, W_Batch[nl], W_Channels[nl], W_Height[nl], W_Width[nl]);
//    }
//    #pragma endregion
//
//    //Read weights file data to figure out how many convolution and fully connected layers are there.
//    #pragma region
//    uint16_t CNV_Layers = 0;
//    uint16_t LIN_Layers = 0;
//    for(uint16_t i =0; i < Total_Layers; i++){
//        if(W_Height[i] != 0 && W_Width[i] != 0){
//            CNV_Layers++;
//        }
//    }
//    LIN_Layers = Total_Layers - CNV_Layers;
//    printf("\nNumber of Convolution Layers: %d",CNV_Layers);
//    printf("\nNumber of Linear Layers: %d",LIN_Layers);
//    #pragma endregion
//
//    //IMC + Convolution + MaxPool + Linear Constants
//    #pragma region
//    uint16_t** X_Padding = (uint16_t**)calloc(CNV_Layers,sizeof(uint16_t*)); //Padding
//    uint16_t** X_Stride = (uint16_t**)calloc(CNV_Layers,sizeof(uint16_t*));  //Stride
//    for(uint16_t i = 0; i < CNV_Layers; i++){
//        X_Padding[i] = (uint16_t*)calloc(2,sizeof(uint16_t));
//        X_Stride[i] = (uint16_t*)calloc(2,sizeof(uint16_t));
//    }
//    uint16_t* WordLines = (uint16_t*)calloc(CNV_Layers,sizeof(uint16_t)); //Number of word lines available for IMC at a time.
//    uint16_t XDL = XDL_HW; // XDL is the number of image DLs 8
//	uint16_t XBL = XBL_HW; // XBL is the number of image pxl mantissa bits being fed
//	uint16_t WDL = WDL_HW; // WDL is the number of weight DLs 72
//	uint16_t WBL = WBL_HW; // WBL is the number of weight mantissa bits being fed
//
//    char *convParamsBuf = (char *)calloc(20*CNV_Layers, sizeof(char));
//	char fileName_c[]="convP.bin";
//	Status = ReadFile( fileName_c,(u32)convParamsBuf);
//    for(uint16_t nl = 0; nl<CNV_Layers; nl++){
//        X_Stride[nl][0] = (uint16_t)bf16tofloat(convParamsBuf[20*nl + 0],convParamsBuf[20*nl + 1],convParamsBuf[20*nl + 2],convParamsBuf[20*nl + 3]);
//        X_Stride[nl][1] = (uint16_t)bf16tofloat(convParamsBuf[20*nl + 4],convParamsBuf[20*nl + 5],convParamsBuf[20*nl + 6],convParamsBuf[20*nl + 7]);
//        X_Padding[nl][0] = (uint16_t)bf16tofloat(convParamsBuf[20*nl + 8],convParamsBuf[20*nl + 9],convParamsBuf[20*nl + 10],convParamsBuf[20*nl + 11]);
//        X_Padding[nl][1] =(uint16_t)bf16tofloat(convParamsBuf[20*nl + 12],convParamsBuf[20*nl + 13],convParamsBuf[20*nl + 14],convParamsBuf[20*nl + 15]);
//        WordLines[nl] = (uint16_t)bf16tofloat(convParamsBuf[20*nl + 16],convParamsBuf[20*nl + 17],convParamsBuf[20*nl + 18],convParamsBuf[20*nl + 19]);
//        printf("\nStride, Padding, WordLines for cnv layers %d : {%d %d} {%d %d} {%d}", nl, X_Stride[nl][0],X_Stride[nl][1],
//                            X_Padding[nl][0],X_Padding[nl][1], WordLines[nl]);
//    }
//
//
//
////    uint16_t* WordLinesL = (uint16_t*)calloc(LIN_Layers,sizeof(uint16_t));
////    char *linParamsBuf = (char *)calloc(4*LIN_Layers, sizeof(char));
////	char fileName_l[] = "LinP.bin";
////	Status = ReadFile( fileName_l,(u32)linParamsBuf);
////    for(uint16_t nl = 0; nl<LIN_Layers; nl++){
////        WordLinesL[nl] = (uint16_t)bf16tofloat(linParamsBuf[20*nl + 0],linParamsBuf[20*nl + 1],linParamsBuf[20*nl + 2],linParamsBuf[20*nl + 3]);
////        printf("\n WordLines for lin layer %d : {%d}", nl,WordLinesL[nl]);
////    }
//
//    //Read MaxPool Parameters. How many maxpool == cnv??
//    uint16_t AvgPool_Layers = 1;
//    uint16_t** M_KSize = (uint16_t**)calloc(AvgPool_Layers,sizeof(uint16_t*)); //Kernals
//    uint16_t** M_Padding = (uint16_t**)calloc(AvgPool_Layers,sizeof(uint16_t*)); //Padding
//    uint16_t** M_Stride = (uint16_t**)calloc(AvgPool_Layers,sizeof(uint16_t*));  //Stride
//    uint16_t** M_Dilation = (uint16_t**)calloc(AvgPool_Layers,sizeof(uint16_t*)); //Dilation
//    for(uint16_t i = 0; i < AvgPool_Layers; i++){
//        M_Padding[i] = (uint16_t*)calloc(2,sizeof(uint16_t));
//        M_Stride[i] = (uint16_t*)calloc(2,sizeof(uint16_t));
//        M_KSize[i] = (uint16_t*)calloc(2,sizeof(uint16_t));
//        M_Dilation[i] = (uint16_t*)calloc(2,sizeof(uint16_t));
//    }
//
//    char *maxParamsBuf = (char *)calloc(32*AvgPool_Layers, sizeof(char));
//	char fileName_p[]="MPDat.bin";
//	Status = ReadFile( fileName_p,(u32)maxParamsBuf);
//    for(uint16_t nl = 0; nl<AvgPool_Layers; nl++){
//        M_KSize[nl][0] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 0],maxParamsBuf[32*nl + 1],maxParamsBuf[32*nl + 2],maxParamsBuf[32*nl + 3]);
//        M_KSize[nl][1] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 4],maxParamsBuf[32*nl + 5],maxParamsBuf[32*nl + 6],maxParamsBuf[32*nl + 7]);
//        M_Stride[nl][0] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 8],maxParamsBuf[32*nl + 9],maxParamsBuf[32*nl + 10],maxParamsBuf[32*nl + 11]);
//        M_Stride[nl][1] =(uint16_t)bf16tofloat(maxParamsBuf[32*nl + 12],maxParamsBuf[32*nl + 13],maxParamsBuf[32*nl + 14],maxParamsBuf[32*nl + 15]);
//        M_Padding[nl][0] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 16],maxParamsBuf[32*nl + 17],maxParamsBuf[32*nl + 18],maxParamsBuf[32*nl + 19]);
//        M_Padding[nl][1] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 20],maxParamsBuf[32*nl + 21],maxParamsBuf[32*nl + 22],maxParamsBuf[32*nl + 23]);
//        M_Dilation[nl][0] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 24],maxParamsBuf[32*nl + 25],maxParamsBuf[32*nl + 26],maxParamsBuf[32*nl + 29]);
//        M_Dilation[nl][1] = (uint16_t)bf16tofloat(maxParamsBuf[32*nl + 28],maxParamsBuf[32*nl + 29],maxParamsBuf[32*nl + 30],maxParamsBuf[32*nl + 31]);
//        printf("\n Kernal, Stride, Padding, Dilation for maxpool %d : {%d %d} {%d %d} {%d %d} {%d %d}", nl, M_KSize[nl][0], M_KSize[nl][1],
//                            M_Stride[nl][0],M_Stride[nl][1], M_Padding[nl][0],M_Padding[nl][1],M_Dilation[nl][0],M_Dilation[nl][1]);
//    }
//
//    #pragma endregion
//
//        //Add X mat, W Buffer, Bias Buffer, BatchNorm Buffer loading part here.
//    #pragma region
//    //Memory for Image Matrix
//    float **** X_Mat = (float****)calloc(X_Batch,sizeof(float***));
//    for(uint16_t b = 0; b < X_Batch; b++){
//        X_Mat[b] = (float***)calloc(X_Channels,sizeof(float**));
//        for(uint16_t ch = 0; ch < X_Channels; ch++){
//            X_Mat[b][ch] = (float**)calloc(X_Height,sizeof(float*));
//            for(uint16_t h = 0; h<X_Height;h++){
//                X_Mat[b][ch][h] = (float*)calloc(X_Width,sizeof(float));
//            }
//        }
//    }
//
//    //Memory for Image Matrix
//	float **** X_Mat2 = (float****)calloc(X_Batch2,sizeof(float***));
//	for(uint16_t b = 0; b < X_Batch2; b++){
//		X_Mat2[b] = (float***)calloc(X_Channels2,sizeof(float**));
//		for(uint16_t ch = 0; ch < X_Channels2; ch++){
//			X_Mat2[b][ch] = (float**)calloc(X_Height2,sizeof(float*));
//			for(uint16_t h = 0; h<X_Height2;h++){
//				X_Mat2[b][ch][h] = (float*)calloc(X_Width2,sizeof(float));
//			}
//		}
//	}
//
//    //Read Image from binary file
//    float t_var;
//	char fileName_xd[] = "XDat.bin" ;
//	uint32_t imBuf_size=(X_Batch*X_Channels*X_Height*X_Width*4);
//   //int imBuf_size= 0;
//	char *imageBuffer_xdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_xd,(u32)imageBuffer_xdata);
//	int X_Batch_start = 0000 ;//IMG SEARCH
//	uint16_t X_Batch_manual = 1024 ;
//	X_Batch = ( X_Batch <= X_Batch_manual ) ? X_Batch : X_Batch_manual ;//IMG SEARCH
//	unsigned long int bufIndex=0;
//	for(uint16_t i=0; i<X_Batch; i++)
//	{
//		for(uint16_t j=0; j<X_Channels;j++)
//		{
//			for(uint16_t k=0; k<X_Height; k++)
//			{
//				for(uint16_t l=0; l<X_Width; l++)
//				{
//					int i_see = i ;
//					int j_see = j ;
//					int k_see = k ;
//					int l_see = l ;
//					t_var= bf16tofloat(imageBuffer_xdata[bufIndex],imageBuffer_xdata[bufIndex+1],imageBuffer_xdata[bufIndex+2],imageBuffer_xdata[bufIndex+3]);
//					X_Mat[i_see][j_see][k_see][l_see]=t_var;
//					bufIndex+=4;
//				}
//			}
//		}
//		int pauseval=0;
//	}
////	free(imageBuffer_xdata);
//
//	//Read Image2 from binary file
//	float t_var2;
//	char fileName_xd2[] = "XDa2.bin" ;
//	uint32_t imBuf_size2=(X_Batch2*X_Channels2*X_Height2*X_Width2*4);
//   //int imBuf_size= 0;
////	char *imageBuffer_xdata2 = (char *)calloc(imBuf_size2, sizeof(char));
//	Status = ReadFile( fileName_xd2,(u32)imageBuffer_xdata);
//	int X_Batch_start2 = 0 ;//IMG SEARCH
//	X_Batch2 = X_Batch ;
//	unsigned long int bufIndex2=0;
//	for(uint16_t i=0; i<X_Batch2; i++)
//	{
//		for(uint16_t j=0; j<X_Channels2;j++)
//		{
//			for(uint16_t k=0; k<X_Height2; k++)
//			{
//				for(uint16_t l=0; l<X_Width2; l++)
//				{
//					int i_see = i ;
//					int j_see = j ;
//					int k_see = k ;
//					int l_see = l ;
//					t_var2= bf16tofloat(imageBuffer_xdata[bufIndex2],imageBuffer_xdata[bufIndex2+1],imageBuffer_xdata[bufIndex2+2],imageBuffer_xdata[bufIndex2+3]);
//					X_Mat2[i_see][j_see][k_see][l_see]=t_var2;
//					bufIndex2+=4;
//				}
//			}
//		}
//		int pauseval=0;
//	}
//	free(imageBuffer_xdata);
//
//    //Read Weights from binary file.
//    char fileName_wd[]="WDat.bin";
//	imBuf_size= 0;
//    for(uint16_t nl = 0; nl < Total_Layers; nl++){
//        if(W_Height[nl] == 0){
//        	imBuf_size += W_Batch[nl] * W_Channels[nl] *  4;
//        }
//        else{
//        	imBuf_size += W_Batch[nl] * W_Channels[nl] * W_Height[nl] * W_Width[nl] * 4; //4 bytes = float
//        }
//    }
//	char *imageBuffer_wdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_wd,(u32)imageBuffer_wdata);
//
//    ////Read Bias data from binary file.
//    char fileName_b[]="BDat.bin";
//	imBuf_size= 0;
//    for(uint16_t nl = 0; nl < Total_Layers; nl++){
//        imBuf_size += W_Batch[nl] * 4; //4 bytes = float
//    }
//	char *imageBuffer_biasdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_b,(u32)imageBuffer_biasdata);
//
//    //Read BatchNorm data from binary file.
//    char fileName_bn[]="bnDat.bin";
//	imBuf_size= 0;
//    for(uint16_t nl = 0; nl < CNV_Layers; nl++){
//        imBuf_size += W_Batch[nl] * 16; //4 bytes = float and 4 floats = (mu,var,weight,bias)
//    }
//	char *imageBuffer_bndata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_bn,(u32)imageBuffer_bndata);
//
//    //Read References
//	 char fileName_rdim[]="RDim.bin";
//	 char *imageBuffer_rdim = (char *)calloc(4, sizeof(char));
//	 Status = ReadFile( fileName_rdim,(u32)imageBuffer_rdim);
//	 uint32_t RBytes = bf16tofloat(imageBuffer_rdim[0],imageBuffer_rdim[1],imageBuffer_rdim[2],imageBuffer_rdim[3]);
//	 //245772;
//	 char fileName_rdata[]="RDat.bin";
//	 char *imageBuffer_rdata = (char *)calloc(RBytes, sizeof(char));
//     Status = ReadFile( fileName_rdata,(u32)imageBuffer_rdata);
//
//     //Read XMAX and WMAX data.
//     char fileName_XMAX[]="XM.bin";
//     char *imageBuffer_XMAX = (char *)calloc(4*Total_Layers*4, sizeof(char));
//     Status = ReadFile( fileName_XMAX,(u32)imageBuffer_XMAX);
//
//     char fileName_WMAX[]="WM.bin";
//     char *imageBuffer_WMAX = (char *)calloc(4*Total_Layers*4, sizeof(char));
//     Status = ReadFile( fileName_WMAX,(u32)imageBuffer_WMAX);
//     //Create array to store XMAX Data.
//     float** XMAX_Data = (float**)calloc(Total_Layers,sizeof(float));
//     float** WMAX_Data = (float**)calloc(Total_Layers,sizeof(float));
//     for(uint16_t i = 0; i < Total_Layers; i++){
//    	 XMAX_Data[i] = (float*)calloc(4,sizeof(float));
//    	 WMAX_Data[i] = (float*)calloc(4,sizeof(float));
//     }
//     bufIndex2 = 0;
//     for(uint16_t i = 0; i < Total_Layers; i++){
//    	 for(uint16_t j = 0; j < 4; j++){
//    		 XMAX_Data[i][j] = bf16tofloat(imageBuffer_XMAX[bufIndex2],imageBuffer_XMAX[bufIndex2+1],imageBuffer_XMAX[bufIndex2+2],imageBuffer_XMAX[bufIndex2+3]);
//    		 WMAX_Data[i][j] = bf16tofloat(imageBuffer_WMAX[bufIndex2],imageBuffer_WMAX[bufIndex2+1],imageBuffer_WMAX[bufIndex2+2],imageBuffer_WMAX[bufIndex2+3]);
//    		 bufIndex2 += 4;
//    	 }
//     }
//
//    #pragma endregion
//
//
//     //First Layer Convolution + Relu .
//    uint16_t Layer_ID = 0;
//    #pragma region
//
//    //Convolution.
//    #pragma region
//    //Create memory for CNV_Out. batch * kernels * Height * Width.
//    uint16_t X_paddedHeight = X_Height + 2 * X_Padding[Layer_ID][0];
//    uint16_t X_paddedWidth = X_Width + 2 * X_Padding[Layer_ID][1];
//    uint16_t X_out_h1 = ((X_paddedHeight - W_Height[Layer_ID]) / X_Stride[Layer_ID][0] + 1 );
//    uint16_t X_out_w1 = ((X_paddedWidth - W_Width[Layer_ID]) / X_Stride[Layer_ID][1] + 1 );
//    uint16_t W_BitMat_Batch1 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
//    uint16_t X_BitMat_Batch1 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
//
//    float**** CNV_Out1 = (float****)calloc(X_BitMat_Batch1,sizeof(float***));
//    for(uint16_t b = 0; b < X_BitMat_Batch1; b++){
//        CNV_Out1[b] = (float***)calloc(W_BitMat_Batch1,sizeof(float**));
//        for(uint16_t c = 0; c < W_BitMat_Batch1; c++){
//            CNV_Out1[b][c] = (float**)calloc(X_out_h1,sizeof(float*));
//            for(uint16_t h = 0; h < X_out_h1; h++){
//                CNV_Out1[b][c][h] = (float*)calloc(X_out_w1,sizeof(float));
//            }
//        }
//    }
//    //Create and load weight matrix for this layer.
//    //Memory for Weight Matrix
//    float **** W_Mat = (float****)calloc(W_Batch[Layer_ID],sizeof(float***));
//    for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
//        W_Mat[b] = (float***)calloc(W_Channels[Layer_ID],sizeof(float**));
//        for(uint16_t ch = 0; ch < W_Channels[Layer_ID]; ch++){
//            W_Mat[b][ch] = (float**)calloc(W_Height[Layer_ID],sizeof(float*));
//            for(uint16_t h = 0; h<W_Height[Layer_ID];h++){
//                W_Mat[b][ch][h] = (float*)calloc(W_Width[Layer_ID],sizeof(float));
//            }
//        }
//    }
//    //Extract W_Mat of Layer_ID from buffer.
//    getWeightsForLayer(imageBuffer_wdata,Total_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat);
//
//    //Create memory for bias.
//    float* Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//
//    //Extract Bias from buffer.
//    getBiasForLayer(imageBuffer_biasdata,Total_Layers,Layer_ID,W_Batch,Bias);
//
//    //Extract Reference from buffer
//    //Create memory for ref array = chunks * 2 * na(8) * nwl
//	uint16_t chunks = ceil((float)(X_out_h1*X_out_w1)/(float)WordLines[Layer_ID]); //Input Features/Wordlines
//	float**** Ref = (float****)calloc(chunks,sizeof(float***));
//	for(uint16_t c = 0;c < chunks; c++){
//		Ref[c] = (float***)calloc(2,sizeof(float**));
//		for(uint16_t i = 0;i < 2; i++){
//			Ref[c][i] = (float**)calloc(8,sizeof(float*));
//			for(uint16_t n = 0; n < 8; n++)
//				Ref[c][i][n] = (float*)calloc(WordLines[Layer_ID],sizeof(float)); //need to make it layer specific
//		}
//	}
//	//Create memory for RefB array. chunks * 2 * na(4) * nwl
//	float*** RefB = (float***)calloc(chunks,sizeof(float**));
//	for(uint16_t c = 0;c < chunks; c++){
//		RefB[c] = (float**)calloc(4,sizeof(float*));
//		for(uint16_t n = 0; n < 4; n++){
//			RefB[c][n] = (float*)calloc(WordLines[Layer_ID],sizeof(float)); //need to make it layer specific
//		}
//	}
//	//offset bits till this layer.
//	uint32_t offset_bytes = 0; //Chunks[:] * 2 * Na (8 + 4) * WordLines[:]
//
//	//get ref matrix.
//	float* RefMax = (float*)calloc(chunks,sizeof(float));;
//	getRefForLayer(imageBuffer_rdata,offset_bytes,chunks,8,4,WordLines[Layer_ID],Ref,RefB,RefMax);
//
//	//float XMAX = 2.082469463348388671875;
//	//float WMAX = 0.21883629262447357177734375;
//	// CONVOLUTION LAYER STARTS
//	{
//	mode_type mode;
//		//	mode.mode=mode_OCC;
//		mode.mode=mode_ICQ;
//		mode.LSB=0.56;
//		//mode.LC=-16.9;
//		mode.I_mean=7.406149405110749e-07; // from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\034_Alphas_Ashwin\1_XFER\MODELLING_RESULTS.mat -> variable Imean
//		mode.slopecorrect=1;
//		mode.slopecorrect_chip=0.9051; // from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\032_SystemMAC_DryRuns_ASHWIN\cifar100R20\CONV2_66_L23_FX\2_INTERNAL_XADC_floatFix\xm11\LAYER1_XADC\DP_MAC_RX.m
//		mode.slopecorrect_XADC=1;
//	// method1: batched (starts)
//		int mini_batch_size_lim=8;//mini batch size search
//		int mini_batch_size=( X_Batch > mini_batch_size_lim ? mini_batch_size_lim : X_Batch );
//		for( int b=X_Batch_start; b<X_Batch; b+=mini_batch_size ){
//			currObj = convolve_FP(&X_Mat[b],mini_batch_size,X_Channels,X_Height,X_Width ,X_Padding[Layer_ID][0],X_Stride[Layer_ID][0],W_Mat,W_Batch[Layer_ID],W_Channels[Layer_ID],W_Height[Layer_ID],W_Width[Layer_ID],
//							W_Padding, W_Stride, WordLines[Layer_ID], XDL, XBL, WDL, WBL, IMC, Bias,Ref,RefB,RefMax,XMAX_Data[Layer_ID],WMAX_Data[Layer_ID],&CNV_Out1[b],mode,slopeSetting,currObj);
//			// ADDED BY ASHWIN STARTS
//			// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT END OF CONVOLVE
//			{
//				u8 syncdata=0;
//				// UART ADDED CODE STARTS HERE
//				uint16_t returnWords[]={convolvelinear_end_word};
//				if( b + mini_batch_size >= X_Batch ){
//						 returnWords[0]=convolvelinear_batchend_word;//0 is stride
//				}
//				int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
//				sendWordData( returnWords, numWords, myUart ) ;
//				// UART ADDED CODE ENDS HERE
//				// WAITING FOR SYNC DATA FROM MATLAB
//				do
//				{
//					u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
//				}while(syncdata!=syncdata_val);
//			}}
//			// ADDED BY ASHWIN ENDS
//			// method1: batched (ends)
//	}
//	// CONVOLUTION LAYER ENDS
//	// Write out the layer output starts
//	{
//		int _b=X_BitMat_Batch1;
//		int _c=W_BitMat_Batch1;
//		int _h=X_out_h1;
//		int _w=X_out_w1;
//
//		float D_see[_b][_c][_h][_w] ;
//			for(uint16_t b = 0; b < _b; b++){
//				for(uint16_t c = 0; c < _c; c++){
//					for(uint16_t h = 0; h < _h; h++){
//						for(uint16_t w = 0; w < _w; w++){
//							D_see[b][c][h][w]=CNV_Out1[b][c][h][w];
//						}
//					}
//				}
//			}
//			{
//			char FileName_outI[]="LO1.bin";
//			u32 size=_b*_c*_h*_w*sizeof(D_see[0][0][0][0]);
//			u32 SourceAddress;
//
//
//				float size_params[]={_b,_c,_h,_w};
//				int size_params_size=sizeof( size_params ) ;
//				FRESULT rc=WriteFile_custom(FileName_outI, size_params_size, size_params);
//						rc=WriteFile_fseekappend(FileName_outI, size, D_see);
//			}
//	}
//	// Write out the layer output ends
//	// ADDED BY ASHWIN STARTS HERE (TO INDICATE TO MATLAB THAT THERE ARE SOME LAYERS LEFT)
//	{
//		uint16_t returnWords[]={layers_continue_word};
//		int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
//		sendWordData( returnWords, numWords, myUart ) ;
//	}
//
//
//
//    //Deallocate Bias Mat
//    free(Bias);
//
//    //Deallocate Weight Matrix
//    for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
//        for(uint16_t ch = 0; ch < W_Channels[Layer_ID]; ch++){
//            for(uint16_t h = 0; h < W_Height[Layer_ID]; h++){
//                free(W_Mat[b][ch][h]);
//            }
//            free(W_Mat[b][ch]);
//        }
//        free(W_Mat[b]);
//    }
//    free(W_Mat);
//
//    //Deallocate Ref Matrix.
//
//	for(uint16_t c = 0;c < chunks; c++){
//		for(uint16_t i = 0;i < 2; i++){
//			for(uint16_t n = 0; n < 8; n++){
//				free(Ref[c][i][n]);
//			}
//			free(Ref[c][i]);
//		}
//		free(Ref[c]);
//	}
//	free(Ref);
//
//	for(uint16_t c = 0;c < chunks; c++){
//		for(uint16_t n = 0; n < 4; n++){
//			free(RefB[c][n]);
//		}
//		free(RefB[c]);
//	}
//	free(RefB);
//
//	free(RefMax);
//
//    #pragma endregion
//
//	//Batch Norm.
//	#pragma region
//	//Create memory for batch norm parameters.
//	float ** BN_Data = (float**)calloc(4,sizeof(float*)); //4 = mu,var,weight,bias
//	for(uint8_t i = 0; i < 4; i++){
//		BN_Data[i] = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//	}
//
//	//Extract batch norm parameters for LayerID from buffer.
//	getBNForLayer(imageBuffer_bndata,Total_Layers,Layer_ID,W_Batch,BN_Data);
//
//	//Perform Batch Normalisation. Replaces same CNV_Out.
//	BatchNorm(CNV_Out1,X_Batch,W_Batch[Layer_ID],X_out_h1,X_out_w1,BN_Data[0],BN_Data[1],BN_Data[2],BN_Data[3],CNV_Out1);
//
//	//Deallocate batch norm parameters
//	for(uint8_t i = 0; i < 4; i++){
//		free(BN_Data[i]);
//	}
//	free(BN_Data);
//
//	#pragma endregion
//
//
//
//    //End of the Layer
//
//	//Add X_Mat2 and CNV_Out1
//	add4DArrayElementwise(CNV_Out1,X_Mat2, X_Batch2,X_Channels2,X_out_h1,X_out_w1);
//
//	//ReLU
//	#pragma region
//
//	//perform ReLU here. Replaces CNV_Out
//	ReLU(CNV_Out1,X_Batch,W_Batch[Layer_ID],X_out_h1,X_out_w1,CNV_Out1);
//
//	#pragma endregion
//
//	#pragma endregion
//
//	//End of the Layer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    //First Layer Convolution + Relu .
//   Layer_ID = 1;
//   #pragma region
//
//   //Convolution.
//   #pragma region
//   //Create memory for CNV_Out. batch * kernels * Height * Width.
//   X_paddedHeight = X_out_h1 + 2 * X_Padding[Layer_ID][0];
//   X_paddedWidth = X_out_w1 + 2 * X_Padding[Layer_ID][1];
//   uint16_t X_out_h2 = ((X_paddedHeight - W_Height[Layer_ID]) / X_Stride[Layer_ID][0] + 1 );
//   uint16_t X_out_w2 = ((X_paddedWidth - W_Width[Layer_ID]) / X_Stride[Layer_ID][1] + 1 );
//   uint16_t W_BitMat_Batch2 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
//   uint16_t X_BitMat_Batch2 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
//
//   float**** CNV_Out2 = (float****)calloc(X_BitMat_Batch2,sizeof(float***));
//   for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//       CNV_Out2[b] = (float***)calloc(W_BitMat_Batch2,sizeof(float**));
//       for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
//           CNV_Out2[b][c] = (float**)calloc(X_out_h2,sizeof(float*));
//           for(uint16_t h = 0; h < X_out_h2; h++){
//               CNV_Out2[b][c][h] = (float*)calloc(X_out_w2,sizeof(float));
//           }
//       }
//   }
//   //Create and load weight matrix for this layer.
//   //Memory for Weight Matrix
//   W_Mat = (float****)calloc(W_Batch[Layer_ID],sizeof(float***));
//   for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
//       W_Mat[b] = (float***)calloc(W_Channels[Layer_ID],sizeof(float**));
//       for(uint16_t ch = 0; ch < W_Channels[Layer_ID]; ch++){
//           W_Mat[b][ch] = (float**)calloc(W_Height[Layer_ID],sizeof(float*));
//           for(uint16_t h = 0; h<W_Height[Layer_ID];h++){
//               W_Mat[b][ch][h] = (float*)calloc(W_Width[Layer_ID],sizeof(float));
//           }
//       }
//   }
//   //Extract W_Mat of Layer_ID from buffer.
//   getWeightsForLayer(imageBuffer_wdata,Total_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat);
//
//   //Create memory for bias.
//   Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//
//   //Extract Bias from buffer.
//   getBiasForLayer(imageBuffer_biasdata,Total_Layers,Layer_ID,W_Batch,Bias);
//
//   //Extract Reference from buffer
//   //Create memory for ref array = chunks * 2 * na(8) * nwl
//   //offset bits till this layer.
//   	//offset_bytes += chunks * 20 * WordLines[Layer_ID - 1] * 4; //Chunks[:] * 2 * Na (8 + 4) * WordLines[:]
//    offset_bytes += ( (chunks * ( ( 2 * 8 + 4 ) * WordLines[Layer_ID-1] + 1 )  ) * 4); // MODIFIED BY ASHWIN TO INCLUDE THE 4 extra for REFMAX. Na=8 Nb=4
//	chunks = ceil((float)(X_out_h2*X_out_w2)/(float)WordLines[Layer_ID]); //Input Features/Wordlines
//	Ref = (float****)calloc(chunks,sizeof(float***));
//	for(uint16_t c = 0;c < chunks; c++){
//		Ref[c] = (float***)calloc(2,sizeof(float**));
//		for(uint16_t i = 0;i < 2; i++){
//			Ref[c][i] = (float**)calloc(8,sizeof(float*));
//			for(uint16_t n = 0; n < 8; n++)
//				Ref[c][i][n] = (float*)calloc(WordLines[Layer_ID],sizeof(float)); //need to make it layer specific
//		}
//	}
//	//Create memory for RefB array. chunks * 2 * na(4) * nwl
//	RefB = (float***)calloc(chunks,sizeof(float**));
//	for(uint16_t c = 0;c < chunks; c++){
//		RefB[c] = (float**)calloc(4,sizeof(float*));
//		for(uint16_t n = 0; n < 4; n++){
//			RefB[c][n] = (float*)calloc(WordLines[Layer_ID],sizeof(float)); //need to make it layer specific
//		}
//	}
//
//
//	//get ref matrix.
//	RefMax = (float*)calloc(chunks,sizeof(float));;
//	getRefForLayer(imageBuffer_rdata,offset_bytes,chunks,8,4,WordLines[Layer_ID],Ref,RefB,RefMax);
//	// Write out the layer input starts
//	{
//		int _b=X_BitMat_Batch1;
//		int _c=W_BitMat_Batch1;
//		int _h=X_out_h1;
//		int _w=X_out_w1;
//
//		float D_see[_b][_c][_h][_w] ;
//			for(uint16_t b = 0; b < _b; b++){
//				for(uint16_t c = 0; c < _c; c++){
//					for(uint16_t h = 0; h < _h; h++){
//						for(uint16_t w = 0; w < _w; w++){
//							D_see[b][c][h][w]=CNV_Out1[b][c][h][w];
//						}
//					}
//				}
//			}
//			{
//			char FileName_outI[]="LI2.bin";
//			u32 size=_b*_c*_h*_w*sizeof(D_see[0][0][0][0]);
//			u32 SourceAddress;
//
//
//				float size_params[]={_b,_c,_h,_w};
//				int size_params_size=sizeof( size_params ) ;
//				FRESULT rc=WriteFile_custom(FileName_outI, size_params_size, size_params);
//						rc=WriteFile_fseekappend(FileName_outI, size, D_see);
//			}
//	}
//	// Write out the layer input ends
//	// CONVOLUTION LAYER STARTS
//	{
//	mode_type mode;
//		//	mode.mode=mode_OCC;
//		mode.mode=mode_ICQ;
//		mode.LSB=0.56;
//		//mode.LC=-16.9;
//		mode.I_mean=7.406149405110749e-07; // from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\034_Alphas_Ashwin\1_XFER\MODELLING_RESULTS.mat -> variable Imean
//		mode.slopecorrect=1;
//		mode.slopecorrect_chip=0.9175;// from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\032_SystemMAC_DryRuns_ASHWIN\cifar100R20\CONV2_66_L23_FX\2_INTERNAL_XADC_floatFix\xm11\LAYER1_then_LAYER2_XADC\DP_MAC_RX.m
//		mode.slopecorrect_XADC=1;
//	// method1: batched (starts)
//		int mini_batch_size_lim=8;//mini batch size search
//		int mini_batch_size=( X_Batch > mini_batch_size_lim ? mini_batch_size_lim : X_Batch );
//		for( int b=X_Batch_start; b<X_Batch; b+=mini_batch_size ){
//		currObj = convolve_FP(&CNV_Out1[b],mini_batch_size,W_Batch[Layer_ID - 1],X_out_h2 ,X_out_w2,X_Padding[Layer_ID][0],X_Stride[Layer_ID][0],W_Mat,W_Batch[Layer_ID],W_Channels[Layer_ID],W_Height[Layer_ID],W_Width[Layer_ID],
//						W_Padding, W_Stride, WordLines[Layer_ID], XDL, XBL, WDL, WBL, IMC, Bias,Ref,RefB,RefMax,XMAX_Data[Layer_ID],WMAX_Data[Layer_ID],&CNV_Out2[b],mode,slopeSetting,currObj);
//	// ADDED BY ASHWIN STARTS
//		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT END OF CONVOLVE
//		{
//			u8 syncdata=0;
//			// UART ADDED CODE STARTS HERE
//			uint16_t returnWords[]={convolvelinear_end_word};
//			if( b + mini_batch_size >= X_Batch ){
//					 returnWords[0]=convolvelinear_batchend_word;//0 is stride
//			}
//			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
//			sendWordData( returnWords, numWords, myUart ) ;
//			// UART ADDED CODE ENDS HERE
//			// WAITING FOR SYNC DATA FROM MATLAB
//			do
//			{
//				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
//			}while(syncdata!=syncdata_val);
//		}}
//		// ADDED BY ASHWIN ENDS
//		// method1: batched (ends)
//	}
//	//Deallocate Bias Mat
//	    free(Bias);
//
//	    //Deallocate Weight Matrix
//	    for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
//	        for(uint16_t ch = 0; ch < W_Channels[Layer_ID]; ch++){
//	            for(uint16_t h = 0; h < W_Height[Layer_ID]; h++){
//	                free(W_Mat[b][ch][h]);
//	            }
//	            free(W_Mat[b][ch]);
//	        }
//	        free(W_Mat[b]);
//	    }
//	    free(W_Mat);
//
//	    //Deallocate Ref Matrix.
//
//		for(uint16_t c = 0;c < chunks; c++){
//			for(uint16_t i = 0;i < 2; i++){
//				for(uint16_t n = 0; n < 8; n++){
//					free(Ref[c][i][n]);
//				}
//				free(Ref[c][i]);
//			}
//			free(Ref[c]);
//		}
//		free(Ref);
//
//		for(uint16_t c = 0;c < chunks; c++){
//			for(uint16_t n = 0; n < 4; n++){
//				free(RefB[c][n]);
//			}
//			free(RefB[c]);
//		}
//		free(RefB);
//
//		free(RefMax);
//
//
//	// CONVOLUTION LAYER ENDS
//	//Batch Norm starts
//		#pragma region
//		//Create memory for batch norm parameters.
//		BN_Data = (float**)calloc(4,sizeof(float*)); //4 = mu,var,weight,bias
//		for(uint8_t i = 0; i < 4; i++){
//			BN_Data[i] = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//		}
//
//		//Extract batch norm parameters for LayerID from buffer.
//		getBNForLayer(imageBuffer_bndata,Total_Layers,Layer_ID,W_Batch,BN_Data);
//
//		//Perform Batch Normalisation. Replaces same CNV_Out.
//		BatchNorm(CNV_Out2,X_Batch,W_Batch[Layer_ID],X_out_h2,X_out_w2,BN_Data[0],BN_Data[1],BN_Data[2],BN_Data[3],CNV_Out2);
//
//		//Deallocate batch norm parameters
//			for(uint8_t i = 0; i < 4; i++){
//				free(BN_Data[i]);
//			}
//			free(BN_Data);
//	//Batch Norm ends
//	//perform ReLU here
//		ReLU(CNV_Out2,X_Batch,W_Batch[Layer_ID],X_out_h2,X_out_w2,CNV_Out2);
//	// Write out the layer output starts
//		{
//			int _b=X_BitMat_Batch2;
//			int _c=W_BitMat_Batch2;
//			int _h=X_out_h2;
//			int _w=X_out_w2;
//
//			float D_see[_b][_c][_h][_w] ;
//				for(uint16_t b = 0; b < _b; b++){
//					for(uint16_t c = 0; c < _c; c++){
//						for(uint16_t h = 0; h < _h; h++){
//							for(uint16_t w = 0; w < _w; w++){
//								D_see[b][c][h][w]=CNV_Out2[b][c][h][w];
//							}
//						}
//					}
//				}
//				{
//				char FileName_outI[]="LO2.bin";
//				u32 size=_b*_c*_h*_w*sizeof(D_see[0][0][0][0]);
//				u32 SourceAddress;
//
//
//					float size_params[]={_b,_c,_h,_w};
//					int size_params_size=sizeof( size_params ) ;
//					FRESULT rc=WriteFile_custom(FileName_outI, size_params_size, size_params);
//							rc=WriteFile_fseekappend(FileName_outI, size, D_see);
//				}
//		}
//		// Write out the layer output ends
//		// ADDED BY ASHWIN STARTS HERE (TO INDICATE TO MATLAB THAT ALL THE LAYERS ARE DONE )
//		{
//			uint16_t returnWords[]={layers_end_word};
//			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
//			sendWordData( returnWords, numWords, myUart ) ;
//		}
//		// ADDED BY ASHWIN ENDS HERE (TO INDICATE TO MATLAB THAT ALL THE LAYERS ARE DONE )
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    //Deallocate Buffers
//    #pragma region
//
//    //Deallocate Image Matrix
//    for(uint16_t b = 0; b < X_Batch; b++){
//        for(uint16_t ch = 0; ch < X_Channels; ch++){
//            for(uint16_t h = 0; h<X_Height;h++){
//                free(X_Mat[b][ch][h]);
//            }
//            free(X_Mat[b][ch]);
//        }
//        free(X_Mat[b]);
//    }
//    free(X_Mat);
//
//    //Deallocate CNV_Out1
//    for(uint16_t b = 0; b < X_BitMat_Batch1; b++){
//        for(uint16_t c = 0; c < W_BitMat_Batch1; c++){
//            for(uint16_t h = 0; h < X_out_h1; h++){
//                free(CNV_Out1[b][c][h]);
//            }
//            free(CNV_Out1[b][c]);
//        }
//        free(CNV_Out1[b]);
//    }
//    free(CNV_Out1);
//
//    //Deallocate CNV_Out1
//	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//		for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
//			for(uint16_t h = 0; h < X_out_h2; h++){
//				free(CNV_Out2[b][c][h]);
//			}
//			free(CNV_Out2[b][c]);
//		}
//		free(CNV_Out2[b]);
//	}
//	free(CNV_Out2);
//
//
//    //Deallocate file buffers.
//    free(imageBuffer_xdim);
//    free(imageBuffer_wdim);
//    //free(imageBuffer_xdata);
//    free(imageBuffer_wdata);
//    free(imageBuffer_bndata);
//    free(imageBuffer_biasdata);
//    free(convParamsBuf);
//    free(maxParamsBuf);
//    free(imageBuffer_XMAX);
//    free(imageBuffer_WMAX);
//    free(WordLines);
//    //free(WordLinesL);
//    free(W_Batch);
//    free(W_Channels);
//    free(W_Height);
//    free(W_Width);
//    for(uint16_t i = 0; i < AvgPool_Layers; i++){
//        free(X_Padding[i]);
//        free(X_Stride[i]);
//        free(M_Stride[i]);
//        free(M_Padding[i]);
//        free(M_Dilation[i]);
//        free(M_KSize[i]);
//    }
//    for(uint16_t i = 0; i < Total_Layers; i++){
//    	free(XMAX_Data[i]);
//    	free(WMAX_Data[i]);
//    }
//    free(XMAX_Data);
//    free(WMAX_Data);
//    free(X_Padding);
//    free(X_Stride);
//    free(M_Stride);
//    free(M_Padding);
//    free(M_Dilation);
//    free(M_KSize);
//    free(imageBuffer_rdim);
//    free(imageBuffer_rdata);
//    Status=SD_Eject(&fatfs);
//    #pragma endregion
//
//    printf("\nCode Exited Succesfully");
//
//
//    return 0;
//}
//
//
//
