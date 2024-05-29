/*
 * custom_h.h
 *
 *  Created on: Mar 5, 2021
 *      Author: Sri Hari
 */

#ifndef PS7_CORTEXA9_0_INCLUDE_CUSTOM_H_H_
#define PS7_CORTEXA9_0_INCLUDE_CUSTOM_H_H_

// Extra Added for ADC starts here

#include "xsysmon.h"
//#include "xparameters.h"
#include "xstatus.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "sleep.h"

#define XPAR_AXI_XADC_0_DEVICE_ID 0


// Extra Added for 2ADC starts here
//#include "xadcps.h"
// Below statement added by Ashwin Balagopal. Used in adcRead.c
//#define XADCPS_CH_AUX00 XADCPS_CH_AUX_MIN+0
//#define XADCPS_CH_AUX07 XADCPS_CH_AUX_MIN+7
//#define XADCPS_CH_AUX08 XADCPS_CH_AUX_MIN+8
//void adcRead(u8* returnBytes, u16 repeatAvg, XAdcPs XADC_Driver_Instance);
//void adcRead(u8* returnBytes, u16 repeatAvg, XAdcPs *XADC_VPVN_Instance);
// Extra Added for 2ADC ends here

// Extra Added for 1ADC starts here
//#define C_BASEADDR 0x43C00000
//void adcRead(u8* returnBytes, u16 repeatAvg);
// Extra Added for 1ADC ends here

// Extra Added for LAB PDF CODE STARTS HERE
#include "xsysmon.h"
#define SYSMON_DEVICE_ID XPAR_SYSMON_0_DEVICE_ID //ID of xadc_wiz_0
void adcRead(u8* returnBytes, u16 repeatAvg );
// Extra Added for LAB PDF CODE ENDS HERE

#endif /* PS7_CORTEXA9_0_INCLUDE_CUSTOM_H_H_ */

#include<xgpio.h>
// GOT BANK MAPPING FROM https://docs.google.com/spreadsheets/d/1qA65VvTDkKlL3Ou3-Us68UFqh0Ad6-piOz4uEJEVTds/edit#gid=0
// Banks Mapping
#define GPIO_PCB_32INPUTS 		0
#define GPIO_PCB_32INPUTS_1		1
#define GPIO_PCB_32INPUTS_2		2
#define GPIO_PCB_32OUTPUTS		3
#define GPIO_PCB_32OUTPUTS_1	4
#define GPIO_PCB_32OUTPUTS_2	5
// for ease of use
#define SCQOUT_BANK GPIO_PCB_32OUTPUTS_2

// stringCurr
#define RWL_EN_EXT	22
#define SC_DIN	29
#define ScanSel_3	31
#define ScanSel_2	32
#define ScanSel_1	33
#define ScanSel_0	34
#define CLK_A	4
#define CLK_B	5
#define CLK_C	6
#define OEb_1	36
#define RBL_PRE_EN_B	20
#define OEb_2	37
#define WWL_EN	35
#define OEb_3	38
#define IMG_Sel_Dec_2	14
#define OEb_4	39
#define IMG_Sel_Dec_1	15
#define OEb_9	44
#define IMG_Sel_Dec_0	16
#define OEb_10	45
#define RW_SA_EN	27
#define ADC0_S0	47
#define SUBVoltPADConnect	30
#define ADC0_S1	46
#define MODE_R_WB	17
#define PRE_DR_EN	18
#define unused0	50
#define unused1	51
#define CS_0	10
#define CS_1	9
#define ADC1_S0	49
#define ADC1_S1	48
#define BL_Sel_Dec_EXT_0	3
#define BL_Sel_Dec_EXT_1	2
#define unused2	52
#define unused3	53
#define unused4	54
#define unused5	55
#define RESET_GLOBALTIMER	21
#define unused6	56
#define CLK_DFF_EXT	7
#define unused7	57
#define DataCM_EN_EXT	11
#define OEb_5	40
#define DataSUB_EN_EXT	13
#define OEb_6	41
#define Reset_SARLOGIC_EXT	26
#define unused8	58
#define DataResis_EN	12
#define unused9	59
#define RefResis_EN	24
#define unused10	60
#define CLK_SARLOGIC_IN_EXT	8
#define unused11	61
#define unused12	62
#define SA_EN_EXT	28
#define AllVIN_n_EXT	1
#define RefSUB_EN_EXT	25
#define unused13	63
#define unused14	64
#define unused15	65
#define RefCM_EN_EXT	23
#define unused16	66
#define OEb_7	42
#define ADCSTART_EXT	0
#define PRE_VCM_EXT	19
#define OEb_8	43
#define FPGA_RESET	67

