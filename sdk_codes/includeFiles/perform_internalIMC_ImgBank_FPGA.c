/*
 * perform_hybridIMC_BLIMG_FPGA.c
 *
 *  Created on: Aug 11, 2022
 *      Author: Balaji Vijayakumar
 */

#include <custom_h.h>

scan_out_internal perform_internalIMC_ImgBankFPGA(curr_obj currObj, u8 ImgBank)
{

	//u8 columnSelect;
	u8 scanChain;
	u8 signals[1];
	u8 currentValues[1];
	u8 values[1];
	scan_out_internal scanoutInternal;
	scan_out scanOut;
	int signals_len;

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

	//3. High-Speed Internal Mode Verilog Operations
	{
		//SELECT MODE INTERNAL
		u8 GPIO_MODE_IMC_DATA=2;//0=C-INPUT 1=MODE_HYBRID; 2=MODE_INTERNAL; >2 = RESTRICTED-DO NOT USE
		XGpio_DiscreteWrite(&currObj.GPIO_MODE_IMC_Inst,1,GPIO_MODE_IMC_DATA);
	}

		{
//			pulse01FPGA_RESET_handshake( currObj );
			pulse01FPGA_RESETRelaxed_handshake( currObj );
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
	int len_vecScanIn=110;
	//Hybrid Mode Pattern Choosen
	//u8 vecScanIn_SARTIMING_IN_SC[110]={0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
	//External Mode Pattern Choosen
	u8 vecScanIn_SARTIMING_IN_SC[110]={0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,1,1,0,0,0,1,0,0,1,1,0,0,0};
	currObj = scanInVector_uncompressed( currObj, vecScanIn_SARTIMING_IN_SC, len_vecScanIn, scanChain ) ;

	//6. Select ADC Scan Chain
	scanChain=ADC_SC;
	currObj=scanSelect(currObj,scanChain);

	int len_scanOUT=644;
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
			scanOut=scanOutVector( currObj, scanChain ) ;
			currObj=scanOut.currObj;
			for (k=0;k<644;k++)
			{
				scanoutInternal.scanOutBits_ADC_SC_IMGBL[k + len_scanOUT * (j + i * 4)]=scanOut.scanOutBits_ADC_SC[k];
			}

		}
	}
	scanoutInternal.currObj=currObj;
	return scanoutInternal;

}

