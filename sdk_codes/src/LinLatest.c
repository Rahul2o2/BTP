/*
 * LinLatest.c
 *
 *  Created on: Aug 26, 2023
 *      Author: ashlo
 */
/*
 * linear.c
 *
 *  Created on: Aug 24, 2023
 *      Author: ashlo
 */


#include <custom_h.h>
#include "utilsinclude.h"
int main()
{
//	u16 xyz=0.1;

	static XSysMon SysMonInst; //a sysmon instance
	//u8 SeqMode;
	int xStatus;
	XSysMon_Config *SysMonConfigPtr;
	XSysMon *SysMonInstPtr = &SysMonInst;
	SysMonConfigPtr = XSysMon_LookupConfig(SYSMON_DEVICE_ID);
	xStatus = XSysMon_CfgInitialize(SysMonInstPtr, SysMonConfigPtr,SysMonConfigPtr->BaseAddress);
	XSysMon_GetStatus(SysMonInstPtr); // Clear the old status
// Extra Added for lab pdf adc code ends here

	u32 status;
	u32 receivedBytes=0;
	u32 totalFunctionCallBytes=0;
	u32 totalTransmittedBytes=0;
	u32 transmittedBytes=0;
	XUartPs_Config *myUartConfig;//in uartps
	XUartPs myUart;

	myUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);//in parameters
	status = XUartPs_CfgInitialize(&myUart,myUartConfig,myUartConfig->BaseAddress);
	if(status != XST_SUCCESS)
	print("UART initialization failed\n\r");
	status= XUartPs_SetBaudRate(&myUart,BAUDRATE);
	if(status != XST_SUCCESS)
	print("Baud rate initialization failed\n\r");
	status = XUartPs_SelfTest(&myUart);
	if (status != XST_SUCCESS)
	return XST_FAILURE;

	curr_obj currObj;
	currObj=initialisation_code(currObj);

	scan_out scanOut;
	scan_out_internal scanoutInternal;
	int signals_len;
	int k=0,i=0,marker_pos=0;
	//Added by Ashwin Balagopal(Starts)
	u32 receivedBytes2=0;
	int fileSize_return;
	u8 syncdata=0;
	u8 scanChain;
	u8 abcd[200000];
	//u8 counter_data ;

	//** Srinidhi added part starts
	u32 ttlSentBytes;
	u32 cSentBytes;
	u32 totalSentBytes;
	u32 sentBytes;
	u32 totalReceivedBytes;

	//** Srinidhi added part sends

	int scan_len;
	u8 bitLine;
	//int l1;
	u8 numPadZeroes_vecScanIn;
	int flag;
/*
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);
	{
		// UART ADDED CODE STARTS HERE
		uint16_t returnWords[]={power_supply_start_word};//0 is stride
		int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
		sendWordData( returnWords, numWords, myUart ) ;
		// UART ADDED CODE ENDS HERE

		do
		{
			receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
		}while(syncdata!=syncdata_val);


	}
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);
	currObj=defaultState(currObj);*/

//	currObj=init_XADC(currObj);//TESTED OK to have the init_internal happen at this location in the code
//	currObj=init_internal(currObj);//TESTED OK to have the init_internal happen at this location in the code
//	currObj=init_internal_2X(currObj);//TESTED OK to have the init_internal happen at this location in the code
//long t1 = getMicrotime();
    //Initialise all the constants here.
    #pragma region
	XTime tStart;
	XTime_GetTime(&tStart);

    FRESULT Status;
    Status = SD_Init(&fatfs);

