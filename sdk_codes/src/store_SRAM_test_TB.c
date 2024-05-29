///*
// * LinLatest.c
// *
// *  Created on: Aug 26, 2023
// *      Author: ashlo
// */
///*
// * linear.c
// *
// *  Created on: Aug 24, 2023
// *      Author: ashlo
// */
//
//
//#include <custom_h.h>
//#include "utilsinclude.h"
//int main()
//{
//	curr_obj currObj;
//	currObj=initialisation_code(currObj);
//
//	scan_out scanOut;
//	scan_out_internal scanoutInternal;
//
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//	currObj=defaultState(currObj);
//
////long t1 = getMicrotime();
//    //Initialise all the constants here.
//    #pragma region
//	XTime tStart;
//	XTime_GetTime(&tStart);
//
//    int Status;
//    Status = SD_Init(&fatfs);
//
//    //long t1 = getMicrotime();
//	//Initialise all the constants here.
//	#pragma region
//	//Image Matrix = X matrix
//	uint16_t X_Batch = 0; //Batch size for Images
//	uint16_t X_Channels = 0; //No of channels in each image
//	uint16_t X_Height = 0; //Height of the each image
//	uint16_t X_Width = 0; //Width of the input image
//
//
//	//Read the .bin and update the input image dimensions.
//	char* imageBuffer_xdim = (char*)calloc(16,sizeof(char));
//	char fileName_xm[] = "XDim.bin";
//	Status = ReadFile( fileName_xm,(u32)imageBuffer_xdim);
//	X_Batch=(uint16_t)bf16tofloat(imageBuffer_xdim[0],imageBuffer_xdim[1],imageBuffer_xdim[2],imageBuffer_xdim[3]);
//	X_Channels=(uint16_t)bf16tofloat(imageBuffer_xdim[4],imageBuffer_xdim[5],imageBuffer_xdim[6],imageBuffer_xdim[7]);
//	printf("\nImage Dimensions : %d %d %d %d", X_Batch, X_Channels, X_Height, X_Width);
//
//	//Read the .bin to get NL -> Number of Layers.
//	char* NL_Buffer = (char*)calloc(4,sizeof(char));
//	char fileName_nl[] = "WNL.bin";
//	Status = ReadFile( fileName_nl,(u32)NL_Buffer);
//	uint16_t Total_Layers = (uint16_t)bf16tofloat(NL_Buffer[0],NL_Buffer[1],NL_Buffer[2],NL_Buffer[3]);
//	printf("\nNumber of Total Layers: %d",Total_Layers);
//
//	//Kernel Matrix = W matrix. This will be per layer.
//	uint16_t* W_Batch = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Batch size for kernels
//	uint16_t* W_Channels = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //No of channels in each kernel
//	uint16_t* W_Height = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Height of the each kernel
//	uint16_t* W_Width = (uint16_t*)calloc(Total_Layers,sizeof(uint16_t)); //Width of the input kernel
//	uint16_t W_Padding = 0; //Padding  (Required because same unfolding function as image is used) (Do not change)
//	uint16_t W_Stride = 1;  //Stride (Required because same unfolding function as image is used) (Do not change)
//
//	char *imageBuffer_wdim = (char *)calloc(16*Total_Layers, sizeof(char));
//	char fileName_wm[]="WDim.bin";
//	Status = ReadFile( fileName_wm,(u32)imageBuffer_wdim);
//
//	for(uint16_t nl = 0; nl<Total_Layers; nl++){
//		W_Batch[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 0],imageBuffer_wdim[16*nl + 1],imageBuffer_wdim[16*nl + 2],imageBuffer_wdim[16*nl + 3]);
//		W_Channels[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 4],imageBuffer_wdim[16*nl + 5],imageBuffer_wdim[16*nl + 6],imageBuffer_wdim[16*nl + 7]);
//		W_Height[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 8],imageBuffer_wdim[16*nl + 9],imageBuffer_wdim[16*nl + 10],imageBuffer_wdim[16*nl + 11]);
//		W_Width[nl] = (uint16_t)bf16tofloat(imageBuffer_wdim[16*nl + 12],imageBuffer_wdim[16*nl + 13],imageBuffer_wdim[16*nl + 14],imageBuffer_wdim[16*nl + 15]);
//		printf("\nWeight Dimensions for layer %d : %d %d %d %d", nl, W_Batch[nl], W_Channels[nl], W_Height[nl], W_Width[nl]);
//	}
//	#pragma endregion
//
//
//	uint16_t XDL = XDL_HW; // XDL is the number of image DLs 8
//	uint16_t XBL = XBL_HW; // XBL is the number of image pxl mantissa bits being fed
//	uint16_t WDL = WDL_HW; // WDL is the number of weight DLs 72
//	uint16_t WBL = WBL_HW; // WBL is the number of weight mantissa bits being fed
//
//
//	//Read lin layer input dims.
//	uint16_t Lin_Layers = Total_Layers;
//	uint16_t* WordLinesL = (uint16_t*)calloc(Lin_Layers,sizeof(uint16_t));
//	char *linParamsBuf = (char *)calloc(4*Lin_Layers, sizeof(char));
//	char fileName_l[] = "LinP.bin";
//	Status = ReadFile( fileName_l,(u32)linParamsBuf);
//	for(uint16_t nl = 0; nl<Lin_Layers; nl++){
//		WordLinesL[nl] = (uint16_t)bf16tofloat(linParamsBuf[20*nl + 0],linParamsBuf[20*nl + 1],linParamsBuf[20*nl + 2],linParamsBuf[20*nl + 3]);
//		printf("\n WordLines for lin layer %d : {%d}", nl,WordLinesL[nl]);
//	}
//
//
//	//Add X mat, W Buffer, Bias Buffer, BatchNorm Buffer loading part here.
//	#pragma region
//	//Memeory for Image Matrix
//	float ** X_Mat = (float**)calloc(X_Batch,sizeof(float*));
//	for(uint16_t b = 0; b < X_Batch; b++){
//		X_Mat[b] = (float*)calloc(X_Channels,sizeof(float));
//	}
//
//	//Read Image from binary file
//	float t_var;
//	char fileName_xd[] = "XDat.bin" ;
//	int imBuf_size=(X_Batch*X_Channels*4);
//	char *imageBuffer_xdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_xd,(u32)imageBuffer_xdata);
//
//	unsigned long int bufIndex=0;
//	for(uint16_t i=0; i<X_Batch; i++)
//	{
//		for(uint16_t j=0; j<X_Channels;j++)
//		{
//			t_var= bf16tofloat(imageBuffer_xdata[bufIndex],imageBuffer_xdata[bufIndex+1],imageBuffer_xdata[bufIndex+2],imageBuffer_xdata[bufIndex+3]);
//			X_Mat[i][j]=t_var;
//			bufIndex+=4;
//		}
//	}
//
//	//Read Weights from binary file.
//	char fileName_wd[]="WDat.bin";
//	imBuf_size= 0;
//	for(uint16_t nl = 0; nl < Total_Layers; nl++){
//		if(W_Height[nl] == 0){
//			W_Height[nl] = 1;
//			W_Width[nl] = 1;
//		}
//		imBuf_size += W_Batch[nl] * W_Channels[nl] * W_Height[nl] * W_Width[nl] * 4; //4 bytes = float
//	}
//	char *imageBuffer_wdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_wd,(u32)imageBuffer_wdata);
//
//	////Read Bias data from binary file.
//	char fileName_b[]="BDat.bin";
//	imBuf_size= 0;
//	for(uint16_t nl = 0; nl < Total_Layers; nl++){
//		imBuf_size += W_Batch[nl] * 4; //4 bytes = float
//	}
//	char *imageBuffer_biasdata = (char *)calloc(imBuf_size, sizeof(char));
//	Status = ReadFile( fileName_b,(u32)imageBuffer_biasdata);
//
//	 //Read References
//	 char fileName_rdim[]="RDim.bin";
//	 char *imageBuffer_rdim = (char *)calloc(4, sizeof(char));
//	 Status = ReadFile( fileName_rdim,(u32)imageBuffer_rdim);
//	 uint16_t RBytes = bf16tofloat(imageBuffer_rdim[0],imageBuffer_rdim[1],imageBuffer_rdim[2],imageBuffer_rdim[3]);
//
//	 char fileName_rdata[]="RDat.bin";
//	 char *imageBuffer_rdata = (char *)calloc(RBytes, sizeof(char));
//	 Status = ReadFile( fileName_rdata,(u32)imageBuffer_rdata);
////	 Status=SD_Eject(&fatfs);
//	#pragma endregion
//
//
//	bool IMC = true;
//	X_Batch = 8;
//
//	//Layer Linear
//	uint16_t Layer_ID = 0;
//	//Linear
//	#pragma region
//	//Create memory for LIN_Out.
//	uint16_t W_BitMat_Batch2 = ceil((float)W_Batch[Layer_ID]/(float)WDL) * WDL; //Extra padded zeros
//	uint16_t X_BitMat_Batch2 = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
//
//	float** LIN_Out1 = (float**)calloc(X_BitMat_Batch2,sizeof(float*));
//	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//		LIN_Out1[b] = (float*)calloc(W_BitMat_Batch2,sizeof(float));
//	}
//
//	float** LIN_Out_Accurate = (float**)calloc(X_BitMat_Batch2,sizeof(float*));
//		for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//			LIN_Out_Accurate[b] = (float*)calloc(W_BitMat_Batch2,sizeof(float));
//		}
//
//	//Create and load weight matrix for this layer.
//	//Memory for Weight Matrix
//	float ** W_Mat_Lin = (float**)calloc(W_Batch[Layer_ID],sizeof(float***));
//	for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
//		W_Mat_Lin[b] = (float*)calloc(W_Channels[Layer_ID],sizeof(float));
//	}
//	//Extract W_Mat of Layer_ID from buffer.
//	getWeightsForLinear(imageBuffer_wdata,Total_Layers,Layer_ID,W_Batch,W_Channels,W_Height,W_Width,W_Mat_Lin);
//
//	//Create memory for bias.
//	float* Bias = (float*)calloc(W_Batch[Layer_ID],sizeof(float));
//
//	//Extract Bias from buffer.
//	getBiasForLayer(imageBuffer_biasdata,Total_Layers,Layer_ID,W_Batch,Bias);
//
//	//Extract Reference from buffer
//	//Create memory for ref array = chunks * 2 * na(8) * nwl
//	uint16_t chunks = ceil((float)X_Channels/(float)WordLinesL[0]); //Input Features/Wordlines
//	float**** Ref = (float****)calloc(chunks,sizeof(float***));
//	for(uint16_t c = 0;c < chunks; c++){
//		Ref[c] = (float***)calloc(2,sizeof(float**));
//		for(uint16_t i = 0;i < 2; i++){
//			Ref[c][i] = (float**)calloc(8,sizeof(float*));
//			for(uint16_t n = 0; n < 8; n++)
//				Ref[c][i][n] = (float*)calloc(WordLinesL[0],sizeof(float)); //need to make it layer specific
//		}
//	}
//	//Create memory for RefB array. chunks * 2 * na(4) * nwl
//	float*** RefB = (float***)calloc(chunks,sizeof(float**));
//	for(uint16_t c = 0;c < chunks; c++){
//		RefB[c] = (float**)calloc(8,sizeof(float*));
//		for(uint16_t n = 0; n < 4; n++){
//			RefB[c][n] = (float*)calloc(WordLinesL[0],sizeof(float)); //need to make it layer specific
//		}
//	}
//	//offset bits till this layer.
//	uint32_t offset_bytes = 0; //Chunks[:] * 2 * Na (8 + 4) * WordLines[:]
//
//	//get ref matrix.
//	float* RefMax = (float*)calloc(chunks,sizeof(float));;
//	getRefForLayer(imageBuffer_rdata,offset_bytes,chunks,8 ,4 ,       784,Ref,RefB,RefMax);
//  //getRefForLayer(Buffer           ,offset_bytes,chunks,na,nb,wordlines ,Ref,RefB,RefMax)
//	// getRefForLayer(imageBuffer_rdata,offset_bytes,chunks,8,4,WordLinesL[0],Ref,RefB,RefMax);
//
//	//Call linear here.
//	currObj = store_SRAM_lin_test(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
//			XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out1,currObj);
//
////	currObj = linear_NotOnChip(X_Mat,X_Batch,X_Channels,W_Mat_Lin,W_Batch[Layer_ID],Bias,WordLinesL[0],
////				XDL,XBL,WDL,WBL,IMC,Ref,RefB,RefMax,LIN_Out_Accurate,currObj);
//
//	float* Flattened_Lin = (float*)calloc(X_BitMat_Batch2*W_BitMat_Batch2,sizeof(float));
//		flatten2D(LIN_Out1,X_BitMat_Batch2,W_BitMat_Batch2,Flattened_Lin);
//
//	char FileName_outIMC[]="OUTI.bin";
//	u32 size=X_BitMat_Batch2*W_BitMat_Batch2*sizeof(float);
//	int rc=WriteFile_custom(FileName_outIMC, size, (void *) Flattened_Lin);
//
//	char* Flattened_Lin_RB = (char*)calloc(size,sizeof(char));
//	Status = ReadFile( FileName_outIMC,(void *)Flattened_Lin_RB);
//
//	Status=SD_Eject(&fatfs);
//
////	free(Flattened_Lin_Accurate);
////	free(Flattened_Lin_Accurate_RB);
//	free(Flattened_Lin);
//	free(Flattened_Lin_RB);
//	//Deallocate Bias Mat
//	free(Bias);
//
//	//Deallocate Weight Matrix
//	for(uint16_t b = 0; b < W_Batch[Layer_ID]; b++){
//		free(W_Mat_Lin[b]);
//	}
//	free(W_Mat_Lin);
//	//Deallocate Ref Matrix.
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
//	//End of the layer.
//	#pragma endregion
//
//	printf("\nLayer Done\n");
//
//	//Deallocate Buffers
//	#pragma region
//	//Deallocate Image Matrix
//	for(uint16_t b = 0; b < X_Batch; b++){
//		free(X_Mat[b]);
//	}
//	free(X_Mat);
//
//	//Deallocate Lin_Out1
//	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//		free(LIN_Out1[b]);
//	}
//	free(LIN_Out1);
//
//	for(uint16_t b = 0; b < X_BitMat_Batch2; b++){
//			free(LIN_Out_Accurate[b]);
//		}
//		free(LIN_Out_Accurate);
//
//	//Deallocate file buffers.
//	free(imageBuffer_xdim);
//	free(imageBuffer_wdim);
//	free(imageBuffer_xdata);
//	free(imageBuffer_wdata);
//	free(imageBuffer_rdata);
//	free(imageBuffer_rdim);
//	free(imageBuffer_wdata);
//
//	free(imageBuffer_biasdata);
//
//	free(linParamsBuf);
//
//	free(WordLinesL);
//	free(W_Batch);
//	free(W_Channels);
//	free(W_Height);
//	free(W_Width);
//
//
//	#pragma endregion
//
//	printf("\nCode Exited Succesfully");
//
//return 0;
//}
//
//
//
