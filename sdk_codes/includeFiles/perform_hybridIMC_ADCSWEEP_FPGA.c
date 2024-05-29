/*
 * perform_hybridIMC_ADCSWEEP_FPGA.c
 *
 *  Created on: Aug 11, 2022
 *      Author: Balaji Vijayakumar
 */

#include <custom_h.h>

scan_out perform_hybridIMC_ADCSWEEP_FPGA(curr_obj currObj,u8 bitLine, u8 IMG)
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

	//3. Enable DATA_Resis and RedResis and Retain, Remove Pre-charge on RBL
	u8 signals_preVerilogSET[3]={DataResis_EN,RefResis_EN,RBL_PRE_EN_B};
	u8 values_preVerilogSET[3]={0,0,1};
	signals_len = 3 ;
	currObj=bitsSet( currObj, signals_preVerilogSET, signals_len, values_preVerilogSET );

	//4. High-Speed Hybrd Mode Verilog Operations
	{
		//SELECT MODE HYBRID
		u8 GPIO_MODE_IMC_DATA=1;//0=C-INPUT 1=MODE_HYBRID; 2=MODE_INTERNAL; >2 = RESTRICTED-DO NOT USE
		XGpio_DiscreteWrite(&currObj.GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
	}

		{
			pulse01FPGA_RESET_handshake( currObj );
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

