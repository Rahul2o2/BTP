#include "utilsinclude.h"
#include <custom_h.h>

//Function to get variable in FP16 format.
FP16 getFP16(float var){

    FP16 result = {0.00,0,0,false};  //initialise defaults.
    result.sign = var<0.00? 1:0;  //1 is negative 0 is positive.
    var = var<0.00? (-1*var):var; //making var always positive
    var += pow(2,-120); // To avoide zero always add 2^-120.
    result.exponent = (int)floor(log2(var));
    result.mantissa = (float)var/(float)pow(2,result.exponent);

    if(result.exponent > 128){
        result.overflow = true;
        return result;
    }
    if(result.exponent < -127){
        result.overflow = true;
        return result;
    }
    else{
        return result;
    }
}

//Function to add padding in 3d array, batch is not considered. Also it does not add padding across depth
//This function creates dynamic array and returs it. Make sure to free it when goes out of scope.
float*** pad3DArray(float*** array, uint16_t height, uint16_t width, uint16_t depth, uint16_t padding) {
    uint16_t paddedHeight = height + 2 * padding;
    uint16_t paddedWidth = width + 2 * padding;

    // Allocate memory for the padded array
    float*** paddedArray = (float***)malloc(paddedHeight * sizeof(float**));
    for (uint16_t i = 0; i < paddedHeight; i++) {
        paddedArray[i] = (float**)malloc(paddedWidth * sizeof(float*));
        for (uint16_t j = 0; j < paddedWidth; j++) {
            paddedArray[i][j] = (float*)malloc(depth * sizeof(float));
        }
    }

    // Initialize padded array with zeros
    for (uint16_t i = 0; i < paddedHeight; i++) {
        for (uint16_t j = 0; j < paddedWidth; j++) {
            for (uint16_t k = 0; k < depth; k++) {
                paddedArray[i][j][k] = 0.0f;
            }
        }
    }

    // Copy the original array to the center of the padded array
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
            for (uint16_t k = 0; k < depth; k++) {
                paddedArray[i + padding][j + padding][k] = array[i][j][k];
            }
        }
    }

    return paddedArray;
}


//Flatten function. b*c*h*w -> b*features.
void flatten(float**** array,uint16_t batch, uint16_t channels, uint16_t height, uint16_t width, float** flattened_array){

    for(uint16_t b = 0; b < batch; b++){
        uint16_t index = 0;
        for (uint16_t c = 0; c < channels; c++) {
            for (uint16_t i = 0; i < height; i++) {
                for (uint16_t j = 0; j < width; j++) {
                    flattened_array[b][index++] = array[b][c][i][j];
                }
            }
        }
    }
}

//Flatten function. b*c*h*w -> b*features.
void flatten2D(float** array,uint16_t batch, uint16_t channels,float* flattened_array){
	uint32_t index = 0;
    for(uint16_t b = 0; b < batch; b++){
        for (uint16_t c = 0; c < channels; c++) {
        	flattened_array[index++] = array[b][c];
        }
    }
}


//Function to rearrange multi channel image per batch into 4d array of data across Batch,Stride,k= chunks required,n= chunk size.
//make sure output array is pre initialised with appropriate dimensions.
//Batch is considered.
void rearrange4DImage(float**** input, uint16_t inputBatch, uint16_t inputDepth, uint16_t inputHeight, uint16_t inputWidth,
                   uint16_t kernelHeight, uint16_t kernelWidth,
                   uint16_t stride, uint16_t padding, uint16_t chunksize, float**** output){

    uint16_t paddedHeight = inputHeight + 2 * padding;
    uint16_t paddedWidth = inputWidth + 2 * padding;

    for(uint16_t b = 0; b < inputBatch; b++ ){
        uint16_t ch_ndx = 0;
        for (uint16_t i = 0; i <= paddedHeight - kernelHeight; i += stride) {
            for (uint16_t j = 0; j <= paddedWidth - kernelWidth; j += stride){

                uint16_t xcount = 0;
                for (uint16_t m = 0; m < inputDepth; m++) {
                    for(uint16_t k = 0; k < kernelHeight; k++){
                        for (uint16_t l = 0; l < kernelWidth; l++) {
                            int img_pos_H = (i + k) - padding;
                            int img_pos_W = (j + l) - padding;

                            uint16_t y_ndx = floor((float)xcount / (float)chunksize);
                            uint16_t x_ndx = xcount % chunksize;
                            // printf("OutDepth = %d %d %d %d ",b,ch,yndx,xndx);
                            //Check the padded region.
                            if(img_pos_H < 0 || img_pos_H >= inputHeight || img_pos_W < 0 || img_pos_W >= inputWidth){
                                output[b][ch_ndx][y_ndx][x_ndx] = (float)0.0;
                            }
                            else{
                                output[b][ch_ndx][y_ndx][x_ndx] = input[b][m][img_pos_H][img_pos_W];
                            }
                            xcount +=1;
                        }
                    }
                }
                ch_ndx +=1;
            }
        }
    }
}

//Gives 9 bit 2s complement representation of decimal integers.
void nineBit2sComp(int decimal, uint8_t* binary){
    if (decimal < -256 || decimal > 255) {
        //better error code than print is required
        //printf("Decimal value is out of range for 9 bits\n");
        return;
    }
    binary[8] = 0;
    // If the decimal is negative, convert it to positive
    if (decimal < 0) {
        decimal = abs(decimal);
        decimal ^= 0b11111111;  // Flip all the bits
        decimal += 0b00000001;
        binary[8] = 1;
    }

    // Convert the decimal to binary and store it in the array
    int i;
    for (i = 0; i < 8; i++) {
        binary[i] = decimal & 1;
        decimal >>= 1;
    }
};

//Gives prealigned mantissa and weight combination for vector using FP 16 format.
//Mantissa is represented as 9 bit 2s comp after taking 2^-7 cammon and including sign bit.
//Aligned weight is returned after adding -7 in max exponent
void preAlignVector(float* vec, uint16_t height, int * aligned_w, uint8_t ** binary_m){
    int max_exponant = -128; //minimum value possible for 8 bit exponant
    //Create temporary array of size dim to store FP16 converted data.
    FP16 * FP16Vec = (FP16 *)malloc(height*sizeof(FP16)); //This array can be passed as input and reused
    for(uint16_t i= 0; i < height; i++){
        FP16Vec[i] = getFP16(vec[i]); //need to see how can we handle overflow.
        //printf("%d %d %d %f\n",FP16Vec[i].overflow, FP16Vec[i].sign, FP16Vec[i].exponent, FP16Vec[i].mantissa);
        //get the max exponent required for prealignment
        max_exponant = FP16Vec[i].exponent > max_exponant ? FP16Vec[i].exponent:max_exponant;
    }
    //getFP16 returns weight without adding bias.
    *aligned_w = max_exponant - 7; // 2^-7 is taken common from 7 bit mantissa.
    for(uint16_t i = 0; i<height; i++){
        //Ignore fractional part after *128 as it is >7 bits values.
        int aligned_m = floor(FP16Vec[i].mantissa * 128 / pow(2,(max_exponant - FP16Vec[i].exponent)) ); // 2^-7 is taken common.
        aligned_m = FP16Vec[i].sign == 1? (-1)*aligned_m : aligned_m; //give sign to mantissa.
        nineBit2sComp(aligned_m, binary_m[i]); //take 9 bit 2s complement representation and add it in output
    }
    free(FP16Vec);
}

// Replication of In Memory Compute. Matrix mul of kernels*n*9 bits and n*9 bits = kernels*1.
// out will be of dimension kernels*1 decimal array.
// Call this function Per Image in Batch -> Per Stride - > Per K
void replicateIMC(uint8_t*** w_mat, uint16_t n_size, uint16_t n_kernals, int* we, int xe, uint8_t** x_mat, float* out ){
    memset(out,0,n_kernals*sizeof(int)); //initialise out to 0 as this array is being reused in loop
    uint16_t n_bits = 9; //assume number of bits of mantissa are 9 after alignement.
    //for all bits in w_mat
    for(uint16_t i=0;i<9;i++){
        //for all bits in x_mat
        for(uint16_t j=0;j<9;j++){
            //for all kernels = m
            for(uint16_t k = 0;k<n_kernals;k++){
                //for n elems = chunksize in IMC
                for(uint16_t n =0; n < n_size; n++){
                    float weight  = ((i == 8) ^ (j==8))? (-1*pow(2,we[k]+xe+i+j)) :  pow(2,we[k]+xe+i+j);
                    out[k]  += (w_mat[k][n][i] * x_mat[n][j] * weight);
                }

            }
        }
    }

}

//Function to add array element wise and store it in a
void addArrayElemwise(float* a, float* b, uint16_t size){
    for(uint16_t i = 0; i < size; i++){
        a[i] += b[i];
    }
}

//Function to convoluate unfolded matrices.
//Make sure CNV_Out is initialised with zeros
void normal_CNV(float**** Unfolded_X, uint16_t X_Batch, uint16_t X_Depth, uint16_t X_Height, uint16_t X_Width, float**** Unfolded_W,
                uint16_t W_Batch, uint16_t W_Depth, uint16_t W_Height, uint16_t W_Width, uint16_t X_out_w, float**** CNV_Out ){

    for(uint16_t xb = 0; xb < X_Batch; xb++){ //Per image in batch
        for(uint16_t xs = 0; xs < X_Depth; xs++){ // Per Stride in image
            for(uint16_t wb = 0; wb < W_Batch; wb ++){ //Per Kernel
                for(uint16_t xk = 0; xk < X_Height; xk++){ //Add per K chunks
                    for(uint16_t xn = 0; xn < X_Width; xn++){ // Per element in chunk.
                        CNV_Out[xb][wb][(int)floor((float)xs/(float)X_out_w)][xs%X_out_w] +=  (Unfolded_X[xb][xs][xk][xn] * Unfolded_W[wb][0][xk][xn]);
                    }
                }
            }
        }
    }
}

//2D Max Pooling function.
//Input batch*channels*height*width -> Output batch*channels*out_h*out_w
void MaxPooling(float**** Array,uint16_t batch, uint16_t depth, uint16_t height, uint16_t width, uint16_t pool_h,
                uint16_t pool_w, uint16_t padding, uint16_t stride, float**** Out){

    uint16_t padded_d = depth; //No padding added across depth.
    uint16_t padded_h = height + 2* padding;
    uint16_t padded_w = width + 2* padding;
    float elem = 0;
    for(uint16_t b = 0; b < batch; b++){
        for(uint16_t d = 0; d < padded_d; d++){
            for(uint16_t h = 0; h <= padded_h - pool_h; h +=stride){
                for(uint16_t w = 0; w <= padded_w - pool_w; w +=stride){
                    float max = -1 * pow(2,128);
                    for(uint16_t i = 0; i < pool_h; i++){
                        for(uint16_t j = 0; j < pool_w; j++){
                            int tmp_h = h + i - padding;
                            int tmp_w = w + j - padding;
                            if(tmp_h < 0 || tmp_h >= height || tmp_w < 0 || tmp_w >= width){
                                elem = -1 * pow(2,128); //pad with biggest negative number possible -> -infinity
                            }
                            else{
                                elem = Array[b][d][tmp_h][tmp_w];
                            }
                            max = max > elem? max:elem;
                        }
                    }
                    Out[b][d][h/stride][w/stride] = max;
                }
            }
        }
    }
}


//2D Avg Pooling function.
//Input batch*channels*height*width -> Output batch*channels*out_h*out_w
void AvgPooling(float**** Array,uint16_t batch, uint16_t depth, uint16_t height, uint16_t width, uint16_t pool_h,
                uint16_t pool_w, uint16_t padding, uint16_t stride,bool include_padding ,float**** Out){

    uint16_t padded_d = depth; //No padding added across depth.
    uint16_t padded_h = height + 2* padding;
    uint16_t padded_w = width + 2* padding;
    float elem = 0;
    for(uint16_t b = 0; b < batch; b++){
        for(uint16_t d = 0; d < padded_d; d++){
            for(uint16_t h = 0; h <= padded_h - pool_h; h +=stride){
                for(uint16_t w = 0; w <= padded_w - pool_w; w +=stride){
                    float sum = 0;
                    uint16_t count = 0;
                    for(uint16_t i = 0; i < pool_h; i++){
                        for(uint16_t j = 0; j < pool_w; j++){
                            int tmp_h = h + i - padding;
                            int tmp_w = w + j - padding;
                            if((tmp_h < 0 || tmp_h >= height || tmp_w < 0 || tmp_w >= width)){
                                if(include_padding){
                                    //sum += 0; //pad with 0
                                    count++;
                                }
                            }
                            else{
                                sum += Array[b][d][tmp_h][tmp_w];
                                count++;
                            }
                        }
                    }
                    Out[b][d][h/stride][w/stride] = sum/(float)count;
                }
            }
        }
    }
}

//4D ReLU function. Input = Output. Batch*Channels*Height*Width
//ReLU(x) = max(o,x)
void ReLU(float**** Array, uint16_t batch, uint16_t channels, uint16_t height, uint16_t width, float**** Out){
    for(uint16_t b = 0; b < batch; b++){
        for(uint16_t c = 0; c < channels; c++){
            for(uint16_t h = 0; h < height; h++){
                for(uint16_t w = 0; w < width; w++){
                    Out[b][c][h][w] = Array[b][c][h][w] > 0? Array[b][c][h][w]:0;
                }
            }
        }
    }
}


//4D Batch Normalisation.
//Running mean and variance are fed not computed.
//weight and bias is taken from training.
void BatchNorm(float**** Input, uint16_t batch, uint16_t kernels, uint16_t height, uint16_t width, float* mean,
              float* var, float* weight, float* bias, float**** Out){
	float eps=1e-05;
    for(uint16_t b = 0; b < batch; b++){//Per batch
        for(uint16_t k = 0; k < kernels; k++){//Per kernal
            for(uint16_t h = 0; h < height; h++){//Per height
                for(uint16_t w = 0; w < width; w++){//Per width
                    Out[b][k][h][w] = ((Input[b][k][h][w] - mean[k])/sqrt(var[k]+eps)) * weight[k] + bias[k];
                 }
            }
        }
    }
}

//Dummy IMC-HW function.
void replicateIMCHW(uint8_t**** W_BitMat,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,
                    uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,int** W_Weight,
                     int*** X_Weight, uint16_t X_out_w, float**** CNV_Out ){
    for(uint16_t kn = oc;kn < oc+ WDL; kn++){//For set of kernels
        for(uint16_t n = 0; n < WordLines; n++){//For wordline
            for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
                for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                    for(uint16_t j = xb; j < xb + XBL; j++){// For image bit
                        float weight  = ((i == 8) ^ (j==8))? (-1*pow(2,W_Weight[kn][c]+X_Weight[bt][s][c]+i+j)) :  pow(2,W_Weight[kn][c]+X_Weight[bt][s][c]+i+j);
                        CNV_Out[bt][kn][(int)floor((float)s/(float)X_out_w)][s%X_out_w] += (W_BitMat[kn][c][n][i] * X_BitMat[bt][s][c][n][j] * weight);
                    }
                }
            }
        }
    }
}


//Convolution function.
//Does unfolding->Pre Alignement->Convolution Operations.
//bool HW_CNV enables IMC. Else it does normal convolution.
curr_obj convolve(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj){
	// SD WRITE ADDED CODE STARTS HERE
//			FRESULT rc;
	// SD WRITE ADDED CODE ENDS HERE

	// Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.

    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.


    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_Bits=9;
        uint16_t W_BitMat_Bits = ceil((float)W_Bits/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }


        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector(Unfolded_W[b][c][k],WordLines,&W_Weight[b][k],W_BitMat[b][k]);
                }
            }

        }


//        int W_Weight_see[W_BitMat_Batch][W_outHeight];
//		for(uint16_t b = 0; b<W_BitMat_Batch;b++){
//				for(uint16_t k =0; k< W_outHeight; k++){
//					W_Weight_see[b][k]=W_Weight[b][k];
//			}
//		}
        // SD WRITE ADDED CODE STARTS HERE
//			FIL fil_Wexp;
//			UINT br_Wexp;
//			int size_Wexp=W_BitMat_Batch*W_outHeight*sizeof(W_Weight_see[0][0]);
//			char FileName_Wexp[]="WEXC.BIN";
//			rc = f_open(&fil_Wexp, FileName_Wexp, FA_WRITE | FA_CREATE_ALWAYS);
//			rc = f_write(&fil_Wexp, W_Weight_see, size_Wexp, &br_Wexp);
//			rc = f_close(&fil_Wexp);
		// SD WRITE ADDED CODE ENDS HERE

        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_BitMat_Bits = ceil((float)9/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
			for(uint16_t c = 0; c < X_outDepth; c++){
					X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
				}
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector(Unfolded_X[b][c][k],WordLines,&X_Weight[b][c][k],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }

//        int X_Weight_see[X_BitMat_Batch][X_outDepth][X_outHeight];
//		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
//			for(uint16_t c =0; c< X_outDepth; c++){
//				for(uint16_t k =0; k< X_outHeight; k++){
//					X_Weight_see[b][c][k]=X_Weight[b][c][k];
//				}
//			}
//		}
		// SD WRITE ADDED CODE STARTS HERE
//			FIL fil_Xexp;
//			UINT br_Xexp;
//			int size_Xexp=X_BitMat_Batch*X_outDepth*X_outHeight*sizeof(X_Weight_see[0][0][0]);
//			char FileName_Xexp[]="XEXC.BIN";
//			rc = f_open(&fil_Xexp, FileName_Xexp, FA_WRITE | FA_CREATE_ALWAYS);
//			rc = f_write(&fil_Xexp, X_Weight_see, size_Xexp, &br_Xexp);
//			rc = f_close(&fil_Xexp);
		// SD WRITE ADDED CODE ENDS HERE

        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);

        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
        float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
                }
            }
        }
