//Fully Connected Layers -> Linear Function.
#include<utilsinclude.h>
#include<custom_h.h>
curr_obj store_SRAM_lin_test(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, curr_obj currObj){

    //Unfold X_Mat and W_Mat
    #pragma region
    //Allocate memory for Unfolded_X
    uint16_t X_W_Chunks = ceil((float)X_Features/(float)WordLines); //Chunks will be same for both X_Mat and W_Mat.
    float*** Unfolded_X = (float***)calloc(X_Batch,sizeof(float**));
    for(uint16_t b = 0; b < X_Batch; b++){
        Unfolded_X[b] = (float**)calloc(X_W_Chunks,sizeof(float*));
        for(uint16_t c = 0; c < X_W_Chunks; c++){
            Unfolded_X[b][c] = (float*)calloc(WordLines,sizeof(float));
        }
    }
    //Get Unfolded_X mat.
    rearrange2DImage(X_Mat, X_Batch, X_Features, WordLines, Unfolded_X);

    //Allocate memory for Unfolded_W
    float*** Unfolded_W = (float***)calloc(W_Features,sizeof(float**));
    for(uint16_t b = 0; b < W_Features; b++){
        Unfolded_W[b] = (float**)calloc(X_W_Chunks,sizeof(float*));
        for(uint16_t c = 0; c < X_W_Chunks; c++){
            Unfolded_W[b][c] = (float*)calloc(WordLines,sizeof(float));
        }
    }
    //Get Unfolded_W mat.
    rearrange2DImage(W_Mat, W_Features, X_Features, WordLines, Unfolded_W);

    #pragma endregion

    if(HW_LIN){

        //Get Unfolded_X and Unfolded_W in form of prealigned matrices.
        //Add padding if required. padded batch * chunks * wordlines * padded bits.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Features/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_BitMat_Bits = ceil((float)9/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(X_W_Chunks,sizeof(uint8_t**));
            for(uint16_t k =0; k< X_W_Chunks; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(X_W_Chunks,sizeof(int));
        }

        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Features; b++){
            for(uint16_t k = 0; k < X_W_Chunks; k++){
                preAlignVector(Unfolded_W[b][k],WordLines,&W_Weight[b][k],W_BitMat[b][k]);
            }
        }

        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_BitMat_Bits = ceil((float)9/(float)XBL) * XBL; //Extra padded zeros

        uint8_t**** X_BitMat = (uint8_t****)calloc(X_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t***)calloc(X_W_Chunks,sizeof(uint8_t**));
            for(uint16_t k =0; k< X_W_Chunks; k++){
                X_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    X_BitMat[b][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int** X_Weight = (int**)calloc(X_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_Weight[b] = (int*)calloc(X_W_Chunks,sizeof(int));
        }

        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
                for(uint16_t k = 0; k < X_W_Chunks; k++){
                    preAlignVector(Unfolded_X[b][k],WordLines,&X_Weight[b][k],X_BitMat[b][k]);
                }
        }

        #pragma endregion

        //Linear IMC part here.
        #pragma region
        //Create array to store IMC results.
        uint16_t ****IMC_Out = (uint16_t****)calloc(XDL,sizeof(uint16_t***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (uint16_t***)calloc(WDL,sizeof(uint16_t**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (uint16_t**)calloc(XBL,sizeof(uint16_t*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (uint16_t*)calloc(4,sizeof(uint16_t));
                }
            }
        }

//        XTime tStart_outerLoop;
//		XTime tStop_outerLoop;
//		XTime tElapsed_outerLoop;
//		double tElapsed_outerLoop_msec;
//		XTime_GetTime(&tStart_outerLoop);

        // Declaring Memory for the ReadOut Arrays
        // Read_out_weight->68x1024x4
		u8 ***Read_out_weight = (u8***)calloc(68,sizeof(u8**));
		for(uint16_t wd = 0; wd < 68; wd++){
			Read_out_weight[wd] = (u8**)calloc(1024,sizeof(u8*));
			for(uint16_t wl = 0; wl < 1024; wl++){
				Read_out_weight[wd][wl] = (u8*)calloc(4,sizeof(u8));
			}
		}


		// Read_out_ref->2x8x1024x4
		u8 ****Read_out_reference = (u8****)calloc(2,sizeof(u8***));
		for(uint16_t lh = 0; lh < 2; lh++){
			Read_out_reference[lh] = (u8***)calloc(4,sizeof(u8**));
			for(uint16_t bl = 0; bl < 4; bl++){
				Read_out_reference[lh][bl] = (u8**)calloc(8,sizeof(u8*));
				for(uint16_t rd = 0; rd < 8; rd++){
					Read_out_reference[lh][bl][rd] = (u8*)calloc(1024,sizeof(u8));
				}
			}
		}


		// Read_out_referenceAdderTree->1024x4
		u8 **Read_out_referenceAdderTree = (u8**)calloc(1024,sizeof(u8*));
		for(uint16_t wl = 0; wl < 1024; wl++){
			Read_out_referenceAdderTree[wl] = (u8*)calloc(4,sizeof(u8));
		}


		// Read_out_image->8x1024
		u8 **Read_out_image = (u8**)calloc(8,sizeof(u8*));
		for(uint16_t rd = 0; rd < 8; rd++){
			Read_out_image[rd] = (u8*)calloc(1024,sizeof(u8));
		}


		int count_image_fails=0;
		int count_weight_fails=0;
		int count_ref_fails=0;
        int count_addrtee_fails=0;

        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<X_W_Chunks; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                    //Store W_BitMat[oc:oc+WDL][c][:][wb:wb+WBL] in SRAM.
                	currObj = store_weights_SRAM_lin(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch // ACTUAL CODE ****************
                        for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
                        	currObj = store_images_SRAM_lin(X_BitMat, b , XDL, c, WordLines, xb, XBL, currObj);
                        	currObj=readSRAMSliced(currObj, Read_out_image, Read_out_weight, Read_out_reference, Read_out_referenceAdderTree);
                        	// COMPARING IMAGE ARRAY
                        	// Read_out_image->8x1024
							for(int j=0;j<XDL;j++)//image DLs
							{
								for(int i=0;i<WordLines;i++)
								{
									count_image_fails+=(Read_out_image[j][i]!=X_BitMat[b+j][c][i][xb]);
								}
							}

							// Read_out_image->8x1024
                        	// Read_out_weight->68x1024x4
                        	// Read_out_ref->2x8x1024x4
                        	// Read_out_AT->1024x4
                        }
                    }
                    // COMPARING WEIGHT ARRAY
                    // Read_out_weight->68x1024x4


					for(int j=0;j<WDL;j++)//weight DLs
					{
						for(int bitLine=0;bitLine<WBL;bitLine++)
							{
							for(int i=0;i<WordLines;i++)
							{
								count_weight_fails+=(Read_out_weight[j][i][bitLine]!=W_BitMat[oc+j][c][i][wb+bitLine]);
							}
						}
					}
                    // COMPARING Ref ARRAY
					// Read_out_ref->2x4x8x1024
                    for(int _dl=0;_dl<8;_dl++)//Reference DLs
					{
                    	for(int _bl=0;_bl<WBL;_bl++)
						{
							for(int _wl=0;_wl<WordLines;_wl++)
							{
								for(int H=0;H<2;H++)
								{
									count_ref_fails+=(Read_out_reference[H][_bl][_dl][_wl]!=(u8)Ref[c][H][_dl][_wl]);
								}
							}
						}
					}
					// COMPARING Addertree ARRAY
                    // Read_out_AT->1024x4
					for(int bitLine=0;bitLine<WBL;bitLine++)
					{
						for(int i=0;i<WordLines;i++)
						{
							count_addrtee_fails+=(Read_out_referenceAdderTree[i][bitLine]!=(u8)RefB[c][bitLine][i]);
						}
					}
                }
            }
        }
//        XTime_GetTime(&tStop_outerLoop);
//		tElapsed_outerLoop=tStop_outerLoop-tStart_outerLoop;
//		tElapsed_outerLoop_msec=1000*((double)tElapsed_outerLoop)/((double)COUNTS_PER_SECOND);
//		int pauseval=0;
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< X_W_Chunks; k++){
                for(uint16_t n=0; n<WordLines; n++){
                    free(W_BitMat[b][k][n]);
                }
                free(W_BitMat[b][k]);
            }
            free(W_BitMat[b]);
        }
        free(W_BitMat);

        //Deallocate W_Weight matrix
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            free(W_Weight[b]);
        }
        free(W_Weight);


        //Deallocate X_BitMat matrix
        for(uint16_t b = 0; b < X_BitMat_Batch; b++){
            for(uint16_t k = 0; k < X_W_Chunks; k++){
                for(uint16_t n= 0; n < WordLines; n++){
                    free(X_BitMat[b][k][n]);
                }
                free(X_BitMat[b][k]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            free(X_Weight[b]);
        }
        free(X_Weight);

        //Deallocate IMC Out.
        for(uint16_t xd = 0; xd < XDL; xd++){
            for(uint16_t wd = 0; wd < WDL; wd++){
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    free(IMC_Out[xd][wd][xb]);
                }
                free(IMC_Out[xd][wd]);
            }
            free(IMC_Out[xd]);
        }
        free(IMC_Out);

        #pragma endregion

    }
    else{
        //Normal Linear Function
        normal_LIN(Unfolded_X, X_Batch, X_W_Chunks, WordLines, Unfolded_W, W_Features, LIN_Out);
    }
    //Add bias into results.
    //Only unpadded ndx is enough.
    for(uint16_t b = 0; b < X_Batch; b++){
        for(uint16_t w = 0; w < W_Features; w++){
            LIN_Out[b][w] += Bias[w];
        }
    }
    //Deallocate buffers common between IMC and normal linear function.
    #pragma region
    //Deallocate Unfolded_X mat
    for(uint16_t b = 0; b < X_Batch; b++){
        for(uint16_t c = 0; c < X_W_Chunks; c++){
            free(Unfolded_X[b][c]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    //Deallocate Unfolded_W mat
    for(uint16_t b = 0; b < W_Features; b++){
        for(uint16_t c = 0; c < X_W_Chunks; c++){
            free(Unfolded_W[b][c]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);
    #pragma endregion
    return currObj;

}