//End of stringCurr #define

#define SC_QOUT 67


//Scanchain macros
#define SA_SC	0
#define ADC_SC	1
#define AdderTree_SC	2
#define SARTIMING_OUT_SC	3
#define WRITE_2_SC	4
#define WRITE_1_SC	5
#define WRITE_0_SC	6
#define IMC_RBL_SC	7
#define IMC_VOut_SC	8
#define WWL_SC	9
#define FLOATADC_SC	10
#define SARTIMING_IN_SC	11
#define ControlPath_SC	12
#define Reset_DFF	13
#define SC_DEFAULT	SA_SC
//Added by Ashwin Balagopal(ends)
typedef struct{

	int file_transmit_size;
	u8 data[4];

}bitsSet_obj;
#define STOPBYTE 127
#define PARAMETERMARKERBYTE 126

#define stringCurr_LEN 68
#define MAX_SC_LENGTH 1160
typedef struct{
	XGpio GPIO_PCB_32INPUTS_Inst;
	XGpio GPIO_PCB_32OUTPUTS_Inst;
	XGpio GPIO_PCB_32INPUTS_1_Inst;
	XGpio GPIO_PCB_32OUTPUTS_1_Inst;
	XGpio GPIO_PCB_32INPUTS_2_Inst;
	XGpio GPIO_PCB_32OUTPUTS_2_Inst;
	XGpio GPIO_PCB_32OUTPUTS_100_Inst;
	XGpio GPIO_PCB_HS_Inst;
	XGpio GPIO_PCB_HS_Relaxed_Inst;
	XGpio GPIO_MODE_READ_WRITE_Inst;
	XGpio GPIO_MODE_IMC_Inst;
// FASTSCAN GPIOs
	XGpio GPIO_FASTSCAN_RESET_FASTSCAN_Inst;
	XGpio GPIO_FASTSCAN_GPIO_CONTROL_SETTINGS_Inst;
	XGpio GPIO_FASTSCAN_GPIO_WIDTH_SETTINGS_Inst;

	XGpio GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK_Inst[4];

	XGpio GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK_Inst[4];

	XGpio GPIO_FASTSCAN_SCAN_DONE_Inst;
// TOPSW GPIOs
	XGpio GPIO_TOPSW_Inst;
	u8 stringCurr[stringCurr_LEN];// these are all the FMC pin values. To update, call read_FMC_PINS
	u8 stringCurr_DEFAULT[stringCurr_LEN];// these are the input pins' default values
	u8 scanOutBits[MAX_SC_LENGTH];//Contains latest scanned out vector.
// GPIO_HYBRIDPIPELINETOPSW_VARS
	XGpio GPIO_HYBRIDPIPELINETOPSW_VARS_Inst;
}curr_obj;

#define GPIO_BANKHS_CASENUM 4
#define GPIO_BANK_HS_Relaxed_CASENUM 5
// ADDED BY ASHWIN BALAGOPAL(starts)
typedef struct{
	u8 bits[7];
}seven_bits;
seven_bits get7BitsFrom1Byte(u8 inputByte);
curr_obj defaultState(curr_obj currObj);
// ADDED BY ASHWIN BALAGOPAL(ends)

curr_obj bitsSet( curr_obj currObj,u8 *signals,int signals_len,u8 *values );
curr_obj scanSelect(curr_obj currObj,u8 scanChain);
curr_obj scanInVector(curr_obj currObj,u8 *vecScanIn,int len_vecScanIn, u8 numPadZeroes_vecScanIn,u8 scanChain);