//        float IMC_Out_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];
//
//        float ****IMC_Out_XADC = (float****)calloc(XDL,sizeof(float***));
//		for(uint16_t xd = 0; xd < XDL; xd++){
//			IMC_Out_XADC[xd] = (float***)calloc(WDL,sizeof(float**));
//			for(uint16_t wd = 0; wd < WDL; wd++){
//				IMC_Out_XADC[xd][wd] = (float**)calloc(XBL,sizeof(float*));
//				for(uint16_t xb = 0; xb < XBL; xb ++){
//					IMC_Out_XADC[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
//				}
//			}
//		}
//		float IMC_Out_XADC_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];

//        XTime tStart_outerLoop;
//		XTime tStop_outerLoop;
//		XTime tElapsed_outerLoop;
//		double tElapsed_outerLoop_msec;
//		XTime_GetTime(&tStart_outerLoop);

		// SD WRITE ADDED CODE STARTS HERE
//			FIL fil_IMCC;
//			FIL fil_RDSC;
//			FIL fil_IMCC_XADC;
//			UINT br_IMCC;
//			UINT br_RDSC;
//			UINT br_IMCC_XADC;
//			u32 file_size;
//			int size=XDL*WDL*XBL*WBL*4;
//			char FileName_IMCC[]="IMCC.bin";
//			char FileName_RDSC[]="RDSC.bin";
//			char FileName_IMCC_XADC[]="IC2X.bin";
//			rc = f_open(&fil_IMCC, FileName_IMCC, FA_WRITE | FA_CREATE_ALWAYS);
//			rc = f_open(&fil_RDSC, FileName_RDSC, FA_WRITE | FA_CREATE_ALWAYS);
//			rc = f_open(&fil_IMCC_XADC, FileName_IMCC_XADC, FA_WRITE | FA_CREATE_ALWAYS);
//				// extra added by ashwin on 20-Sep-2023 starts
//				FIL fil_volt;
//				FIL fil_volt_xadc;
//
//				UINT br_volt;
//				UINT br_volt_xadc;
//
//				int size_volt=XDL*WDL*XBL*WBL*4;
//				int size_volt_xadc=XDL*WDL*XBL*WBL*4;
//
//				char FileName_volt[]="V.bin";
//				char FileName_volt_xadc[]="VX.bin";
//
//				rc = f_open(&fil_volt, FileName_volt, FA_WRITE | FA_CREATE_ALWAYS);
//				rc = f_open(&fil_volt_xadc, FileName_volt_xadc, FA_WRITE | FA_CREATE_ALWAYS);
//				// extra added by ashwin on 20-Sep-2023 ends
		// SD WRITE ADDED CODE ENDS HERE

		// UART ADDED CODE STARTS HERE
		u32 totalTransmittedBytes=0;
		u32 transmittedBytes=0;
		XUartPs myUart=initialise_uart();

		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
		{
			u8 syncdata=0;
			// UART ADDED CODE STARTS HERE
			uint16_t returnWords[]={innerloop_start_word};//0 is stride
			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
			sendWordData( returnWords, numWords, myUart ) ;
			// UART ADDED CODE ENDS HERE
			// WAITING FOR SYNC DATA FROM MATLAB
			do
			{
				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
			}while(syncdata!=syncdata_val);
		}
		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

	    // ADDED BY ASHWIN STARTS HERE
//		X_outDepth=1;//strides search
	    // ADDED BY ASHWIN ENDS HERE

		{
//		// SD WRITE ADDED CODE STARTS HERE
//		// LAYER PARAMETERS WRITEOUT
//		char FileName_LYPM[]="LYPM.bin";
//		FIL fil_LYPM;
//		UINT br_LYPM;
//		rc = f_open(&fil_LYPM, FileName_LYPM, FA_WRITE | FA_CREATE_ALWAYS);
//
//		uint16_t LYPM[]={X_outDepth,W_BitMat_Batch,W_Batch,X_Batch,W_Bits,W_BitMat_Bits,WordLines,W_outHeight,W_outWidth,X_BitMat_Batch,X_outHeight};
//		int size_LYPM_bytes = sizeof( LYPM ) ;
//		rc = f_write(&fil_LYPM, LYPM, size_LYPM_bytes, &br_LYPM);
//		rc = f_close(&fil_LYPM);
//		// SD WRITE ADDED CODE ENDS HERE
		}

        //Run the IMC part
//		// TIME MEASUREMENT CODE STARTS HERE
//        XTime tStart_writeSram_FAST;
//	    XTime tStop_writeSram_FAST;
//	    XTime tElapsed_writeSram_FAST;
//	    double tElapsed_writeSram_FAST_sec;
//	    // TIME MEASUREMENT CODE ENDS HERE
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
            	// uint16_t wb=8;{//for debugging, wb=0 is LSB (pytorch_kw=8) wb=8 is unpadded MSB (pytorch_kw=0), wb=11 is padded MSB
                    //Store W_BitMat[oc:oc+WDL][c][:][wb:wb+WBL] in SRAM.
                	//Ref[c][0] Ref[c][1]
//                    currObj=store_weights_SRAM(W_BitMat, oc ,WDL, c, WordLines, wb, WBL, currObj);
                    currObj = store_weights_SRAM(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                    // uint16_t s = 1;{//for debugging
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                    	// uint16_t b = 24 ;{//for debugging, b:b+7 is the batch of 8 images
                             for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
                        	// uint16_t xb = 7;{//for debugging, xb=0 is LSB (pytorch_kx=8) xb=8 is MSB (pytorch _kx=0)
//                            	// TIME MEASUREMENT CODE STARTS HERE
//                            	XTime tStart_innerLoop;
//								XTime tStop_innerLoop;
//								XTime tElapsed_innerLoop;
//								double tElapsed_innerLoop_msec;
//								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE

                            	// Store X_BitMat[b:b+XDL][s][c][:][xb:xb+XBL]
                                // IMC_Out has dim -> XDL * WDL * XBL * WBL.
                                //memset(IMC_Out,0,XDL*WDL*XBL*WBL*sizeof(uint16_t));

                            	currObj=store_images_SRAM(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, currObj); // 650 ms
//                            	// TIME MEASUREMENT CODE STARTS HERE
//                            	XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//								// TIME MEASUREMENT CODE ENDS HERE
//								int pauseval1=0;
                                //Compute MAC. Get results from Chip.
                                //Combine chip results and multiply by weights(POT)
                                //Multiply2Power_CNV(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,W_Weight,X_Weight,X_out_w,CNV_Out);
//                                replicateIMCHW(W_BitMat, oc, WDL, c, WordLines, wb, WBL, X_BitMat, b, XDL, s, xb, XBL, W_Weight, X_Weight,X_out_w, CNV_Out);
//                            	currObj=IMC_TIMING(XDL, XBL, WBL, IMC_Out, currObj);


//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
							// SD WRITE ADDED CODE STARTS HERE
//							int numDLs=97;
//							int numWWLs=1092;
//							int numBLs=4;
//							u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
//							for(uint16_t wd = 0; wd < numDLs; wd++){
//								readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
//								for(uint16_t wl = 0; wl < numWWLs; wl++){
//									readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
//								}
//							}
//							currObj=readSRAMAll(currObj, readSRAMTotal);
//
//							u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));
//
//							for(uint16_t wb = 0; wb < numBLs; wb++){
//								for(uint16_t wl = 0; wl < numWWLs; wl++){
//									for(uint16_t wd = 0; wd < numDLs; wd++){
//										readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
//									}
//								}
//							}
//
//							u32 size_RDSC=numDLs*numWWLs*numBLs*sizeof(u8);
//							rc = f_write(&fil_RDSC, readSRAMTotal_FLAT, size_RDSC, &br_RDSC);
							// SD WRITE ADDED CODE ENDS HERE
//							 DEBUG CODE STARTS HERE
//                            {
//                    			double sumVal=0;
//                    			for(uint16_t _wb = 1; _wb < numBLs; _wb++){// when wb == 8 then _wb = 1,2,3 corresponds to the 2^(9,10,11) bits respvely
//									for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
//										for(uint16_t _wd = 0; _wd < WDL_HW; _wd++){
//                    							sumVal+=readSRAMTotal[DL_nums_usable[_wd]][_wl][_wb] ;
//                    						}
//                    					}
//                    				}
//                    			if( wb == 8 && sumVal != 0 )
//                    			{
//                    				int pause_local=0;
//                    			}
//                            }
//							 DEBUG CODE ENDS HERE
//							// TIME MEASUREMENT CODE STARTS HERE
//                        		XTime_GetTime(&tStart_innerLoop);
//							// TIME MEASUREMENT CODE ENDS HERE
							currObj=goldenGate(IMC_Out,mode,slopeSetting,RefMax,c,currObj);
//							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE
							// CORRECTION FOR THE PADDED BITS STARTS
							for(uint16_t _xd = 0; _xd < XDL; _xd++){
								for(uint16_t _wd = 0; _wd < WDL; _wd++){
									for(uint16_t _xb = 0; _xb < XBL; _xb++){
										for(uint16_t _wb = 0; _wb < WBL; _wb++){
											if( ( ( wb == 8 ) && _wb > 0 ) ||
													b >= X_Batch           ||
												( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Batch%WDL ) ) ) ){
												// 0s were padded on MSB for weights to make #weight bits a xle of WBL
												// 0s images were created to #batches a xle of XDL
												// 0 neurons were created to make #neurons a xle of WDL
												IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
//												IMC_Out_XADC[_xd][_wd][_xb][_wb] = 0 ;
											}
										}
									}
								}
							}
							// CORRECTION FOR THE PADDED BITS ENDS
							// SD WRITE ADDED CODE STARTS
//							for(uint16_t _xd = 0; _xd < XDL; _xd++){
//								for(uint16_t _wd = 0; _wd < WDL; _wd++){
//									for(uint16_t _xb = 0; _xb < XBL; _xb++){
//										for(uint16_t _wb = 0; _wb < WBL; _wb++){
//											IMC_Out_temp[_xd][_wd][_xb][_wb]=IMC_Out[_xd][_wd][_xb][_wb];
//											IMC_Out_XADC_temp[_xd][_wd][_xb][_wb]=IMC_Out_XADC[_xd][_wd][_xb][_wb];
//										}
//									}
//								}
//							}
//////
//////                            int size=XDL*WDL*XBL*WBL*4;
//////                    		char FileName_out[]="IMCO.bin";
//////                    		int rc=WriteFile_custom(FileName_out, size, IMC_Out_temp);
//////                    		int Status=SD_Eject(&fatfs);
//////							int pauseval=1;
//							rc = f_write(&fil_IMCC, IMC_Out_temp, size, &br_IMCC);
//							rc = f_write(&fil_IMCC_XADC, IMC_Out_XADC_temp, size, &br_IMCC_XADC);
//								// extra added by ashwin on 20-Sep-2023 starts
//								rc = f_write(&fil_volt, voltage_see, size_volt, &br_volt);
//								rc = f_write(&fil_volt_xadc, voltage_xadc_see, size_volt_xadc, &br_volt_xadc);
//								// extra added by ashwin on 20-Sep-2023 ends
							// SD WRITE ADDED CODE ENDS
//							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStart_innerLoop);
//							// TIME MEASUREMENT CODE ENDS HERE

//							Multiply2Power_LIN(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,xb,XBL,W_Weight,X_Weight,LIN_Out); // 2 ms
							Multiply2Power_CNV(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,W_Weight,X_Weight,X_out_w,CNV_Out);
//							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
////
//								XTime_GetTime(&tStart_innerLoop);
//							// TIME MEASUREMENT CODE ENDS HERE
						// UART ADDED CODE STARTS HERE
						uint16_t returnWords[]={oc,c,wb,s,b,xb};
						int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
						sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
						// UART ADDED CODE ENDS HERE
						// FREEING MEMORY STARTS HERE
//						for(uint16_t _wd = 0; _wd < numDLs; _wd++){
//							for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
//								free(readSRAMTotal[_wd][_wl]);
//							}
//							free(readSRAMTotal[_wd]);
//						}
//
//						free(readSRAMTotal_FLAT);
						// FREEING MEMORY END HERE
//						// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//						// TIME MEASUREMENT CODE ENDS HERE

                            }
                        }
                    }
                }
            }
        }
        // SD WRITE ADDED CODE STARTS
//		rc = f_close(&fil_IMCC);
//		rc = f_close(&fil_RDSC);
//		rc = f_close(&fil_IMCC_XADC);
//			// extra added by ashwin on 20-Sep-2023 starts
//			rc = f_close(&fil_volt);
//			rc = f_close(&fil_volt_xadc);
//			// extra added by ashwin on 20-Sep-2023 ends
//		Xil_DCacheFlush();
////		int Status=SD_Eject(&fatfs);
//		int pauseval=1;
		// SD WRITE ADDED CODE ENDS
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion

    return currObj;
}