/*
	// TEST CODE STARTS
	{
			uint16_t IMC_Out_temp[2][3][2]={ { { 0 , 1 }, { 2, 3 }, { 4, 5 } }, { { 6+0 , 6+1 }, { 6+2, 6+3 }, { 6+4, 6+5 } } };

		// SD WRITE ADDED CODE STARTS HERE
			FRESULT rc;
			FIL fil_IMCO;
			FIL fil_RDSR;
			UINT br_IMCO;
			UINT br_RDSR;
			u32 file_size;
			int size1=2*3*2*2;
			char FileName_IMCO[]="TEST.bin";
			rc = f_open(&fil_IMCO, FileName_IMCO, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_IMCO, IMC_Out_temp, size1, &br_IMCO);
			rc = f_close(&fil_IMCO);
//			Status=SD_Eject(&fatfs);
		// SD WRITE ADDED CODE ENDS HERE
	}
	// TEST CODE ENDS
*/

    //long t1 = getMicrotime();
	//Initialise all the constants here.
	#pragma region
	//Image Matrix = X matrix
	uint16_t X_Batch = 0; //Batch size for Images
	uint16_t X_Channels = 0; //No of channels in each image
	uint16_t X_Height = 0; //Height of the each image
	uint16_t X_Width = 0; //Width of the input image


	//Read the .bin and update the input image dimensions.
	char* imageBuffer_xdim = (char*)calloc(16,sizeof(char));
	char fileName_xm[] = "XDim.bin";
	Status = ReadFile( fileName_xm,(u32)imageBuffer_xdim);
	X_Batch=(uint16_t)bf16tofloat(imageBuffer_xdim[0],imageBuffer_xdim[1],imageBuffer_xdim[2],imageBuffer_xdim[3]);
	X_Channels=(uint16_t)bf16tofloat(imageBuffer_xdim[4],imageBuffer_xdim[5],imageBuffer_xdim[6],imageBuffer_xdim[7]);
	printf("\nImage Dimensions : %d %d %d %d", X_Batch, X_Channels, X_Height, X_Width);
	X_Batch = 1;
	//Read the .bin to get NL -> Number of Layers.
	char* NL_Buffer = (char*)calloc(4,sizeof(char));
	char fileName_nl[] = "WNL.bin";
	Status = ReadFile( fileName_nl,(u32)NL_Buffer);
	uint16_t Total_Layers = (uint16_t)bf16tofloat(NL_Buffer[0],NL_Buffer[1],NL_Buffer[2],NL_Buffer[3]);
	printf("\nNumber of Total Layers: %d",Total_Layers);

	//Kernel Matrix = W matrix. This will be per layer.
	uint16_t* W_Batch = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Batch size for kernels
	uint16_t* W_Channels = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //No of channels in each kernel
	uint16_t* W_Height = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Height of the each kernel
	uint16_t* W_Width = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Width of the input kernel
	uint16_t W_Padding = 0; //Padding  (Required because same unfolding function as image is used) (Do not change)
	uint16_t W_Stride = 1;  //Stride (Required because same unfolding function as image is used) (Do not change)

	char *imageBuffer_wdim = (char *)calloc(16*Total_Layers, sizeof(char));
	char fileName_wm[]="WDim.bin";
	Status = ReadFile( fileName_wm,(u32)imageBuffer_wdim);

	for(uint16_t nl = 0; nl<Total_Layers; nl++){
		W_Batch[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 0],imageBuffer_wdim[16*nl + 1],imageBuffer_wdim[16*nl + 2],imageBuffer_wdim[16*nl + 3]);
		W_Channels[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 4],imageBuffer_wdim[16*nl + 5],imageBuffer_wdim[16*nl + 6],imageBuffer_wdim[16*nl + 7]);
		W_Height[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 8],imageBuffer_wdim[16*nl + 9],imageBuffer_wdim[16*nl + 10],imageBuffer_wdim[16*nl + 11]);
		W_Width[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 12],imageBuffer_wdim[16*nl + 13],imageBuffer_wdim[16*nl + 14],imageBuffer_wdim[16*nl + 15]);
		printf("\nWeight Dimensions for layer %d : %d %d %d %d", nl, W_Batch[nl], W_Channels[nl], W_Height[nl], W_Width[nl]);
	}
	#pragma endregion


	uint16_t XDL = XDL_HW; // XDL is the number of image DLs 8
	uint16_t XBL = XBL_HW; // XBL is the number of image pxl mantissa bits being fed
	uint16_t WDL = WDL_HW; // WDL is the number of weight DLs 72
	uint16_t WBL = WBL_HW; // WBL is the number of weight mantissa bits being fed
	XDL = 1;

	//Read lin layer input dims.
	uint16_t Lin_Layers = Total_Layers;
	uint16_t* WordLinesL = (uint16_t*)calloc(Lin_Layers,sizeof(uint16_t));
	char *linParamsBuf = (char *)calloc(4*Lin_Layers, sizeof(char));
	char fileName_l[] = "LinP.bin";
	Status = ReadFile( fileName_l,(u32)linParamsBuf);
	for(uint16_t nl = 0; nl<Lin_Layers; nl++){
		WordLinesL[nl] = (uint16_t)bf16tofloat(linParamsBuf[4*nl + 0],linParamsBuf[4*nl + 1],linParamsBuf[4*nl + 2],linParamsBuf[4*nl + 3]);
		printf("\n WordLines for lin layer %d : {%d}", nl,WordLinesL[nl]);
	}


	//Add X mat, W Buffer, Bias Buffer, BatchNorm Buffer loading part here.
	#pragma region
	//Memeory for Image Matrix
	float ** X_Mat = (float**)calloc(X_Batch,sizeof(float*));
	for(uint16_t b = 0; b < X_Batch; b++){
		X_Mat[b] = (float*)calloc(X_Channels,sizeof(float));
	}

	unsigned long imBuf_size;

	//Read Image from binary file
	float t_var;
	char fileName_xd[] = "XDat.bin" ;
	imBuf_size=(X_Batch*X_Channels*4);