#define scan_len_SA_SC 97
#define scan_len_ADC_SC 644
#define scan_len_AdderTree_SC 176
#define scan_len_SARTIMING_OUT_SC 26
#define scan_len_WRITE_2_SC 97
#define scan_len_WRITE_1_SC 97
#define scan_len_WRITE_0_SC 97
#define scan_len_IMC_RBL_SC 88
#define scan_len_IMC_VOut_SC 88
#define scan_len_WWL_SC 1160
#define scan_len_FLOATADC_SC 92
#define scan_len_SARTIMING_IN_SC 110
#define scan_len_ControlPath_SC 17


typedef struct{
	u8 scanOutBits_SA_SC[scan_len_SA_SC];
	u8 scanOutBits_ADC_SC[scan_len_ADC_SC];
	u8 scanOutBits_AdderTree_SC[scan_len_AdderTree_SC];
	u8 scanOutBits_SARTIMING_OUT_SC[scan_len_SARTIMING_OUT_SC];
	u8 scanOutBits_WRITE_2_SC[scan_len_WRITE_2_SC];
	u8 scanOutBits_WRITE_1_SC[scan_len_WRITE_1_SC];
	u8 scanOutBits_WRITE_0_SC[scan_len_WRITE_0_SC];
	u8 scanOutBits_IMC_RBL_SC[scan_len_IMC_RBL_SC];
	u8 scanOutBits_IMC_VOut_SC[scan_len_IMC_VOut_SC];
	u8 scanOutBits_WWL_SC[scan_len_WWL_SC];
	u8 scanOutBits_FLOATADC_SC[scan_len_FLOATADC_SC];
	u8 scanOutBits_SARTIMING_IN_SC[scan_len_SARTIMING_IN_SC];
	u8 scanOutBits_ControlPath_SC[scan_len_ControlPath_SC];
	curr_obj currObj;
}scan_out;

typedef struct{
	u8 scanOutBits_ADC_SC_IMGBL[scan_len_ADC_SC*4*4];
	u8 scanOutBits_AdderTree_SC[scan_len_AdderTree_SC];
	curr_obj currObj;
}scan_out_internal;

#define max_XADCrepeatAvg 200
#define len_XADCVolts 92

typedef struct{
	float adcOut_VPVN[max_XADCrepeatAvg];
	float adcOut_AUX00[max_XADCrepeatAvg];
	curr_obj currObj;
}XADCvolt;

typedef struct{
	float scanOutVolts_XADC[scan_len_FLOATADC_SC*4*4];
	curr_obj currObj;
}read_out_XADCvolt;

scan_out scanOutVector(curr_obj currObj,u8 scanChain);

u8 bitsGet(curr_obj currObj,u8 bank);

curr_obj scanInWriteScanChains(curr_obj currObj,u8 *data, int data_len, u8 numPadZeroes_data,u8 *mask, int mask_len, u8 numPadZeroes_mask);

curr_obj scanInWriteScanChains_new(curr_obj currObj,u8 *data, int data_len, u8 *mask, int mask_len);

curr_obj write_onto_datalines( curr_obj currObj,int *data_1_row_indices,int len_data_1_row_indices,int *data_0_row_indices,int len_data_0_row_indices,u8 *colSel,int colSel_len, u8 *DL_nums, int DL_nums_len);

curr_obj scanInVector_uncompressed(curr_obj currObj,u8 *vecScanIn, int len_vecScanIn, u8 scanChain);

curr_obj scanInWriteScanChains_dataOnly_uncompressed(curr_obj currObj,u8 *data, int data_len); //NOTE: This function cannot be called from MATLAB as it does not have a case statement, it is for internal C use only

curr_obj scanInWriteScanChains_maskOnly_uncompressed(curr_obj currObj,u8 *mask, int mask_len); //NOTE: This function cannot be called from MATLAB as it does not have a case statement, it is for internal C use only

curr_obj scanInWriteScanChains_dataOnly(curr_obj currObj,u8 *data, int data_len, u8 numPadZeroes_data);