//Convolution function.
//Does unfolding->Pre Alignement->Convolution Operations.
//bool HW_CNV enables IMC. Else it does normal convolution.
curr_obj convolve_XADC(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj){
	// SD WRITE ADDED CODE STARTS HERE
			FRESULT rc;
	// SD WRITE ADDED CODE ENDS HERE

	// Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.

    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.


    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_Bits=9;
        uint16_t W_BitMat_Bits = ceil((float)W_Bits/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }


        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector(Unfolded_W[b][c][k],WordLines,&W_Weight[b][k],W_BitMat[b][k]);
                }
            }

        }


        int W_Weight_see[W_BitMat_Batch][W_outHeight];
		for(uint16_t b = 0; b<W_BitMat_Batch;b++){
				for(uint16_t k =0; k< W_outHeight; k++){
					W_Weight_see[b][k]=W_Weight[b][k];
			}
		}
        // SD WRITE ADDED CODE STARTS HERE
			FIL fil_Wexp;
			UINT br_Wexp;
			int size_Wexp=W_BitMat_Batch*W_outHeight*sizeof(W_Weight_see[0][0]);
			char FileName_Wexp[]="WEXC.BIN";
			rc = f_open(&fil_Wexp, FileName_Wexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Wexp, W_Weight_see, size_Wexp, &br_Wexp);
			rc = f_close(&fil_Wexp);
		// SD WRITE ADDED CODE ENDS HERE

        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_BitMat_Bits = ceil((float)9/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
			for(uint16_t c = 0; c < X_outDepth; c++){
					X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
				}
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector(Unfolded_X[b][c][k],WordLines,&X_Weight[b][c][k],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }

        int X_Weight_see[X_BitMat_Batch][X_outDepth][X_outHeight];
		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			for(uint16_t c =0; c< X_outDepth; c++){
				for(uint16_t k =0; k< X_outHeight; k++){
					X_Weight_see[b][c][k]=X_Weight[b][c][k];
				}
			}
		}
		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Xexp;
			UINT br_Xexp;
			int size_Xexp=X_BitMat_Batch*X_outDepth*X_outHeight*sizeof(X_Weight_see[0][0][0]);
			char FileName_Xexp[]="XEXC.BIN";
			rc = f_open(&fil_Xexp, FileName_Xexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Xexp, X_Weight_see, size_Xexp, &br_Xexp);
			rc = f_close(&fil_Xexp);
		// SD WRITE ADDED CODE ENDS HERE

        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);

        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
        float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
                }
            }
        }
        float IMC_Out_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];

        float ****IMC_Out_XADC = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out_XADC[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out_XADC[xd][wd] = (float**)calloc(XBL,sizeof(float*));
				for(uint16_t xb = 0; xb < XBL; xb ++){
					IMC_Out_XADC[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}
		float IMC_Out_XADC_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];

//        XTime tStart_outerLoop;
//		XTime tStop_outerLoop;
//		XTime tElapsed_outerLoop;
//		double tElapsed_outerLoop_msec;
//		XTime_GetTime(&tStart_outerLoop);

		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_IMCC;
			FIL fil_RDSC;
			FIL fil_IMCC_XADC;
			UINT br_IMCC;
			UINT br_RDSC;
			UINT br_IMCC_XADC;
			u32 file_size;
			int size=XDL*WDL*XBL*WBL*4;
			char FileName_IMCC[]="IMCC.bin";
			char FileName_RDSC[]="RDSC.bin";
			char FileName_IMCC_XADC[]="IC2X.bin";
			rc = f_open(&fil_IMCC, FileName_IMCC, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_RDSC, FileName_RDSC, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_IMCC_XADC, FileName_IMCC_XADC, FA_WRITE | FA_CREATE_ALWAYS);
				// extra added by ashwin on 20-Sep-2023 starts
				FIL fil_volt;
				FIL fil_volt_xadc;

				UINT br_volt;
				UINT br_volt_xadc;

				int size_volt=XDL*WDL*XBL*WBL*4;
				int size_volt_xadc=XDL*WDL*XBL*WBL*4;

				char FileName_volt[]="V.bin";
				char FileName_volt_xadc[]="VX.bin";

				rc = f_open(&fil_volt, FileName_volt, FA_WRITE | FA_CREATE_ALWAYS);
				rc = f_open(&fil_volt_xadc, FileName_volt_xadc, FA_WRITE | FA_CREATE_ALWAYS);
				// extra added by ashwin on 20-Sep-2023 ends
		// SD WRITE ADDED CODE ENDS HERE

		// UART ADDED CODE STARTS HERE
		u32 totalTransmittedBytes=0;
		u32 transmittedBytes=0;
		XUartPs myUart=initialise_uart();

		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
		{
			u8 syncdata=0;
			// UART ADDED CODE STARTS HERE
			uint16_t returnWords[]={innerloop_start_word};//0 is stride
			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
			sendWordData( returnWords, numWords, myUart ) ;
			// UART ADDED CODE ENDS HERE
			// WAITING FOR SYNC DATA FROM MATLAB
			do
			{
				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
			}while(syncdata!=syncdata_val);
		}
		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

	    // ADDED BY ASHWIN STARTS HERE
//		X_outDepth=1;//strides search
	    // ADDED BY ASHWIN ENDS HERE

		{
		// SD WRITE ADDED CODE STARTS HERE
		// LAYER PARAMETERS WRITEOUT
		char FileName_LYPM[]="LYPM.bin";
		FIL fil_LYPM;
		UINT br_LYPM;
		rc = f_open(&fil_LYPM, FileName_LYPM, FA_WRITE | FA_CREATE_ALWAYS);

		uint16_t LYPM[]={X_outDepth,W_BitMat_Batch,W_Batch,X_Batch,W_Bits,W_BitMat_Bits,WordLines,W_outHeight,W_outWidth,X_BitMat_Batch,X_outHeight};
		int size_LYPM_bytes = sizeof( LYPM ) ;
		rc = f_write(&fil_LYPM, LYPM, size_LYPM_bytes, &br_LYPM);
		rc = f_close(&fil_LYPM);
		// SD WRITE ADDED CODE ENDS HERE
		}

        //Run the IMC part
        XTime tStart_writeSram_FAST;
	    XTime tStop_writeSram_FAST;
	    XTime tElapsed_writeSram_FAST;
	    double tElapsed_writeSram_FAST_sec;
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
            	// uint16_t wb=8;{//for debugging, wb=0 is LSB (pytorch_kw=8) wb=8 is unpadded MSB (pytorch_kw=0), wb=11 is padded MSB
                    //Store W_BitMat[oc:oc+WDL][c][:][wb:wb+WBL] in SRAM.
                	//Ref[c][0] Ref[c][1]
//                    currObj=store_weights_SRAM(W_BitMat, oc ,WDL, c, WordLines, wb, WBL, currObj);
                    currObj = store_weights_SRAM(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                    // uint16_t s = 1;{//for debugging
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                    	// uint16_t b = 24 ;{//for debugging, b:b+7 is the batch of 8 images
                             for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
                        	// uint16_t xb = 7;{//for debugging, xb=0 is LSB (pytorch_kx=8) xb=8 is MSB (pytorch _kx=0)

                            	XTime tStart_innerLoop;
								XTime tStop_innerLoop;
								XTime tElapsed_innerLoop;
								double tElapsed_innerLoop_msec;
								XTime_GetTime(&tStart_innerLoop);

                            	// Store X_BitMat[b:b+XDL][s][c][:][xb:xb+XBL]
                                // IMC_Out has dim -> XDL * WDL * XBL * WBL.
                                //memset(IMC_Out,0,XDL*WDL*XBL*WBL*sizeof(uint16_t));

                            	currObj=store_images_SRAM(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, currObj); // 650 ms
//                            	XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//								int pauseval1=0;
                                //Compute MAC. Get results from Chip.
                                //Combine chip results and multiply by weights(POT)
                                //Multiply2Power_CNV(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,W_Weight,X_Weight,X_out_w,CNV_Out);
//                                replicateIMCHW(W_BitMat, oc, WDL, c, WordLines, wb, WBL, X_BitMat, b, XDL, s, xb, XBL, W_Weight, X_Weight,X_out_w, CNV_Out);
//                            	currObj=IMC_TIMING(XDL, XBL, WBL, IMC_Out, currObj);


//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
							// SD WRITE ADDED CODE STARTS HERE
							int numDLs=97;
							int numWWLs=1092;
							int numBLs=4;
							u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
							for(uint16_t wd = 0; wd < numDLs; wd++){
								readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
								for(uint16_t wl = 0; wl < numWWLs; wl++){
									readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
								}
							}
							currObj=readSRAMAll(currObj, readSRAMTotal);

							u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));

							for(uint16_t wb = 0; wb < numBLs; wb++){
								for(uint16_t wl = 0; wl < numWWLs; wl++){
									for(uint16_t wd = 0; wd < numDLs; wd++){
										readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
									}
								}
							}

							u32 size_RDSC=numDLs*numWWLs*numBLs*sizeof(u8);
							rc = f_write(&fil_RDSC, readSRAMTotal_FLAT, size_RDSC, &br_RDSC);
							// SD WRITE ADDED CODE ENDS HERE
							// DEBUG CODE STARTS HERE
//                            {
//                    			double sumVal=0;
//                    			for(uint16_t _wb = 1; _wb < numBLs; _wb++){// when wb == 8 then _wb = 1,2,3 corresponds to the 2^(9,10,11) bits respvely
//									for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
//										for(uint16_t _wd = 0; _wd < WDL_HW; _wd++){
//                    							sumVal+=readSRAMTotal[DL_nums_usable[_wd]][_wl][_wb] ;
//                    						}
//                    					}
//                    				}
//                    			if( wb == 8 && sumVal != 0 )
//                    			{
//                    				int pause_local=0;
//                    			}
//                            }
							// DEBUG CODE ENDS HERE
							// TIME MEASUREMENT CODE STARTS HERE
//                        		XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE
							currObj=goldenGate_XADC(IMC_Out_XADC,IMC_Out,mode,slopeSetting,RefMax,c,currObj);
							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
							// TIME MEASUREMENT CODE ENDS HERE
							// CORRECTION FOR THE PADDED BITS STARTS
							for(uint16_t _xd = 0; _xd < XDL; _xd++){
								for(uint16_t _wd = 0; _wd < WDL; _wd++){
									for(uint16_t _xb = 0; _xb < XBL; _xb++){
										for(uint16_t _wb = 0; _wb < WBL; _wb++){
											if( ( ( wb == 8 ) && _wb > 0 ) ||
													b >= X_Batch           ||
												( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Batch%WDL ) ) ) ){
												// 0s were padded on MSB for weights to make #weight bits a xle of WBL
												// 0s images were created to #batches a xle of XDL
												// 0 neurons were created to make #neurons a xle of WDL
												IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
												IMC_Out_XADC[_xd][_wd][_xb][_wb] = 0 ;
											}
										}
									}
								}
							}
							// CORRECTION FOR THE PADDED BITS ENDS
							// SD WRITE ADDED CODE STARTS
							for(uint16_t _xd = 0; _xd < XDL; _xd++){
								for(uint16_t _wd = 0; _wd < WDL; _wd++){
									for(uint16_t _xb = 0; _xb < XBL; _xb++){
										for(uint16_t _wb = 0; _wb < WBL; _wb++){
											IMC_Out_temp[_xd][_wd][_xb][_wb]=IMC_Out[_xd][_wd][_xb][_wb];
											IMC_Out_XADC_temp[_xd][_wd][_xb][_wb]=IMC_Out_XADC[_xd][_wd][_xb][_wb];
										}
									}
								}
							}
////
////                            int size=XDL*WDL*XBL*WBL*4;
////                    		char FileName_out[]="IMCO.bin";
////                    		int rc=WriteFile_custom(FileName_out, size, IMC_Out_temp);
////                    		int Status=SD_Eject(&fatfs);
////							int pauseval=1;
							rc = f_write(&fil_IMCC, IMC_Out_temp, size, &br_IMCC);
							rc = f_write(&fil_IMCC_XADC, IMC_Out_XADC_temp, size, &br_IMCC_XADC);
								// extra added by ashwin on 20-Sep-2023 starts
								rc = f_write(&fil_volt, voltage_see, size_volt, &br_volt);
								rc = f_write(&fil_volt_xadc, voltage_xadc_see, size_volt_xadc, &br_volt_xadc);
								// extra added by ashwin on 20-Sep-2023 ends
							// SD WRITE ADDED CODE ENDS
							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE

//							Multiply2Power_LIN(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,xb,XBL,W_Weight,X_Weight,LIN_Out); // 2 ms
							Multiply2Power_CNV(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,W_Weight,X_Weight,X_out_w,CNV_Out);
							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//
//								XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE
						// UART ADDED CODE STARTS HERE
						uint16_t returnWords[]={oc,c,wb,s,b,xb};
						int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
						sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
						// UART ADDED CODE ENDS HERE
						// FREEING MEMORY STARTS HERE
						for(uint16_t _wd = 0; _wd < numDLs; _wd++){
							for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
								free(readSRAMTotal[_wd][_wl]);
							}
							free(readSRAMTotal[_wd]);
						}

						free(readSRAMTotal_FLAT);
						// FREEING MEMORY END HERE
						// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
						// TIME MEASUREMENT CODE ENDS HERE

                            }
                        }
                    }
                }
            }
        }
        // SD WRITE ADDED CODE STARTS
		rc = f_close(&fil_IMCC);
		rc = f_close(&fil_RDSC);
		rc = f_close(&fil_IMCC_XADC);
			// extra added by ashwin on 20-Sep-2023 starts
			rc = f_close(&fil_volt);
			rc = f_close(&fil_volt_xadc);
			// extra added by ashwin on 20-Sep-2023 ends
		Xil_DCacheFlush();
//		int Status=SD_Eject(&fatfs);
		int pauseval=1;
		// SD WRITE ADDED CODE ENDS
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion
    return currObj;
}

//Linear Layer Unfold Function
//Batch * IF -> Batch * Chunks * WordLines. Make sure that output is initialised with proper dims.
void rearrange2DImage(float** input, uint16_t inputBatch, uint16_t inputFeatures, uint16_t chunksize, float*** output){
    for(uint16_t b = 0; b < inputBatch; b++){
        for(uint16_t i = 0; i < inputFeatures; i++){
            output[b][(int)floor((float)i/(float)chunksize)][i%chunksize] = input[b][i];
        }
    }
}

//Fully Connected Layers -> Linear Function.
curr_obj linear(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, mode_type mode, u8 slopeSetting, curr_obj currObj){

	// SD WRITE ADDED CODE STARTS HERE
			FRESULT rc;
	// SD WRITE ADDED CODE ENDS HERE

    //Unfold X_Mat and W_Mat
    #pragma region
    //Allocate memory for Unfolded_X
    uint16_t X_W_Chunks = ceil((float)X_Features/(float)WordLines); //Chunks will be same for both X_Mat and W_Mat.
    float*** Unfolded_X = (float***)calloc(X_Batch,sizeof(float**));
    for(uint16_t b = 0; b < X_Batch; b++){
        Unfolded_X[b] = (float**)calloc(X_W_Chunks,sizeof(float*));
        for(uint16_t c = 0; c < X_W_Chunks; c++){
            Unfolded_X[b][c] = (float*)calloc(WordLines,sizeof(float));
            memset(Unfolded_X[b][c],0,WordLines*sizeof(Unfolded_X[b][c][0]));
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
            memset(Unfolded_W[b][c],0,WordLines*sizeof(Unfolded_W[b][c][0]));
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

//        uint8_t W_BitMat_see[W_BitMat_Bits][W_BitMat_Batch][WordLines];
//        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
//			for(uint16_t k =0; k< X_W_Chunks; k++){
//				for(uint16_t n=0; n<WordLines; n++){
//					for(uint16_t m=0; m<W_BitMat_Bits; m++){
//						W_BitMat_see[m][b][n] = W_BitMat[b][k][n][m] ;
//					}
//				}
//			}
//		}


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

        // DEBUG CODE STARTS HERE
//        {
//			double sumVal=0;
//			for(uint16_t b = 0; b<W_BitMat_Batch;b++){
//				for(uint16_t k =0; k< X_W_Chunks; k++){
//					for(uint16_t n=0; n<WordLines; n++){
//						for(uint16_t m=9; m<W_BitMat_Bits; m++){
////						for(uint16_t m=0; m<9; m++){
//							sumVal+=W_BitMat[b][k][n][m] ;
//						}
//					}
//				}
//			}
//        }
        // DEBUG CODE ENDS HERE

		int W_Weight_see[W_BitMat_Batch][X_W_Chunks] ;
		for(uint16_t b = 0; b<W_BitMat_Batch;b++){
			for(uint16_t k =0; k< X_W_Chunks; k++){
				W_Weight_see[b][k]=W_Weight[b][k];
			}
		}

		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Wexp;
			UINT br_Wexp;
			int size_Wexp=W_BitMat_Batch*X_W_Chunks*sizeof(W_Weight_see[0][0]);
			char FileName_Wexp[]="Wexp.bin";
			rc = f_open(&fil_Wexp, FileName_Wexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Wexp, W_Weight_see, size_Wexp, &br_Wexp);
			rc = f_close(&fil_Wexp);
		// SD WRITE ADDED CODE ENDS HERE

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

		int X_Weight_see[X_BitMat_Batch][X_W_Chunks];
		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			for(uint16_t k =0; k< X_W_Chunks; k++){
				X_Weight_see[b][k]=X_Weight[b][k];
			}
		}

        // SD WRITE ADDED CODE STARTS HERE
//			FIL fil_Xexp;
//			UINT br_Xexp;
//			int size_Xexp=X_BitMat_Batch*X_W_Chunks*sizeof(X_Weight_see[0][0]);
//			char FileName_Xexp[]="Xexp.bin";
//			rc = f_open(&fil_Xexp, FileName_Xexp, FA_WRITE | FA_CREATE_ALWAYS);
//			rc = f_write(&fil_Xexp, X_Weight_see, size_Xexp, &br_Xexp);
//			rc = f_close(&fil_Xexp);
        // SD WRITE ADDED CODE ENDS HERE

//        uint8_t X_BitMat_see[X_BitMat_Batch][X_W_Chunks][WordLines][X_BitMat_Bits];
//		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
//			for(uint16_t k =0; k< X_W_Chunks; k++){
//				for(uint16_t n=0; n<WordLines; n++){
//					for(uint16_t xb = 0; xb < X_BitMat_Bits; xb ++){
//						X_BitMat_see[b][k][n][xb]=X_BitMat[b][k][n][xb];
//					}
//				}
//			}
//		}



        // ADDED CODE STARTS
//        int size=X_BitMat_Batch*X_W_Chunks*WordLines*X_BitMat_Bits;
//		char FileName_out[]="XBMT.bin";
//		int rc=WriteFile_custom(FileName_out, size, X_BitMat_see);
//		int Status=SD_Eject(&fatfs);
//		int pauseval=1;
		// ADDED CODE STARTS ENDS

        #pragma endregion

        //Linear IMC part here.
        #pragma region
        //Create array to store IMC results.
		float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
                }
            }
        }
        //uint16_t IMC_Out_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];



        // SD WRITE ADDED CODE STARTS HERE
//        FIL fil_IMCO;
//        FIL fil_RDSR;
//		UINT br_IMCO;
//		UINT br_RDSR;
//		u32 file_size;
//		int size=XDL*WDL*XBL*WBL*4;
//		char FileName_IMCO[]="IMCO.bin";
//		char FileName_RDSR[]="RDSR.bin";
//        rc = f_open(&fil_IMCO, FileName_IMCO, FA_WRITE | FA_CREATE_ALWAYS);
//        rc = f_open(&fil_RDSR, FileName_RDSR, FA_WRITE | FA_CREATE_ALWAYS);
        // SD WRITE ADDED CODE ENDS HERE

        // UART ADDED CODE STARTS HERE
        u32 totalTransmittedBytes=0;
        u32 transmittedBytes=0;
        XUartPs myUart=initialise_uart();

//        uint16_t returnBytes_TEST[]={256,256,256};
//		int fileSize_return=sizeof(returnBytes_TEST)/sizeof(returnBytes_TEST[0]);
//
//		while((totalTransmittedBytes/2) < fileSize_return)
//		{
//			//transmit returnBytes[totalTransmittedBytes]
//			transmittedBytes = XUartPs_Send(&myUart,&(returnBytes_TEST[totalTransmittedBytes/2]),2);
//			totalTransmittedBytes+=transmittedBytes;
//		}
//		int pauseval_TEST=1;

        // UART ADDED CODE ENDS HERE
        // For Debugging layer timing STARTS HERE
//      X_BitMat_Batch=8;
//		XTime tStart_outerLoop;
//		XTime tStop_outerLoop;
//		XTime tElapsed_outerLoop;
//		double tElapsed_outerLoop_msec;
//		XTime_GetTime(&tStart_outerLoop);
        // For Debugging layer timing ENDS HERE

        // TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
    	{
        	u8 syncdata=0;
    		// UART ADDED CODE STARTS HERE
    		uint16_t returnWords[]={innerloop_start_word};//0 is stride
    		int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
    		sendWordData( returnWords, numWords, myUart ) ;
    		// UART ADDED CODE ENDS HERE
    		// WAITING FOR SYNC DATA FROM MATLAB
    		do
    		{
    			u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
    		}while(syncdata!=syncdata_val);
    	}
    	// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

