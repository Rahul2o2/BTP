/*
 * bitLineSelect.c
 *
 *  Created on: Jun 11, 2023
 *      Author: Ashwin Balagopal S, Balaji V
 */


#include <custom_h.h>
curr_obj set_modeInternalHybridTiming(curr_obj currObj,u8 internal_hyb_B,u16 IMG_TIME_GPIO_DATA,u8 RWL_EN_DELAY,u8 CM_EN_DELAY,u8 ADCSTART_DELAY,u8 SAMPLING_DELAY,u8 EVALUATE_DELAY,u32 FPGA_RESET_REPEAT_UB_hybrid)
{
	//INTERNAL MODE RELATED TIMING GPIOS
	// 1. Repeat interval for one image
	XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
	XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
	u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=IMG_TIME_GPIO_DATA;//50;//496ns required from simulation
	XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

//HYBRID MODE RELATED TIMING GPIOS
	// 2. Settings for hybrid mode TOPSW
//	u8 RWL_EN_DELAY=1;	u8 CM_EN_DELAY=4;	u8 ADCSTART_DELAY=1;	u8 SAMPLING_DELAY=3;	u8 EVALUATE_DELAY=8;//84ns
	set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

//COMMON to both HYBRID AND INTERNAL MODES
	// 3. Repeat interval for TOPSW periodic wave generation
	u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
	u32 FPGA_RESET_REPEAT_UB;
	if (internal_hyb_B)
	{
		FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
		//FPGA_RESET_REPEAT_UB=FPGA_RESET_REPEAT_UB_internal;
	}
	else
	{
		FPGA_RESET_REPEAT_UB=FPGA_RESET_REPEAT_UB_hybrid;
		//FPGA_RESET_REPEAT_UB=214;//from TB_hybridBLpipeStateDiag simulation 2110ns
	}
	u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
	set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);

	return currObj;
}