curr_obj scanInWriteScanChains_maskOnly(curr_obj currObj,u8 *mask, int mask_len, u8 numPadZeroes_mask);

curr_obj loadBitlinetoCell_FPGA(curr_obj currObj,u8 *bitLines, int bitLines_len);//PERFORM WRITE

curr_obj loadCelltoSACtr_FPGA(curr_obj currObj,u8 *bitLines, int bitLines_len);//PERFORM READ

void pulse01FPGA_RESET_handshake(curr_obj currObj);

void pulse01FPGA_RESETRelaxed_handshake(curr_obj currObj);

scan_out perform_RxOnRowColSel_FPGA(curr_obj currObj,u8 *BL, u16 WLnum );

curr_obj perform_WxOnRowColSel_FPGA_2(curr_obj currObj, u8 *data, int data_len, u8 numPadZeroes_data,u8 *mask, int mask_len, u8 numPadZeroes_mask, u8 *BL, u16 WLnum );

curr_obj perform_WxOnRowColSel_FPGA_2_new(curr_obj currObj, u8 *data, int data_len, u8 *mask, int mask_len, u8 *BL, u16 WLnum );

curr_obj scanInWLSC_toSelectWLnum( curr_obj currObj, u16 WLnum_ADDR ) ;

curr_obj pulse( curr_obj currObj, u8 *signals, u8 *currentValues, int signals_len );

curr_obj writeSRAM_calib(curr_obj currObj, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);

curr_obj fine_coarseSRAM(curr_obj currObj,u8 Fine_cm[88], u8 Coarse_cm[88], u8 Fine_sub[88], u8 Coarse_sub[88]);

curr_obj IMGSelect(curr_obj currObj,u8 IMG);

scan_out perform_hybridIMC_BLIMG_FPGA(curr_obj currObj,u8 bitLine, u8 IMG);

scan_out_internal perform_internalIMC_ImgBankFPGA(curr_obj currObj, u8 ImgBank);
scan_out_internal perform_internalIMC_TOPSW_ImgBankFPGA(curr_obj currObj, u8 ImgBank);

scan_out_internal perform_IMC_Internal_pyTorch(curr_obj currObj, u8 ImgBank);

curr_obj RBL_IMC_Select(curr_obj currObj,u8 RBL);

XADCvolt adcRead_uncompressed(u16 repeatAvg );

read_out_XADCvolt perform_XADC_MAC(curr_obj currObj, u8 ImgBank);

curr_obj init_XADC(curr_obj currObj);

typedef struct{
	u8 SRAMdata[105924];
	curr_obj currObj;
}SRAM_Store;

SRAM_Store readSRAM(curr_obj currObj,u8 bitLine);

curr_obj writeSRAM(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);

curr_obj readSRAMSliced(curr_obj currObj, u8 **Read_out_image, u8 ***Read_out_weight, u8 ****Read_out_reference, u8 **Read_out_referenceAdderTree );

curr_obj readSRAMAll(curr_obj currObj, u8 ***readSRAMTotal);

curr_obj init_internal(curr_obj currObj);

curr_obj init_internal_2X(curr_obj currObj);

curr_obj init_internal_slopeSetting_ICQ(curr_obj currObj, u8 slopeSetting);

curr_obj init_Hybrid(curr_obj currObj);

void set_HYBRIDPIPELINETOPSW_VARS_GPIO(curr_obj currObj, u8 RWL_EN_DELAY,u8 CM_EN_DELAY,u8 ADCSTART_DELAY,u8 SAMPLING_DELAY, u8 EVALUATE_DELAY);

scan_out perform_hybridIMC_TOPSW_FPGA(curr_obj currObj,u8 bitLine, u8 IMG);

curr_obj store_images_SRAM_MultiBitX(uint8_t* X,u8 mode,u8 IMGnum ,curr_obj currObj);

void printString(curr_obj currObj);

#include "ff.h"
#include "xstatus.h"
#include <stdlib.h>
#include "xil_printf.h"
#include "xil_cache.h"

FATFS  fatfs;