//        uint16_t oc = 0;{
         for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
//        	uint16_t c = 0;{
             for(uint16_t c = 0; c<X_W_Chunks; c++){//For chunks
//        		uint16_t wb = 0;{
                 for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                	currObj = store_weights_SRAM_lin(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch // ACTUAL CODE ****************
//                     uint16_t b = 80;{ //For padded image batch // ACTUAL CODE ****************
                        for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
//                    	 uint16_t xb = 7;{ //For padded image batch // ACTUAL CODE ****************
                        	// TIME MEASUREMENT CODE STARTS HERE
//						XTime tStart_innerLoop;
//						XTime tStop_innerLoop;
//						XTime tElapsed_innerLoop;
//						double tElapsed_innerLoop_msec;
                        	// TIME MEASUREMENT CODE ENDS HERE
//
                        	// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStart_innerLoop);
                        	// TIME MEASUREMENT CODE ENDS HERE

                        	currObj = store_images_SRAM_lin(X_BitMat, b , XDL, c, WordLines, xb, XBL, currObj); // 105 ms

//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
                        	// SD WRITE ADDED CODE STARTS HERE
//                        	int numDLs=97;
//                        	int numWWLs=1092;
//                        	int numBLs=4;
//                        	u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
//                        	for(uint16_t wd = 0; wd < numDLs; wd++){
//                        		readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
//                        		for(uint16_t wl = 0; wl < numWWLs; wl++){
//                        			readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
//                        		}
//                        	}
//                        	currObj=readSRAMAll(currObj, readSRAMTotal);
//
//                        	u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));
//
//                        	for(uint16_t wb = 0; wb < numBLs; wb++){
//                        		for(uint16_t wl = 0; wl < numWWLs; wl++){
//                        			for(uint16_t wd = 0; wd < numDLs; wd++){
//                        				readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
//                        			}
//                        		}
//                        	}
//
//                        	u32 size_RDSR=numDLs*numWWLs*numBLs*sizeof(u8);
//                        	rc = f_write(&fil_RDSR, readSRAMTotal_FLAT, size_RDSR, &br_RDSR);
                        	// SD WRITE ADDED CODE ENDS HERE
                            // DEBUG CODE STARTS HERE
//                            {
//                    			double sumVal=0;
//                    			for(uint16_t _wb = 1; _wb < numBLs; _wb++){// when wb == 8 then _wb = 1,2,3 corresponds to the 2^(9,10,11) bits respvely
//									for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
//										for(uint16_t _wd = 0; _wd < WDL_HW; _wd++){
//                    							sumVal+=readSRAMTotal[DL_nums_usable[_wd]][_wl][_wb] ;
//                    						}
//                    					}
//                    				}
//                    			if( wb == 8 && sumVal != 0 )
//                    			{
//                    				int pause_local=0;
//                    			}
//                            }
                            // DEBUG CODE ENDS HERE
                        	// TIME MEASUREMENT CODE STARTS HERE
//                        		XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE
                        		// u8 mode=2;//OCC
                        		// u8 mode=1;//ICQ
                        	currObj=goldenGate(IMC_Out,mode,slopeSetting,RefMax,c,currObj);//303ms
                        	// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
							// TIME MEASUREMENT CODE ENDS HERE
							// CORRECTION FOR THE PADDED BITS STARTS
							for(uint16_t _xd = 0; _xd < XDL; _xd++){
								for(uint16_t _wd = 0; _wd < WDL; _wd++){
									for(uint16_t _xb = 0; _xb < XBL; _xb++){
										for(uint16_t _wb = 0; _wb < WBL; _wb++){
											if( ( ( wb == 8 ) && _wb > 0 ) ||
													b >= X_Batch           ||
											    ( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Features%WDL ) ) ) ){
												// 0s were padded on MSB for weights to make #weight bits a xle of WBL
												// 0s images were created to #batches a xle of XDL
												// 0 neurons were created to make #neurons a xle of WDL
												IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
											}
										}
									}
								}
							}
							// CORRECTION FOR THE PADDED BITS ENDS
                        	// SD WRITE ADDED CODE STARTS
//                            for(uint16_t _xd = 0; _xd < XDL; _xd++){
//                                for(uint16_t _wd = 0; _wd < WDL; _wd++){
//                                    for(uint16_t _xb = 0; _xb < XBL; _xb++){
//                                    	for(uint16_t _wb = 0; _wb < WBL; _wb++){
//                                    		IMC_Out_temp[_xd][_wd][_xb][_wb]=IMC_Out[_xd][_wd][_xb][_wb];
//										}
//                                    }
//                                }
//                            }
//////
//////                            int size=XDL*WDL*XBL*WBL*4;
//////                    		char FileName_out[]="IMCO.bin";
//////                    		int rc=WriteFile_custom(FileName_out, size, IMC_Out_temp);
//////                    		int Status=SD_Eject(&fatfs);
//////							int pauseval=1;
//                    		rc = f_write(&fil_IMCO, IMC_Out_temp, size, &br_IMCO);
                    		// SD WRITE ADDED CODE ENDS
							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE

                            Multiply2Power_LIN(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,xb,XBL,W_Weight,X_Weight,LIN_Out); // 2 ms
                            // TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//
//								XTime_GetTime(&tStart_innerLoop);
                            // TIME MEASUREMENT CODE ENDS HERE
						// UART ADDED CODE STARTS HERE
						uint16_t returnWords[]={oc,c,wb,0,b,xb};//0 is stride
						int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
						sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
						// UART ADDED CODE ENDS HERE
						// FREEING MEMORY STARTS HERE
//						for(uint16_t _wd = 0; _wd < numDLs; _wd++){
//							for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
//								free(readSRAMTotal[_wd][_wl]);
//							}
//							free(readSRAMTotal[_wd]);
//						}
//
//						free(readSRAMTotal_FLAT);
						// FREEING MEMORY END HERE
						// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
						// TIME MEASUREMENT CODE ENDS HERE
						int p0=1;
                        }
                        int p1=1;
                    }
                    int p2=1;
                }
                 int p3=1;
            }
             int p4=1;
        }

        // SD WRITE ADDED CODE STARTS
//        rc = f_close(&fil_IMCO);
//        rc = f_close(&fil_RDSR);
//        Xil_DCacheFlush();
////		int Status=SD_Eject(&fatfs);
//        {int pauseval=1;}
        // SD WRITE ADDED CODE ENDS

		// For Debugging layer timing STARTS HERE
//        XTime_GetTime(&tStop_outerLoop);
//		tElapsed_outerLoop=tStop_outerLoop-tStart_outerLoop;
//		tElapsed_outerLoop_msec=1000*((double)tElapsed_outerLoop)/((double)COUNTS_PER_SECOND);
//		{int pauseval=1;}
		// For Debugging layer timing ENDS HERE
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


//Fully Connected Layers -> Linear Function.
curr_obj linear_XADC(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, mode_type mode,u8 slopeSetting, curr_obj currObj){

	// SD WRITE ADDED CODE STARTS HERE
			FRESULT rc;
	// SD WRITE ADDED CODE ENDS HERE

    //Unfold X_Mat and W_Mat
    #pragma region
    //Allocate memory for Unfolded_X
    uint16_t X_W_Chunks = ceil((float)X_Features/(float)WordLines); //Chunks will be same for both X_Mat and W_Mat.
    float*** Unfolded_X = (float***)calloc(X_Batch,sizeof(float**));
    for(uint16_t b = 0; b < X_Batch; b++){
        Unfolded_X[b] = (float**)calloc(X_W_Chunks,sizeof(float*));
        for(uint16_t c = 0; c < X_W_Chunks; c++){
            Unfolded_X[b][c] = (float*)calloc(WordLines,sizeof(float));
            memset(Unfolded_X[b][c],0,WordLines*sizeof(Unfolded_X[b][c][0]));
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
            memset(Unfolded_W[b][c],0,WordLines*sizeof(Unfolded_W[b][c][0]));
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

//        uint8_t W_BitMat_see[W_BitMat_Bits][W_BitMat_Batch][WordLines];
//        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
//			for(uint16_t k =0; k< X_W_Chunks; k++){
//				for(uint16_t n=0; n<WordLines; n++){
//					for(uint16_t m=0; m<W_BitMat_Bits; m++){
//						W_BitMat_see[m][b][n] = W_BitMat[b][k][n][m] ;
//					}
//				}
//			}
//		}


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

        // DEBUG CODE STARTS HERE
//        {
//			double sumVal=0;
//			for(uint16_t b = 0; b<W_BitMat_Batch;b++){
//				for(uint16_t k =0; k< X_W_Chunks; k++){
//					for(uint16_t n=0; n<WordLines; n++){
//						for(uint16_t m=9; m<W_BitMat_Bits; m++){
////						for(uint16_t m=0; m<9; m++){
//							sumVal+=W_BitMat[b][k][n][m] ;
//						}
//					}
//				}
//			}
//        }
        // DEBUG CODE ENDS HERE

		int W_Weight_see[W_BitMat_Batch][X_W_Chunks] ;
		for(uint16_t b = 0; b<W_BitMat_Batch;b++){
			for(uint16_t k =0; k< X_W_Chunks; k++){
				W_Weight_see[b][k]=W_Weight[b][k];
			}
		}

		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Wexp;
			UINT br_Wexp;
			int size_Wexp=W_BitMat_Batch*X_W_Chunks*sizeof(W_Weight_see[0][0]);
			char FileName_Wexp[]="Wexp.bin";
			rc = f_open(&fil_Wexp, FileName_Wexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Wexp, W_Weight_see, size_Wexp, &br_Wexp);
			rc = f_close(&fil_Wexp);
		// SD WRITE ADDED CODE ENDS HERE

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

		int X_Weight_see[X_BitMat_Batch][X_W_Chunks];
		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			for(uint16_t k =0; k< X_W_Chunks; k++){
				X_Weight_see[b][k]=X_Weight[b][k];
			}
		}

        // SD WRITE ADDED CODE STARTS HERE
			FIL fil_Xexp;
			UINT br_Xexp;
			int size_Xexp=X_BitMat_Batch*X_W_Chunks*sizeof(X_Weight_see[0][0]);
			char FileName_Xexp[]="Xexp.bin";
			rc = f_open(&fil_Xexp, FileName_Xexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Xexp, X_Weight_see, size_Xexp, &br_Xexp);
			rc = f_close(&fil_Xexp);
        // SD WRITE ADDED CODE ENDS HERE

//        uint8_t X_BitMat_see[X_BitMat_Batch][X_W_Chunks][WordLines][X_BitMat_Bits];
//		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
//			for(uint16_t k =0; k< X_W_Chunks; k++){
//				for(uint16_t n=0; n<WordLines; n++){
//					for(uint16_t xb = 0; xb < X_BitMat_Bits; xb ++){
//						X_BitMat_see[b][k][n][xb]=X_BitMat[b][k][n][xb];
//					}
//				}
//			}
//		}



        // ADDED CODE STARTS
//        int size=X_BitMat_Batch*X_W_Chunks*WordLines*X_BitMat_Bits;
//		char FileName_out[]="XBMT.bin";
//		int rc=WriteFile_custom(FileName_out, size, X_BitMat_see);
//		int Status=SD_Eject(&fatfs);
//		int pauseval=1;
		// ADDED CODE STARTS ENDS

        #pragma endregion

        //Linear IMC part here.
        #pragma region
        //Create array to store IMC results.
        float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
                }
            }
        }
        float IMC_Out_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];

        float ****IMC_Out_XADC = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out_XADC[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out_XADC[xd][wd] = (float**)calloc(XBL,sizeof(float*));
				for(uint16_t xb = 0; xb < XBL; xb ++){
					IMC_Out_XADC[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}
		float IMC_Out_XADC_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];


//        XTime tStart_outerLoop;
//		XTime tStop_outerLoop;
//		XTime tElapsed_outerLoop;
//		double tElapsed_outerLoop_msec;
//		XTime_GetTime(&tStart_outerLoop);

        // SD WRITE ADDED CODE STARTS HERE
        FIL fil_IMCO;
        FIL fil_RDSR;
        FIL fil_IMCO_XADC;
		UINT br_IMCO;
		UINT br_RDSR;
		UINT br_IMCO_XADC;
		u32 file_size;
		int size=XDL*WDL*XBL*WBL*4;
		char FileName_IMCO[]="IMCO.bin";
		char FileName_RDSR[]="RDSR.bin";
		char FileName_IMCO_XADC[]="IMCX.bin";
        rc = f_open(&fil_IMCO, FileName_IMCO, FA_WRITE | FA_CREATE_ALWAYS);
        rc = f_open(&fil_RDSR, FileName_RDSR, FA_WRITE | FA_CREATE_ALWAYS);
        rc = f_open(&fil_IMCO_XADC, FileName_IMCO_XADC, FA_WRITE | FA_CREATE_ALWAYS);
			// extra added by ashwin on 20-Sep-2023 starts
			FIL fil_volt;
			FIL fil_volt_xadc;

			UINT br_volt;
			UINT br_volt_xadc;

			int size_volt=XDL*WDL*XBL*WBL*4;
			int size_volt_xadc=XDL*WDL*XBL*WBL*4;

			char FileName_volt[]="V.bin";
			char FileName_volt_xadc[]="VX.bin";

			rc = f_open(&fil_volt, FileName_volt, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_volt_xadc, FileName_volt_xadc, FA_WRITE | FA_CREATE_ALWAYS);
			// extra added by ashwin on 20-Sep-2023 ends
        // SD WRITE ADDED CODE ENDS HERE

        // UART ADDED CODE STARTS HERE
        u32 totalTransmittedBytes=0;
        u32 transmittedBytes=0;
        XUartPs myUart=initialise_uart();

//        uint16_t returnBytes_TEST[]={256,256,256};
//		int fileSize_return=sizeof(returnBytes_TEST)/sizeof(returnBytes_TEST[0]);
//
//		while((totalTransmittedBytes/2) < fileSize_return)
//		{
//			//transmit returnBytes[totalTransmittedBytes]
//			transmittedBytes = XUartPs_Send(&myUart,&(returnBytes_TEST[totalTransmittedBytes/2]),2);
//			totalTransmittedBytes+=transmittedBytes;
//		}
//		int pauseval_TEST=1;

        // UART ADDED CODE ENDS HERE



//        uint16_t oc = 0;{
         for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
//        	uint16_t c = 0;{
             for(uint16_t c = 0; c<X_W_Chunks; c++){//For chunks
//        		uint16_t wb = 0;{
                 for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                	currObj = store_weights_SRAM_lin(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch // ACTUAL CODE ****************
//                     uint16_t b = 80;{ //For padded image batch // ACTUAL CODE ****************
                        for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
//                    	 uint16_t xb = 7;{ //For padded image batch // ACTUAL CODE ****************
                        	// TIME MEASUREMENT CODE STARTS HERE
//						XTime tStart_innerLoop;
//						XTime tStop_innerLoop;
//						XTime tElapsed_innerLoop;
//						double tElapsed_innerLoop_msec;
                        	// TIME MEASUREMENT CODE ENDS HERE
//
                        	// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStart_innerLoop);
                        	// TIME MEASUREMENT CODE ENDS HERE

                        	currObj = store_images_SRAM_lin(X_BitMat, b , XDL, c, WordLines, xb, XBL, currObj); // 105 ms

//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
                        	// SD WRITE ADDED CODE STARTS HERE
                        	int numDLs=97;
                        	int numWWLs=1092;
                        	int numBLs=4;
                        	u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
                        	for(uint16_t wd = 0; wd < numDLs; wd++){
                        		readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
                        		for(uint16_t wl = 0; wl < numWWLs; wl++){
                        			readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
                        		}
                        	}
                        	currObj=readSRAMAll(currObj, readSRAMTotal);

                        	u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));

                        	for(uint16_t wb = 0; wb < numBLs; wb++){
                        		for(uint16_t wl = 0; wl < numWWLs; wl++){
                        			for(uint16_t wd = 0; wd < numDLs; wd++){
                        				readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
                        			}
                        		}
                        	}

                        	u32 size_RDSR=numDLs*numWWLs*numBLs*sizeof(u8);
                        	rc = f_write(&fil_RDSR, readSRAMTotal_FLAT, size_RDSR, &br_RDSR);
                        	// SD WRITE ADDED CODE ENDS HERE
                            // DEBUG CODE STARTS HERE
//                            {
//                    			double sumVal=0;
//                    			for(uint16_t _wb = 1; _wb < numBLs; _wb++){// when wb == 8 then _wb = 1,2,3 corresponds to the 2^(9,10,11) bits respvely
//									for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
//										for(uint16_t _wd = 0; _wd < WDL_HW; _wd++){
//                    							sumVal+=readSRAMTotal[DL_nums_usable[_wd]][_wl][_wb] ;
//                    						}
//                    					}
//                    				}
//                    			if( wb == 8 && sumVal != 0 )
//                    			{
//                    				int pause_local=0;
//                    			}
//                            }
                            // DEBUG CODE ENDS HERE
                        	// TIME MEASUREMENT CODE STARTS HERE
//                        		XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE
                        		// u8 mode=2;//OCC
                        		// u8 mode=1;//ICQ
//                        	currObj=goldenGate(IMC_Out,mode,slopeSetting,RefMax,c,currObj);//303ms

                        	// ADD CODE FOR READING OUT VOLTAGES IN HYBRID MODE STARTS
                        	currObj=goldenGate_XADC(IMC_Out_XADC,IMC_Out,mode,slopeSetting,RefMax,c,currObj);// MUST MAKE
                        	// ADD CODE FOR READING OUT VOLTAGES IN HYBRID MODE ENDS

                        	// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
							// TIME MEASUREMENT CODE ENDS HERE
							// CORRECTION FOR THE PADDED BITS STARTS
							for(uint16_t _xd = 0; _xd < XDL; _xd++){
								for(uint16_t _wd = 0; _wd < WDL; _wd++){
									for(uint16_t _xb = 0; _xb < XBL; _xb++){
										for(uint16_t _wb = 0; _wb < WBL; _wb++){
											if( ( ( wb == 8 ) && _wb > 0 ) ||
													b >= X_Batch           ||
											    ( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Features%WDL ) ) ) ){
												// 0s were padded on MSB for weights to make #weight bits a xle of WBL
												// 0s images were created to #batches a xle of XDL
												// 0 neurons were created to make #neurons a xle of WDL
												IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
												IMC_Out_XADC[_xd][_wd][_xb][_wb] = 0 ;
											}
										}
									}
								}
							}
							// CORRECTION FOR THE PADDED BITS ENDS
                        	// SD WRITE ADDED CODE STARTS
                            for(uint16_t _xd = 0; _xd < XDL; _xd++){
                                for(uint16_t _wd = 0; _wd < WDL; _wd++){
                                    for(uint16_t _xb = 0; _xb < XBL; _xb++){
                                    	for(uint16_t _wb = 0; _wb < WBL; _wb++){
                                    		IMC_Out_temp[_xd][_wd][_xb][_wb]=IMC_Out[_xd][_wd][_xb][_wb];
                                    		IMC_Out_XADC_temp[_xd][_wd][_xb][_wb]=IMC_Out_XADC[_xd][_wd][_xb][_wb];
										}
                                    }
                                }
                            }