//	imBuf_size=1;
	char *imageBuffer_xdata = (char *)calloc(imBuf_size, sizeof(char));
	Status = ReadFile( fileName_xd,(u32)imageBuffer_xdata);

	unsigned long bufIndex=0;
	for(uint16_t i=0; i<X_Batch; i++)
	{
		for(uint16_t j=0; j<X_Channels;j++)
		{
			t_var= bf16tofloat(imageBuffer_xdata[bufIndex],imageBuffer_xdata[bufIndex+1],imageBuffer_xdata[bufIndex+2],imageBuffer_xdata[bufIndex+3]);
			X_Mat[i][j]=t_var;
			bufIndex+=4;
		}
	}
	free(imageBuffer_xdata);
	//Read Weights from binary file.
	char fileName_wd[]="WDat.bin";
	imBuf_size= 0;
	for(uint16_t nl = 0; nl < Total_Layers; nl++){
		if(W_Height[nl] == 0){
			W_Height[nl] = 1;
			W_Width[nl] = 1;
		}
		imBuf_size += W_Batch[nl] * W_Channels[nl] * W_Height[nl] * W_Width[nl] * 4; //4 bytes = float
	}
	char *imageBuffer_wdata = (char *)calloc(100000, sizeof(char));
	Status = ReadFile( fileName_wd,(u32)imageBuffer_wdata);

	////Read Bias data from binary file.
	char fileName_b[]="BDat.bin";
	imBuf_size= 0;
	for(uint16_t nl = 0; nl < Total_Layers; nl++){
		imBuf_size += W_Batch[nl] * 4; //4 bytes = float
	}
	char *imageBuffer_biasdata = (char *)calloc(imBuf_size, sizeof(char));
	Status = ReadFile( fileName_b,(u32)imageBuffer_biasdata);

	 //Read References
	 char fileName_rdim[]="RDim.bin";
	 char *imageBuffer_rdim = (char *)calloc(4, sizeof(char));
	 Status = ReadFile( fileName_rdim,(u32)imageBuffer_rdim);
	 unsigned long RBytes = (unsigned long) bf16tofloat(imageBuffer_rdim[0],imageBuffer_rdim[1],imageBuffer_rdim[2],imageBuffer_rdim[3]);

	 char fileName_rdata[]="RDat.bin";
	 char *imageBuffer_rdata = (char *)calloc(RBytes, sizeof(char));
	 Status = ReadFile( fileName_rdata,(u32)imageBuffer_rdata);