int SD_Eject();
int SD_Init();
int WriteFile(char *FileName, u32 size, u32 SourceAddress);
FRESULT ReadFile(char *FileName, u32 DestinationAddress);


typedef struct{
	u8 bits[8];
}eight_bits;
eight_bits get8BitsFrom1Byte(u8 inputByte);

// functions added for CHIP_2023
//ADDED BY ASHWIN(STARTS)
u8 extractBitFromDWORD(u32 inputDWORD, u8 k);
curr_obj read_FMC_PINS(curr_obj currObj);
curr_obj bitLineSelect(curr_obj currObj,u8 bitLine);
//ADDED BY ASHWIN(ENDS)

// functions added for CHIP_2023
//ADDED BY ASHWIN(STARTS)
u8 extractBitFromDWORD(u32 inputDWORD, u8 k);
curr_obj read_FMC_PINS(curr_obj currObj);
curr_obj bitLineSelect(curr_obj currObj,u8 bitLine);
//Function to write weights in SRAM
//curr_obj store_weights_SRAM(uint8_t**** W_BitMat,uint8_t** Ref_Arr1,uint8_t** Ref_Arr2,uint8_t** Ref_Arr3,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,curr_obj currObj);
curr_obj store_weights_SRAM(uint8_t**** W_BitMat,float**** Ref,float*** RefB,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,curr_obj currObj);
//W_BitMat[oc:oc+WDL][c][0:WordLines][wb:wb+WBL] -> Access like this

//Function to write images in SRAM
curr_obj store_images_SRAM(uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t c, uint16_t WordLines, uint16_t xb, uint16_t XBL, curr_obj currObj);
curr_obj store_images_SRAM_2bit(uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t c, uint16_t WordLines, uint16_t xb, uint16_t XBL,u8 mode_xbit, curr_obj currObj);
curr_obj store_images_SRAM_lin_2bit(uint8_t**** X_BitMat,uint16_t b ,uint16_t XDL, uint16_t c, uint16_t WordLines, uint16_t xb, uint16_t XBL,u8 mode_xbit, curr_obj currObj);
#include <stdbool.h>

#define mode_ICQ 1
#define mode_OCC 2

typedef struct{
	float LSB;
	float LC;
	float slopecorrect;
	float slopecorrect_chip;
	float slopecorrect_XADC;
	float I_mean;
	float vdd;
	int nbit_ADC;
	u8 mode;
}mode_type;

//Convolve2d function.
curr_obj convolve(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj);
curr_obj convolve_XADC(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj);
curr_obj linear(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, mode_type mode,u8 slopeSetting, curr_obj currObj);
curr_obj linear_NotOnChip(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, curr_obj currObj);
curr_obj linear_XADC(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, mode_type mode,u8 slopeSetting, curr_obj currObj);

// functions for Fixed Point
curr_obj convolve_NotOnChip_FP(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,curr_obj currObj);
curr_obj convolve_FP(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj);
curr_obj convolve_FP_XADC(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,curr_obj currObj);
curr_obj convolve_FP_2bit(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,u8 mode_xbit,curr_obj currObj);
curr_obj convolve_FP_2bit_XADC(float**** X_Mat, uint16_t X_Batch, uint16_t X_Channels, uint16_t X_Height, uint16_t X_Width,
              uint16_t X_Padding, uint16_t X_Stride, float****W_Mat, uint16_t W_Batch, uint16_t W_Channels,
              uint16_t W_Height, uint16_t W_Width, uint16_t W_Padding, uint16_t W_Stride ,uint16_t WordLines,
              uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_CNV, float * Bias,float**** Ref,
			  float*** RefB, float* RefMax,float* XMAX_Data, float* WMAX_Data,float**** CNV_Out,mode_type mode,u8 slopeSetting,u8 mode_xbit,curr_obj currObj);