////
////                            int size=XDL*WDL*XBL*WBL*4;
////                    		char FileName_out[]="IMCO.bin";
////                    		int rc=WriteFile_custom(FileName_out, size, IMC_Out_temp);
////                    		int Status=SD_Eject(&fatfs);
////							int pauseval=1;
                    		rc = f_write(&fil_IMCO, IMC_Out_temp, size, &br_IMCO);
                    		rc = f_write(&fil_IMCO_XADC, IMC_Out_XADC_temp, size, &br_IMCO_XADC);
                    			// extra added by ashwin on 20-Sep-2023 starts
								rc = f_write(&fil_volt, voltage_see, size_volt, &br_volt);
								rc = f_write(&fil_volt_xadc, voltage_xadc_see, size_volt_xadc, &br_volt_xadc);
								// extra added by ashwin on 20-Sep-2023 ends
                    		// SD WRITE ADDED CODE ENDS
							// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStart_innerLoop);
							// TIME MEASUREMENT CODE ENDS HERE

                            Multiply2Power_LIN(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,xb,XBL,W_Weight,X_Weight,LIN_Out); // 2 ms
                            // TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//
//								XTime_GetTime(&tStart_innerLoop);
                            // TIME MEASUREMENT CODE ENDS HERE
						// UART ADDED CODE STARTS HERE
//						uint16_t returnWords[]={oc,c,wb,b,xb};
						uint16_t returnWords[]={oc,c,wb,0,b,xb};//0 is stride
						int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
						sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
						// UART ADDED CODE ENDS HERE
						// FREEING MEMORY STARTS HERE
						for(uint16_t _wd = 0; _wd < numDLs; _wd++){
							for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
								free(readSRAMTotal[_wd][_wl]);
							}
							free(readSRAMTotal[_wd]);
						}

						free(readSRAMTotal_FLAT);
						// FREEING MEMORY END HERE
						// TIME MEASUREMENT CODE STARTS HERE
//								XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
						// TIME MEASUREMENT CODE ENDS HERE
						int p0=1;
                        }
                        int p1=1;
                    }
                    int p2=1;
                }
                 int p3=1;
            }
             int p4=1;
        }

        // SD WRITE ADDED CODE STARTS
        rc = f_close(&fil_IMCO);
        rc = f_close(&fil_RDSR);
        rc = f_close(&fil_IMCO_XADC);
			// extra added by ashwin on 20-Sep-2023 starts
			rc = f_close(&fil_volt);
			rc = f_close(&fil_volt_xadc);
			// extra added by ashwin on 20-Sep-2023 ends
        Xil_DCacheFlush();
//		int Status=SD_Eject(&fatfs);
		int pauseval=1;
        // SD WRITE ADDED CODE ENDS

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



//Dummy function for Linear Layer in IMC.
void replicateLINIMCHW(uint8_t**** W_BitMat,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,
                    uint8_t**** X_BitMat,uint16_t b ,uint16_t XDL, uint16_t xb, uint16_t XBL,int** W_Weight,
                     int** X_Weight, float** LIN_Out ){
    for(uint16_t kn = oc;kn < oc+ WDL; kn++){//For set of W_Features
        for(uint16_t n = 0; n < WordLines; n++){//For wordline
            for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
                for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                    for(uint16_t j = xb; j < xb + XBL; j++){// For image bit
                        float weight  = ((i == 8) ^ (j==8))? (-1*pow(2,W_Weight[kn][c]+X_Weight[bt][c]+i+j)) :  pow(2,W_Weight[kn][c]+X_Weight[bt][c]+i+j);
                        LIN_Out[bt][kn] += (W_BitMat[kn][c][n][i] * X_BitMat[bt][c][n][j] * weight);
                    }
                }
            }
        }
    }
}

//Linear function without prealignment.
void normal_LIN(float*** Unfolded_X, uint16_t X_Batch, uint16_t X_W_Chunks, uint16_t WordLines, float*** Unfolded_W,
                uint16_t W_Features, float** LIN_Out ){

    for(uint16_t xb = 0; xb < X_Batch; xb++){ //Per batch
        for(uint16_t wb = 0; wb < W_Features; wb ++){ //Per W Feature
            for(uint16_t xk = 0; xk < X_W_Chunks; xk++){ //Add per K chunks
                for(uint16_t xn = 0; xn < WordLines; xn++){ // Per element in chunk.
                    LIN_Out[xb][wb] +=  (Unfolded_X[xb][xk][xn] * Unfolded_W[wb][xk][xn]);
                }
            }
        }
    }
}

//Seperating individual layers weight matrix from buffer
//Layer ID starts with zero.
void getWeightsForLayer(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, uint16_t* W_Channels, uint16_t* W_Height, uint16_t* W_Width, float**** output){
    //Calculate bytes ofset.
    uint64_t offset_bytes = 0;
    for(uint16_t nl = 0; nl < Layer; nl++){
        if(W_Height[nl]==0 || W_Width[nl] == 0){
            W_Height[nl] = 1; //Reflects everywhere.
            W_Width[nl] = 1;
        }
        offset_bytes += W_Batch[nl] * W_Channels[nl] * W_Height[nl] * W_Width[nl] * 4; //4 bytes = float
    }
    //Get array using required dims and offset
    uint64_t bufIndex=0;
	for(uint16_t i=0; i<W_Batch[Layer]; i++)
	{
		for(uint16_t j=0; j<W_Channels[Layer];j++)
		{
			for(uint16_t k=0; k<W_Height[Layer]; k++)
			{
				for(uint16_t l=0; l<W_Width[Layer]; l++)
				{
					output[i][j][k][l]=bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]);
					bufIndex+=4;
				}
			}
		}
	}
}

//Layer ID starts with zero.
void getWeightsForLinear(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, uint16_t* W_Channels, uint16_t* W_Height, uint16_t* W_Width, float** output){
    //Calculate bytes ofset.
    uint64_t offset_bytes = 0;
    for(uint16_t nl = 0; nl < Layer; nl++){
        if(W_Height[nl]==0 || W_Width[nl] == 0){
            W_Height[nl] = 1; //Reflects everywhere.
            W_Width[nl] = 1;
        }
        offset_bytes += W_Batch[nl] * W_Channels[nl] * W_Height[nl] * W_Width[nl] * 4; //4 bytes = float
    }
    //Get array using required dims and offset
    uint64_t bufIndex=0;
	for(uint16_t i=0; i<W_Batch[Layer]; i++)
	{
		for(uint16_t j=0; j<W_Channels[Layer];j++)
		{
			output[i][j]=bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]);
			bufIndex+=4;
		}
	}
}


//Seperating individual layers bias matrix from buffer.
//Layer ID starts with zero.
void getBiasForLayer(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, float* output){
    //Calculate bytes ofset.
	uint64_t offset_bytes = 0;
    for(uint16_t nl = 0; nl < Layer; nl++){
        offset_bytes += W_Batch[nl] * 4; //4 bytes = float
    }
    //Get array using required dims and offset
    uint64_t bufIndex=0;
	for(uint16_t i=0; i<W_Batch[Layer]; i++)
	{

		output[i]=bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]);
		bufIndex+=4;
	}
}

//Seperating individual layers batch norm matrix from buffer.
//Layer ID starts with zero.
//Output = 4(mu,var,weight,bias)*Kernel for individual layer
void getBNForLayer(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, float** output){
    //Calculate bytes ofset.
	uint64_t offset_bytes = 0;
    for(uint16_t nl = 0; nl < Layer; nl++){
        offset_bytes += W_Batch[nl] * 16; //4 bytes = float and 4 floats = (mu,var,weight,bias)
    }
    uint64_t var_offset = W_Batch[Layer] * 4;
    //Get array using required dims and offset
    uint64_t bufIndex=0;
	for(uint16_t i=0; i<W_Batch[Layer]; i++)
	{

		output[0][i] = bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]); //mu
        output[1][i] = bf16tofloat(Buffer[offset_bytes + bufIndex + var_offset],Buffer[offset_bytes + bufIndex + var_offset + 1],Buffer[offset_bytes + bufIndex + var_offset + 2],Buffer[offset_bytes + bufIndex + var_offset + 3]); //var
        output[2][i] = bf16tofloat(Buffer[offset_bytes + bufIndex + 2*var_offset],Buffer[offset_bytes + bufIndex + 2*var_offset + 1],Buffer[offset_bytes + bufIndex + 2*var_offset + 2],Buffer[offset_bytes + bufIndex + 2*var_offset + 3]); //weight
        output[3][i] = bf16tofloat(Buffer[offset_bytes + bufIndex + 3*var_offset],Buffer[offset_bytes + bufIndex + 3*var_offset + 1],Buffer[offset_bytes + bufIndex + 3*var_offset + 2],Buffer[offset_bytes + bufIndex + 3*var_offset + 3]); //bias
		bufIndex+=4;
	}
}

//Get reference matrix for layer.
void getRefForLayer(char* Buffer, uint32_t offset_bytes,uint16_t chunks,uint16_t na,uint16_t nb, uint16_t wordlines ,float**** Ref,
		float*** RefB,float* RefMax){
	uint64_t bufIndex = 0;
	for(uint16_t c = 0; c < chunks; c++){
		for(uint16_t i = 0; i < 2; i++ ){ // hardcoded to 2 as there are only 2 groups of analog reference DLs on chip
			for(uint16_t n = 0; n < na; n++){
				for(uint16_t w = 0; w < wordlines; w++){
					Ref[c][i][n][w] = bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]);
					bufIndex+=4;
				}
			}
		}
		for(uint16_t n = 0; n < nb; n++){
			for(uint16_t w = 0; w < wordlines; w++){
				RefB[c][n][w] = bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]);
				bufIndex+=4;
			}
		}
		RefMax[c] = bf16tofloat(Buffer[offset_bytes + bufIndex],Buffer[offset_bytes + bufIndex+1],Buffer[offset_bytes + bufIndex+2],Buffer[offset_bytes + bufIndex+3]);
		bufIndex+=4;
	}
}

//Add 4d array elementwise.
//Store it in first array.
void add4DArrayElementwise(float**** a, float**** ba, uint16_t batch, uint16_t channels, uint16_t height, uint16_t width){
    for(uint16_t b = 0; b < batch; b++){
        for(uint16_t c = 0; c < channels; c++){
            for(uint16_t h = 0; h < height; h++){
                for(uint16_t w = 0; w < width; w++){
                    a[b][c][h][w] += ba[b][c][h][w];
                }
            }
        }
    }
}

//Function to multiply by power of 2 after getting results from IMC.
void Multiply2Power_CNV(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,int** W_Weight,
                    int*** X_Weight, uint16_t X_out_w, float**** CNV_Out){

    for(uint16_t kn = oc; kn < oc+ WDL; kn++){//For set of kernels
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                for(uint16_t j = xb; j < xb + XBL; j++){// For image bit
                    float weight  = ((i == 8) ^ (j==8))? (-1*pow(2,W_Weight[kn][c]+X_Weight[bt][s][c]+i+j)) :  pow(2,W_Weight[kn][c]+X_Weight[bt][s][c]+i+j);
                    CNV_Out[bt][kn][(int)floor((float)s/(float)X_out_w)][s%X_out_w] += (IMC_Out[bt - b][kn - oc][j - xb][i - wb] * weight);
                }
            }
        }
    }
}

//Function to multiply by power of 2 after getting results from IMC_LIN.
void Multiply2Power_LIN(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL ,uint16_t xb, uint16_t XBL,int** W_Weight, int** X_Weight,
                    float** LIN_Out){

    for(uint16_t kn = oc;kn < oc+ WDL; kn++){//For set of W_Features
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                for(uint16_t j = xb; j < xb + XBL; j++){// For image bit
                    float weight  = ((i == 8) ^ (j==8))? (-1*pow(2,W_Weight[kn][c]+X_Weight[bt][c]+i+j)) :  pow(2,W_Weight[kn][c]+X_Weight[bt][c]+i+j);
                    LIN_Out[bt][kn] += (IMC_Out[bt - b][kn - oc][j - xb][i - wb] * weight);
                }
            }
        }
    }
}

curr_obj linear_NotOnChip(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, curr_obj currObj){

    //Unfold X_Mat and W_Mat
	HW_LIN = true;
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
        float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
                }
            }
        }

        // UART ADDED CODE STARTS HERE
                u32 totalTransmittedBytes=0;
                u32 transmittedBytes=0;
                XUartPs myUart=initialise_uart();
		// UART ADDED CODE ENDS HERE

//        XTime tStart_outerLoop;
//		XTime tStop_outerLoop;
//		XTime tElapsed_outerLoop;
//		double tElapsed_outerLoop_msec;
//		XTime_GetTime(&tStart_outerLoop);
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<X_W_Chunks; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                    //Store W_BitMat[oc:oc+WDL][c][:][wb:wb+WBL] in SRAM.
                	//Ref[c][0] Ref[c][1]
                	//currObj = store_weights_SRAM_lin(W_BitMat,Ref_Arr, oc ,WDL, c, WordLines, wb, WBL,currObj);
                    for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch // ACTUAL CODE ****************

                	//int b_try=7;// 0 indexed
                	//for(uint16_t b = b_try; b <(b_try+XDL); b += XDL){ //For padded image batch

                        for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
                        	XTime tStart_innerLoop;
							XTime tStop_innerLoop;
							XTime tElapsed_innerLoop;
							double tElapsed_innerLoop_msec;
							XTime_GetTime(&tStart_innerLoop);
                        	//Store X_BitMat[b:b+XDL][c][:][xb:xb+XBL]
                            //IMC_Out has dimensions -> XDL * WDL * XBL * WBL
							//Ref[c][0] Ref[c][1]
                        	//currObj = store_images_SRAM_lin(X_BitMat, b , XDL, c, WordLines, xb, XBL, currObj);

                            //Compute MAC. Get results from Chip.
                            //Combine chip results and multiply by weights(POT)
                        	//currObj=IMC_TIMING(XDL, XBL, WBL, IMC_Out, currObj);
                            //Multiply2Power_LIN(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,xb,XBL,W_Weight,X_Weight,LIN_Out);

                            replicateLINIMCHW(W_BitMat, oc, WDL, c, WordLines, wb, WBL, X_BitMat, b, XDL, xb, XBL, W_Weight, X_Weight, LIN_Out);

                            // UART ADDED CODE STARTS HERE
							uint16_t returnWords[]={oc,c,wb,b,xb};
							int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
							sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
							// UART ADDED CODE ENDS HERE
                            XTime_GetTime(&tStop_innerLoop);
							tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
							tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
							int pauseval=0;
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

// NEW FUNCTIONS FOR Fixed Point ARE BELOW

