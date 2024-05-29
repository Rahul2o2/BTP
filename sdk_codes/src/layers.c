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
//	u8 abcd[200000];
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
//
////long t1 = getMicrotime();
//    //Initialise all the constants here.
//    #pragma region
//	XTime tStart;
//	XTime_GetTime(&tStart);
//    //Image Matrix = X matrix
//	uint16_t X_Batch = 0; //Batch size for Images
//    uint16_t X_Channels = 0; //No of channels in each image
//    uint16_t X_Height = 0; //Height of the each image
//    uint16_t X_Width = 0; //Width of the input image
//
//    int Status;
//    Status = SD_Init(&fatfs);
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
//    //Read the .bin to get NL -> Number of Layers.
//    char* NL_Buffer = (char*)calloc(4,sizeof(char));
//    char fileName_nl[] = "WNL.bin";
//    Status = ReadFile( fileName_nl,(u32)NL_Buffer);
//    uint16_t Num_Layers = (uint16_t)bf16tofloat(NL_Buffer[0],NL_Buffer[1],NL_Buffer[2],NL_Buffer[3]);
//    printf("\nNumber of Layers: %d",Num_Layers);
//
//    //Kernel Matrix = W matrix. This will be per layer.
//    uint16_t* W_Batch = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t)); //Batch size for kernels
//    uint16_t* W_Channels = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t)); //No of channels in each kernel
//    uint16_t* W_Height = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t)); //Height of the each kernel
//    uint16_t* W_Width = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t)); //Width of the input kernel
//    uint16_t W_Padding = 0; //Padding  (Required because same unfolding function as image is used) (Do not change)
//    uint16_t W_Stride = 1;  //Stride (Required because same unfolding function as image is used) (Do not change)
//
//    char *imageBuffer_wdim = (char *)calloc(16*Num_Layers, sizeof(char));
//	char fileName_wm[]="WDim.bin";
//	Status = ReadFile( fileName_wm,(u32)imageBuffer_wdim);
//
//    for(uint16_t nl = 0; nl<Num_Layers; nl++){
//        W_Batch[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 0],imageBuffer_wdim[16*nl + 1],imageBuffer_wdim[16*nl + 2],imageBuffer_wdim[16*nl + 3]);
//        W_Channels[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 4],imageBuffer_wdim[16*nl + 5],imageBuffer_wdim[16*nl + 6],imageBuffer_wdim[16*nl + 7]);
//        W_Height[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 8],imageBuffer_wdim[16*nl + 9],imageBuffer_wdim[16*nl + 10],imageBuffer_wdim[16*nl + 11]);
//        W_Width[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 12],imageBuffer_wdim[16*nl + 13],imageBuffer_wdim[16*nl + 14],imageBuffer_wdim[16*nl + 15]);
//        printf("\nWeight Dimensions for layer %d : %d %d %d %d", nl, W_Batch[nl], W_Channels[nl], W_Height[nl], W_Width[nl]);
//    }
//    #pragma endregion
//
//    //IMC + Convolution Constants
//    #pragma region
//    uint16_t* X_Padding = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t)); //Padding
//    uint16_t* X_Stride = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t));  //Stride
//    uint16_t* WordLines = (uint16_t*)calloc(Num_Layers,sizeof(uint16_t)); //Number of word lines available for IMC at a time.
//    uint16_t XDL = 1; // XDL is the number of image DLs 8
//    uint16_t XBL = 1; // XBL is the number of image pxl mantissa bits being fed
//    uint16_t WDL = 72; // WDL is the number of weight DLs 72
//    uint16_t WBL = 4; // WBL is the number of weight mantissa bits being fed
//
//    char *convParamsBuf = (char *)calloc(12*Num_Layers, sizeof(char));
//	char fileName_c[]="convP.bin";
//	Status = ReadFile( fileName_c,(u32)convParamsBuf);
//    for(uint16_t nl = 0; nl<Num_Layers; nl++){
//        X_Stride[nl] = (uint16_t)bf16tofloat(convParamsBuf[12*nl + 0],convParamsBuf[12*nl + 1],convParamsBuf[12*nl + 2],convParamsBuf[12*nl + 3]);
//        X_Padding[nl] = (uint16_t)bf16tofloat(convParamsBuf[12*nl + 4],convParamsBuf[12*nl + 5],convParamsBuf[12*nl + 6],convParamsBuf[12*nl + 7]);
//        WordLines[nl] = (uint16_t)bf16tofloat(convParamsBuf[12*nl + 8],convParamsBuf[12*nl + 9],convParamsBuf[12*nl + 10],convParamsBuf[12*nl + 11]);
//        printf("\nStride, Padding, WordLines for layer %d : %d %d %d %d", nl, X_Stride[nl], X_Padding[nl], WordLines[nl]);
//    }
//    #pragma endregion
//
//    //Add X mat, W Buffer, Bias Buffer, BatchNorm Buffer loading part here.
//    #pragma region
//    //Memeory for Image Matrix
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
//    //Read Image from binary file
//    float t_var;
//	char fileName_xd[] = "XDat.bin" ;
//	int imBuf_size=(X_Batch*X_Channels*X_Height*X_Width*4);
//	char *imageBuffer_xdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_xd,(u32)imageBuffer_xdata);
//
//	unsigned long int bufIndex=0;
//	for(uint16_t i=0; i<X_Batch; i++)
//	{
//		for(uint16_t j=0; j<X_Channels;j++)
//		{
//			for(uint16_t k=0; k<X_Height; k++)
//			{
//				for(uint16_t l=0; l<X_Width; l++)
//				{
//					t_var= bf16tofloat(imageBuffer_xdata[bufIndex],imageBuffer_xdata[bufIndex+1],imageBuffer_xdata[bufIndex+2],imageBuffer_xdata[bufIndex+3]);
//					X_Mat[i][j][k][l]=t_var;
//					bufIndex+=4;
//				}
//			}
//		}
//	}
//	free(imageBuffer_xdata);
//    //Read Weights from binary file.
//    char fileName_wd[]="WDat.bin";
//	imBuf_size= 0;
//    for(uint16_t nl = 0; nl < Num_Layers; nl++){
//        imBuf_size += W_Batch[nl] * W_Channels[nl] * W_Height[nl] * W_Width[nl] * 4; //4 bytes = float
//    }
//	char *imageBuffer_wdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_wd,(u32)imageBuffer_wdata);
//
//    ////Read Bias data from binary file.
//    char fileName_b[]="BDat.bin";
//	imBuf_size= 0;
//    for(uint16_t nl = 0; nl < Num_Layers; nl++){
//        imBuf_size += W_Batch[nl] * 4; //4 bytes = float
//    }
//	char *imageBuffer_biasdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_b,(u32)imageBuffer_biasdata);
//
//    //Read BatchNorm data from binary file.
//    char fileName_bn[]="bnDat.bin";
//	imBuf_size= 0;
//    for(uint16_t nl = 0; nl < Num_Layers; nl++){
//        imBuf_size += W_Batch[nl] * 16; //4 bytes = float and 4 floats = (mu,var,weight,bias)
//    }
//	char *imageBuffer_bndata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_bn,(u32)imageBuffer_bndata);
//
//	Status=SD_Eject(&fatfs);
//
//    #pragma endregion
//
//    //long t2 = getMicrotime();
//    //printf("\n Time required to load constants from files: %ld usec",t2-t1);
//    //W_Batch = 1;
//    //X_Batch = 1;
//    //WordLines[0] = 9;
//    //WordLines[1] = 144;
//    //WordLines[2] = 144;
//    bool IMC = true;
//    X_Batch = 1;
//
//    //First Layer Convolution + BatchNorm + Relu here.
//    uint16_t Layer_ID = 0;
//    #pragma region
//
//    //Convolution.
//    #pragma region
//    //Create memory for CNV_Out. batch * kernels * Height * Width.
//    uint16_t X_paddedHeight = X_Height + 2 * X_Padding[Layer_ID];
//    uint16_t X_paddedWidth = X_Width + 2 * X_Padding[Layer_ID];
//    uint16_t X_out_h1 = ((X_paddedHeight - W_Height[Layer_ID]) / X_Stride[Layer_ID] + 1 );
//    uint16_t X_out_w1 = ((X_paddedWidth - W_Width[Layer_ID]) / X_Stride[Layer_ID] + 1 );
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
//    getWeightsForLayer(imageBuffer_wdata,Num_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat);
//
//    //Create memory for bias.
//    float* Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//
//    //Extract Bias from buffer.
//    getBiasForLayer(imageBuffer_biasdata,Num_Layers,Layer_ID,W_Batch,Bias);
//
//    currObj=convolve(X_Mat,X_Batch,X_Channels,X_Height,X_Width,X_Padding[Layer_ID],X_Stride[Layer_ID],W_Mat,W_Batch[Layer_ID],W_Channels[Layer_ID],W_Height[Layer_ID],W_Width[Layer_ID],
//            W_Padding, W_Stride, WordLines[Layer_ID], XDL, XBL, WDL, WBL, IMC, Bias, CNV_Out1,
//			currObj);
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
//    #pragma endregion
//
//    //Batch Norm
//    #pragma region
//    //Create memory for batch norm parameters.
//    float ** BN_Data = (float**)calloc(4,sizeof(float*)); //4 = mu,var,weight,bias
//    for(uint8_t i = 0; i < 4; i++){
//        BN_Data[i] = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//    }
//
//    //Extract batch norm parameters for LayerID from buffer.
//    getBNForLayer(imageBuffer_bndata,Num_Layers,Layer_ID,W_Batch,BN_Data);
//
//    //Perform Batch Normalisation. Replaces same CNV_Out.
//    BatchNorm(CNV_Out1,X_Batch,W_Batch[Layer_ID],X_out_h1,X_out_w1,BN_Data[0],BN_Data[1],BN_Data[2],BN_Data[3],CNV_Out1);
//
//    //Deallocate batch norm parameters
//    for(uint8_t i = 0; i < 4; i++){
//        free(BN_Data[i]);
//    }
//    free(BN_Data);
//
//    #pragma endregion
//
//    //ReLU
//    #pragma region
//
//    //perform ReLU here. Replaces CNV_Out
//    ReLU(CNV_Out1,X_Batch,W_Batch[Layer_ID],X_out_h1,X_out_w1,CNV_Out1);
//
//    #pragma endregion
//
//    //End of the Layer
//    #pragma endregion
//
//    printf("\nFirst Layer Done\n");
//    //Second Layer Convolution + BatchNorm + Relu here.
//    Layer_ID = 1;
//    #pragma region
//
//    //Convolution.
//    #pragma region
//    //Create memory for CNV_Out. batch * kernels * Height * Width.
//    X_paddedHeight = X_out_h1 + 2 * X_Padding[Layer_ID];
//    X_paddedWidth = X_out_w1 + 2 * X_Padding[Layer_ID];
//    uint16_t X_out_h2 = ((X_paddedHeight - W_Height[Layer_ID]) / X_Stride[Layer_ID] + 1 );
//    uint16_t X_out_w2 = ((X_paddedWidth - W_Width[Layer_ID]) / X_Stride[Layer_ID] + 1 );
//    uint16_t W_BitMat_Batch2 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
//    uint16_t X_BitMat_Batch2 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
//
//    float**** CNV_Out2 = (float****)calloc(X_BitMat_Batch2,sizeof(float***));
//    for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//        CNV_Out2[b] = (float***)calloc(W_BitMat_Batch2,sizeof(float**));
//        for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
//            CNV_Out2[b][c] = (float**)calloc(X_out_h2,sizeof(float*));
//            for(uint16_t h = 0; h < X_out_h2; h++){
//                CNV_Out2[b][c][h] = (float*)calloc(X_out_w2,sizeof(float));
//            }
//        }
//    }
//    //Create and load weight matrix for this layer.
//    //Memory for Weight Matrix
//    W_Mat = (float****)calloc(W_Batch[Layer_ID],sizeof(float***));
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
//    getWeightsForLayer(imageBuffer_wdata,Num_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat);
//
//    //Create memory for bias.
//    Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//
//    //Extract Bias from buffer.
//    getBiasForLayer(imageBuffer_biasdata,Num_Layers,Layer_ID,W_Batch,Bias);
//
//    currObj=convolve(CNV_Out1,X_Batch,W_Batch[Layer_ID - 1],X_out_h1,X_out_w1,X_Padding[Layer_ID],X_Stride[Layer_ID],W_Mat,W_Batch[Layer_ID],W_Channels[Layer_ID],W_Height[Layer_ID],W_Width[Layer_ID],
//            W_Padding, W_Stride, WordLines[Layer_ID], XDL, XBL, WDL, WBL, IMC, Bias, CNV_Out2,
//			currObj);
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
//    #pragma endregion
//
//    //Batch Norm
//    #pragma region
//    //Create memory for batch norm parameters.
//    BN_Data = (float**)calloc(4,sizeof(float*)); //4 = mu,var,weight,bias
//    for(uint8_t i = 0; i < 4; i++){
//        BN_Data[i] = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//    }
//
//    //Extract batch norm parameters for LayerID from buffer.
//    getBNForLayer(imageBuffer_bndata,Num_Layers,Layer_ID,W_Batch,BN_Data);
//
//    //Perform Batch Normalisation. Replaces same CNV_Out.
//    BatchNorm(CNV_Out2,X_Batch,W_Batch[Layer_ID],X_out_h2,X_out_w2,BN_Data[0],BN_Data[1],BN_Data[2],BN_Data[3],CNV_Out2);
//
//    //Deallocate batch norm parameters
//    for(uint8_t i = 0; i < 4; i++){
//        free(BN_Data[i]);
//    }
//    free(BN_Data);
//
//    #pragma endregion
//
//    //ReLU
//    #pragma region
//
//    //perform ReLU here. Replaces CNV_Out
//    ReLU(CNV_Out2,X_Batch,W_Batch[Layer_ID],X_out_h2,X_out_w2,CNV_Out2);
//
//    #pragma endregion
//
//    //End of the Layer
//    #pragma endregion
//
//    printf("\nSecond Layer Done\n");
//    //Third Layer Convolution + BatchNorm + Add(CNV1,CNV2) + Relu here.
//    Layer_ID = 2;
//    #pragma region
//
//    //Convolution.
//    #pragma region
//    //Create memory for CNV_Out. batch * kernels * Height * Width.
//    X_paddedHeight = X_out_h2 + 2 * X_Padding[Layer_ID];
//    X_paddedWidth = X_out_w2 + 2 * X_Padding[Layer_ID];
//    uint16_t X_out_h3 = ((X_paddedHeight - W_Height[Layer_ID]) / X_Stride[Layer_ID] + 1 );
//    uint16_t X_out_w3 = ((X_paddedWidth - W_Width[Layer_ID]) / X_Stride[Layer_ID] + 1 );
//    uint16_t W_BitMat_Batch3 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
//    uint16_t X_BitMat_Batch3 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
//
//    float**** CNV_Out3 = (float****)calloc(X_BitMat_Batch3,sizeof(float***));
//    for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
//        CNV_Out3[b] = (float***)calloc(W_BitMat_Batch3,sizeof(float**));
//        for(uint16_t c = 0; c < W_BitMat_Batch3; c++){
//            CNV_Out3[b][c] = (float**)calloc(X_out_h3,sizeof(float*));
//            for(uint16_t h = 0; h < X_out_h3; h++){
//                CNV_Out3[b][c][h] = (float*)calloc(X_out_w3,sizeof(float));
//            }
//        }
//    }
//    //Create and load weight matrix for this layer.
//    //Memory for Weight Matrix
//    W_Mat = (float****)calloc(W_Batch[Layer_ID],sizeof(float***));
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
//    getWeightsForLayer(imageBuffer_wdata,Num_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat);
//
//    //Create memory for bias.
//    Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//
//    //Extract Bias from buffer.
//    getBiasForLayer(imageBuffer_biasdata,Num_Layers,Layer_ID,W_Batch,Bias);
//
//    currObj=convolve(CNV_Out2,X_Batch,W_Batch[Layer_ID - 1],X_out_h2,X_out_w2,X_Padding[Layer_ID],X_Stride[Layer_ID],W_Mat,W_Batch[Layer_ID],W_Channels[Layer_ID],W_Height[Layer_ID],W_Width[Layer_ID],
//            W_Padding, W_Stride, WordLines[Layer_ID], XDL, XBL, WDL, WBL, IMC, Bias, CNV_Out3,
//			currObj);
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
//    #pragma endregion
//
//    //Batch Norm
//    #pragma region
//    //Create memory for batch norm parameters.
//    BN_Data = (float**)calloc(4,sizeof(float*)); //4 = mu,var,weight,bias
//    for(uint8_t i = 0; i < 4; i++){
//        BN_Data[i] = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//    }
//
//    //Extract batch norm parameters for LayerID from buffer.
//    getBNForLayer(imageBuffer_bndata,Num_Layers,Layer_ID,W_Batch,BN_Data);
//
//    //Perform Batch Normalisation. Replaces same CNV_Out.
//    BatchNorm(CNV_Out3,X_Batch,W_Batch[Layer_ID],X_out_h3,X_out_w3,BN_Data[0],BN_Data[1],BN_Data[2],BN_Data[3],CNV_Out3);
//
//    //Deallocate batch norm parameters
//    for(uint8_t i = 0; i < 4; i++){
//        free(BN_Data[i]);
//    }
//    free(BN_Data);
//
//    #pragma endregion
//
//    //Add
//    #pragma region
//
//    //Add(CNV1,CNV3)
//    add4DArrayElementwise(CNV_Out3,CNV_Out1,X_Batch,W_Batch[Layer_ID],X_out_h3,X_out_w3);
//
//    #pragma endregion
//
//    //ReLU
//    #pragma region
//
//    //perform ReLU here. Replaces CNV_Out
//    ReLU(CNV_Out3,X_Batch,W_Batch[Layer_ID],X_out_h3,X_out_w3,CNV_Out3);
//
//    #pragma endregion
//
//    //End of the Layer
//    #pragma endregion
//
//    XTime tEnd;
//    XTime_GetTime(&tEnd);
//    double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);
//    printf("\nThird Layer Done\n");
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
//    //Deallocate CNV_Out2
//    for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//        for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
//            for(uint16_t h = 0; h < X_out_h2; h++){
//                free(CNV_Out2[b][c][h]);
//            }
//            free(CNV_Out2[b][c]);
//        }
//        free(CNV_Out2[b]);
//    }
//    free(CNV_Out2);
//
//    //Deallocate CNV_Out3
//    for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
//        for(uint16_t c = 0; c < W_BitMat_Batch3; c++){
//            for(uint16_t h = 0; h < X_out_h3; h++){
//                free(CNV_Out3[b][c][h]);
//            }
//            free(CNV_Out3[b][c]);
//        }
//        free(CNV_Out3[b]);
//    }
//    free(CNV_Out3);
//
//    //Deallocate file buffers.
//    free(imageBuffer_xdim);
//    free(imageBuffer_wdim);
//    //free(imageBuffer_xdata);
//    free(imageBuffer_wdata);
//    free(imageBuffer_bndata);
//    free(imageBuffer_biasdata);
//    free(convParamsBuf);
//    free(X_Padding);
//    free(X_Stride);
//    free(WordLines);
//    free(W_Batch);
//    free(W_Channels);
//    free(W_Height);
//    free(W_Width);
//
//    #pragma endregion
//
//    printf("\nCode Exited Succesfully");
//
//
//return 0;
//}
//
