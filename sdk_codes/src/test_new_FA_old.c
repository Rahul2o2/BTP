/*
 * test_new4.c
 *
 *  Created on: Mar 13, 2021
 *      Author: Sri Hari
 */

//** 4 bytes after 4 bytes transmission, receiving ; extra padded char(124) also sent  back to uart. **//

#include <custom_h.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xuartps.h"
#include "xparameters.h"
#include "sleep.h"

#include "xgpio.h"

////#define BAUDRATE 57600
//#define BAUDRATE 115200
//#define GPIO_PCB_32INPUTS_ID XPAR_GPIO_PCB_32INPUTS_DEVICE_ID
//#define GPIO_PCB_32OUTPUTS_ID XPAR_GPIO_PCB_32OUTPUTS_DEVICE_ID
//#define GPIO_PCB_32INPUTS_1_ID XPAR_GPIO_PCB_32INPUTS_1_DEVICE_ID
//#define GPIO_PCB_32OUTPUTS_1_ID XPAR_GPIO_PCB_32OUTPUTS_1_DEVICE_ID
//#define GPIO_PCB_32INPUTS_2_ID XPAR_GPIO_PCB_32INPUTS_2_DEVICE_ID
//#define GPIO_PCB_32OUTPUTS_2_ID XPAR_GPIO_PCB_32OUTPUTS_2_DEVICE_ID
//#define GPIO_PCB_32OUTPUTS_100_ID XPAR_GPIO_PCB_SCQOUT_OUTPUTS_100_DEVICE_ID
//#define GPIO_PCB_HS_ID XPAR_GPIO_PCB_HARDWARESTOP_DEVICE_ID
//
//#define GPIO_MODE_READ_WRITE_ID XPAR_GPIO_MODE_READ_WRITE_DEVICE_ID
//
////READ TIMING GPIO IDS
//#define READ_TIMING_1_MODE_R_WB_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_1_MODE_R_WB_START_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_ID XPAR_GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_DEVICE_ID
//#define GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_ID XPAR_GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_DEVICE_ID
//
////WRITE TIMING GPIO IDS
//#define GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_DEVICE_ID
//#define GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_DEVICE_ID
//#define GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_DEVICE_ID
//#define GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_DEVICE_ID
//#define GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_DEVICE_ID
//#define GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_ID XPAR_GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_DEVICE_ID
//
////IMC Mode GPIO IDS
//#define GPIO_MODE_IMC_ID XPAR_GPIO_MODE_IMC_DEVICE_ID
//
////HYBRID MODE IMC TIMING GPIO IDS
//#define GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_DEVICE_ID
//#define GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_ID XPAR_GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_DEVICE_ID
//
////INTERNAL MODE IMC TIMING GPIO
//#define GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID XPAR_GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DEVICE_ID

// Extra Added for 2ADC starts here
// Vacation Thesis: https://ethz.ch/content/dam/ethz/special-interest/phys/quantum-electronics/tiqi-dam/documents/semester_theses/vacationthesis-Pascal_Engeler
// pg 13 for ADC stuff
//#define XADC_DEVICE_ID XPAR_PS7_XADC_0_DEVICE_ID
// Extra Added for 2ADC ends here

#define maxBufferSize 32