//	 Status=SD_Eject(&fatfs);
	#pragma endregion


	bool IMC = true;
//	X_Batch = 5000	 ;//IMG SEARCH

	//Layer Linear
	uint16_t Layer_ID = 0;
	//Linear
	#pragma region
	//Create memory for LIN_Out.
	uint16_t W_BitMat_Batch2 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
	uint16_t X_BitMat_Batch2 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros

/*
	float** LIN_Out1 = (float**)calloc(X_BitMat_Batch2,sizeof(float*));
	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
		LIN_Out1[b] = (float*)calloc(W_BitMat_Batch2,sizeof(float));
	}
*/

	float** LIN_Out_Accurate = (float**)calloc(X_BitMat_Batch2,sizeof(float*));
		for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
			LIN_Out_Accurate[b] = (float*)calloc(W_BitMat_Batch2,sizeof(float));
		}

	//Create and load weight matrix for this layer.
	//Memory for Weight Matrix
	float ** W_Mat_Lin = (float**)calloc(W_Batch[Layer_ID],sizeof(float***));
	for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
		W_Mat_Lin[b] = (float*)calloc(W_Channels[Layer_ID],sizeof(float));
	}
	//Extract W_Mat of Layer_ID from buffer.
	getWeightsForLinear(imageBuffer_wdata,Total_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat_Lin);
	free(imageBuffer_wdata);
	//Create memory for bias.
	float* Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));

	//Extract Bias from buffer.
	getBiasForLayer(imageBuffer_biasdata,Total_Layers,Layer_ID,W_Batch,Bias);
	free(imageBuffer_biasdata);
	//Extract Reference from buffer
	//Create memory for ref array = chunks * 2 * na(8) * nwl
	uint16_t chunks = ceil((float)X_Channels/(float)WordLinesL[0]); //Input Features/Wordlines
	float**** Ref = (float****)calloc(chunks,sizeof(float***));
	for(uint16_t c = 0;c < chunks; c++){
		Ref[c] = (float***)calloc(2,sizeof(float**));
		for(uint16_t i = 0;i < 2; i++){
			Ref[c][i] = (float**)calloc(8,sizeof(float*));
			for(uint16_t n = 0; n < 8; n++)
				Ref[c][i][n] = (float*)calloc(WordLinesL[0],sizeof(float)); //need to make it layer specific
		}
	}
	//Create memory for RefB array. chunks * 2 * na(4) * nwl
	float*** RefB = (float***)calloc(chunks,sizeof(float**));
	for(uint16_t c = 0;c < chunks; c++){
		RefB[c] = (float**)calloc(8,sizeof(float*));
		for(uint16_t n = 0; n < 4; n++){
			RefB[c][n] = (float*)calloc(WordLinesL[0],sizeof(float)); //need to make it layer specific
		}
	}
	//offset bits till this layer.
	uint32_t offset_bytes = 0; //Chunks[:] * 2 * Na (8 + 4) * WordLines[:]

	//get ref matrix.
	float* RefMax = (float*)calloc(chunks,sizeof(float));;
  //getRefForLayer(           Buffer,offset_bytes,chunks,na,nb,wordlines ,Ref,RefB,RefMax)
	getRefForLayer(imageBuffer_rdata,offset_bytes,chunks, 8, 4,784,Ref,RefB,RefMax);
	free(imageBuffer_rdata);
	//Call linear here.
////	 u8 mode=2;//OCC
//	 u8 mode=1;//ICQ

	mode_type mode;
	//	mode.mode=mode_OCC;
	mode.mode=mode_ICQ;
	mode.LSB=1.59;
	mode.LC=-26;
	// mode.k=0.5222E-6;
	mode.slopecorrect=0.7945;
	mode.slopecorrect_chip=0.86;
	mode.slopecorrect_XADC=0.55;

//	 u8 slopeSetting=1;
	 u8 slopeSetting=2;
//	 u8 slopeSetting=4;