curr_obj convolve_NotOnChip_FP(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,curr_obj currObj){
    // Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.
	HW_CNV = true;
    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_BitMat_Bits = ceil((float)WMAX_Data[1]/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }

        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector_FP(Unfolded_W[b][c][k],WordLines,WMAX_Data[0],WMAX_Data[2],WMAX_Data[1],W_BitMat[b][k]);
                }
            }

        }

        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_BitMat_Bits = ceil((float)XMAX_Data[1]/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
        X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
        for(uint16_t c = 0; c < X_outDepth; c++){
                X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
            }
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector_FP(Unfolded_X[b][c][k],WordLines,XMAX_Data[0],XMAX_Data[2],XMAX_Data[1],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }
        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);

        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
        float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
        for(uint16_t xd = 0; xd < XDL; xd++){
            IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
            for(uint16_t wd = 0; wd < WDL; wd++){
                IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
                for(uint16_t xb = 0; xb < XBL; xb ++){
                    IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
                }
            }
        }

        //Run the IMC part
        XTime tStart_writeSram_FAST;
	    XTime tStop_writeSram_FAST;
	    XTime tElapsed_writeSram_FAST;
	    double tElapsed_writeSram_FAST_sec;
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                    //Store W_BitMat[oc:oc+WDL][c][:][wb:wb+WBL] in SRAM.
                	//Ref[c][0] Ref[c][1]
                    //currObj=store_weights_SRAM(W_BitMat, oc ,WDL, c, WordLines, wb, WBL, currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                            for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
                            	XTime tStart_innerLoop;
								XTime tStop_innerLoop;
								XTime tElapsed_innerLoop;
								double tElapsed_innerLoop_msec;
								XTime_GetTime(&tStart_innerLoop);

                            	// Store X_BitMat[b:b+XDL][s][c][:][xb:xb+XBL]
                                // IMC_Out has dim -> XDL * WDL * XBL * WBL.
                                //memset(IMC_Out,0,XDL*WDL*XBL*WBL*sizeof(uint16_t));

                            	//currObj=store_images_SRAM(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, currObj); // 650 ms
//                            	XTime_GetTime(&tStop_innerLoop);
//								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//								int pauseval1=0;
                                //Compute MAC. Get results from Chip.
                                //Combine chip results and multiply by weights(POT)
                                //replicateIMCHW(W_BitMat, oc, WDL, c, WordLines, wb, WBL, X_BitMat, b, XDL, s, xb, XBL, W_Weight, X_Weight,X_out_w, CNV_Out);
                            	//currObj=IMC_TIMING(XDL, XBL, WBL, IMC_Out, currObj);
								ReplicateIMCTiming(W_BitMat,oc ,WDL, c, wb, WBL,X_BitMat,b ,XDL, s,  xb,  XBL,  WordLines, IMC_Out);
                            	Multiply2Power_CNV_FP(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,X_out_w,XMAX_Data[2], WMAX_Data[2],
                            			XMAX_Data[0], WMAX_Data[0],XMAX_Data[1],WMAX_Data[1],CNV_Out);

                            	XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
								int pauseval=0;
                            }
                        }
                    }
                    XTime_GetTime(&tStop_writeSram_FAST);
                    tElapsed_writeSram_FAST=tStop_writeSram_FAST-tStart_writeSram_FAST;
                    tElapsed_writeSram_FAST_sec=1000*((double)tElapsed_writeSram_FAST)/((double)COUNTS_PER_SECOND);
                }
            }
        }
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion
    return currObj;
}


curr_obj convolve_FP(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj){
    // Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.
	HW_CNV = true;
    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_BitMat_Bits = ceil((float)WMAX_Data[1]/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }

        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector_FP(Unfolded_W[b][c][k],WordLines,WMAX_Data[0],WMAX_Data[2],WMAX_Data[1],W_BitMat[b][k]);
                }
            }

        }

        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_BitMat_Bits = ceil((float)XMAX_Data[1]/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
        X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
        for(uint16_t c = 0; c < X_outDepth; c++){
                X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
            }
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector_FP(Unfolded_X[b][c][k],WordLines,XMAX_Data[0],XMAX_Data[2],XMAX_Data[1],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }
        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);

        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
		float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
				for(uint16_t xb = 0; xb < XBL; xb ++){
					IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}

		// UART ADDED CODE STARTS HERE
		u32 totalTransmittedBytes=0;
		u32 transmittedBytes=0;
		XUartPs myUart=initialise_uart();

		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
		{
			u8 syncdata=0;
			// UART ADDED CODE STARTS HERE
			uint16_t returnWords[]={innerloop_start_word};//0 is stride
			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
			sendWordData( returnWords, numWords, myUart ) ;
			// UART ADDED CODE ENDS HERE
			// WAITING FOR SYNC DATA FROM MATLAB
			do
			{
				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
			}while(syncdata!=syncdata_val);
		}
		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

        //Run the IMC part
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                	currObj = store_weights_SRAM(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                            for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
//                            	// TIME MEASUREMENT CODE STARTS HERE
                            	XTime tStart_innerLoop;
								XTime tStop_innerLoop;
								XTime tElapsed_innerLoop;
								double tElapsed_innerLoop_msec_store_images_SRAM;
								double tElapsed_innerLoop_msec_goldenGate;
								double tElapsed_innerLoop_msec_padBitCorrection;
								double tElapsed_innerLoop_msec_Multiply2Power_CNV_FP;
								double tElapsed_innerLoop_msec_UART;
//								// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	currObj=store_images_SRAM(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, currObj);
//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_store_images_SRAM=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	currObj=goldenGate(IMC_Out,mode,slopeSetting,RefMax,c,currObj);
//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_goldenGate=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	// CORRECTION FOR THE PADDED BITS STARTS
								for(uint16_t _xd = 0; _xd < XDL; _xd++){
									for(uint16_t _wd = 0; _wd < WDL; _wd++){
										for(uint16_t _xb = 0; _xb < XBL; _xb++){
											for(uint16_t _wb = 0; _wb < WBL; _wb++){
												if( ( ( wb == 8 ) && _wb > 0 ) ||
														b >= X_Batch           ||
													( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Batch%WDL ) ) ) ){
													// 0s were padded on MSB for weights to make #weight bits a xle of WBL
													// 0s images were created to #batches a xle of XDL
													// 0 neurons were created to make #neurons a xle of WDL
													IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
												}
											}
										}
									}
								}
								// CORRECTION FOR THE PADDED BITS ENDS

//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_padBitCorrection=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE

                            	Multiply2Power_CNV_FP(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,X_out_w,XMAX_Data[2], WMAX_Data[2],
                            			XMAX_Data[0], WMAX_Data[0],XMAX_Data[1],WMAX_Data[1],CNV_Out);

//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_Multiply2Power_CNV_FP=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	// UART ADDED CODE STARTS HERE
								uint16_t returnWords[]={oc,c,wb,s,b,xb};
								int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
								sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
								// UART ADDED CODE ENDS HERE
//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_UART=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

								int pauseval=1;
                            }
                        }
                    }
                }
            }
        }
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion
    return currObj;
}


curr_obj convolve_FP_XADC(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj){
	// SD WRITE ADDED CODE STARTS HERE
			FRESULT rc;
	// SD WRITE ADDED CODE ENDS HERE
	// Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.
	HW_CNV = true;
    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_Bits=WMAX_Data[1];
        float W_Max=WMAX_Data[0];
		float W_Signed=WMAX_Data[2];
        uint16_t W_BitMat_Bits = ceil((float)WMAX_Data[1]/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }

        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector_FP(Unfolded_W[b][c][k],WordLines,WMAX_Data[0],WMAX_Data[2],WMAX_Data[1],W_BitMat[b][k]);
                }
            }

        }
        int W_Weight_see[W_BitMat_Batch][W_outHeight];
		for(uint16_t b = 0; b<W_BitMat_Batch;b++){
				for(uint16_t k =0; k< W_outHeight; k++){
					W_Weight_see[b][k]=W_Weight[b][k];
			}
		}
		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Wexp;
			UINT br_Wexp;
			int size_Wexp=W_BitMat_Batch*W_outHeight*sizeof(W_Weight_see[0][0]);
			char FileName_Wexp[]="WEXC.BIN";
			rc = f_open(&fil_Wexp, FileName_Wexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Wexp, W_Weight_see, size_Wexp, &br_Wexp);
			rc = f_close(&fil_Wexp);
		// SD WRITE ADDED CODE ENDS HERE
        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_Bits=XMAX_Data[1];
		float X_Max=XMAX_Data[0];
		float X_Signed=XMAX_Data[2];
        uint16_t X_BitMat_Bits = ceil((float)XMAX_Data[1]/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
        X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
        for(uint16_t c = 0; c < X_outDepth; c++){
                X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
            }
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector_FP(Unfolded_X[b][c][k],WordLines,XMAX_Data[0],XMAX_Data[2],XMAX_Data[1],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }


        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);
        int X_Weight_see[X_BitMat_Batch][X_outDepth][X_outHeight];
		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			for(uint16_t c =0; c< X_outDepth; c++){
				for(uint16_t k =0; k< X_outHeight; k++){
					X_Weight_see[b][c][k]=X_Weight[b][c][k];
				}
			}
		}
		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Xexp;
			UINT br_Xexp;
			int size_Xexp=X_BitMat_Batch*X_outDepth*X_outHeight*sizeof(X_Weight_see[0][0][0]);
			char FileName_Xexp[]="XEXC.BIN";
			rc = f_open(&fil_Xexp, FileName_Xexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Xexp, X_Weight_see, size_Xexp, &br_Xexp);
			rc = f_close(&fil_Xexp);
		// SD WRITE ADDED CODE ENDS HERE
        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
		float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out[xd][wd] = (float**)calloc(XBL,sizeof(float*));
				for(uint16_t xb = 0; xb < XBL; xb ++){
					IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}
		float IMC_Out_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];

		float ****IMC_Out_XADC = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out_XADC[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out_XADC[xd][wd] = (float**)calloc(XBL,sizeof(float*));
				for(uint16_t xb = 0; xb < XBL; xb ++){
					IMC_Out_XADC[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}
		float IMC_Out_XADC_temp[XDL_HW][WDL_HW][XBL_HW][WBL_HW];

		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_IMCC;
			FIL fil_RDSC;
			FIL fil_IMCC_XADC;
			UINT br_IMCC;
			UINT br_RDSC;
			UINT br_IMCC_XADC;
			u32 file_size;
			int size=XDL*WDL*XBL*WBL*4;
			char FileName_IMCC[]="IMCC.bin";
			char FileName_RDSC[]="RDSC.bin";
			char FileName_IMCC_XADC[]="IC2X.bin";
			rc = f_open(&fil_IMCC, FileName_IMCC, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_RDSC, FileName_RDSC, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_IMCC_XADC, FileName_IMCC_XADC, FA_WRITE | FA_CREATE_ALWAYS);
				// extra added by ashwin on 20-Sep-2023 starts
				FIL fil_volt;
				FIL fil_volt_xadc;

				UINT br_volt;
				UINT br_volt_xadc;

				int size_volt=XDL*WDL*XBL*WBL*4;
				int size_volt_xadc=XDL*WDL*XBL*WBL*4;

				char FileName_volt[]="V.bin";
				char FileName_volt_xadc[]="VX.bin";

				rc = f_open(&fil_volt, FileName_volt, FA_WRITE | FA_CREATE_ALWAYS);
				rc = f_open(&fil_volt_xadc, FileName_volt_xadc, FA_WRITE | FA_CREATE_ALWAYS);
				// extra added by ashwin on 20-Sep-2023 ends
		// SD WRITE ADDED CODE ENDS HERE
		// UART ADDED CODE STARTS HERE
		u32 totalTransmittedBytes=0;
		u32 transmittedBytes=0;
		XUartPs myUart=initialise_uart();

		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
		{
			u8 syncdata=0;
			// UART ADDED CODE STARTS HERE
			uint16_t returnWords[]={innerloop_start_word};//0 is stride
			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
			sendWordData( returnWords, numWords, myUart ) ;
			// UART ADDED CODE ENDS HERE
			// WAITING FOR SYNC DATA FROM MATLAB
			do
			{
				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
			}while(syncdata!=syncdata_val);
		}
		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

		{
		// SD WRITE ADDED CODE STARTS HERE
		// LAYER PARAMETERS WRITEOUT
		char FileName_LYPM[]="LYPM.bin";
		FIL fil_LYPM;
		UINT br_LYPM;
		rc = f_open(&fil_LYPM, FileName_LYPM, FA_WRITE | FA_CREATE_ALWAYS);

		float LYPM[]={X_outDepth,W_BitMat_Batch,W_Batch,X_Batch,W_Bits,W_BitMat_Bits,WordLines,W_outHeight,W_outWidth,X_BitMat_Batch,X_outHeight,X_Bits,W_Max,X_Max,X_Signed,W_Signed};
		int size_LYPM_bytes = sizeof( LYPM ) ;
		rc = f_write(&fil_LYPM, LYPM, size_LYPM_bytes, &br_LYPM);
		rc = f_close(&fil_LYPM);
		// SD WRITE ADDED CODE ENDS HERE
		}
        //Run the IMC part
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                	currObj = store_weights_SRAM(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                            for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits

                            	currObj=store_images_SRAM(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, currObj);
                            	// SD WRITE ADDED CODE STARTS HERE
								int numDLs=97;
								int numWWLs=1092;
								int numBLs=4;
								u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
								for(uint16_t wd = 0; wd < numDLs; wd++){
									readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
									for(uint16_t wl = 0; wl < numWWLs; wl++){
										readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
									}
								}
								currObj=readSRAMAll(currObj, readSRAMTotal);

								u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));

								for(uint16_t wb = 0; wb < numBLs; wb++){
									for(uint16_t wl = 0; wl < numWWLs; wl++){
										for(uint16_t wd = 0; wd < numDLs; wd++){
											readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
										}
									}
								}

								u32 size_RDSC=numDLs*numWWLs*numBLs*sizeof(u8);
								rc = f_write(&fil_RDSC, readSRAMTotal_FLAT, size_RDSC, &br_RDSC);
								// SD WRITE ADDED CODE ENDS HERE
								currObj=goldenGate_XADC(IMC_Out_XADC,IMC_Out,mode,slopeSetting,RefMax,c,currObj);
                            	// CORRECTION FOR THE PADDED BITS STARTS
								for(uint16_t _xd = 0; _xd < XDL; _xd++){
									for(uint16_t _wd = 0; _wd < WDL; _wd++){
										for(uint16_t _xb = 0; _xb < XBL; _xb++){
											for(uint16_t _wb = 0; _wb < WBL; _wb++){
												if( ( ( wb == 8 ) && _wb > 0 ) ||
														b >= X_Batch           ||
													( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Batch%WDL ) ) ) ){
													// 0s were padded on MSB for weights to make #weight bits a xle of WBL
													// 0s images were created to #batches a xle of XDL
													// 0 neurons were created to make #neurons a xle of WDL
													IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
													IMC_Out_XADC[_xd][_wd][_xb][_wb] = 0 ;
												}
											}
										}
									}
								}
								// CORRECTION FOR THE PADDED BITS ENDS
								// SD WRITE ADDED CODE STARTS
								for(uint16_t _xd = 0; _xd < XDL; _xd++){
									for(uint16_t _wd = 0; _wd < WDL; _wd++){
										for(uint16_t _xb = 0; _xb < XBL; _xb++){
											for(uint16_t _wb = 0; _wb < WBL; _wb++){
												IMC_Out_temp[_xd][_wd][_xb][_wb]=IMC_Out[_xd][_wd][_xb][_wb];
												IMC_Out_XADC_temp[_xd][_wd][_xb][_wb]=IMC_Out_XADC[_xd][_wd][_xb][_wb];
											}
										}
									}
								}
	////
	////                            int size=XDL*WDL*XBL*WBL*4;
	////                    		char FileName_out[]="IMCO.bin";
	////                    		int rc=WriteFile_custom(FileName_out, size, IMC_Out_temp);
	////                    		int Status=SD_Eject(&fatfs);
	////							int pauseval=1;
								rc = f_write(&fil_IMCC, IMC_Out_temp, size, &br_IMCC);
								rc = f_write(&fil_IMCC_XADC, IMC_Out_XADC_temp, size, &br_IMCC_XADC);
									// extra added by ashwin on 20-Sep-2023 starts
									rc = f_write(&fil_volt, voltage_see, size_volt, &br_volt);
									rc = f_write(&fil_volt_xadc, voltage_xadc_see, size_volt_xadc, &br_volt_xadc);
									// extra added by ashwin on 20-Sep-2023 ends
								// SD WRITE ADDED CODE ENDS
                            	Multiply2Power_CNV_FP(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,X_out_w,XMAX_Data[2], WMAX_Data[2],
                            			XMAX_Data[0], WMAX_Data[0],XMAX_Data[1],WMAX_Data[1],CNV_Out);
                            	// UART ADDED CODE STARTS HERE
								uint16_t returnWords[]={oc,c,wb,s,b,xb};
								int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
								sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
								// UART ADDED CODE ENDS HERE
								// FREEING MEMORY STARTS HERE
								for(uint16_t _wd = 0; _wd < numDLs; _wd++){
									for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
										free(readSRAMTotal[_wd][_wl]);
									}
									free(readSRAMTotal[_wd]);
								}

								free(readSRAMTotal_FLAT);
								// FREEING MEMORY END HERE
                            }
                        }
                    }
                }
            }
        }
        // SD WRITE ADDED CODE STARTS
		rc = f_close(&fil_IMCC);
		rc = f_close(&fil_RDSC);
		rc = f_close(&fil_IMCC_XADC);
			// extra added by ashwin on 20-Sep-2023 starts
			rc = f_close(&fil_volt);
			rc = f_close(&fil_volt_xadc);
			// extra added by ashwin on 20-Sep-2023 ends
		Xil_DCacheFlush();
