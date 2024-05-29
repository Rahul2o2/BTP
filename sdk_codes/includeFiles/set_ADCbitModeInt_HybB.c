/*
 * bitLineSelect.c
 *
 *  Created on: Jun 11, 2023
 *      Author: Ashwin Balagopal S, Balaji V
 */


#include <custom_h.h>
curr_obj set_ADCbitModeInt_HybB(curr_obj currObj,u8 internal_hyb_B,u8 ADC_bitMode)
{

	switch (ADC_bitMode)
	{

		case 2:
		{

		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=26;//258ns required from simulation
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=3;
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=135;//from TB_hybridBLpipeStateDiag simulation 1340ns from reset to done
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);

			break;
		}
		case 3:
		{
		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=31;//305ns required from sim
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=4;
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=150;//from TB_hybridBLpipeStateDiag simulation 1470ns
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);

			break;
		}
		case 4:
		{
		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=36;//353ns required from sim
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=5;
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=167;//from TB_hybridBLpipeStateDiag simulation 1640ns
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
			break;
		}
		case 5:
		{
		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=40;//400ns required from sim
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=6;
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=182;//from TB_hybridBLpipeStateDiag simulation 1790ns
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
			break;
		}
		case 6:
		{
		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=45;//448ns required from simulation
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=7;//72ns required round to 70ns
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=198;//from TB_hybridBLpipeStateDiag simulation 1950ns
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
			break;
		}
		case 7:
		{

		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=50;//496ns required from simulation
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=8;//84ns
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=214;//from TB_hybridBLpipeStateDiag simulation 2110ns
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
			break;
		}
		case 8: //7-b ADC + 1X-SUB
		{

		//INTERNAL MODE RELATED TIMING GPIOS
				// 1. Repeat interval for one image
				XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
				XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
				u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=50;//496ns required from simulation
				XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

			//HYBRID MODE RELATED TIMING GPIOS
				// 2. Settings for hybrid mode TOPSW
				u8 RWL_EN_DELAY=1;
				u8 CM_EN_DELAY=4;
				u8 ADCSTART_DELAY=1;
				u8 SAMPLING_DELAY=3;
				u8 EVALUATE_DELAY=8;//84ns
				set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

			//COMMON to both HYBRID AND INTERNAL MODES
				// 3. Repeat interval for TOPSW periodic wave generation
				u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
				u32 FPGA_RESET_REPEAT_UB;
				if (internal_hyb_B)
				{
					FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
				}
				else
				{
					FPGA_RESET_REPEAT_UB=214;//from TB_hybridBLpipeStateDiag simulation 2110ns
				}
				u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
				set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
				break;
		}
		case 9: //7-b ADC + 2X-SUB
		{

		//INTERNAL MODE RELATED TIMING GPIOS
				// 1. Repeat interval for one image
				XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
				XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
				u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=50;//496ns required from simulation
				XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

			//HYBRID MODE RELATED TIMING GPIOS
				// 2. Settings for hybrid mode TOPSW
				u8 RWL_EN_DELAY=1;
				u8 CM_EN_DELAY=4;
				u8 ADCSTART_DELAY=1;
				u8 SAMPLING_DELAY=3;
				u8 EVALUATE_DELAY=8;//84ns
				set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

			//COMMON to both HYBRID AND INTERNAL MODES
				// 3. Repeat interval for TOPSW periodic wave generation
				u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
				u32 FPGA_RESET_REPEAT_UB;
				if (internal_hyb_B)
				{
					FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
				}
				else
				{
					FPGA_RESET_REPEAT_UB=214;//from TB_hybridBLpipeStateDiag simulation 2110ns
				}
				u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
				set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
				break;
		}
		case 10: //7-b ADC + 4X-SUB
		{
		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=50;//496ns required from simulation
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=4;
			u8 ADCSTART_DELAY=1;
			u8 SAMPLING_DELAY=3;
			u8 EVALUATE_DELAY=8;//84ns
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=214;//from TB_hybridBLpipeStateDiag simulation 2110ns
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
			break;
		}

		default://Accuracy mode
		{
		//INTERNAL MODE RELATED TIMING GPIOS
			// 1. Repeat interval for one image
			XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
			XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
			u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=170;//258ns required from simulation
			XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);

		//HYBRID MODE RELATED TIMING GPIOS
			// 2. Settings for hybrid mode TOPSW
			u8 RWL_EN_DELAY=1;
			u8 CM_EN_DELAY=10;
			u8 ADCSTART_DELAY=10;
			u8 SAMPLING_DELAY=10;
			u8 EVALUATE_DELAY=100;
			set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);

		//COMMON to both HYBRID AND INTERNAL MODES
			// 3. Repeat interval for TOPSW periodic wave generation
			u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
			u32 FPGA_RESET_REPEAT_UB;
			if (internal_hyb_B)
			{
				FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
			}
			else
			{
				FPGA_RESET_REPEAT_UB=130;//from TB_hybridBLpipeStateDiag simulation
			}
			u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
			set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);


			break;
		}
	}
	return currObj;
}
