/*
 * perform_hybridIMC_BLIMG_FPGA.c
 *
 *  Created on: Aug 11, 2022
 *      Author: Balaji Vijayakumar
 */

#include <custom_h.h>

scan_out_internal perform_internalIMC_TOPSW_ImgBankFPGA(curr_obj currObj, u8 ImgBank)
{

	// TIME MEASURING CODE STARTS
//		XTime tStart_innerLoop;
//		XTime tStop_innerLoop;
//		XTime tElapsed_innerLoop;
//		double tElapsed_innerLoop_msec;
	// TIME MEASURING CODE ENDS

	// TIME MEASURING CODE STARTS
//		XTime_GetTime(&tStart_innerLoop);
	// TIME MEASURING CODE ENDS
	//u8 columnSelect;
	u8 scanChain;
	u8 signals[1];
	u8 currentValues[1];
	u8 values[1];
	scan_out_internal scanoutInternal;
	scan_out scanOut;
	int signals_len;

	//0. Internal mode in SARTIMING_IN_SC scan chain
//	{
//		u8 scanChain=SARTIMING_IN_SC;
//		int len_vecScanIn=scan_len_SARTIMING_IN_SC;
//		//6-Bit ADC Set in Internal Mode as of 14/09/2023 7:07 PM
////		u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		//6-Bit ADC Set in Internal Mode relaxed BL-0 timing of 80ns, BL-1,2,3=68ns before sampler
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		//6-Bit ADC Set in Internal Mode relaxed BL-0 and ADC with ~480ns for 1IMG as opposed to ~300ns
//		u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,0};
//		//BELOW IS EXPERIMENTAL INTERNAL MODE FAST PATTERN ADDED BY ASHWIN: SOURCE IS D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\032_SystemMAC_DryRuns_ASHWIN\FCNN78410010_RUNS\INTERNAL_2X_RUNS\INTERNAL_MODE_62ns\get_GLOBALTIMER_scanInPattern.m
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		//BELOW IS EXPERIMENTAL INTERNAL MODE FAST PATTERN ADDED BY ASHWIN: SOURCE IS D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\032_SystemMAC_DryRuns_ASHWIN\FCNN78410010_RUNS\INTERNAL_2X_RUNS\INTERNAL_MODE_66ns\get_GLOBALTIMER_scanInPattern.m
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		//BELOW IS EXPERIMENTAL INTERNAL MODE FAST PATTERN ADDED BY ASHWIN: SOURCE IS D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\032_SystemMAC_DryRuns_ASHWIN\FCNN78410010_RUNS\INTERNAL_2X_RUNS\INTERNAL_MODE_70ns\get_GLOBALTIMER_scanInPattern.m
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		//currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
//		//BELOW IS EXPERIMENTAL INTERNAL MODE FAST PATTERN ADDED BY ASHWIN: SOURCE IS D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\031_ADC_Ashwin\InternalSynthetic\TIMING91\try_6b\get_GLOBALTIMER_scanInPattern.m
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		//BELOW IS EXPERIMENTAL INTERNAL MODE FAST PATTERN ADDED BY ASHWIN: SOURCE IS D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\031_ADC_Ashwin\InternalSynthetic\TIMING5\try_6b\get_GLOBALTIMER_scanInPattern.m
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,0};
//		//BELOW IS EXPERIMENTAL INTERNAL MODE FAST PATTERN ADDED BY ASHWIN: SOURCE IS D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\031_ADC_Ashwin\InternalSynthetic\TIMING0\try_6b\get_GLOBALTIMER_scanInPattern.m
//		//u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,0,0,1,1,0,0,0};
//		  currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
//	}

	//1. Image Selection
	signals[0]=IMG_Sel_Dec_2;//If IMG_Sel_Dec_2=1 => 4,5,6,7;    else:  => 0,1,2,3
	values[0]=ImgBank;
	signals_len = 1 ;
	currObj=bitsSet( currObj, signals, signals_len, values);


	//2. Enable DATA_Resis and RedResis and Retain, Remove Pre-charge on RBL
	u8 signals_preVerilogSET[3]={DataResis_EN,RefResis_EN,RBL_PRE_EN_B};
	u8 values_preVerilogSET[3]={1,1,1};
	signals_len = 3 ;
	currObj=bitsSet( currObj, signals_preVerilogSET, signals_len, values_preVerilogSET );

	//3. High-Speed Internal Mode TOPSW Verilog Operations
	{
		//SELECT MODE INTERNAL TOPSW
		u8 GPIO_MODE_IMC_DATA=3;//0=C-INPUT 1=MODE_HYBRID; 2=MODE_INTERNAL; 3=INTERNAL TOPSW; 4=HYBRID PIPELINE TOPSWW >5 = RESTRICTED-DO NOT USE
		XGpio_DiscreteWrite(&currObj.GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
	}



	{

//		XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
//		XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=60;//For TOPSW RUNS
//		XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);
	//INTERNAL MODE RELATED TIMING GPIOS
//		XGpio GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst;
//		XGpio_Initialize(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst, GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_ID);
		// TOPSW RUN WAS WITH 60 30/10/2023 08:30 PM
		//u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=60;//Actually a 10 bit number in Block Diagram=> Accuracy was with 170 (26/10/2023 12:41AM)

//		// Internal-1X ADC in 2-bit mode- 258ns required as per simulation TOPSW RUN WAS on 02/02/2024 04:28 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=26;//260ns
//		// Internal-1X ADC in 3-bit mode- 305ns required as per simulation TOPSW RUN WAS on 02/02/2024 04:28 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=31;//310ns
//// Internal-1X ADC in 4-bit mode- 351ns required as per simulation TOPSW RUN WAS on 03/02/2024 02:10 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=36;//353ns
// Internal-1X ADC in 5-bit mode- 400ns required as per simulation TOPSW RUN WAS on 03/02/2024 3:01 PM
//		u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=40;//400ns
//
//		// ACCURACY RUN
//		//u16 GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=170;
//		XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);



//		u32 FPGA_RESET_REPEAT_LB=1;//13 bits, i.e. at most make it 2^12 - 1
//		u32 FPGA_RESET_REPEAT_UB=(GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA+1)*4+1;//13 bits, i.e. at most make it 2^12 - 1
//		//u32 FPGA_RESET_REPEAT_UB=292;//13 bits, i.e. at most make it 2^12 - 1
//		u32 FPGA_RESET_REPEAT_NUMREPEAT=4;//for infinity set the value as 4
//		set_TOPSW_GPIO_bank1(currObj, FPGA_RESET_REPEAT_LB, FPGA_RESET_REPEAT_UB, FPGA_RESET_REPEAT_NUMREPEAT);


		{
				u8 signals[1]={ FPGA_RESET } ;
				u8  values[1]={     0      } ;
				int signals_len = sizeof( signals ) / sizeof( signals[0] ) ;
			currObj=bitsSet(currObj, signals, signals_len, values) ;
		}
//		GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA=170;//Default for Accuracy RUNS
//		XGpio_DiscreteWrite(&GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_Inst,1,GPIO_INTERNAL_TIMING_1_IMG_TIME_GPIO_DATA);
		while(1)
			{

			};
	}

	{
		//DESELECT MODE INTERNAL
		u8 GPIO_MODE_IMC_DATA=0;//0=C-INPUT 1=MODE_HYBRID; 2=MODE_INTERNAL; >2 = RESTRICTED-DO NOT USE
		XGpio_DiscreteWrite(&currObj.GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
	}

	//4. Restore Pre-charge on RBL
	signals[0]=RBL_PRE_EN_B;
	values[0]=0;
	signals_len = 1 ;
	currObj=bitsSet( currObj, signals, signals_len, values );

	//5. Change to either Hybrid or External Mode to control BLselect
	scanChain = SARTIMING_IN_SC;
	currObj=scanSelect(currObj,scanChain);
	int len_vecScanIn=scan_len_SARTIMING_IN_SC;
	//Hybrid Mode Pattern Choosen
	//u8 vecScanIn_SARTIMING_IN_SC[110]={0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
	//External Mode Pattern Choosen
	u8 vecScanIn_SARTIMING_IN_SC[scan_len_SARTIMING_IN_SC]={0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,1,1,0,0,0,1,0,0,1,1,0,0,0};
	currObj = scanInVector_uncompressed_FAST( currObj, vecScanIn_SARTIMING_IN_SC, len_vecScanIn, scanChain ) ;

	// TIME MEASURING CODE STARTS
//		XTime_GetTime(&tStop_innerLoop);
//		tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//		tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
	// TIME MEASURING CODE ENDS
	// TIME MEASURING CODE STARTS
//		XTime_GetTime(&tStart_innerLoop);
	// TIME MEASURING CODE ENDS

	//6. Select ADC Scan Chain
	scanChain=ADC_SC;
	currObj=scanSelect(currObj,scanChain);

	int len_scanOUT=scan_len_ADC_SC;
	//u8 scanOutADC_vector[len_scanOUT];
	u8 i,j;
	u8 IMG;
	int k;
	for (i=0;i<4;i++)
	{
		IMG=((u8)ImgBank<<2)+i;
		currObj=IMGSelect(currObj,IMG);
		for( j=0 ; j<4 ; j++ )
		{
			//6. Select ADC Scan Chain
			scanChain=ADC_SC;
			currObj=scanSelect(currObj,scanChain);

			//currObj=bitLineSelect(currObj,j);
			currObj=RBL_IMC_Select(currObj, j);

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
			scanOut=scanOutVector_FAST( currObj, scanChain ) ;//scanOutVector_FAST is the issue
			//scanOut=scanOutVector( currObj, scanChain ) ;
			currObj=scanOut.currObj;

			//10. Copy the ADC scanOut Bits
			for (k=0;k<len_scanOUT;k++)
			{
				scanoutInternal.scanOutBits_ADC_SC_IMGBL[k + len_scanOUT * (j + i * 4)]=scanOut.scanOutBits_ADC_SC[k];
			}
//			memcpy(&scanoutInternal.scanOutBits_ADC_SC_IMGBL[len_scanOUT * (j + i * 4)],scanOut.scanOutBits_ADC_SC,len_scanOUT);
		}
	}

	// TIME MEASURING CODE STARTS
//		XTime_GetTime(&tStop_innerLoop);
//		tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
//		tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
	// TIME MEASURING CODE ENDS

	//11. Select ADC Scan Chain
	scanChain=AdderTree_SC;
	currObj=scanSelect(currObj,scanChain);

	//12. Pulse CLK_C
	signals[0]=CLK_C;
	currentValues[0]=0;
	signals_len=1;
	currObj=pulse(currObj, signals, currentValues, signals_len);

	//13. Pulse CLK_B
	signals[0]=CLK_B;
	currentValues[0]=0;
	currObj=pulse(currObj, signals, currentValues, signals_len);

	//14. Scan out	the AdderTree Data
	scanChain=AdderTree_SC;
	scanOut=scanOutVector_FAST( currObj, scanChain ) ;
	currObj=scanOut.currObj;

	//15. Copy the AdderTree scanOut Bits
	memcpy(scanoutInternal.scanOutBits_AdderTree_SC,scanOut.scanOutBits_AdderTree_SC,scan_len_AdderTree_SC);


	scanoutInternal.currObj=currObj;
	return scanoutInternal;

}