//		int Status=SD_Eject(&fatfs);
		int pauseval=1;
		// SD WRITE ADDED CODE ENDS
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion
    return currObj;
}


curr_obj convolve_FP_2bit(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,u8 mode_xbit,curr_obj currObj){
    // Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.
	HW_CNV = true;
    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_BitMat_Bits = ceil((float)WMAX_Data[1]/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }

        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector_FP(Unfolded_W[b][c][k],WordLines,WMAX_Data[0],WMAX_Data[2],WMAX_Data[1],W_BitMat[b][k]);
                }
            }

        }

        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_BitMat_Bits = ceil((float)XMAX_Data[1]/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
        X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
        for(uint16_t c = 0; c < X_outDepth; c++){
                X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
            }
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector_FP(Unfolded_X[b][c][k],WordLines,XMAX_Data[0],XMAX_Data[2],XMAX_Data[1],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }
        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);

        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
		float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out[xd][wd] = (float**)calloc(1,sizeof(float*));
				for(uint16_t xb = 0; xb < 1; xb ++){
					IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}

		// UART ADDED CODE STARTS HERE
		u32 totalTransmittedBytes=0;
		u32 transmittedBytes=0;
		XUartPs myUart=initialise_uart();

		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
		{
			u8 syncdata=0;
			// UART ADDED CODE STARTS HERE
			uint16_t returnWords[]={innerloop_start_word};//0 is stride
			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
			sendWordData( returnWords, numWords, myUart ) ;
			// UART ADDED CODE ENDS HERE
			// WAITING FOR SYNC DATA FROM MATLAB
			do
			{
				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
			}while(syncdata!=syncdata_val);
		}
		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

        //Run the IMC part
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                	currObj = store_weights_SRAM(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                            for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits
//                            	// TIME MEASUREMENT CODE STARTS HERE
                            	XTime tStart_innerLoop;
								XTime tStop_innerLoop;
								XTime tElapsed_innerLoop;
								double tElapsed_innerLoop_msec_store_images_SRAM;
								double tElapsed_innerLoop_msec_goldenGate;
								double tElapsed_innerLoop_msec_padBitCorrection;
								double tElapsed_innerLoop_msec_Multiply2Power_CNV_FP;
								double tElapsed_innerLoop_msec_UART;
//								// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	currObj=store_images_SRAM_2bit(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, mode_xbit, currObj);
//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_store_images_SRAM=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	currObj=goldenGate_2bitX(IMC_Out,mode,slopeSetting,RefMax,c,currObj);
//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_goldenGate=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	// CORRECTION FOR THE PADDED BITS STARTS
								for(uint16_t _xd = 0; _xd < XDL; _xd++){
									for(uint16_t _wd = 0; _wd < WDL; _wd++){
										for(uint16_t _xb = 0; _xb < 1; _xb++){
											for(uint16_t _wb = 0; _wb < WBL; _wb++){
												if( ( ( wb == 8 ) && _wb > 0 ) ||
														b >= X_Batch           ||
													( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Batch%WDL ) ) ) ){
													// 0s were padded on MSB for weights to make #weight bits a xle of WBL
													// 0s images were created to #batches a xle of XDL
													// 0 neurons were created to make #neurons a xle of WDL
													IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
												}
											}
										}
									}
								}
								// CORRECTION FOR THE PADDED BITS ENDS

//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_padBitCorrection=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE

								Multiply2Power_CNV_FP_2bitX(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,X_out_w,XMAX_Data[2], WMAX_Data[2],
                            			XMAX_Data[0], WMAX_Data[0],XMAX_Data[1],WMAX_Data[1],CNV_Out);

//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_Multiply2Power_CNV_FP=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

//                            	// TIME MEASUREMENT CODE STARTS HERE
								XTime_GetTime(&tStart_innerLoop);
//								// TIME MEASUREMENT CODE ENDS HERE
                            	// UART ADDED CODE STARTS HERE
								uint16_t returnWords[]={oc,c,wb,s,b,xb};
								int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
								sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
								// UART ADDED CODE ENDS HERE
//							// TIME MEASUREMENT CODE STARTS HERE
                        		XTime_GetTime(&tStop_innerLoop);
								tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
								tElapsed_innerLoop_msec_UART=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//							// TIME MEASUREMENT CODE ENDS HERE

								int pauseval=1;
                            }
                        }
                    }
                }
            }
        }
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion
    return currObj;
}


