#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <xtime_l.h>
//#include <custom_h.h>
typedef struct FP16_members {
  float mantissa;
  int exponent;
  int sign; //0 is positive 1 is negative
  bool overflow; //
}FP16;

//Function to get variable in FP16 format.
FP16 getFP16(float var);

//Function to add padding in 3d array
float*** pad3DArray(float*** array, uint16_t height, uint16_t width, uint16_t depth, uint16_t padding);

//Flatten function. b*c*h*w -> b*features.
void flatten(float**** array,uint16_t batch, uint16_t channels, uint16_t height, uint16_t width, float** flattened_array);

//Function to flatten 3d image same as pytorch unfold.
void rearrange4DImage(float**** input, uint16_t inputBatch, uint16_t inputDepth, uint16_t inputHeight, uint16_t inputWidth,
                   uint16_t kernelHeight, uint16_t kernelWidth,
                   uint16_t stride, uint16_t padding, uint16_t chunksize, float**** output);

// 9 bit 2s complement representation.
void nineBit2sComp(int decimal, uint8_t* binary);

//Does prealignment of decimal in FP16 format.
void preAlignVector(float* vec, uint16_t height, int * aligned_w, uint8_t ** binary_m);

//C function replicating IMC functionality
void replicateIMC(uint8_t*** w_mat, uint16_t n_size, uint16_t n_kernals, int* we, int xe, uint8_t** x_mat, float* out );

//Function to add array element wise and store it in a
void addArrayElemwise(float* a, float* b, uint16_t size);

//Function to convoluate unfolded matrices.
void normal_CNV(float**** Unfolded_X, uint16_t X_Batch, uint16_t X_Depth, uint16_t X_Height, uint16_t X_Width, float**** Unfolded_W,
                uint16_t W_Batch, uint16_t W_Depth, uint16_t W_Height, uint16_t W_Width,uint16_t X_out_w,float**** CNV_Out );

//Max pooling function.
void MaxPooling(float**** Array,uint16_t batch, uint16_t depth, uint16_t height, uint16_t width,
                uint16_t pool_h, uint16_t pool_w, uint16_t padding, uint16_t stride, float**** Out);

//Avg pooling function.
void AvgPooling(float**** Array,uint16_t batch, uint16_t depth, uint16_t height, uint16_t width, uint16_t pool_h,
                uint16_t pool_w, uint16_t padding, uint16_t stride,bool include_padding ,float**** Out);

//4D ReLU function
void ReLU(float**** Array, uint16_t batch, uint16_t channels, uint16_t height, uint16_t width, float**** Out);

//4D BatchNorm
void BatchNorm(float**** Input, uint16_t batch, uint16_t kernels, uint16_t height, uint16_t width, float* mean,
              float* var, float* weight, float* bias, float**** Out);

////X_BitMat[b:b+XDL][s][c][0:WordLines][xb:xb+XBL] -> Access like this

//Function to multiply by power of 2 after getting results from IMC_CNV.
void Multiply2Power_CNV(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,int** W_Weight,
                    int*** X_Weight, uint16_t X_out_w, float**** CNV_Out);

//Function to multiply by power of 2 after getting results from IMC_LIN.
void Multiply2Power_LIN(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL ,uint16_t xb, uint16_t XBL,int** W_Weight, int** X_Weight,
                    float** LIN_Out);

//Dummy function to check results using IMC
void replicateIMCHW(uint8_t**** W_BitMat,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,
                    uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,int** W_Weight,
                     int*** X_Weight, uint16_t X_out_w, float**** CNV_Out );

//Windows file read function.
void readFileWindows(char *FileName, char* DestinationAddress);


////Convolve2d function.
//void convolve(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
//              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
//              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
//              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float* Bias, float**** CNV_Out);

//Unfolding function for Linear Layers.
void rearrange2DImage(float** input, uint16_t inputBatch, uint16_t inputFeatures, uint16_t chunksize, float*** output);

//Dummy function for Linear Layer in IMC.
void replicateLINIMCHW(uint8_t**** W_BitMat,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,
                    uint8_t**** X_BitMat,uint16_t b ,uint16_t XDL, uint16_t xb, uint16_t XBL,int** W_Weight,
                     int** X_Weight, float** LIN_Out );

//Linear function without prealignment.
void normal_LIN(float*** Unfolded_X, uint16_t X_Batch, uint16_t X_W_Chunks, uint16_t WordLines, float*** Unfolded_W,
                uint16_t W_Features, float** LIN_Out );

//Fully Connected Layers -> Linear Function.
//void linear(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
//             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float** LIN_Out, curr_obj currObj);

//Seperating individual layers weight matrix from buffer
//Layer ID starts with zero.
void getWeightsForLayer(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, uint16_t* W_Channels, uint16_t* W_Height, uint16_t* W_Width, float**** output);

void getWeightsForLinear(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, uint16_t* W_Channels, uint16_t* W_Height, uint16_t* W_Width, float** output);
//Seperating individual layers bias matrix from buffer.
//Layer ID starts with zero.
void getBiasForLayer(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, float* output);

//Seperating individual layers batch norm matrix from buffer.
//Layer ID starts with zero.
//Output = 4(mu,var,weight,bias)*Kernel for individual layer
void getBNForLayer(char* Buffer,uint16_t Num_layers, uint16_t Layer, uint16_t* W_Batch, float** output);

//Reference matrix for layer.
void getRefForLayer(char* Buffer, uint32_t offset_bytes,uint16_t chunks,uint16_t na,uint16_t nb, uint16_t wordlines ,float**** Ref,
		float*** RefB,float* RefMax);
//Add 4d array elementwise.
//Store it in first array.
void add4DArrayElementwise(float**** a, float**** ba, uint16_t batch, uint16_t channels, uint16_t height, uint16_t width);

float bf16tofloat(unsigned char sentArr1,unsigned char sentArr2,unsigned char sentArr3, unsigned char sentArr4);

void flatten2D(float** array,uint16_t batch, uint16_t channels,float* flattened_array);


// NEW FUNCTIONS FOR FIXED POINT IMPLEMENTATIONS

void intTo8BitsArray(int num, uint8_t* bitArray);

void preAlignVector_FP(float* vec, uint16_t height, float Max, bool Signed,uint16_t Bits, uint8_t ** binary_m);

//Function to multiply by power of 2 after getting results from IMC.
void Multiply2Power_CNV_FP(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,
                    uint16_t X_out_w,bool X_Signed, bool W_Signed ,float XMax,float WMax,uint16_t XBits,uint16_t WBits,float**** CNV_Out);

void Multiply2Power_LIN_FP(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL ,uint16_t xb, uint16_t XBL,bool X_Signed, bool W_Signed, float XMax, float WMax,
					uint16_t XBits,uint16_t WBits,float** LIN_Out);

void Multiply2Power_CNV_FP_2bitX(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL,
                    uint16_t X_out_w,bool X_Signed, bool W_Signed ,float XMax,float WMax,uint16_t XBits,uint16_t WBits,float**** CNV_Out);

void Multiply2Power_LIN_FP_2bitX(float**** IMC_Out,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
                    uint16_t b ,uint16_t XDL ,uint16_t xb, uint16_t XBL,bool X_Signed, bool W_Signed, float XMax, float WMax,
					uint16_t XBits,uint16_t WBits,float** LIN_Out);