curr_obj IMC_TIMING(uint16_t XDL, uint16_t XBL,uint16_t WBL,uint16_t**** IMC_Out,curr_obj currObj);
curr_obj writeSRAM_1024(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);
curr_obj scanInVector_uncompressed_FAST(curr_obj currObj,u8 *vecScanIn, int len_vecScanIn, u8 scanChain);
void get32BitsFrom1DWORD(u32 inputDword,u8 *populateArray,u8 limit);
void get32BitsFrom1DWORD(u32 inputDword,u8 *populateArray, u8 len_populateArray);
int max(int a, int b);
int min(int a, int b);
u16 get_scan_len( u8 scanChain );
scan_out populate_scanout( scan_out scanOut, u8 *vecScanOut, u16 scan_len, u8 scanChain );
void set_fastScan_Control_GPIO(curr_obj currObj, u8 FASTSCAN_EXT_LOCB, u8 SCANOUT_INB, u16 numScanInAtOnce);
void set_fastScan_Width_GPIO(curr_obj currObj, u8 CLK_A_LB,u8 CLK_A_UB,u8 CLK_B_LB,u8 CLK_B_UB);
void pulse01_fastScan_reset_GPIO(curr_obj currObj);
scan_out scanOutVector_FAST(curr_obj currObj,u8 scanChain);
curr_obj initialisation_code(curr_obj currObj);
curr_obj scanInWriteScanChains_maskOnly_uncompressed_FAST(curr_obj currObj,u8 *mask, int mask_len);
curr_obj scanInWriteScanChains_dataOnly_uncompressed_FAST(curr_obj currObj,u8 *data, int data_len);
curr_obj scanInWriteScanChains_dataOnly_then_pulse_WWL_FAST(curr_obj currObj,u8 *data, int data_len);
curr_obj writeSRAM_1024_FAST(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);
curr_obj writeSRAM_1024_Images_FAST(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);
void set_fastScan_Control_SPL_GPIO(curr_obj currObj, u8 ScanSel_WWL_SC, u8 ScanSel_WRITE_1_SC, u8 ScanSel_WRITE_0_SC, u8 ScanSel_SC_DEFAULT, u8 SCANOUT_SPL);
curr_obj store_weights_SRAM_lin(uint8_t**** W_BitMat,float**** Ref,float*** RefB,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t WordLines, uint16_t wb, uint16_t WBL,curr_obj currObj);
curr_obj store_images_SRAM_lin(uint8_t**** X_BitMat,uint16_t b ,uint16_t XDL, uint16_t c, uint16_t WordLines, uint16_t xb, uint16_t XBL, curr_obj currObj);
curr_obj writeSRAM_1024_FAST_NEW(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);
curr_obj writeSRAM_1024_Images_FAST_NEW(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len);
// TOPSW GPIO FUNCTIONS
void set_TOPSW_GPIO_bank1(curr_obj currObj, u32 FPGA_RESET_REPEAT_LB, u32 FPGA_RESET_REPEAT_UB, u32 FPGA_RESET_REPEAT_NUMREPEAT);
void set_TOPSW_GPIO_bank2(curr_obj currObj);
curr_obj set_modeInternalHybridTiming(curr_obj currObj,u8 internal_hyb_B,u16 IMG_TIME_GPIO_DATA,u8 RWL_EN_DELAY,u8 CM_EN_DELAY,u8 ADCSTART_DELAY,u8 SAMPLING_DELAY,u8 EVALUATE_DELAY,u32 FPGA_RESET_REPEAT_UB_hybrid);
curr_obj set_ADCbitModeInt_HybB(curr_obj currObj,u8 internal_hyb_B,u8 ADC_bitMode);

#include <stdlib.h>
#include "xil_types.h"
#include "xuartps.h"
#include "xparameters.h"
#include "sleep.h"

#include "xgpio.h"

#include <stdio.h>