curr_obj convolve_FP_2bit_XADC(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,u8 mode_xbit,curr_obj currObj){
	// SD WRITE ADDED CODE STARTS HERE
			FRESULT rc;
	// SD WRITE ADDED CODE ENDS HERE
	// Unfold X and W
    #pragma region
    //Unfolding X-Mat required arrays.
	HW_CNV = true;
    uint16_t X_paddedHeight = X_Height + 2 * X_Padding;
    uint16_t X_paddedWidth = X_Width + 2 * X_Padding;

    uint16_t X_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t X_out_h = ((X_paddedHeight - W_Height) / X_Stride + 1 );
    uint16_t X_out_w = ((X_paddedWidth - W_Width) / X_Stride + 1 );
    uint16_t X_outDepth = X_out_h * X_out_w; //Total number of strides
    uint16_t X_outHeight = ceil((float)X_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    X_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_X array
    float**** Unfolded_X  = (float****)calloc(X_Batch , sizeof(float***));
    for(uint16_t b =0; b<X_Batch ;b++){
        Unfolded_X[b]  = (float***)calloc(X_outDepth , sizeof(float**));
        for(uint16_t n=0; n<X_outDepth; n++){
            Unfolded_X[b][n]  = (float**)calloc(X_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < X_outHeight; k++) {
                Unfolded_X[b][n][k] = (float*)calloc(X_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the X_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(X_Mat, X_Batch, X_Channels, X_Height, X_Width,
            W_Height, W_Width, X_Stride, X_Padding, WordLines, Unfolded_X);


    //Unfolding W-Mat required arrays.


    uint16_t W_outWidth = W_Height * W_Width * W_Channels; // Make sure that W_Channels= I_Channels
    uint16_t W_outDepth = 1; //Total number of strides This shall be 1
    uint16_t W_outHeight = ceil((float)W_outWidth / (float)WordLines); // Number of WordLines sized chunks required

    W_outWidth = WordLines; //new outHeight after making chunks.

    // Allocate memory for the Unfolded_W array
    float**** Unfolded_W  = (float****)calloc(W_Batch , sizeof(float***));
    for(uint16_t b =0; b<W_Batch ;b++){
        Unfolded_W[b]  = (float***)calloc(W_outDepth , sizeof(float**));
        for(uint16_t n=0; n<W_outDepth; n++){
            Unfolded_W[b][n]  = (float**)calloc(W_outHeight , sizeof(float*));
            for (uint16_t k = 0; k < W_outHeight; k++) {
                Unfolded_W[b][n][k] = (float*)calloc(W_outWidth , sizeof(float));
            }
        }
    }

    //Unfold the W_Mat. It assumes W_Channels = X_Channels.
    rearrange4DImage(W_Mat, W_Batch, W_Channels, W_Height, W_Width,
            W_Height, W_Width, W_Stride, W_Padding, WordLines, Unfolded_W);

    #pragma endregion
    if(HW_CNV){
        //Get W_Unfolded matrix in form of prealigned Kernels * K(Chunks) * WordLines * 9 bits //Contains only one stride
        //Get X_Unfolded matrix in form of prealigned Batch * Stride * K(Chunks) * WordLines * 9 bits.
        //Also add padding if required.
        #pragma region
        //Create matrix to store W_BitMat.
        //Get padded dimensions.
        uint16_t W_BitMat_Batch = ceil((float)W_Batch/(float)WDL) * WDL; //Extra padded zeros
        uint16_t W_Bits=WMAX_Data[1];
        float W_Max=WMAX_Data[0];
		float W_Signed=WMAX_Data[2];
        uint16_t W_BitMat_Bits = ceil((float)WMAX_Data[1]/(float)WBL) * WBL; //Extra padded zeros

        uint8_t**** W_BitMat = (uint8_t****)calloc(W_BitMat_Batch,sizeof(uint8_t***));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_BitMat[b] = (uint8_t***)calloc(W_outHeight,sizeof(uint8_t**));
            for(uint16_t k =0; k< W_outHeight; k++){
                W_BitMat[b][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                for(uint16_t n=0; n<WordLines; n++){
                    W_BitMat[b][k][n] = (uint8_t*)calloc(W_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                }
            }
        }

        //Create W_Weight matrix to store weights Kernel * Chunks(K)
        int** W_Weight = (int**)calloc(W_BitMat_Batch,sizeof(int*));
        for(uint16_t b = 0; b<W_BitMat_Batch;b++){
            W_Weight[b] = (int*)calloc(W_outHeight,sizeof(int));
        }

        //Extract the prealigned W_BitMat and W_Weight using W_Unfolded
        for(uint16_t b = 0; b < W_Batch; b++){
            for(uint16_t c = 0; c < W_outDepth; c++){ //this shall run only once
                for(uint16_t k = 0; k < W_outHeight; k++){
                    preAlignVector_FP(Unfolded_W[b][c][k],WordLines,WMAX_Data[0],WMAX_Data[2],WMAX_Data[1],W_BitMat[b][k]);
                }
            }

        }
        int W_Weight_see[W_BitMat_Batch][W_outHeight];
		for(uint16_t b = 0; b<W_BitMat_Batch;b++){
				for(uint16_t k =0; k< W_outHeight; k++){
					W_Weight_see[b][k]=W_Weight[b][k];
			}
		}
		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Wexp;
			UINT br_Wexp;
			int size_Wexp=W_BitMat_Batch*W_outHeight*sizeof(W_Weight_see[0][0]);
			char FileName_Wexp[]="WEXC.BIN";
			rc = f_open(&fil_Wexp, FileName_Wexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Wexp, W_Weight_see, size_Wexp, &br_Wexp);
			rc = f_close(&fil_Wexp);
		// SD WRITE ADDED CODE ENDS HERE
        //Create matrix to store X_BitMat.
        //Get padded dimensions.
        uint16_t X_BitMat_Batch = ceil((float)X_Batch/(float)XDL) * XDL; //Extra padded zeros
        uint16_t X_Bits=XMAX_Data[1];
		float X_Max=XMAX_Data[0];
		float X_Signed=XMAX_Data[2];
        uint16_t X_BitMat_Bits = ceil((float)XMAX_Data[1]/(float)XBL) * XBL; //Extra padded zeros

        uint8_t***** X_BitMat = (uint8_t*****)calloc(X_BitMat_Batch,sizeof(uint8_t****));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            X_BitMat[b] = (uint8_t****)calloc(X_outDepth,sizeof(uint8_t***));
            for(uint16_t d = 0; d < X_outDepth; d++){
                X_BitMat[b][d] = (uint8_t***)calloc(X_outHeight,sizeof(uint8_t**));
                for(uint16_t k =0; k< X_outHeight; k++){
                    X_BitMat[b][d][k] = (uint8_t**)calloc(WordLines,sizeof(uint8_t*));
                    for(uint16_t n=0; n<WordLines; n++){
                        X_BitMat[b][d][k][n] = (uint8_t*)calloc(X_BitMat_Bits,sizeof(uint8_t)); //Assuming 9 bit mantissa + padding
                    }
                }
            }
        }

        //Create X_Weight matrix to store weights Batch * Strides * Chunks(K)
        int*** X_Weight = (int***)calloc(X_BitMat_Batch,sizeof(int**));
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
        X_Weight[b] = (int**)calloc(X_outDepth,sizeof(int*));
        for(uint16_t c = 0; c < X_outDepth; c++){
                X_Weight[b][c] = (int*)calloc(X_outHeight,sizeof(int));
            }
        }
        XTime tStart;
        XTime_GetTime(&tStart);
        xil_printf("HERE");
        //Extract the prealigned X_BitMat and X_Weight using X_Unfolded
        for(uint16_t b = 0; b < X_Batch; b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    preAlignVector_FP(Unfolded_X[b][c][k],WordLines,XMAX_Data[0],XMAX_Data[2],XMAX_Data[1],X_BitMat[b][c][k]);
                }
                xil_printf("ITER: %d",c);
            }

        }


        XTime tEnd;
        XTime_GetTime(&tEnd);
        double elapsedTime = (tEnd - tStart)/(COUNTS_PER_SECOND);
        int X_Weight_see[X_BitMat_Batch][X_outDepth][X_outHeight];
		for(uint16_t b = 0; b<X_BitMat_Batch;b++){
			for(uint16_t c =0; c< X_outDepth; c++){
				for(uint16_t k =0; k< X_outHeight; k++){
					X_Weight_see[b][c][k]=X_Weight[b][c][k];
				}
			}
		}
		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_Xexp;
			UINT br_Xexp;
			int size_Xexp=X_BitMat_Batch*X_outDepth*X_outHeight*sizeof(X_Weight_see[0][0][0]);
			char FileName_Xexp[]="XEXC.BIN";
			rc = f_open(&fil_Xexp, FileName_Xexp, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_write(&fil_Xexp, X_Weight_see, size_Xexp, &br_Xexp);
			rc = f_close(&fil_Xexp);
		// SD WRITE ADDED CODE ENDS HERE
        #pragma endregion

        //IMC part here
        #pragma region
        //Create array to store IMC results.
		float ****IMC_Out = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out[xd][wd] = (float**)calloc(1,sizeof(float*));
				for(uint16_t xb = 0; xb < 1; xb ++){
					IMC_Out[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}
		float IMC_Out_temp[XDL_HW][WDL_HW][1][WBL_HW];

		float ****IMC_Out_XADC = (float****)calloc(XDL,sizeof(float***));
		for(uint16_t xd = 0; xd < XDL; xd++){
			IMC_Out_XADC[xd] = (float***)calloc(WDL,sizeof(float**));
			for(uint16_t wd = 0; wd < WDL; wd++){
				IMC_Out_XADC[xd][wd] = (float**)calloc(1,sizeof(float*));
				for(uint16_t xb = 0; xb < 1; xb ++){
					IMC_Out_XADC[xd][wd][xb] = (float*)calloc(WBL,sizeof(float));
				}
			}
		}
		float IMC_Out_XADC_temp[XDL_HW][WDL_HW][1][WBL_HW];

		// SD WRITE ADDED CODE STARTS HERE
			FIL fil_IMCC;
			FIL fil_RDSC;
			FIL fil_IMCC_XADC;
			UINT br_IMCC;
			UINT br_RDSC;
			UINT br_IMCC_XADC;
			u32 file_size;
			int size=XDL*WDL*1*WBL*4;
			char FileName_IMCC[]="IMCC.bin";
			char FileName_RDSC[]="RDSC.bin";
			char FileName_IMCC_XADC[]="IC2X.bin";
			rc = f_open(&fil_IMCC, FileName_IMCC, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_RDSC, FileName_RDSC, FA_WRITE | FA_CREATE_ALWAYS);
			rc = f_open(&fil_IMCC_XADC, FileName_IMCC_XADC, FA_WRITE | FA_CREATE_ALWAYS);
				// extra added by ashwin on 20-Sep-2023 starts
				FIL fil_volt;
				FIL fil_volt_xadc;

				UINT br_volt;
				UINT br_volt_xadc;

				int size_volt=XDL*WDL*1*WBL*4;
				int size_volt_xadc=XDL*WDL*1*WBL*4;

				char FileName_volt[]="V.bin";
				char FileName_volt_xadc[]="VX.bin";

				rc = f_open(&fil_volt, FileName_volt, FA_WRITE | FA_CREATE_ALWAYS);
				rc = f_open(&fil_volt_xadc, FileName_volt_xadc, FA_WRITE | FA_CREATE_ALWAYS);
				// extra added by ashwin on 20-Sep-2023 ends
		// SD WRITE ADDED CODE ENDS HERE
		// UART ADDED CODE STARTS HERE
		u32 totalTransmittedBytes=0;
		u32 transmittedBytes=0;
		XUartPs myUart=initialise_uart();

		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (STARTS)
		{
			u8 syncdata=0;
			// UART ADDED CODE STARTS HERE
			uint16_t returnWords[]={innerloop_start_word};//0 is stride
			int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
			sendWordData( returnWords, numWords, myUart ) ;
			// UART ADDED CODE ENDS HERE
			// WAITING FOR SYNC DATA FROM MATLAB
			do
			{
				u32 receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
			}while(syncdata!=syncdata_val);
		}
		// TO INDICATE TO MATLAB THAT THE CODE HAS ARRIVED AT THE INNERLOOP (ENDS)

		{
		// SD WRITE ADDED CODE STARTS HERE
		// LAYER PARAMETERS WRITEOUT
		char FileName_LYPM[]="LYPM.bin";
		FIL fil_LYPM;
		UINT br_LYPM;
		rc = f_open(&fil_LYPM, FileName_LYPM, FA_WRITE | FA_CREATE_ALWAYS);

		float LYPM[]={X_outDepth,W_BitMat_Batch,W_Batch,X_Batch,W_Bits,W_BitMat_Bits,WordLines,W_outHeight,W_outWidth,X_BitMat_Batch,X_outHeight,X_Bits,W_Max,X_Max,X_Signed,W_Signed};
		int size_LYPM_bytes = sizeof( LYPM ) ;
		rc = f_write(&fil_LYPM, LYPM, size_LYPM_bytes, &br_LYPM);
		rc = f_close(&fil_LYPM);
		// SD WRITE ADDED CODE ENDS HERE
		}
        //Run the IMC part
        for(uint16_t oc = 0; oc< W_BitMat_Batch; oc += WDL){ //For padded kernals
            for(uint16_t c = 0; c<W_outHeight; c++){//For chunks
                for(uint16_t wb = 0; wb<W_BitMat_Bits; wb += WBL){ //For padded weight bits
                	currObj = store_weights_SRAM(W_BitMat,Ref,RefB,oc,WDL,c,WordLines,wb,WBL,currObj);
                    for(uint16_t s = 0; s < X_outDepth; s++){ //For strides of image
                        for(uint16_t b = 0; b <X_BitMat_Batch; b += XDL){ //For padded image batch
                            for(uint16_t xb = 0; xb < X_BitMat_Bits; xb+=XBL){ //For padded image bits

                            	currObj=store_images_SRAM_2bit(X_BitMat, b , XDL, s, c, WordLines, xb, XBL, mode_xbit, currObj);
                            	// SD WRITE ADDED CODE STARTS HERE
								int numDLs=97;
								int numWWLs=1092;
								int numBLs=4;
								u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
								for(uint16_t wd = 0; wd < numDLs; wd++){
									readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
									for(uint16_t wl = 0; wl < numWWLs; wl++){
										readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
									}
								}
								currObj=readSRAMAll(currObj, readSRAMTotal);

								u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));

								for(uint16_t wb = 0; wb < numBLs; wb++){
									for(uint16_t wl = 0; wl < numWWLs; wl++){
										for(uint16_t wd = 0; wd < numDLs; wd++){
											readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
										}
									}
								}

								u32 size_RDSC=numDLs*numWWLs*numBLs*sizeof(u8);
								rc = f_write(&fil_RDSC, readSRAMTotal_FLAT, size_RDSC, &br_RDSC);
								// SD WRITE ADDED CODE ENDS HERE
								//currObj=goldenGate_2bitX_XADC(IMC_Out_XADC,IMC_Out,mode,slopeSetting,RefMax,c,currObj);
                            	// CORRECTION FOR THE PADDED BITS STARTS
								for(uint16_t _xd = 0; _xd < XDL; _xd++){
									for(uint16_t _wd = 0; _wd < WDL; _wd++){
										for(uint16_t _xb = 0; _xb < 1; _xb++){
											for(uint16_t _wb = 0; _wb < WBL; _wb++){
												if( ( ( wb == 8 ) && _wb > 0 ) ||
														b >= X_Batch           ||
													( ( ( oc == ( W_BitMat_Batch - 1 ) ) ) && ( _wd >= ( W_Batch%WDL ) ) ) ){
													// 0s were padded on MSB for weights to make #weight bits a xle of WBL
													// 0s images were created to #batches a xle of XDL
													// 0 neurons were created to make #neurons a xle of WDL
													IMC_Out[_xd][_wd][_xb][_wb] = 0 ;
													IMC_Out_XADC[_xd][_wd][_xb][_wb] = 0 ;
												}
											}
										}
									}
								}
								// CORRECTION FOR THE PADDED BITS ENDS
								// SD WRITE ADDED CODE STARTS
								for(uint16_t _xd = 0; _xd < XDL; _xd++){
									for(uint16_t _wd = 0; _wd < WDL; _wd++){
										for(uint16_t _xb = 0; _xb < 1; _xb++){
											for(uint16_t _wb = 0; _wb < WBL; _wb++){
												IMC_Out_temp[_xd][_wd][_xb][_wb]=IMC_Out[_xd][_wd][_xb][_wb];
												IMC_Out_XADC_temp[_xd][_wd][_xb][_wb]=IMC_Out_XADC[_xd][_wd][_xb][_wb];
											}
										}
									}
								}
	////
	////                            int size=XDL*WDL*XBL*WBL*4;
	////                    		char FileName_out[]="IMCO.bin";
	////                    		int rc=WriteFile_custom(FileName_out, size, IMC_Out_temp);
	////                    		int Status=SD_Eject(&fatfs);
	////							int pauseval=1;
								rc = f_write(&fil_IMCC, IMC_Out_temp, size, &br_IMCC);
								rc = f_write(&fil_IMCC_XADC, IMC_Out_XADC_temp, size, &br_IMCC_XADC);
									// extra added by ashwin on 20-Sep-2023 starts
									rc = f_write(&fil_volt, voltage_see_2bitX, size_volt, &br_volt);
									rc = f_write(&fil_volt_xadc, voltage_xadc_2bitX_see, size_volt_xadc, &br_volt_xadc);
									// extra added by ashwin on 20-Sep-2023 ends
								// SD WRITE ADDED CODE ENDS
									Multiply2Power_CNV_FP_2bitX(IMC_Out,oc ,WDL,c,wb,WBL,b,XDL,s,xb,XBL,X_out_w,XMAX_Data[2], WMAX_Data[2],
                            			XMAX_Data[0], WMAX_Data[0],XMAX_Data[1],WMAX_Data[1],CNV_Out);
                            	// UART ADDED CODE STARTS HERE
								uint16_t returnWords[]={oc,c,wb,s,b,xb};
								int numWords=sizeof(returnWords)/sizeof(returnWords[0]);
								sendWordData( returnWords, numWords, myUart ) ; // 0.005 ms
								// UART ADDED CODE ENDS HERE
								// FREEING MEMORY STARTS HERE
								for(uint16_t _wd = 0; _wd < numDLs; _wd++){
									for(uint16_t _wl = 0; _wl < numWWLs; _wl++){
										free(readSRAMTotal[_wd][_wl]);
									}
									free(readSRAMTotal[_wd]);
								}

								free(readSRAMTotal_FLAT);
								// FREEING MEMORY END HERE
                            }
                        }
                    }
                }
            }
        }
        // SD WRITE ADDED CODE STARTS
		rc = f_close(&fil_IMCC);
		rc = f_close(&fil_RDSC);
		rc = f_close(&fil_IMCC_XADC);
			// extra added by ashwin on 20-Sep-2023 starts
			rc = f_close(&fil_volt);
			rc = f_close(&fil_volt_xadc);
			// extra added by ashwin on 20-Sep-2023 ends
		Xil_DCacheFlush();
//		int Status=SD_Eject(&fatfs);
		int pauseval=1;
		// SD WRITE ADDED CODE ENDS
        #pragma endregion

        //Delete Buffers allocated for IMC
        #pragma region
        //Deallocate W_BitMat matrix
        for(uint16_t b = 0; b<W_BitMat_Batch; b++){
            for(uint16_t k =0; k< W_outHeight; k++){
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
            for(uint16_t c = 0; c < X_outDepth; c++){
                for(uint16_t k = 0; k < X_outHeight; k++){
                    for(uint16_t n= 0; n < WordLines; n++){
                        free(X_BitMat[b][c][k][n]);
                    }
                    free(X_BitMat[b][c][k]);
                }
                free(X_BitMat[b][c]);
            }
            free(X_BitMat[b]);
        }
        free(X_BitMat);

        //Deallocate X_Weight matrix
        for(uint16_t b = 0; b<X_BitMat_Batch;b++){
            for(uint16_t c = 0; c < X_outDepth; c++){
                free(X_Weight[b][c]);
            }
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
        // Normal Convolution to Crosscheck results. Comment this section when not required.
        normal_CNV(Unfolded_X, X_Batch, X_outDepth, X_outHeight, X_outWidth, Unfolded_W,
                    W_Batch, W_outDepth, W_outHeight, W_outWidth,X_out_w,CNV_Out);
    }

    //Add bias to results.
    #pragma region
    //Only unpadded ndx is enough
    for(uint16_t b = 0; b < X_Batch; b ++){
        for(uint16_t k = 0; k < W_Batch; k ++){
            for(uint16_t h = 0; h < X_out_h; h++){
                for(uint16_t w = 0; w < X_out_w; w++){
                    CNV_Out[b][k][h][w] += Bias[k];
                }
            }
        }
    }
    #pragma endregion

    //Delete all dynamically allocated memory. Common between IMC and Normal CNV
    #pragma region
    // Deallocate memory for the Unfolded_X array
    for(uint16_t b =0; b<X_Batch ;b++){
        for(uint16_t n=0; n<X_outDepth; n++){
            for (uint16_t k = 0; k < X_outHeight; k++) {
                free(Unfolded_X[b][n][k]);
            }
            free(Unfolded_X[b][n]);
        }
        free(Unfolded_X[b]);
    }
    free(Unfolded_X);

    // Deallocate memory for the Unfolded_W array
    for(uint16_t b =0; b<W_Batch ;b++){
        for(uint16_t n=0; n<W_outDepth; n++){
            for (uint16_t k = 0; k < W_outHeight; k++) {
                free(Unfolded_W[b][n][k]);
            }
            free(Unfolded_W[b][n]);
        }
        free(Unfolded_W[b]);
    }
    free(Unfolded_W);

    #pragma endregion
    return currObj;
}



// Function to convert an integer to an array of bits (LSB at zeroeth index)
void intTo8BitsArray(int num, uint8_t* bitArray) {
    for (uint8_t i = 0; i < 8; i++) {
    	bitArray[i] = num & 1;
        num >>= 1;
    }
}


//New fixed point pre alignment.
void preAlignVector_FP(float* vec, uint16_t height, float Max, bool Signed,uint16_t Bits, uint8_t ** binary_m){
	if(Signed){
		int Power = pow(2,Bits-1);
		for(uint16_t i = 0; i < height; i++){
			if(vec[i] < 0)
				vec[i] = fabs(vec[i]) > Max? -1*Max: vec[i];
			else
				vec[i] = fabs(vec[i]) > Max? Max: vec[i];

			int twos_comp = round((vec[i] / Max) * Power);
			twos_comp = twos_comp == Power? (int)(Power - 1) : twos_comp;
			intTo8BitsArray(twos_comp,binary_m[i]);
		}
	}
	else{
		int Power = pow(2,Bits);
		for(uint16_t i = 0; i < height; i++){
			vec[i] = fabs(vec[i]);
			vec[i] = vec[i] > Max? Max: vec[i];
			int input = round((vec[i] / Max) * Power);
			input = input == Power? (int)(Power - 1) : input;
			intTo8BitsArray(input,binary_m[i]);
		}
	}
}

//Function to multiply by power of 2 after getting results from IMC.
void Multiply2Power_CNV_FP(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,
                    uint16_t X_out_w,bool X_Signed, bool W_Signed ,float XMax,float WMax,uint16_t XBits,uint16_t WBits,float**** CNV_Out){
	int X_Weight = X_Signed? -(XBits-1):-XBits;
	int W_Weight = W_Signed? -(WBits-1):-WBits;
	float weight = 1;
    for(uint16_t kn = oc; kn < oc+ WDL; kn++){//For set of kernels
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                for(uint16_t j = xb; j < xb + XBL; j++){// For image bit

                	float XP2  = 1 - 2 * ((int)X_Signed) * ( j == XBits - 1 ) ;
                	XP2 = XP2 * pow(2,X_Weight+j);

                	float WP2 = 1 - 2 * ((int)W_Signed) * ( i == WBits - 1 ) ;
					WP2 = WP2 * pow(2,W_Weight+i);

					weight = XP2 * WP2 ;



//                	if(X_Signed && W_Signed)
//                		weight  = ((i == WBits-1) ^ (j==XBits-1))? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
//                	if(X_Signed && !W_Signed)
//                	    weight  = (j==XBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
//                	if(!X_Signed && W_Signed)
//                	    weight  = (i==WBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
//                	if(!X_Signed && !W_Signed)
//                	    weight  = pow(2,X_Weight+W_Weight+i+j);



                    CNV_Out[bt][kn][(int)floor((float)s/(float)X_out_w)][s%X_out_w] += (IMC_Out[bt - b][kn - oc][j - xb][i - wb] * weight * XMax * WMax);
                }
            }
        }
    }
}

//Function to multiply by power of 2 after getting results from IMC.
void Multiply2Power_CNV_FP_2bitX(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,
                    uint16_t X_out_w,bool X_Signed, bool W_Signed ,float XMax,float WMax,uint16_t XBits,uint16_t WBits,float**** CNV_Out){
	int X_Weight = X_Signed? -(XBits-1):-XBits;
	int W_Weight = W_Signed? -(WBits-1):-WBits;
	float weight = 1;
    for(uint16_t kn = oc; kn < oc+ WDL; kn++){//For set of kernels
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                //for(uint16_t j = xb; j < xb + XBL; j++){// For image bit // ****** OLD CODE ****** // won't work for 2 bit X
            	for(uint16_t j = xb; j < xb + XBL; j+=XBL){// For image bit // ****** CODE CHANGE BY ASHWIN ****** // won't work if X is signed

                	float XP2  = 1 - 2 * ((int)X_Signed) * ( j == XBits - 1 ) ;
                	XP2 = XP2 * pow(2,X_Weight+j);

                	float WP2 = 1 - 2 * ((int)W_Signed) * ( i == WBits - 1 ) ;
					WP2 = WP2 * pow(2,W_Weight+i);

					weight = XP2 * WP2 ;



//                	if(X_Signed && W_Signed)
//                		weight  = ((i == WBits-1) ^ (j==XBits-1))? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
//                	if(X_Signed && !W_Signed)
//                	    weight  = (j==XBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
//                	if(!X_Signed && W_Signed)
//                	    weight  = (i==WBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
//                	if(!X_Signed && !W_Signed)
//                	    weight  = pow(2,X_Weight+W_Weight+i+j);



                    CNV_Out[bt][kn][(int)floor((float)s/(float)X_out_w)][s%X_out_w] += (IMC_Out[bt - b][kn - oc][j - xb][i - wb] * weight * XMax * WMax);
                }
            }
        }
    }
}


//Function to multiply by power of 2 after getting results from IMC_LIN.
void Multiply2Power_LIN_FP_2bitX(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL ,uint16_t xb, uint16_t XBL,bool X_Signed, bool W_Signed, float XMax, float WMax,
					uint16_t XBits,uint16_t WBits,float** LIN_Out){

	int X_Weight = X_Signed? -(XBits-1):-XBits;
	int W_Weight = W_Signed? -(WBits-1):-WBits;
	float weight = 1;
    for(uint16_t kn = oc;kn < oc+ WDL; kn++){//For set of W_Features
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                for(uint16_t j = xb; j < xb + XBL; j+=XBL){// For image bit // ****** CODE CHANGE BY ASHWIN ****** // won't work if X is signed
                	if(X_Signed && W_Signed)
                		weight  = ((i == WBits -1) ^ (j==XBits-1))? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
                	if(X_Signed && !W_Signed)
                	    weight  = (j==XBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
                	if(!X_Signed && W_Signed)
                	    weight  = (i==WBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
                	if(!X_Signed && !W_Signed)
                	    weight  = pow(2,X_Weight+W_Weight+i+j);

                    LIN_Out[bt][kn] += (IMC_Out[bt - b][kn - oc][j - xb][i - wb] * weight * XMax * WMax);
                }
            }
        }
    }
}

//Function to multiply by power of 2 after getting results from IMC_LIN.
void Multiply2Power_LIN_FP(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL ,uint16_t xb, uint16_t XBL,bool X_Signed, bool W_Signed, float XMax, float WMax,
					uint16_t XBits,uint16_t WBits,float** LIN_Out){

	int X_Weight = X_Signed? -(XBits-1):-XBits;
	int W_Weight = W_Signed? -(WBits-1):-WBits;
	float weight = 1;
    for(uint16_t kn = oc;kn < oc+ WDL; kn++){//For set of W_Features
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                for(uint16_t j = xb; j < xb + XBL; j++){// For image bit
                	if(X_Signed && W_Signed)
                		weight  = ((i == WBits -1) ^ (j==XBits-1))? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
                	if(X_Signed && !W_Signed)
                	    weight  = (j==XBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
                	if(!X_Signed && W_Signed)
                	    weight  = (i==WBits-1)? (-1*pow(2,X_Weight+W_Weight+i+j)) :  pow(2,X_Weight+W_Weight+i+j);
                	if(!X_Signed && !W_Signed)
                	    weight  = pow(2,X_Weight+W_Weight+i+j);

                    LIN_Out[bt][kn] += (IMC_Out[bt - b][kn - oc][j - xb][i - wb] * weight * XMax * WMax);
                }
            }
        }
    }
}

void ReplicateIMCTiming(uint8_t**** W_BitMat,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
						uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL, uint16_t wordlines, float**** IMC_Out){
	for(int i = 0; i < XDL; i++)
		for(int j = 0; j < WDL; j++)
			for(int k = 0; k < XBL; k++)
				for(int l = 0; l < WBL; l++)
					IMC_Out[i][j][k][l] = 0;

    for(uint16_t kn = oc; kn < oc+ WDL; kn++){//For set of kernels
        for(uint16_t i = wb; i < wb + WBL;i++){//For weight bit
            for(uint16_t bt = b; bt < b + XDL; bt ++){//For batch
                for(uint16_t j = xb; j < xb + XBL; j++){// For image bit
                    for(uint16_t k = 0; k < wordlines; k++){
                    	IMC_Out[bt - b][kn - oc][j - xb][i - wb] += W_BitMat[kn][c][k][i] * X_BitMat[bt][s][c][k][j];
                    }
                }
            }
        }
    }
}