//	currObj = linear(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
//			XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out1,mode,slopeSetting,currObj);
//	 currObj = linear_XADC(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
//	 			XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out1,mode,slopeSetting,currObj);

	currObj = linear_NotOnChip(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
				XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out_Accurate,currObj);

/*	//Create memory for flattened array.
	float* Flattened_Lin_Accurate = (float*)calloc(X_BitMat_Batch2*W_BitMat_Batch2,sizeof(float));
	flatten2D(LIN_Out_Accurate,X_BitMat_Batch2,W_BitMat_Batch2,Flattened_Lin_Accurate);
	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
			for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
				Flattened_Lin_Accurate[c+b*W_BitMat_Batch2] = LIN_Out_Accurate[b][c];
			}
		}*/
/*
	float* Flattened_Lin = (float*)calloc(X_BitMat_Batch2*W_BitMat_Batch2,sizeof(float));
//		flatten2D(LIN_Out1,X_BitMat_Batch2,W_BitMat_Batch2,Flattened_Lin);
	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
		for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
			Flattened_Lin[c+b*W_BitMat_Batch2] = LIN_Out1[b][c];
		}
	}
*/

/*

	char FileName_outA[]="OUTA.bin";
	char FileName_outI[]="OUTI.bin";
	u32 size=X_BitMat_Batch2*W_BitMat_Batch2*sizeof(float);
	u32 SourceAddress;
	{
//		FRESULT rc=WriteFile_custom(FileName_outA, size, Flattened_Lin_Accurate);
	}
	{
	//	FRESULT rc=WriteFile_custom(FileName_outI, size, Flattened_Lin);
//		FRESULT rc=WriteFile_fseekappend(FileName_outI, size, Flattened_Lin);

	}
//	char* Flattened_Lin_Accurate_RB = (char*)calloc(size,sizeof(char));
//	Status = ReadFile( FileName_outC,(u32)Flattened_Lin_Accurate_RB);

	Status=SD_Eject(&fatfs); // SEARCH FOR THIS, ENDS HERE
*/

//	free(Flattened_Lin_Accurate);
//	free(Flattened_Lin);
//	free(Flattened_Lin_Accurate_RB);
	//Deallocate Bias Mat
	free(Bias);

	//Deallocate Weight Matrix
	for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
		free(W_Mat_Lin[b]);
	}
	free(W_Mat_Lin);
	//Deallocate Ref Matrix.

	for(uint16_t c = 0;c < chunks; c++){
		for(uint16_t i = 0;i < 2; i++){
			for(uint16_t n = 0; n < 8; n++){
				free(Ref[c][i][n]);
			}
			free(Ref[c][i]);
		}
		free(Ref[c]);
	}
	free(Ref);

	for(uint16_t c = 0;c < chunks; c++){
		for(uint16_t n = 0; n < 4; n++){
			free(RefB[c][n]);
		}
		free(RefB[c]);
	}
	free(RefB);

	free(RefMax);
	//End of the layer.
	#pragma endregion

	printf("\nLayer Done\n");

	//ReLU(LIN_Out_Accurate, uint16_t batch, uint16_t channels, uint16_t height, uint16_t width, LIN_Out_Accurate);
	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
	        for(uint16_t c = 0; c < W_BitMat_Batch2; c++){
	        	LIN_Out_Accurate[b][c] = LIN_Out_Accurate[b][c] > 0? LIN_Out_Accurate[b][c]:0;
	        }
	}

	//////////////////////////////////

	//Layer Linear 1
	Layer_ID = 1;
	//Linear
	#pragma region
	//Create memory for LIN_Out.
	uint16_t W_BitMat_Batch3 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
	uint16_t X_BitMat_Batch3 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros

/*	float** LIN_Out2 = (float**)calloc(X_BitMat_Batch3,sizeof(float*));
	for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
		LIN_Out2[b] = (float*)calloc(W_BitMat_Batch3,sizeof(float));
	}*/

	float** LIN_Out_Accurate2 = (float**)calloc(X_BitMat_Batch3,sizeof(float*));
		for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
			LIN_Out_Accurate2[b] = (float*)calloc(W_BitMat_Batch3,sizeof(float));
		}

	//Create and load weight matrix for this layer.
	//Memory for Weight Matrix
	W_Mat_Lin = (float**)calloc(W_Batch[Layer_ID],sizeof(float***));
	for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
		W_Mat_Lin[b] = (float*)calloc(W_Channels[Layer_ID],sizeof(float));
	}
	//Extract W_Mat of Layer_ID from buffer.
	getWeightsForLinear(imageBuffer_wdata,Total_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat_Lin);
	free(imageBuffer_wdata);
	//Create memory for bias.
	Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));

	//Extract Bias from buffer.
	getBiasForLayer(imageBuffer_biasdata,Total_Layers,Layer_ID,W_Batch,Bias);
	free(imageBuffer_biasdata);
	//Extract Reference from buffer
	//Create memory for ref array = chunks * 2 * na(8) * nwl
	chunks = ceil((float)X_Channels/(float)WordLinesL[0]); //Input Features/Wordlines
	 Ref = (float****)calloc(chunks,sizeof(float***));
	for(uint16_t c = 0;c < chunks; c++){
		Ref[c] = (float***)calloc(2,sizeof(float**));
		for(uint16_t i = 0;i < 2; i++){
			Ref[c][i] = (float**)calloc(8,sizeof(float*));
			for(uint16_t n = 0; n < 8; n++)
				Ref[c][i][n] = (float*)calloc(WordLinesL[0],sizeof(float)); //need to make it layer specific
		}
	}
	//Create memory for RefB array. chunks * 2 * na(4) * nwl
	RefB = (float***)calloc(chunks,sizeof(float**));
	for(uint16_t c = 0;c < chunks; c++){
		RefB[c] = (float**)calloc(8,sizeof(float*));
		for(uint16_t n = 0; n < 4; n++){
			RefB[c][n] = (float*)calloc(WordLinesL[0],sizeof(float)); //need to make it layer specific
		}
	}
	//offset bits till this layer.
	 offset_bytes += ( (chunks * ( ( 2 * 8 + 4 ) * WordLinesL[Layer_ID-1] + 1 )  ) * 4); // MODIFIED BY ASHWIN TO INCLUDE THE 4 extra for REFMAX. Na=8 Nb=4

	//get ref matrix.
	RefMax = (float*)calloc(chunks,sizeof(float));;
  //getRefForLayer(           Buffer,offset_bytes,chunks,na,nb,wordlines ,Ref,RefB,RefMax)
	getRefForLayer(imageBuffer_rdata,offset_bytes,chunks, 8, 4,784,Ref,RefB,RefMax);
	free(imageBuffer_rdata);
	//Call linear here.
////	 u8 mode=2;//OCC
//	 u8 mode=1;//ICQ

	//mode_type mode;
	//	mode.mode=mode_OCC;
	mode.mode=mode_ICQ;
	mode.LSB=1.59;
	mode.LC=-26;
	// mode.k=0.5222E-6;
	mode.slopecorrect=0.7945;
	mode.slopecorrect_chip=0.86;
	mode.slopecorrect_XADC=0.55;

//	 u8 slopeSetting=1;
	 slopeSetting=2;
//	 u8 slopeSetting=4;


//	currObj = linear(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
//			XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out1,mode,slopeSetting,currObj);
//	 currObj = linear_XADC(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
//	 			XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out1,mode,slopeSetting,currObj);

	currObj = linear_NotOnChip(LIN_Out_Accurate,X_BitMat_Batch2,W_BitMat_Batch2,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
				XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out_Accurate2,currObj);

/*	//Create memory for flattened array.
	float* Flattened_Lin_Accurate2 = (float*)calloc(X_BitMat_Batch3*W_BitMat_Batch3,sizeof(float));
	flatten2D(LIN_Out_Accurate2,X_BitMat_Batch3,W_BitMat_Batch3,Flattened_Lin_Accurate2);
	for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
			for(uint16_t c = 0; c < W_BitMat_Batch3; c++){
				Flattened_Lin_Accurate2[c+b*W_BitMat_Batch3] = LIN_Out_Accurate2[b][c];
			}
		}*/
