/*
 * perform_hybridIMC_BLIMG_FPGA.c
 *
 *  Created on: Aug 11, 2022
 *      Author: Balaji Vijayakumar
 */

#include <custom_h.h>

scan_out perform_hybridIMC_TOPSW_FPGA(curr_obj currObj,u8 bitLine, u8 IMG)
{


	u8 scanChain;
	u8 signals[1];
	u8 currentValues[1];
	u8 values[1];
	scan_out scanOut;
	int signals_len;

	//1. Image Selection
	currObj = IMGSelect(currObj,IMG);

	//2. Bitline Selection
	currObj=RBL_IMC_Select(currObj, bitLine);

	//3. Enable DATA_Resis and RefResis and Retain, Remove Pre-charge on RBL
	u8 signals_preVerilogSET[3]={DataResis_EN,RefResis_EN,RBL_PRE_EN_B};
	//u8 values_preVerilogSET[3]={1,1,1};
	//u8 values_preVerilogSET[3]={1,1,1};
	u8 values_preVerilogSET[3]={0,0,1};
//	u8 values_preVerilogSET[3]={0,0,1};
	signals_len = 3 ;
	currObj=bitsSet( currObj, signals_preVerilogSET, signals_len, values_preVerilogSET );

	//4. High-Speed Hybrid Mode Verilog Operations
	{
		//SELECT MODE HYBRID TOPS/W
		u8 GPIO_MODE_IMC_DATA=4;//0=C-INPUT 1=MODE_HYBRID; 2=MODE_INTERNAL; >2 = RESTRICTED-DO NOT USE
		XGpio_DiscreteWrite(&currObj.GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
	}



	//u32 FPGA_RESET_REPEAT_UB=467;//13 bits, i.e. at most make it 2^12 - 1 //changed on 26/10/23
	//u32 FPGA_RESET_REPEAT_UB=57;//13 bits, i.e. at most make it 2^12 - 1 //changed on 30/01/24



	//setting HYBRIDPIPELINETOPSW_VARS : To determine values from behavioral simulation/ VIO timing
	// ADC 2-bit mode
	// modified on 03/03/2024
	// ADC 2-bit mode

	// setting set_TOPSW_GPIO_bank1
	u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1


//		// Internal-1X ADC in 2-bit mode- 258ns required as per simulation TOPSW RUN WAS on 02/02/2024 04:28 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=26;//260ns
		// Internal-1X ADC in 3-bit mode- 305ns required as per simulation TOPSW RUN WAS on 02/02/2024 04:28 PM
		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=31;//310ns
//// Internal-1X ADC in 4-bit mode- 351ns required as per simulation TOPSW RUN WAS on 03/02/2024 02:10 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=36;//353ns
//// Internal-1X ADC in 5-bit mode- 400ns required as per simulation TOPSW RUN WAS on 03/02/2024 3:01 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=40;//400ns

	u32 FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
	u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4

	//setting for set_HYBRIDPIPELINETOPSW_VARS_GPIO

//	//ADC 2-bit mode
//	u8 RWL_EN_DELAY=1;
//	u8 CM_EN_DELAY=4;
//	u8 ADCSTART_DELAY=1;
//	u8 SAMPLING_DELAY=3;
//	u8 EVALUATE_DELAY=3;

//	//ADC 3-bit mode
//	u8 RWL_EN_DELAY=1;
//	u8 CM_EN_DELAY=4;
//	u8 ADCSTART_DELAY=1;
//	u8 SAMPLING_DELAY=3;
//	u8 EVALUATE_DELAY=4;

//	//ADC 4-bit mode
//	u8 RWL_EN_DELAY=1;
//	u8 CM_EN_DELAY=4;
//	u8 ADCSTART_DELAY=1;
//	u8 SAMPLING_DELAY=3;
//	u8 EVALUATE_DELAY=5;//48ns eval from sims
//
//	//ADC 5-bit mode
//	u8 RWL_EN_DELAY=1;
//	u8 CM_EN_DELAY=4;
//	u8 ADCSTART_DELAY=1;
//	u8 SAMPLING_DELAY=3;
//	u8 EVALUATE_DELAY=6;//60ns eval from sims
//
//	set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);
//	set_HYBRIDPIPELINETOPSW_VARS_GPIO(currObj, RWL_EN_DELAY, CM_EN_DELAY, ADCSTART_DELAY, SAMPLING_DELAY,  EVALUATE_DELAY);
//



		{
			//pulse01FPGA_RESET_handshake( currObj );
			{
					u8 signals[1]={ FPGA_RESET } ;
					u8  values[1]={     0      } ;
					int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
				currObj=bitsSet(currObj, signals, signals_len, values) ;
			}
			while(1)
				{

				};
		}

	{
		//DESELECT MODE HYBRID
		u8 GPIO_MODE_IMC_DATA=0;//0=C-INPUT 1=MODE_HYBRID; 2=MODE_INTERNAL; >2 = RESTRICTED-DO NOT USE
		XGpio_DiscreteWrite(&currObj.GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
	}

	//5. Restore Pre-charge on RBL
	signals[0]=RBL_PRE_EN_B;
	values[0]=0;
	signals_len = 1 ;
	currObj=bitsSet( currObj, signals, signals_len, values );

	//6. Select ADC Scan Chain
	scanChain=ADC_SC;
	currObj=scanSelect(currObj,scanChain);

	//7. Pulse CLK_C
	signals[0]=CLK_C;
	currentValues[0]=0;
	signals_len=1;
	currObj=pulse(currObj, signals, currentValues, signals_len);

	//8. Pulse CLK_B
	signals[0]=CLK_B;
	currentValues[0]=0;
	currObj=pulse(currObj, signals, currentValues, signals_len);

	//9. Scan out	the ADC Data
	scanChain=ADC_SC;
	scanOut=scanOutVector( currObj, scanChain ) ;

	return scanOut;

}