//#define BAUDRATE 57600
#define BAUDRATE 115200
#define GPIO_PCB_32INPUTS_ID XPAR_GPIO_PCB_32INPUTS_DEVICE_ID
#define GPIO_PCB_32OUTPUTS_ID XPAR_GPIO_PCB_32OUTPUTS_DEVICE_ID
#define GPIO_PCB_32INPUTS_1_ID XPAR_GPIO_PCB_32INPUTS_1_DEVICE_ID
#define GPIO_PCB_32OUTPUTS_1_ID XPAR_GPIO_PCB_32OUTPUTS_1_DEVICE_ID
#define GPIO_PCB_32INPUTS_2_ID XPAR_GPIO_PCB_32INPUTS_2_DEVICE_ID
#define GPIO_PCB_32OUTPUTS_2_ID XPAR_GPIO_PCB_32OUTPUTS_2_DEVICE_ID
#define GPIO_PCB_32OUTPUTS_100_ID XPAR_GPIO_PCB_SCQOUT_OUTPUTS_100_DEVICE_ID
#define GPIO_PCB_HS_ID XPAR_GPIO_PCB_HARDWARESTOP_DEVICE_ID
#define GPIO_PCB_HS_Relaxed_ID XPAR_GPIO_PCB_HARDWARESTOPRELAXED_DEVICE_ID

#define GPIO_COUNTSTOP_Relaxed_ID XPAR_GPIO_COUNTSTOP_RELAXED_DEVICE_ID

#define GPIO_MODE_READ_WRITE_ID XPAR_GPIO_MODE_READ_WRITE_DEVICE_ID

//READ TIMING GPIO IDS
#define READ_TIMING_1_MODE_R_WB_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_1_MODE_R_WB_START_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_DEVICE_ID
#define GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_DEVICE_ID

//WRITE TIMING GPIO IDS
#define GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_DEVICE_ID
#define GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_DEVICE_ID
#define GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_DEVICE_ID
#define GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_DEVICE_ID
#define GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_DEVICE_ID
#define GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_DEVICE_ID

//IMC Mode GPIO IDS
#define GPIO_MODE_IMC_ID XPAR_GPIO_MODE_IMC_DEVICE_ID

//HYBRID MODE IMC TIMING GPIO IDS
#define GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
#define GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID

//INTERNAL MODE IMC TIMING GPIO
#define GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID XPAR_GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DEVICE_ID

// FASTSCAN GPIOs
#define GPIO_FASTSCAN_RESET_FASTSCAN_DEVICE_ID XPAR_GPIO_FASTSCAN_RESET_FASTSCAN_DEVICE_ID
#define GPIO_FASTSCAN_GPIO_CONTROL_SETTINGS_DEVICE_ID XPAR_GPIO_FASTSCAN_GPIO_CONTROL_SETTINGS_DEVICE_ID
#define GPIO_FASTSCAN_GPIO_WIDTH_SETTINGS_DEVICE_ID XPAR_GPIO_FASTSCAN_GPIO_WIDTH_SETTINGS_DEVICE_ID

#define GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK67_DEVICE_ID XPAR_GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK67_DEVICE_ID
#define GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK45_DEVICE_ID XPAR_GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK45_DEVICE_ID
#define GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK23_DEVICE_ID XPAR_GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK23_DEVICE_ID
#define GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK01_DEVICE_ID XPAR_GPIO_FASTSCANIN_GPIO_PARALLEL_IN_BANK01_DEVICE_ID

#define GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK67_DEVICE_ID XPAR_GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK67_DEVICE_ID
#define GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK45_DEVICE_ID XPAR_GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK45_DEVICE_ID
#define GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK23_DEVICE_ID XPAR_GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK23_DEVICE_ID
#define GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK01_DEVICE_ID XPAR_GPIO_FASTSCANOUT_GPIO_PARALLEL_OUT_BANK01_DEVICE_ID

#define GPIO_FASTSCAN_SCAN_DONE_DEVICE_ID XPAR_GPIO_FASTSCAN_SCAN_DONE_DEVICE_ID

#define maxBufferSize 32
// TOPSW GPIOs
#define GPIO_TOPSW_DEVICE_ID XPAR_GPIO_TOPSW_DEVICE_ID

// GPIO_HYBRIDPIPELINETOPSW_VARS
#define GPIO_HYBRIDPIPELINETOPSW_VARS_DEVICE_ID  XPAR_GPIO_HYBRIDPIPELINETOPSW_VARS_GPIO_DEVICE_ID