/*

	float* Flattened_Lin2 = (float*)calloc(X_BitMat_Batch3*W_BitMat_Batch3,sizeof(float));
//		flatten2D(LIN_Out1,X_BitMat_Batch2,W_BitMat_Batch2,Flattened_Lin);
	for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
		for(uint16_t c = 0; c < W_BitMat_Batch3; c++){
			Flattened_Lin2[c+b*W_BitMat_Batch2] = LIN_Out2[b][c];
		}
	}
*/



/*	char FileName_outA[]="OUTA.bin";
	char FileName_outI[]="OUTI.bin";
	u32 size=X_BitMat_Batch2*W_BitMat_Batch2*sizeof(float);
	u32 SourceAddress;
	{
//		FRESULT rc=WriteFile_custom(FileName_outA, size, Flattened_Lin_Accurate);
	}
	{
		FRESULT rc=WriteFile_custom(FileName_outI, size, Flattened_Lin);
//		FRESULT rc=WriteFile_fseekappend(FileName_outI, size, Flattened_Lin);

	}*/
//	char* Flattened_Lin_Accurate_RB = (char*)calloc(size,sizeof(char));
//	Status = ReadFile( FileName_outC,(u32)Flattened_Lin_Accurate_RB);

	Status=SD_Eject(&fatfs); // SEARCH FOR THIS, ENDS HERE

//	free(Flattened_Lin_Accurate);
//	free(Flattened_Lin);
//	free(Flattened_Lin_Accurate_RB);
	//Deallocate Bias Mat
	free(Bias);

	//Deallocate Weight Matrix
	for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
		free(W_Mat_Lin[b]);
	}
	free(W_Mat_Lin);
	//Deallocate Ref Matrix.

	for(uint16_t c = 0;c < chunks; c++){
		for(uint16_t i = 0;i < 2; i++){
			for(uint16_t n = 0; n < 8; n++){
				free(Ref[c][i][n]);
			}
			free(Ref[c][i]);
		}
		free(Ref[c]);
	}
	free(Ref);

	for(uint16_t c = 0;c < chunks; c++){
		for(uint16_t n = 0; n < 4; n++){
			free(RefB[c][n]);
		}
		free(RefB[c]);
	}
	free(RefB);

	free(RefMax);
	//End of the layer.
	#pragma endregion

	printf("\nLayer Done\n");
//////////////////////////////////////////////////////////////////




	//Deallocate Buffers
	#pragma region
	//Deallocate Image Matrix
	for(uint16_t b = 0; b < X_Batch; b++){
		free(X_Mat[b]);
	}
	free(X_Mat);

	//Deallocate Lin_Out1
/*	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
		free(LIN_Out1[b]);
	}
	free(LIN_Out1);

	//Deallocate Lin_Out2
	for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
		free(LIN_Out2[b]);
	}
	free(LIN_Out2)*/;

	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
			free(LIN_Out_Accurate[b]);
		}
	free(LIN_Out_Accurate);

	for(uint16_t b = 0; b < X_BitMat_Batch3; b++){
			free(LIN_Out_Accurate2[b]);
		}
	free(LIN_Out_Accurate2);



	//Deallocate file buffers.
	free(imageBuffer_xdim);
	free(imageBuffer_wdim);
	free(imageBuffer_xdata);
	free(imageBuffer_wdata);
	free(imageBuffer_rdata);
	free(imageBuffer_rdim);
	free(imageBuffer_wdata);

	free(imageBuffer_biasdata);

	free(linParamsBuf);

	free(WordLinesL);
	free(W_Batch);
	free(W_Channels);
	free(W_Height);
	free(W_Width);


	#pragma endregion

	printf("\nCode Exited Succesfully");

return 0;
}