int maind()
{

// Extra Added for 2ADC ends here
//
	//char imageBuffer[126*10000];//126*8=1008

	int Status;

	Status = SD_Init(&fatfs);
//// LOAD TOP IMAGES (STARTS)
//	char *fileName_t = "10kt.bin" ;
//	Status = ReadFile( fileName_t,(u32)&imageBuffer);


//	//Read the .bin and update the input image dimensions.
//	char* imageBuffer_xdim = (char*)calloc(16,sizeof(char));
//	char fileName_xm[] = "XDim.bin";
//	Status = ReadFile( fileName_xm,(u32)imageBuffer_xdim);

	//char fileName_wd[];
    int R=97,C=1092;
	int input_size=(R*C);
    char *wdata_0 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW0.bin",(u32)wdata_0);


	char *wdata_1 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW1.bin",(u32)wdata_1);


	char *wdata_2 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW2.bin",(u32)wdata_2);


	char *wdata_3 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW3.bin",(u32)wdata_3);


	char *wdata_4 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW4.bin",(u32)wdata_4);


	char *wdata_5 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW5.bin",(u32)wdata_5);


	char *wdata_6 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile( "dW6.bin",(u32)wdata_6);


	char *wdata_7 = (char *)calloc(input_size, sizeof(char));
	Status = ReadFile(  "dW7.bin",(u32)wdata_7);

//	Status=SD_Eject(&fatfs);

//	char *imageSee;imageSee=imageBuffer;
//
//	u8 images_t[10000][126];
//	int tempVar;
//	u8 *see;
//		for(int il=0;il<10000;il++)
//		{
//			for(int jl=0;jl<126;jl++)
//			{
//				tempVar=imageBuffer[il*126+jl];
//				images_t[il][jl]=tempVar;
//
//			}
//			see=&(images_t[il][0]);
//		}
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

//XGpio GPIO_PCB_32INPUTS_Inst;
//XGpio_Initialize(&GPIO_PCB_32INPUTS_Inst, GPIO_PCB_32INPUTS_ID);
//XGpio GPIO_PCB_32OUTPUTS_Inst;
//XGpio_Initialize(&GPIO_PCB_32OUTPUTS_Inst, GPIO_PCB_32OUTPUTS_ID);
//XGpio GPIO_PCB_32INPUTS_1_Inst;
//XGpio_Initialize(&GPIO_PCB_32INPUTS_1_Inst, GPIO_PCB_32INPUTS_1_ID);
//XGpio GPIO_PCB_32OUTPUTS_1_Inst;
//XGpio_Initialize(&GPIO_PCB_32OUTPUTS_1_Inst, GPIO_PCB_32OUTPUTS_1_ID);
//XGpio GPIO_PCB_32INPUTS_2_Inst;
//XGpio_Initialize(&GPIO_PCB_32INPUTS_2_Inst, GPIO_PCB_32INPUTS_2_ID);
//XGpio GPIO_PCB_32OUTPUTS_2_Inst;
//XGpio_Initialize(&GPIO_PCB_32OUTPUTS_2_Inst, GPIO_PCB_32OUTPUTS_2_ID);
//XGpio GPIO_PCB_32OUTPUTS_100_Inst;
//XGpio_Initialize(&GPIO_PCB_32OUTPUTS_100_Inst, GPIO_PCB_32OUTPUTS_100_ID);
//XGpio GPIO_PCB_HS_Inst;
//XGpio_Initialize(&GPIO_PCB_HS_Inst, GPIO_PCB_HS_ID);
//
////GPIO_MODE_READ_WRITE
//XGpio GPIO_MODE_READ_WRITE_Inst;
//XGpio_Initialize(&GPIO_MODE_READ_WRITE_Inst, GPIO_MODE_READ_WRITE_ID);
//u8 GPIO_MODE_READ_WRITE_DATA=0;//0=C-INPUT 1=MODE_WRITE; 2=MODE_READ; >2 = RESTRICTED-DO NOT USE// MODE_C set as default
//XGpio_DiscreteWrite(&GPIO_MODE_READ_WRITE_Inst,1,GPIO_MODE_READ_WRITE_DATA);
//
////READ TIMING RELATED GPIOS
//XGpio READ_TIMING_1_MODE_R_WB_START_READ_GPIO_Inst;
//XGpio_Initialize(&READ_TIMING_1_MODE_R_WB_START_READ_GPIO_Inst, READ_TIMING_1_MODE_R_WB_START_READ_GPIO_ID);
//u8 READ_TIMING_1_MODE_R_WB_START_READ_GPIO_DATA=0;
//XGpio_DiscreteWrite(&READ_TIMING_1_MODE_R_WB_START_READ_GPIO_Inst,1,READ_TIMING_1_MODE_R_WB_START_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_Inst, GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_DATA=0;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_Inst,1,GPIO_READ_TIMING_2_MODE_R_WB_STOP_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_Inst, GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_DATA=6;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_Inst,1,GPIO_READ_TIMING_3_PRE_DR_EN_START_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_Inst, GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_DATA=10;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_Inst,1,GPIO_READ_TIMING_4_PRE_DR_EN_STOP_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_Inst, GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_DATA=6;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_Inst,1,GPIO_READ_TIMING_5_WWL_EN_START_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_Inst, GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_DATA=8;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_Inst,1,GPIO_READ_TIMING_6_WWL_EN_STOP_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_Inst, GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_DATA=7;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_Inst,1,GPIO_READ_TIMING_7_RW_SA_EN_START_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_Inst, GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_DATA=10;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_Inst,1,GPIO_READ_TIMING_8_RW_SA_EN_STOP_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_Inst, GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_DATA=10;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_Inst,1,GPIO_READ_TIMING_9_CLK_B_START_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_Inst, GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_DATA=11;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_Inst,1,GPIO_READ_TIMING_10_CLK_B_STOP_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_Inst, GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_DATA=8;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_Inst,1,GPIO_READ_TIMING_11_CLK_C_START_READ_GPIO_DATA);
//
//XGpio GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_Inst;
//XGpio_Initialize(&GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_Inst, GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_ID);
//u8 GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_DATA=9;
//XGpio_DiscreteWrite(&GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_Inst,1,GPIO_READ_TIMING_12_CLK_C_STOP_READ_GPIO_DATA);
//
////WRITE TIMING RELATED GPIOS
//
//XGpio GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_Inst;
//XGpio_Initialize(&GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_Inst, GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_ID);
//u8 GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_DATA=6;
//XGpio_DiscreteWrite(&GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_Inst,1,GPIO_WRITE_TIMING_1_MODE_R_WB_START_WRITE_GPIO_DATA);
//
//XGpio GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_Inst;
//XGpio_Initialize(&GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_Inst, GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_ID);
//u8 GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_DATA=7;
//XGpio_DiscreteWrite(&GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_Inst,1,GPIO_WRITE_TIMING_2_MODE_R_WB_STOP_WRITE_GPIO_DATA);
//
//XGpio GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_Inst;
//XGpio_Initialize(&GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_Inst, GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_ID);
//u8 GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_DATA=6;
//XGpio_DiscreteWrite(&GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_Inst,1,GPIO_WRITE_TIMING_3_PRE_DR_EN_START_WRITE_GPIO_DATA);
//
//XGpio GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_Inst;
//XGpio_Initialize(&GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_Inst, GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_ID);
//u8 GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_DATA=7;
//XGpio_DiscreteWrite(&GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_Inst,1,GPIO_WRITE_TIMING_4_PRE_DR_EN_STOP_WRITE_GPIO_DATA);
//
//XGpio GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_Inst;
//XGpio_Initialize(&GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_Inst, GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_ID);
//u8 GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_DATA=6;
//XGpio_DiscreteWrite(&GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_Inst,1,GPIO_WRITE_TIMING_5_WWL_EN_START_WRITE_GPIO_DATA);
//
//XGpio GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_Inst;
//XGpio_Initialize(&GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_Inst, GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_ID);
//u8 GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_DATA=7;
//XGpio_DiscreteWrite(&GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_Inst,1,GPIO_WRITE_TIMING_6_WWL_EN_STOP_WRITE_GPIO_DATA);
//
//
////HYBRID MODE RELATED TIMING GPIOS
//
//
//XGpio GPIO_MODE_IMC_Inst;
//XGpio_Initialize(&GPIO_MODE_IMC_Inst, GPIO_MODE_IMC_ID);
//u8 GPIO_MODE_IMC_DATA=0;
//XGpio_DiscreteWrite(&GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
//
//XGpio GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA=7;//7 is exact//first try =40
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA=31;//15 is exact//first try =120//second try=31
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA-1;//14 is exact
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_3_CLK_DFF_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_4_CLK_DFF_EXT_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA;//2 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_5_DATACM_EN_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_6_DATACM_EN_EXT_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA;//2 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_7_DATASUB_EN_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_8_DATASUB_EN_EXT_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_DATA=1;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_9_RESET_GLOBALTIMER_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_10_RESET_GLOBALTIMER_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA;//1 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_11_RWL_EN_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_12_RWL_EN_EXT_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA;//2 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_13_REFCM_EN_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_14_REFCM_EN_EXT_STOP_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_1_ADCSTART_EXT_START_HYBRID_GPIO_DATA;//2 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_15_REFSUB_EN_EXT_START_HYBRID_GPIO_DATA);
//
//XGpio GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_Inst;
//XGpio_Initialize(&GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_Inst, GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_ID);
//u8 GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_DATA=GPIO_HYBRID_TIMING_2_ADCSTART_EXT_STOP_HYBRID_GPIO_DATA;//15 is exact;
//XGpio_DiscreteWrite(&GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_Inst,1,GPIO_HYBRID_TIMING_16_REFSUB_EN_EXT_STOP_HYBRID_GPIO_DATA);
//
////INTERNAL MODE RELATED TIMING GPIOS
//XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
//XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
//u8 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=30;
//XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);



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

//currObj.GPIO_PCB_32INPUTS_Inst=GPIO_PCB_32INPUTS_Inst;
//currObj.GPIO_PCB_32OUTPUTS_Inst=GPIO_PCB_32OUTPUTS_Inst;
//currObj.GPIO_PCB_32INPUTS_1_Inst=GPIO_PCB_32INPUTS_1_Inst;
//currObj.GPIO_PCB_32OUTPUTS_1_Inst=GPIO_PCB_32OUTPUTS_1_Inst;
//currObj.GPIO_PCB_32INPUTS_2_Inst=GPIO_PCB_32INPUTS_2_Inst;
//currObj.GPIO_PCB_32OUTPUTS_2_Inst=GPIO_PCB_32OUTPUTS_2_Inst;
//currObj.GPIO_PCB_32OUTPUTS_100_Inst=GPIO_PCB_32OUTPUTS_100_Inst;
//currObj.GPIO_MODE_READ_WRITE_Inst=GPIO_MODE_READ_WRITE_Inst;
//currObj.GPIO_MODE_IMC_Inst=GPIO_MODE_IMC_Inst;
//currObj.GPIO_PCB_HS_Inst=GPIO_PCB_HS_Inst;
//
///// stringCurr initialise SOURCE : https://docs.google.com/spreadsheets/d/1qA65VvTDkKlL3Ou3-Us68UFqh0Ad6-piOz4uEJEVTds/edit#gid=0
//currObj.stringCurr_DEFAULT[FPGA_RESET]=1;//RWL_EN_EXT
//currObj.stringCurr_DEFAULT[RWL_EN_EXT]=0;//RWL_EN_EXT
//currObj.stringCurr_DEFAULT[SC_DIN]=0;//SC_DIN
//currObj.stringCurr_DEFAULT[ScanSel_3]=0;//ScanSel_3
//currObj.stringCurr_DEFAULT[ScanSel_2]=0;//ScanSel_2
//currObj.stringCurr_DEFAULT[ScanSel_1]=0;//ScanSel_1
//currObj.stringCurr_DEFAULT[ScanSel_0]=0;//ScanSel_0
//currObj.stringCurr_DEFAULT[CLK_A]=0;//CLK_A
//currObj.stringCurr_DEFAULT[CLK_B]=0;//CLK_B
//currObj.stringCurr_DEFAULT[CLK_C]=0;//CLK_C
//currObj.stringCurr_DEFAULT[OEb_1]=0;//OEb_1
//currObj.stringCurr_DEFAULT[RBL_PRE_EN_B]=0;//RBL_PRE_EN_B
//currObj.stringCurr_DEFAULT[OEb_2]=0;//OEb_2
//currObj.stringCurr_DEFAULT[WWL_EN]=0;//WWL_EN
//currObj.stringCurr_DEFAULT[OEb_3]=0;//OEb_3
//currObj.stringCurr_DEFAULT[IMG_Sel_Dec_2]=0;//IMG_Sel_Dec_2
//currObj.stringCurr_DEFAULT[OEb_4]=0;//OEb_4
//currObj.stringCurr_DEFAULT[IMG_Sel_Dec_1]=0;//IMG_Sel_Dec_1
//currObj.stringCurr_DEFAULT[OEb_9]=0;//OEb_9
//currObj.stringCurr_DEFAULT[IMG_Sel_Dec_0]=0;//IMG_Sel_Dec_0
//currObj.stringCurr_DEFAULT[OEb_10]=0;//OEb_10
//currObj.stringCurr_DEFAULT[RW_SA_EN]=0;//RW_SA_EN
//currObj.stringCurr_DEFAULT[ADC0_S0]=0;//ADC0_S0
//currObj.stringCurr_DEFAULT[SUBVoltPADConnect]=0;//SUBVoltPADConnect
//currObj.stringCurr_DEFAULT[ADC0_S1]=0;//ADC0_S1
//currObj.stringCurr_DEFAULT[MODE_R_WB]=1;//MODE_R_WB
//currObj.stringCurr_DEFAULT[PRE_DR_EN]=0;//PRE_DR_EN
//currObj.stringCurr_DEFAULT[unused0]=0;//unused0
//currObj.stringCurr_DEFAULT[unused1]=0;//unused1
//currObj.stringCurr_DEFAULT[CS_0]=0;//CS_0
//currObj.stringCurr_DEFAULT[CS_1]=0;//CS_1
//currObj.stringCurr_DEFAULT[ADC1_S0]=0;//ADC1_S0
//currObj.stringCurr_DEFAULT[ADC1_S1]=0;//ADC1_S1
//currObj.stringCurr_DEFAULT[BL_Sel_Dec_EXT_0]=0;//BL_Sel_Dec_EXT_0
//currObj.stringCurr_DEFAULT[BL_Sel_Dec_EXT_1]=0;//BL_Sel_Dec_EXT_1
//currObj.stringCurr_DEFAULT[unused2]=0;//unused2
//currObj.stringCurr_DEFAULT[unused3]=0;//unused3
//currObj.stringCurr_DEFAULT[unused4]=0;//unused4
//currObj.stringCurr_DEFAULT[unused5]=0;//unused5
//currObj.stringCurr_DEFAULT[RESET_GLOBALTIMER]=1;//RESET_GLOBALTIMER
//currObj.stringCurr_DEFAULT[unused6]=0;//unused6
//currObj.stringCurr_DEFAULT[CLK_DFF_EXT]=0;//CLK_DFF_EXT
//currObj.stringCurr_DEFAULT[unused7]=0;//unused7
//currObj.stringCurr_DEFAULT[DataCM_EN_EXT]=0;//DataCM_EN_EXT
//currObj.stringCurr_DEFAULT[OEb_5]=0;//OEb_5
//currObj.stringCurr_DEFAULT[DataSUB_EN_EXT]=0;//DataSUB_EN_EXT
//currObj.stringCurr_DEFAULT[OEb_6]=0;//OEb_6
//currObj.stringCurr_DEFAULT[Reset_SARLOGIC_EXT]=1;//Reset_SARLOGIC_EXT
//currObj.stringCurr_DEFAULT[unused8]=0;//unused8
//currObj.stringCurr_DEFAULT[DataResis_EN]=1;//DataResis_EN
//currObj.stringCurr_DEFAULT[unused9]=0;//unused9
//currObj.stringCurr_DEFAULT[RefResis_EN]=1;//RefResis_EN
//currObj.stringCurr_DEFAULT[unused10]=0;//unused10
//currObj.stringCurr_DEFAULT[CLK_SARLOGIC_IN_EXT]=0;//CLK_SARLOGIC_IN_EXT
//currObj.stringCurr_DEFAULT[unused11]=0;//unused11
//currObj.stringCurr_DEFAULT[unused12]=0;//unused12
//currObj.stringCurr_DEFAULT[SA_EN_EXT]=0;//SA_EN_EXT
//currObj.stringCurr_DEFAULT[AllVIN_n_EXT]=0;//AllVIN_n_EXT
//currObj.stringCurr_DEFAULT[RefSUB_EN_EXT]=0;//RefSUB_EN_EXT
//currObj.stringCurr_DEFAULT[unused13]=0;//unused13
//currObj.stringCurr_DEFAULT[unused14]=0;//unused14
//currObj.stringCurr_DEFAULT[unused15]=0;//unused15
//currObj.stringCurr_DEFAULT[RefCM_EN_EXT]=0;//RefCM_EN_EXT
//currObj.stringCurr_DEFAULT[unused16]=0;//unused16
//currObj.stringCurr_DEFAULT[OEb_7]=0;//OEb_7
//currObj.stringCurr_DEFAULT[ADCSTART_EXT]=0;//ADCSTART_EXT
//currObj.stringCurr_DEFAULT[PRE_VCM_EXT]=0;//PRE_VCM_EXT
//currObj.stringCurr_DEFAULT[OEb_8]=0;//OEb_8

currObj=initialisation_code(currObj);

scan_out scanOut;
scan_out_internal scanoutInternal;
int signals_len;
int k=0,i=0,marker_pos=0;
//Added by Ashwin Balagopal(Starts)
u32 receivedBytes2;
int fileSize_return;
u8 syncdata;
u8 scanChain;
u8 abcd[200000];
//u8 counter_data ;
u32 totalReceivedBytes2;//***changed

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
currObj=defaultState(currObj);
while(1)
	{
	totalTransmittedBytes=0;
	totalFunctionCallBytes=0;
	totalReceivedBytes2=0;
	totalReceivedBytes=0;
	syncdata=99;
	//** Srinidhi added part starts
	totalSentBytes=0;
	//** Srinidhi added part ends
	u8 numSentBytesAtATime = 16 ;
	do{

		ttlSentBytes=0;

		receivedBytes2=0;

		while(ttlSentBytes < numSentBytesAtATime) {
			// ADDED BY ASHWIN STARTS
			u8 rcvData;
			// ADDED BY ASHWIN ENDS
			receivedBytes=0;
			// ORIGINAL STATEMENTS START
			receivedBytes = XUartPs_Recv( &myUart, &(abcd[totalReceivedBytes]), 1 ) ;// Non-Blocking
			// ORIGINAL STATEMENTS END
			// CHANGED STATEMENTS START
			// receivedBytes = XUartPs_Recv( &myUart, rcvData, 1 );// Non-Blocking
			if( receivedBytes == 1 ) {
				rcvData = abcd[totalReceivedBytes] ;
			}
			// CHANGED STATEMENTS END
			ttlSentBytes+=receivedBytes;
	/*		if(abcd[totalFunctionCallBytes]!=124)
			totalFunctionCallBytes+=receivedBytes;//increment totalFunctionCallBytes only when '7C' is not received. Thus '7C' is exposed for overwriting.*/

			totalReceivedBytes+=receivedBytes;

		}
		//<transmit received data back>
		flag=0; //**
		cSentBytes=0;
		while(cSentBytes < numSentBytesAtATime)
		{
	//		flag=0; //**
			sentBytes=0;
			sentBytes=XUartPs_Send( &myUart, &abcd[totalSentBytes], 1 );
			cSentBytes+=sentBytes;
			totalSentBytes+=sentBytes;// 0<totalSentBytes <= totalFunctionCallBytes
		if(abcd[totalSentBytes-1]==127)
			{	flag=1;
				totalFunctionCallBytes=totalSentBytes;
				//break; //**
			}
		}
	}while(flag==0);//***
	/*
	* Call the function (case) and then transmit back outputData
	* function call must return fileSize_transmit, outputData
	* */

	u8 function_index= abcd[0]; //abcd[totalFunctionCallBytes-1]=127
	switch (function_index)
		{
					case 64:
						//bitsSet ( added by Ashwin )
						//uart function call:64 signals parameter_marker(126) values(u8) stopByte(127)
						//sig: clka clkb
						//values:011000000
						//a=bitsSet(abcd,totalFunctionCallBytes);


						//64 signals[0].. signals[26] parameter_marker(126) values(u8)[0]..values[4] stopByte(127)
						//marker_pos;
					{
						for(i=1;i<totalFunctionCallBytes;i++)
						{
							if(abcd[i]==126)
							{
								marker_pos=i;
								break;
							}
						}
						signals_len=marker_pos-1;
						u8 signals[signals_len];
						//long temp=sizeof(signals)/sizeof(signals[0]);
						u8 values_bytes[signals_len];
						//temp=sizeof(values_bytes)/sizeof(values_bytes[0]);
						int j=0,l=0;


						for(k=0;k<signals_len;k++)
						{
							signals[k]=abcd[k+1];

						}


						u8 values[(totalFunctionCallBytes-2-marker_pos)];
						seven_bits sevenBitsObj;

							for(k=0;k<(totalFunctionCallBytes-2-marker_pos);k++)
												{
													values[k]=abcd[k+marker_pos+1];//
													sevenBitsObj=get7BitsFrom1Byte(values[k]);
													for(j=0;j<6;j++)
													{
														if(l<signals_len)
															{
																	values_bytes[l]=sevenBitsObj.bits[5-j];
																	l++;
															}
													}
												}


						{
							int signals_len = sizeof(signals) / sizeof(signals[0]) ;
								currObj=bitsSet(currObj, signals, signals_len, values_bytes);
						}
						receivedBytes2=0;
						do {
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);
						transmittedBytes=0;


						u8 returnBytes=127;
						fileSize_return=1;

						while(totalTransmittedBytes < fileSize_return){
						//transmit the output
							transmittedBytes = XUartPs_Send(&myUart,&returnBytes,1);
							totalTransmittedBytes+=transmittedBytes;}



						break;
					}
					case 73:
					{
						//defaultState
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=65 stopByte=abcd[end]=127

						currObj=defaultState(currObj);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}
					case 67:
					{
						//scanSelect
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=67 scanChain=abcd[1] stopByte=abcd[2]=127
						scanChain=abcd[1];
						currObj=scanSelect(currObj,scanChain);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;

					}

					case 68:
					{
						//scanInVector
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=68 scanChain=abcd[1] vecScanIn=abcd[2:end-2] numPadZeroes_vecScanIn=abcd[end-1] stopByte=abcd[end]=127
						//a=bitsSet(abcd,totalFunctionCallBytes);
						int len_vecScanIn=totalFunctionCallBytes-4;
						u8 vecScanIn[len_vecScanIn];
						u8 scanChain=abcd[1];
						for(k=0;k<len_vecScanIn;k++)
						{
							vecScanIn[k]=abcd[k+2];
						}
						numPadZeroes_vecScanIn=abcd[totalFunctionCallBytes-2];
						currObj=scanInVector(currObj, vecScanIn, len_vecScanIn, numPadZeroes_vecScanIn, scanChain);

						//back to main
						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

					case 69:
					{
						//scanOutVector
						//authored by: Ashwin Balagopal S, Balaji V
						//uart function call: fnNameByte=abcd[0]=69 scanChain=abcd[1] stopByte=abcd[2]=127
						//a=bitsSet(abcd,totalFunctionCallBytes);
						scanChain=abcd[1];
						scanOut=scanOutVector(currObj,scanChain);//MARKER
						//back to main
						currObj=scanOut.currObj;
						receivedBytes2=0;
						switch(scanChain)
						{
							case SA_SC:
							{
								scan_len=97;
								break;
							}
							case ADC_SC:
							{
								scan_len=644;
								break;
							}
							case AdderTree_SC:
							{
								scan_len=176;
								break;
							}
							case SARTIMING_OUT_SC:
							{
								scan_len=26;
								break;
							}
							case WRITE_2_SC:
							{
								scan_len=97;
								break;
							}
							case WRITE_1_SC:
							{
								scan_len=97;
								break;
							}
							case WRITE_0_SC:
							{
								scan_len=97;
								break;
							}
							case IMC_RBL_SC:
							{
								scan_len=88;
								break;
							}
							case IMC_VOut_SC:
							{
								scan_len=88;
								break;
							}
							case WWL_SC:
							{
								scan_len=1160;
								break;
							}
							case FLOATADC_SC:
							{
								scan_len=92;
								break;
							}
							case SARTIMING_IN_SC:
							{
								scan_len=110;
								break;
							}
							case ControlPath_SC:
							{
								scan_len=17;
								break;
							}
						}

						u8 returnBytes[scan_len];

						switch(scanChain)
						{
							case SA_SC:
							{
								for(k=0;k<scan_len;k++)
								{
									returnBytes[k]=scanOut.scanOutBits_SA_SC[k];
								}
								break;
							}
							case ADC_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
								break;
							}
							case AdderTree_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_AdderTree_SC[k];
								break;
							}
							case SARTIMING_OUT_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_SARTIMING_OUT_SC[k];
								break;
							}
							case WRITE_2_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_WRITE_2_SC[k];
								break;
							}
							case WRITE_1_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_WRITE_1_SC[k];
								break;
							}
							case WRITE_0_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_WRITE_0_SC[k];
								break;
							}
							case IMC_RBL_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_IMC_RBL_SC[k];
								break;
							}
							case IMC_VOut_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_IMC_VOut_SC[k];
								break;
							}
							case WWL_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_WWL_SC[k];
								break;
							}
							case FLOATADC_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_FLOATADC_SC[k];
								break;
							}
							case SARTIMING_IN_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_SARTIMING_IN_SC[k];
								break;
							}
							case ControlPath_SC:
							{
								for(k=0;k<scan_len;k++)
									returnBytes[k]=scanOut.scanOutBits_ControlPath_SC[k];
								break;
							}

						}

						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
	//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;
					}

					case 70:
					{
						//bitLineSelect
						//authored by: Ashwin Balagopal S, Balaji V
						//uart function call: fnNameByte=abcd[0]=70 bitLine=abcd[1] stopByte=abcd[2]=127

						bitLine=abcd[1];
						currObj=bitLineSelect(currObj,bitLine);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

					case 72:
					{
						//pulse
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=72 signals=abcd[1:signals_len] currentValues=abcd[signals_len+1:end-1] stopByte=abcd[end]=127

						int signals_len=(totalFunctionCallBytes-2)/2;
						u8 signals[signals_len];
						u8 currentValues[signals_len];
						for(k=0;k<signals_len;k++)
						{
							signals[k]=abcd[k+1];
							currentValues[k]=abcd[k+signals_len+1];
						}
						currObj=pulse(currObj, signals, currentValues, signals_len);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

					case 74:
					{
						//scanInWriteScanChains_dataOnly
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=74 data=abcd[1:N] numPadZeroes_data=abcd[N+1] stopByte=abcd[end]=127
						int data_len=(totalFunctionCallBytes-3);
						u8 data[data_len];
						for(k=0;k<data_len;k++)
						{
							data[k]=abcd[1+k];
						}
						u8 numPadZeroes_data=abcd[data_len+1];
						currObj=scanInWriteScanChains_dataOnly( currObj, data, data_len, numPadZeroes_data );
						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}
					case 75:
					{
						//scanInWriteScanChains_maskOnly
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=74 mask=abcd[1:N] numPadZeroes_mask=abcd[N+1] stopByte=abcd[end]=127
						int mask_len=(totalFunctionCallBytes-3);
						u8 mask[mask_len];
						for(k=0;k<mask_len;k++)
						{
							mask[k]=abcd[1+k];
						}
						u8 numPadZeroes_mask=abcd[mask_len+1];
						currObj=scanInWriteScanChains_maskOnly( currObj, mask, mask_len, numPadZeroes_mask );
						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

					case 76:
					{
						//scanInWriteScanChains
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=76 data=abcd[1:N] numPadZeroes_data=abcd[N+1] mask=abcd[N+2:2N+1] numPadZeroes_mask=abcd[2N+2] stopByte=abcd[2N+3]=127
						// OLD(Starts)
	//					int data_len=(totalFunctionCallBytes-4)/2;
	//					u8 data[data_len];
	//					for(k=0;k<data_len;k++)
	//					{
	//						data[k]=abcd[1+k];
	//					}
	//					u8 numPadZeroes_data=abcd[data_len+1];
	//					currObj=scanInWriteScanChains_dataOnly( currObj, data, data_len, numPadZeroes_data );
	//
	//					int mask_len=data_len;
	//					u8 mask[mask_len];
	//					for(k=0;k<mask_len;k++)
	//					{
	//						mask[k]=abcd[data_len+2+k];
	//					}
	//					u8 numPadZeroes_mask=abcd[data_len+1+mask_len+1];
	//					currObj=scanInWriteScanChains_maskOnly( currObj, mask, mask_len, numPadZeroes_mask );
						// OLD(Ends)
						// NEW (Starts)
						int data_len=(totalFunctionCallBytes-4)/2;
						u8 data[data_len];
						for(k=0;k<data_len;k++)
						{
							data[k]=abcd[1+k];
						}
						u8 numPadZeroes_data=abcd[data_len+1];
	//					currObj=scanInWriteScanChains_dataOnly( currObj, data, data_len, numPadZeroes_data );

						int mask_len=data_len;
						u8 mask[mask_len];
						for(k=0;k<mask_len;k++)
						{
							mask[k]=abcd[data_len+2+k];
						}
						u8 numPadZeroes_mask=abcd[data_len+1+mask_len+1];
	//					currObj=scanInWriteScanChains_maskOnly( currObj, mask, mask_len, numPadZeroes_mask );
						currObj=scanInWriteScanChains(currObj, data, data_len, numPadZeroes_data, mask, mask_len, numPadZeroes_mask);
						// NEW (Ends)

						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

					case 77:
					{
						//loadBitlinetoCell_FPGA
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=77 bitLines=abcd[1:end-1] stopByte=abcd[end]=127
						int bitLines_len=(totalFunctionCallBytes-2);
						u8 bitLines[bitLines_len];
						for(k=0;k<bitLines_len;k++)
						{
							bitLines[k]=abcd[k+1];
						}
						currObj=loadBitlinetoCell_FPGA( currObj, bitLines, bitLines_len );

						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}
					case 78:
					{
						//loadCelltoSACtr_FPGA
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=78 bitLines=abcd[1:end-1] stopByte=abcd[end]=127
						int bitLines_len=(totalFunctionCallBytes-2);
						u8 bitLines[bitLines_len];
						for(k=0;k<bitLines_len;k++)
						{
							bitLines[k]=abcd[k+1];
						}
						currObj=loadCelltoSACtr_FPGA( currObj, bitLines, bitLines_len );
//						{
//							//readback added ( by Ashwin ) for top-level debug starts here
//								u32 dataReadBack = XGpio_DiscreteRead(&currObj.PCB_32INPUTS_Inst, 1);
//								dataReadBack = emptyfunction(dataReadBack);
//							//readback added ( by Ashwin ) for top-level debug ends here
//						}
						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}
					case 80:
					{
						//write_onto_datalines
						//authored by: Balaji V, Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=80 data_DL=abcd[1:182](1092 bits compressed into 1092/6= 182 bytes) colSel=abcd[183:183+colSel_len-1] parameterMarkerByte=abcd[183+colSel_len]=126 DL_nums=abcd[183+colSel_len+1:183+colSel_len+1+(2*DL_nums_len)-1 stopByte=abcd[183+colSel_len+1+(2*DL_nums_len)]=127
						//1024+68=1092 bits;   1092 bits compressed into 1092/6 = 182 bytes
						int len_data_DL = 182 ;
						u8 data_DL[182];
						for( i=0 ; i<len_data_DL ; i++ )
						{
							data_DL[i] = abcd[1+i];
						}

						int mark = len_data_DL+1 ;
						i = mark ;

						u8 colSel[4];

						do
						{
							colSel[ i - mark ] = abcd[i];

							i++;

						}while(abcd[i]!=126); //i holds index of parameterMarkerByte

						int len_colSel = i - mark;



						int j = i+1;
						int k = 0;
						u8 DL_nums[97];

						do
						{
							DL_nums[k] = (abcd[j]<<6) + (abcd[j+1]);//handled by twoByteSplit in MATLAB
							j+=2;
							k++;
						}while(abcd[j]!=127);


						int len_DL_nums = k;

						//START of Additional functionality to morph the 1092 bits into 1160 bits with additional unconnected bits
						seven_bits sevenBitsObj;
						int dl_row=0;
						u8 data_DLbitwiseWithUnused[1160];
						int len_data_DLbitwiseWithUnused=1160;
						//INITIALISE UNUSED SCAN BITS TO ZERO HERE
						int unusedBits[68]={32,33,66,67,100,101,134,135,168,169,202,203,236,237,270,271,304,305,338,339,372,373,406,407,440,441,474,475,508,509,542,543,576,577,610,611,644,645,678,679,712,713,746,747,780,781,814,815,848,849,882,883,916,917,950,951,984,985,1018,1019,1052,1053,1086,1087,1088,1089,1158,1159};
						int len_unusedBits=68;
						for (i=0;i<len_unusedBits;i++)
						{
							data_DLbitwiseWithUnused[unusedBits[i]]=0;//MAKING ALL UNUSED BITS AS ZERO
						}
						//POPULATING USEFUL DATA

						for(i=0;i<len_data_DL;i++)
						{
							sevenBitsObj = get7BitsFrom1Byte(data_DL[i]);

							for(j=5;j>=0;j--)
							{
								if(sevenBitsObj.bits[j]==1)
								{
									data_DLbitwiseWithUnused[dl_row]=1;
								}
								else
								{
									data_DLbitwiseWithUnused[dl_row]=0;
								}
								// if dlRow is in the unusedbits then do dlrow++
								if (dl_row%34 == 31 && dl_row != 1085 && dl_row < 1089)
								{
									dl_row += 3;
								}
								else if (dl_row == 1085)
								{
								   dl_row += 5;
								}
								else if (dl_row != 1157)
								{
									dl_row++;
								}
							}

						}
							//END of Additional functionality to morph the 1092 bits into 1160 bits with additional unconnected bits
							//seven_bits sevenBitsObj;

							i=0,j=0,k=0,dl_row=0;
							int data_1_i=0,data_0_i=0;
							int data_1_row_indices[1160];
							int data_0_row_indices[1160];

							for(i=0;i<len_data_DLbitwiseWithUnused;i++)
							{
								if(data_DLbitwiseWithUnused[i]==1)
								{
									data_1_row_indices[data_1_i] = dl_row;
									data_1_i++;
								}
								else
								{
									data_0_row_indices[data_0_i] = dl_row;
									data_0_i++;
								}
								dl_row++;
							}

							int len_data_1_row_indices = data_1_i;
							int len_data_0_row_indices = data_0_i;



						currObj = write_onto_datalines(currObj,data_1_row_indices,len_data_1_row_indices,data_0_row_indices,len_data_0_row_indices,colSel,len_colSel,DL_nums,len_DL_nums);

						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						u8 returnBytes[1]={ 127 };
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}

						break;
				}

					case 83:
					{
						//adcRead
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=69 repeatAvg=abcd[1] stopByte=abcd[2]=127
						//a=bitsSet(abcd,totalFunctionCallBytes); a=adcRead(repeatAvg);
						u16 repeatAvg;
						repeatAvg=(u16)(abcd[1]);
						u8 returnBytes[200];//At max only 100 Averages can be taken
		//				1ADC STARTS HERE
		//				adcRead(returnBytes,repeatAvg);//MARKER
		//				1ADC ENDS HERE
		//				2ADC STARTS HERE
		//				adcRead(returnBytes,repeatAvg,&XADC_VPVN_Instance);//MARKER
		//				2ADC ENDS HERE
		//				LAB PDF CODE STARTS HERE
		//				TAKEN FROM https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxydGNsYWIwOXxneDoyYTgxZDNjMmVjZjRmZmVm
						adcRead( returnBytes, repeatAvg );//MARKER
		//				LAB PDF CODE ENDS HERE
						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=repeatAvg*4;// VPVN and AUX00 both contribute 2 bytes each, thus 4 bytes per repeatAvg

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
		//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;
					}

			   case 90:
						{
						//perform_RxOnRowColSel_FPGA_2
						//fnNameByte  = 90 ;
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=90 WLnum_MSD=abcd[1] WLnum_LSD=abcd[2] abcd[3]=BL stopByte=abcd[4]=127

						// get the data to the read scan chain
								u16 WLnum=abcd[1]*100+abcd[2];
								u8 BL=abcd[3];
							scanOut=perform_RxOnRowColSel_FPGA(currObj, &BL, WLnum) ;

							currObj=scanOut.currObj;
						// sending the scanned out data back
							receivedBytes2=0;
						// loading returnBytes array
							scan_len=97;
							u8 returnBytes[97];

							for(k=0;k<scan_len;k++)
							{
								returnBytes[k]=scanOut.scanOutBits_SA_SC[k];
							}

							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);


							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
		//						usleep(3000);
							}

							returnBytes[0]=127;
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}


							break;
						}
			   case 91:
						{
						//perform_WxOnRowColSel_FPGA_2
						//fnNameByte  = 91 ;
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=76 data=abcd[1:N] numPadZeroes_data=abcd[N+1] mask=abcd[N+2:2N+1] numPadZeroes_mask=abcd[2N+2] WLnum_MSD=abcd[end-3] WLnum_LSD=abcd[end-2] abcd[end-1]=BL stopByte=abcd[end]=127

						// get the data to the read scan chain
	// ---------------------------------------------------------------------------------------------------------------- //
								int data_len=(totalFunctionCallBytes-7)/2;
								u8 data[data_len];
								for(k=0;k<data_len;k++)
								{
									data[k]=abcd[1+k];
								}
								u8 numPadZeroes_data=abcd[data_len+1];

								int mask_len=data_len;
								u8 mask[mask_len];
								for(k=0;k<mask_len;k++)
								{
									mask[k]=abcd[data_len+2+k];
								}
								u8 numPadZeroes_mask=abcd[data_len+1+mask_len+1];
	// ---------------------------------------------------------------------------------------------------------------- //
								u16 WLnum=abcd[1+data_len+1+mask_len+1]*100+abcd[1+data_len+1+mask_len+1+1];
								 u8 BL=abcd[1+data_len+1+mask_len+1+1+1];
							currObj=perform_WxOnRowColSel_FPGA_2(currObj, data, data_len, numPadZeroes_data, mask, mask_len, numPadZeroes_mask, &BL, WLnum) ;


							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);

							u8 returnBytes[1] ;

							returnBytes[0]=127;
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}


							break;
						}



					case 94:
					{
					//adcRead_BLs
					//Description:To adcread while switching through xle bitlines as given
					//fnNameByte  = 94 ;
					//authored by: Ashwin Balagopal S
					//uart function call: fnNameByte=abcd[0]=94 repeatAvg=abcd[1] bitLines=abcd[2+(0:N1-1)] stopByte=abcd[N+1]=127

						int N1=0;// This is the number of bitlines
						while( abcd[2+N1] != PARAMETERMARKERBYTE )
							{
								N1++;
							}
						u8 bitLines_94[N1];
						int i_94;
						for(i_94 = 0 ; i_94<N1 ; i_94++)
							{
								bitLines_94[i_94]=abcd[1+1+i_94];
							}
						u16 repeatAvg;
						repeatAvg=(u16)(abcd[1]);
						u8 returnBytes[4*repeatAvg*N1];//At max only 100 Averages can be taken
		//				LAB PDF CODE STARTS HERE
		//				TAKEN FROM https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxydGNsYWIwOXxneDoyYTgxZDNjMmVjZjRmZmVm

						for(int j_94 = 0 ; j_94<N1 ; j_94++)
						{
								bitLine = bitLines_94[j_94];
							currObj=bitLineSelect(currObj,bitLine);
							usleep(20000);
							adcRead( &(returnBytes[4*repeatAvg*(i_94*N1+j_94)]), repeatAvg );//MARKER
						}

						// HANDSHAKE AND RETURN DATA
						receivedBytes2=0;
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=(4*repeatAvg)*N1;// VPVN and AUX00 both contribute 2 bytes each, thus 4 bytes per repeatAvg

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
		//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;
					}
					case 84:
					{
						//wait
						//authored by: Ashwin Balagopal S
						//uart function call: fnNameByte=abcd[0]=84 waitTime=abcd[1] stopByte=abcd[2]=127

						u8 waitTime;
						waitTime=abcd[1];// in 100s of microseconds, i.e. max wait is 12700 microseconds
						u16 actualWaitTime = ( (u16) (waitTime) ) * 100 ;

						usleep( actualWaitTime );//MARKER

						receivedBytes2=0;

						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1];
						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;

					}


					case 104:
					{
						//readSRAM
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=104 bitLine=abcd[1] stopByte=abcd[2]=127
						bitLine=abcd[1];
						SRAM_Store SRAMStore;
						SRAMStore = readSRAM(currObj,bitLine);
						currObj=SRAMStore.currObj;
						int lenSRAMdata=97*1092;
						u8 returnBytes[lenSRAMdata];
						for(k=0;k<lenSRAMdata;k++)
						{
							returnBytes[k]=SRAMStore.SRAMdata[k];
						}
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
	//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;
				}

					case 105:
					{
						//writeSRAM
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=105 data_DL=abcd[1:182](1092 bits compressed into 1092/6= 182 bytes) colSel=abcd[183:183+colSel_len-1] parameterMarkerByte=abcd[183+colSel_len]=126 DL_nums=abcd[183+colSel_len+1:183+colSel_len+1+(2*DL_nums_len)-1 stopByte=abcd[183+colSel_len+1+(2*DL_nums_len)]=127
						//1024+68=1092 bits;   1092 bits compressed into 1092/6 = 182 bytes
						int len_SRAMdata = 1092*97 ;
						u8 writeSRAMdata[len_SRAMdata];
						for( i=0 ; i<len_SRAMdata ; i++ )
						{
							writeSRAMdata[i] = abcd[1+i];
						}

						int mark = len_SRAMdata+1 ;
						i = mark ;

						u8 colSel[1];

						do
						{
							colSel[ i - mark ] = abcd[i];

							i++;

						}while(abcd[i]!=126); //i holds index of parameterMarkerByte


						int j = i+1;
						int k = 0;
						u8 DL_nums[97];

						do
						{
							DL_nums[k] = (abcd[j]<<6) + (abcd[j+1]);//handled by twoByteSplit in MATLAB
							j+=2;
							k++;
						}while(abcd[j]!=127);


						int len_DL_nums = k;



						bitLine=colSel[0];

						currObj=writeSRAM(currObj,bitLine, writeSRAMdata,  DL_nums, len_DL_nums);



						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						u8 returnBytes[1]={ 127 };
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}

						break;


				}
					case 106:
						{
								//fine_coarseSRAM
								//authored by: Balaji V
								//uart function call: fnNameByte=abcd[0]=106 Fine_cm=abcd[1:88] Fine_sub=abcd[89:176] Coarse_cm=abcd[177:264] Coarse_sub=abcd[265:352] stopByte=abcd[353]=127
							u8 Fine_cm[88],Coarse_cm[88],Fine_sub[88],Coarse_sub[88];
							for(int i=0;i<88;i++)
							{
								Fine_cm[i]=abcd[1+i];
								Fine_sub[i]=abcd[89+i];
								Coarse_cm[i]=abcd[177+i];
								Coarse_sub[i]=abcd[264+i];
							}
							currObj=fine_coarseSRAM(currObj,Fine_cm,Coarse_cm,Fine_sub,Coarse_sub);

							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);

							u8 returnBytes[1]={127};
							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}
							break;
						}

					case 107:
					{
						//IMGSelect
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=107 IMG=abcd[1] stopByte=abcd[2]=127
						u8 IMG=abcd[1];
						currObj=IMGSelect(currObj,IMG);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

					 case 108:
						{
						//perform_hybridIMC_BLIMG_FPGA
						//fnNameByte  = 108 ;
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=108 abcd[1]=BL abcd[2]=IMG stopByte=abcd[4]=127

						// get the data to the read scan chain

							u8 BL=abcd[1];
							u8 IMG=abcd[2];
							scanOut=perform_hybridIMC_BLIMG_FPGA(currObj, BL, IMG) ;

							currObj=scanOut.currObj;
						// sending the scanned out data back
							receivedBytes2=0;
						// loading returnBytes array
							scan_len=644;
							u8 returnBytes[644];

							for(k=0;k<scan_len;k++)
							{
								returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
							}

							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);


							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
		//						usleep(3000);
							}

							returnBytes[0]=127;
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}


							break;
						}

					 case 109:
						{
						//perform_internalIMC_ImgBankFPGA
						//fnNameByte  = 109 ;
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=108 abcd[1]=ImgBank stopByte=abcd[4]=127

						// get the data to the read scan chain

//							u8 BLs=abcd[1];//BLs take 1-4

							u8 ImgBank=abcd[1];//ImgBank takes 0 or 1
							scanoutInternal=perform_internalIMC_ImgBankFPGA(currObj,  ImgBank) ;
							//scanoutInternal=perform_IMC_Internal_pyTorch(currObj,  ImgBank) ;//Comment THIS STATEMENT
							currObj=scanoutInternal.currObj;

						// sending the scanned out data back
							receivedBytes2=0;
						// loading returnBytes array
							scan_len=644*4*4;
							u8 returnBytes[644*4*4];

							for(k=0;k<scan_len;k++)
							{
								returnBytes[k]=scanoutInternal.scanOutBits_ADC_SC_IMGBL[k];
							}

							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);


							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
		//						usleep(3000);
							}

							returnBytes[0]=127;
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}

							break;
						}

						case 110:
						{
							//LoadwriteSRAM_SDCard
							//authored by: Balaji V
							//uart function call: fnNameByte=abcd[0]=110 WtSlice=abcd[1] stopByte=abcd[2]=127
							u8 WtSlice=abcd[1];
							u8 bitLine;
							u8 DL_nums[97]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96};
							int len_DL_nums=97;
							int len_SRAMdata=R*C;
							u8 writeSRAMdata[len_SRAMdata];
							if (WtSlice==0)
							{
								for( bitLine = 0 ; bitLine < 4 ; bitLine++ )
								{
									switch (bitLine)
									{
										case 0:
										{
												memcpy(writeSRAMdata,wdata_0,105924);

												break;
										}
										case 1:
										{
												memcpy(writeSRAMdata,wdata_1,105924);

												break;
										}
										case 2:
										{
												memcpy(writeSRAMdata,wdata_2,105924);

										}		break;
										case 3:
										{
												memcpy(writeSRAMdata,wdata_3,105924);

												break;
										}
									}
									currObj=writeSRAM(currObj,bitLine, writeSRAMdata,  DL_nums, len_DL_nums);
								}
							}
							else
							{
								for( bitLine = 0 ; bitLine < 4 ; bitLine++ )
								{
									switch (bitLine)
									{
										case 0:
										{
												memcpy(writeSRAMdata,wdata_4,105924);

												break;
										}
										case 1:
										{
												memcpy(writeSRAMdata,wdata_5,105924);

												break;
										}
										case 2:
										{
												memcpy(writeSRAMdata,wdata_6,105924);

										}		break;
										case 3:
										{
												memcpy(writeSRAMdata,wdata_7,105924);

												break;
										}
									}
									currObj=writeSRAM(currObj,bitLine, writeSRAMdata,  DL_nums, len_DL_nums);
								}

							}


							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);

							u8 returnBytes[1]={127};
							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}
							break;
						}

						case 111:
						{
							//RBL_IMC_Select
							//authored by: Balaji V
							//uart function call: fnNameByte=abcd[0]=70 RBL=abcd[1] stopByte=abcd[2]=127

							u8 RBL=abcd[1];
							currObj=RBL_IMC_Select(currObj, RBL);
							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);

							u8 returnBytes[1]={127};
							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}
							break;
						}

						case 112:
						{
							//writeSRAM_1024_FAST_NEW
							//authored by: Balaji V
							//uart function call: fnNameByte=abcd[0]=112 data_DL=abcd[1:182](1024 bits compressed into 1024/6= 182 bytes) colSel=abcd[183:183+colSel_len-1] parameterMarkerByte=abcd[183+colSel_len]=126 DL_nums=abcd[183+colSel_len+1:183+colSel_len+1+(2*DL_nums_len)-1 stopByte=abcd[183+colSel_len+1+(2*DL_nums_len)]=127
							//1024+68=1092 bits;   1092 bits compressed into 1092/6 = 182 bytes
							int len_SRAMdata = 1024*97 ;
							u8 writeSRAMdata[len_SRAMdata];
							for( i=0 ; i<len_SRAMdata ; i++ )
							{
								writeSRAMdata[i] = abcd[1+i];
							}

							int mark = len_SRAMdata+1 ;
							i = mark ;

							u8 colSel[1];

							do
							{
								colSel[ i - mark ] = abcd[i];

								i++;

							}while(abcd[i]!=126); //i holds index of parameterMarkerByte


							int j = i+1;
							int k = 0;
							u8 DL_nums[97];

							do
							{
								DL_nums[k] = (abcd[j]<<6) + (abcd[j+1]);//handled by twoByteSplit in MATLAB
								j+=2;
								k++;
							}while(abcd[j]!=127);


							int len_DL_nums = k;



							bitLine=colSel[0];

							//writeSRAM_1024_FAST_NEW(curr_obj currObj,u8 bitLine, u8 *writeSRAMdata,  u8 *DL_nums, int DL_nums_len)
							currObj=writeSRAM_1024_FAST_NEW(currObj,bitLine, writeSRAMdata,  DL_nums, len_DL_nums);




							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);


							u8 returnBytes[1]={ 127 };
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}

							break;


					}


						case 113:
						{
							//scanInVector_uncompressed_FAST
							//authored by: Ashwin Balagopal S
							//uart function call: fnNameByte=abcd[0]=113 scanChain=abcd[1] vecScanIn=abcd[2:end-1] stopByte=abcd[end]=127
							//a=bitsSet(abcd,totalFunctionCallBytes);
							int len_vecScanIn=totalFunctionCallBytes-3;
							u8 vecScanIn[len_vecScanIn];
							u8 scanChain=abcd[1];
							for(k=0;k<len_vecScanIn;k++)
							{
								vecScanIn[k]=abcd[k+2];
							}
							// currObj=scanInVector_uncompressed_FAST(currObj, vecScanIn, len_vecScanIn, numPadZeroes_vecScanIn, scanChain);
							currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);

							//back to main
							receivedBytes2=0;
							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);

							u8 returnBytes[1]={127};
							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}
							break;
						}

						case 114:
						{
							//scanOutVector_FAST
							//authored by: Ashwin Balagopal S, Balaji V
							//uart function call: fnNameByte=abcd[0]=114 scanChain=abcd[1] stopByte=abcd[2]=127
							//a=bitsSet(abcd,totalFunctionCallBytes);
							scanChain=abcd[1];
							scanOut=scanOutVector_FAST(currObj,scanChain);//MARKER
							//back to main
							currObj=scanOut.currObj;
							receivedBytes2=0;
							switch(scanChain)
							{
								case SA_SC:
								{
									scan_len=97;
									break;
								}
								case ADC_SC:
								{
									scan_len=644;
									break;
								}
								case AdderTree_SC:
								{
									scan_len=176;
									break;
								}
								case SARTIMING_OUT_SC:
								{
									scan_len=26;
									break;
								}
								case WRITE_2_SC:
								{
									scan_len=97;
									break;
								}
								case WRITE_1_SC:
								{
									scan_len=97;
									break;
								}
								case WRITE_0_SC:
								{
									scan_len=97;
									break;
								}
								case IMC_RBL_SC:
								{
									scan_len=88;
									break;
								}
								case IMC_VOut_SC:
								{
									scan_len=88;
									break;
								}
								case WWL_SC:
								{
									scan_len=1160;
									break;
								}
								case FLOATADC_SC:
								{
									scan_len=92;
									break;
								}
								case SARTIMING_IN_SC:
								{
									scan_len=110;
									break;
								}
								case ControlPath_SC:
								{
									scan_len=17;
									break;
								}
							}

							u8 returnBytes[scan_len];

							switch(scanChain)
							{
								case SA_SC:
								{
									for(k=0;k<scan_len;k++)
									{
										returnBytes[k]=scanOut.scanOutBits_SA_SC[k];
									}
									break;
								}
								case ADC_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
									break;
								}
								case AdderTree_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_AdderTree_SC[k];
									break;
								}
								case SARTIMING_OUT_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_SARTIMING_OUT_SC[k];
									break;
								}
								case WRITE_2_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_WRITE_2_SC[k];
									break;
								}
								case WRITE_1_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_WRITE_1_SC[k];
									break;
								}
								case WRITE_0_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_WRITE_0_SC[k];
									break;
								}
								case IMC_RBL_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_IMC_RBL_SC[k];
									break;
								}
								case IMC_VOut_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_IMC_VOut_SC[k];
									break;
								}
								case WWL_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_WWL_SC[k];
									break;
								}
								case FLOATADC_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_FLOATADC_SC[k];
									break;
								}
								case SARTIMING_IN_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_SARTIMING_IN_SC[k];
									break;
								}
								case ControlPath_SC:
								{
									for(k=0;k<scan_len;k++)
										returnBytes[k]=scanOut.scanOutBits_ControlPath_SC[k];
									break;
								}

							}

							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);


							fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
		//						usleep(3000);
							}

							returnBytes[0]=127;
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}


							break;
						}

						case 115:
						{
							//readSRAMSliced
							//authored by: Balaji V
							//uart function call: fnNameByte=abcd[0]=115 stopByte=abcd[end]=127


							// Declaring Memory for the ReadOut Arrays
							// Read_out_weight->68x1024x4
							u8 ***Read_out_weight = (u8***)calloc(68,sizeof(u8**));
							for(uint16_t wd = 0; wd < 68; wd++){
								Read_out_weight[wd] = (u8**)calloc(1024,sizeof(u8*));
								for(uint16_t wl = 0; wl < 1024; wl++){
									Read_out_weight[wd][wl] = (u8*)calloc(4,sizeof(u8));
								}
							}


							// Read_out_reference->2x8x1024x4
							u8 ****Read_out_reference = (u8****)calloc(2,sizeof(u8***));
							for(uint16_t lh = 0; lh < 2; lh++){
								Read_out_reference[lh] = (u8***)calloc(8,sizeof(u8**));
								for(uint16_t rd = 0; rd < 8; rd++){
									Read_out_reference[lh][rd] = (u8**)calloc(1024,sizeof(u8*));
									for(uint16_t wl = 0; wl < 1024; wl++){
										Read_out_reference[lh][rd][wl] = (u8*)calloc(4,sizeof(u8));
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


							readSRAMSliced(currObj, Read_out_image, Read_out_weight, Read_out_reference, Read_out_referenceAdderTree);



							{// Read_out_weight->68x1024x4
								u8 Read_out_weight_FLAT[68*1024*4];
								for(int i = 0; i < 4; i++)
								{
									for(int j = 0; j < 1024; j++)
									{
										for(int k = 0; k < 68; k++)
										{
												Read_out_weight_FLAT[k+j*(68)+i*(68*1024)]=Read_out_weight[k][j][i];
										}
									}
								}



								char FileName_Rwt[]="Rwt.bin";
								u32 size=(68*1024*4)*sizeof(u8);
								int rc=WriteFile_custom(FileName_Rwt, size, (void *) Read_out_weight_FLAT);
							}





							{// Read_out_ref->2x8x1024x4
								u8 Read_out_ref_FLAT[2*8*1024*4];
								for(int i = 0; i < 4; i++)
								{
									for(int j = 0; j < 1024; j++)
									{
										for(int k = 0; k < 8; k++)
										{
											for(int l = 0; l < 2; l++)
											{
												Read_out_ref_FLAT[l+k*2+j*(2*8)+i*(2*8*1024)]=Read_out_reference[l][k][j][i];
											}
										}
									}
								}



								char FileName_RRef[]="RRef.bin";
								u32 size=(2*8*1024*4)*sizeof(u8);
								int rc=WriteFile_custom(FileName_RRef, size, (void *) Read_out_ref_FLAT);
							}


							{// Read_out_referenceAdderTree->1024x4
								u8 Read_out_referenceAdderTree_FLAT[1024*4];
								for(int i = 0; i < 4; i++){
									for(int j = 0; j < 1024; j++){
										Read_out_referenceAdderTree_FLAT[j+i*1024]=Read_out_referenceAdderTree[j][i];
									}
								}

								char FileName_RATr[]="RATr.bin";
								u32 size=(1024*4)*sizeof(u8);
								int rc=WriteFile_custom(FileName_RATr, size, (void *) Read_out_referenceAdderTree_FLAT);
							}

							{// Read_out_image->8x1024
								u8 Read_out_image_FLAT[8*1024];
								for(int i = 0; i < 1024; i++){
									for(int j = 0; j < 8; j++){
										Read_out_image_FLAT[j+i*8]=Read_out_image[j][i];
									}
								}

								char FileName_RIMG[]="RIMG.bin";
								u32 size=(8*1024)*sizeof(u8);

								int rc=WriteFile_custom(FileName_RIMG, size, (void *) Read_out_image_FLAT);
							}


							do
							{
								receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
								totalReceivedBytes2+=receivedBytes2;
							}while(syncdata!=125);

							u8 returnBytes[1];
							returnBytes[0]=127;
							fileSize_return=1;
							totalTransmittedBytes=0;

							while(totalTransmittedBytes < fileSize_return)
							{
								//transmit returnBytes[totalTransmittedBytes]
								transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
								totalTransmittedBytes+=transmittedBytes;
							}


							break;
					}
					case 116:
					{
					//perform_hybridIMC_ADCSWEEP_FPGA
					//fnNameByte  = 116 ;
					//authored by: Ashwin
					//uart function call: fnNameByte=abcd[0]=116 abcd[1]=BL abcd[2]=IMG stopByte=abcd[4]=127

					// get the data to the read scan chain

						u8 BL=abcd[1];
						u8 IMG=abcd[2];
						scanOut=perform_hybridIMC_ADCSWEEP_FPGA(currObj, BL, IMG) ;

						currObj=scanOut.currObj;
					// sending the scanned out data back
						receivedBytes2=0;
					// loading returnBytes array
						scan_len=644;
						u8 returnBytes[644];

						for(k=0;k<scan_len;k++)
						{
							returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
						}

						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
	//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;
					}
					case 117:
					{
					//perform_internalIMC_TOPSW_ImgBankFPGA
					//fnNameByte  = 117 ;
					//authored by: Balaji V
					//uart function call: fnNameByte=abcd[0]=117 abcd[1]=ImgBank stopByte=abcd[4]=127

					// get the data to the read scan chain
						u8 ImgBank=abcd[1];//ImgBank takes 0 or 1
						scanoutInternal=perform_internalIMC_TOPSW_ImgBankFPGA(currObj,  ImgBank) ;
						//scanoutInternal=perform_IMC_Internal_pyTorch(currObj,  ImgBank) ;//Comment THIS STATEMENT
						currObj=scanoutInternal.currObj;

					// sending the scanned out data back
						receivedBytes2=0;
					// loading returnBytes array
						scan_len=644*4*4;
						u8 returnBytes[644*4*4];

						for(k=0;k<scan_len;k++)
						{
							returnBytes[k]=scanoutInternal.scanOutBits_ADC_SC_IMGBL[k];
						}

						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
	//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}

						break;
					}

					case 118:
					{
					//perform_hybridIMC_TOPSW_FPGA
					//fnNameByte  = 118 ;
					//authored by: Balaji V
					//uart function call: fnNameByte=abcd[0]=118 abcd[1]=BL abcd[2]=IMG stopByte=abcd[4]=127

					// get the data to the read scan chain

						u8 BL=abcd[1];
						u8 IMG=abcd[2];
						scanOut=perform_hybridIMC_TOPSW_FPGA(currObj, BL, IMG) ;

						currObj=scanOut.currObj;
					// sending the scanned out data back
						receivedBytes2=0;
					// loading returnBytes array
						scan_len=644;
						u8 returnBytes[644];

						for(k=0;k<scan_len;k++)
						{
							returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
						}

						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);


						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
	//						usleep(3000);
						}

						returnBytes[0]=127;
						fileSize_return=1;
						totalTransmittedBytes=0;

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}


						break;
					}

					case 119:
					{
					//store_images_SRAM_MultiBitX
					//authored by: Rahul
					    //uart function call: fnNameByte=abcd[0]=119 IMGnum=abcd[1] mode=abcd[2] X=abcd[3:end-1] stopByte=abcd[end]=127
					u8 X[1024],IMGnum,mode;
					IMGnum=abcd[1];
					mode=abcd[2];
					for(int i=0;i<1024;i++)
					{
					X[i]=abcd[3+i];
					}
					currObj=store_images_SRAM_MultiBitX(X,mode,IMGnum ,currObj);

					do
					{
					receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
					totalReceivedBytes2+=receivedBytes2;
					}while(syncdata!=125);

					u8 returnBytes[1]={127};
					fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

					while(totalTransmittedBytes < fileSize_return)
					{
					//transmit returnBytes[totalTransmittedBytes]
					transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
					totalTransmittedBytes+=transmittedBytes;
					}
					break;
					}


					case 120:
					{
						//set_modeInternalHybridTiming
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=120 internal_hyb_B=abcd[1] IMG_TIME_GPIO_DATA=abcd[2]<<8+abcd[3] RWL_EN_DELAY=abcd[4]; CM_EN_DELAY=abcd[5]; ADCSTART_DELAY=abcd[6]; SAMPLING_DELAY=abcd[7]; EVALUATE_DELAY=abcd[8]; FPGA_RESET_REPEAT_UB_hybrid=abcd[9];  stopByte=abcd[10]=127

						u8 internal_hyb_B=abcd[1];
						u16 IMG_TIME_GPIO_DATA=(abcd[2]<<6)+abcd[3];
						u8 RWL_EN_DELAY=abcd[4];
						u8 CM_EN_DELAY=abcd[5];
						u8 ADCSTART_DELAY=abcd[6];
						u8 SAMPLING_DELAY=abcd[7];
						u8 EVALUATE_DELAY=abcd[8];
						u32 FPGA_RESET_REPEAT_UB_hybrid=(u32)((abcd[9]<<6)+abcd[10]);

						currObj= set_modeInternalHybridTiming(currObj,internal_hyb_B,IMG_TIME_GPIO_DATA,RWL_EN_DELAY,CM_EN_DELAY,ADCSTART_DELAY,SAMPLING_DELAY,EVALUATE_DELAY,FPGA_RESET_REPEAT_UB_hybrid);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}



					case 121:
					{
						//set_ADCbitModeInt_HybB
						//authored by: Balaji V
						//uart function call: fnNameByte=abcd[0]=121 internal_hyb_B=abcd[1];ADC_bitMode=abcd[2];  stopByte=abcd[10]=127

						u8 internal_hyb_B=abcd[1];
						u8 ADC_bitMode=abcd[2];

						currObj= set_ADCbitModeInt_HybB(currObj,internal_hyb_B,ADC_bitMode);
						do
						{
							receivedBytes2 = XUartPs_Recv(&myUart,&syncdata,1);// Non-Blocking
							totalReceivedBytes2+=receivedBytes2;
						}while(syncdata!=125);

						u8 returnBytes[1]={127};
						fileSize_return=sizeof(returnBytes)/sizeof(returnBytes[0]);//to be checked

						while(totalTransmittedBytes < fileSize_return)
						{
							//transmit returnBytes[totalTransmittedBytes]
							transmittedBytes = XUartPs_Send(&myUart,&(returnBytes[totalTransmittedBytes]),1);
							totalTransmittedBytes+=transmittedBytes;
						}
						break;
					}

		}

	}
Status=SD_Eject(&fatfs);
return 0;
}