// IMC HARDWARE HARDCODING //
#define XDL_HW 8 // number of image datalines
#define XBL_HW 1 // number of image bits
#define WBL_HW 4 // number of weight bls
#define WDL_HW 66 // number of weight DLs
#define RDL_L_HW
#define RDL_H_HW
#define ADC_nbits 7
#define Addertree_output_nbits 11

extern u8 map_to_72[WDL_HW];
extern u8 ADC_outs_total[72];
extern u8 DL_nums_total[72];
extern u8 DLs_ref[2][8];
extern u8 IMG_DL_nums[8];
extern int WWL_1024[1023];
extern float ADC_bins[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
extern float ADC_bins_2bitX[XDL_HW][1][WBL_HW][WDL_HW];
extern uint16_t IMC_Out_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
extern u16 Addertree_dec[XDL_HW][WBL_HW];
extern float central_ref[XDL_HW];
extern float MAC[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
extern float m_ADC[92];
extern float c_ADC[92];
extern float voltage_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
extern float voltage_xadc_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
extern float voltage_see_2bitX[XDL_HW][1][WBL_HW][WDL_HW];
extern float voltage_xadc_2bitX_see[XDL_HW][1][WBL_HW][WDL_HW];
extern float m2[72];
extern float m1R_1X[72];
extern float m1R_2X[72];
extern float m1R_4X[72];
extern float LT_new[72];

curr_obj goldenGate(float**** IMC_Out, mode_type mode, u8 slopeSetting, float* RefMax, int c, curr_obj currObj);
curr_obj goldenGate_XADC(float**** IMC_Out_XADC,float**** IMC_Out, mode_type mode, u8 slopeSetting, float* RefMax, int c, curr_obj currObj);

curr_obj goldenGate_2bitX(float**** IMC_Out, mode_type mode, u8 slopeSetting, float* RefMax, int c, curr_obj currObj);
curr_obj goldenGate_2bitX_XADC(float**** IMC_Out_XADC,float**** IMC_Out, mode_type mode, u8 slopeSetting, float* RefMax, int c, curr_obj currObj);

float ADCbin_to_mapped_bin( float ADC_bin, int wdl );
float mapped_bin_to_voltage( float ADC_mapped_bin );
float voltage_to_MAC( float voltage, u8 mode );
curr_obj store_SRAM_lin_test(float** X_Mat, uint16_t X_Batch, uint16_t X_Features, float**W_Mat, uint16_t W_Features,float* Bias,
             uint16_t WordLines, uint16_t XDL, uint16_t XBL, uint16_t WDL, uint16_t WBL, bool HW_LIN, float**** Ref,
			 float*** RefB, float* RefMax, float** LIN_Out, curr_obj currObj);
FRESULT WriteFile_custom(char *FileName, u32 size, void *SourceAddress);
int WriteFile_custom2(char *FileName, u32 size, void *SourceAddress, FIL fil, FRESULT rc, UINT br);
FRESULT WriteFile_fseekappend(char *FileName, u32 size, void *SourceAddress);
XUartPs initialise_uart();
void sendWordData( uint16_t *returnWords, int numWords, XUartPs myUart );
void sendDWordData( uint32_t *returnWords, int numWords, XUartPs myUart );
//void sendByteData( uint8_t *returnBytes, int numBytes, XUartPs myUart );

scan_out perform_hybridIMC_ADCSWEEP_FPGA(curr_obj currObj,u8 bitLine, u8 IMG);

#define power_supply_start_word 256
#define innerloop_start_word 257
#define convolvelinear_end_word 12258
#define convolvelinear_batchend_word 12259
#define layers_end_word 12260
#define layers_continue_word 12261
#define syncdata_val 125

void ReplicateIMCTiming(uint8_t**** W_BitMat,uint16_t oc ,uint16_t WDL, uint16_t c, uint16_t wb, uint16_t WBL,
						uint8_t***** X_BitMat,uint16_t b ,uint16_t XDL,uint16_t s, uint16_t xb, uint16_t XBL, uint16_t wordlines, float**** IMC_Out);

//ADDED BY ASHWIN(ENDS)
