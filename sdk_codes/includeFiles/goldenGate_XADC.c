/*
 * goldenGate.c
 *
 *  Created on: Aug 29, 2023
 *      Author: ashlo
 */

// mode=1 ICQ
// mode=2 OCC
// function hardcoding
// XDL - 8
// XBL - 1 (debatable)
// WBL - 4
#include<custom_h.h>
#include <xtime_l.h>
// below is from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\024_InternalMode\Sno_2_AtSpeed_1XInternalMode_analyse.mlx
//u8 ADC_outs_total[WDL_HW]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,51,52,53,54,55,56,57,58,59,60,61,62,63,73,74,75,76,77,78,79,81,82,83,84,85,86,87,88,89};
// below is from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\028_SystemMAC_DryRuns\get_68_alphas.m
//u8 map_to_72[WDL_HW]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,41,42,43,44,45,46,47,48,49,50,51,52,53,55,56,57,58,59,60,61,63,64,65,66,67,68,69,70,71};
// Allocate memory for the Mapped_bins array
//float ADC_bins[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
//uint16_t IMC_Out_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
u16 Addertree_dec_ADC[XDL_HW][WBL_HW];
//float central_ref[XDL_HW];
//float MAc_ADC[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
// below 2 are from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\023_ADC_HybridMode\INLDNL_Analysis\getINL_TB.m
//float m_ADC[92]={0,0,1.025107,1.032128,1.025489,1.032700,1.024191,1.031287,1.026023,1.030712,1.025060,1.031913,1.026347,1.032243,1.025374,1.031215,1.024484,1.031601,1.025773,1.050062,0.992501,0.997822,0.989477,0.998443,0.994093,0.999253,0.991273,0.999457,1.024376,1.031077,1.024173,1.030725,1.024153,1.029870,1.022494,1.028598,1.023219,1.030670,1.022650,1.029159,1.020680,1.028810,1.022183,1.028108,1.021022,1.027716,1.024668,1.027907,1.021991,1.028341,1.026507,1.026733,1.020768,1.026706,1.021465,1.028195,1.020321,1.026459,1.019143,1.027220,1.019137,1.028125,1.019141,1.025985,0.993579,0.998046,0.992166,0.999723,0.992322,0.998132,0.994262,1.000540,1.067152,1.026526,1.016310,1.021558,1.017316,1.022121,1.016461,1.022199,1.031372,1.020864,1.012468,1.020531,1.010041,1.016970,1.010002,1.018126,1.010373,1.014538,0,0};
//float c_ADC[92]={0,0,-6.034851,-4.905083,-5.885475,-5.286806,-5.308894,-4.890551,-6.363450,-5.376364,-5.763179,-4.893069,-6.139452,-5.150959,-5.879969,-4.617934,-5.278545,-4.773973,-5.529030,-6.235813,-3.082974,-2.640330,-1.871929,-1.599959,-3.344192,-2.193959,-2.285426,-1.847942,-5.596451,-5.016215,-5.919217,-5.001976,-5.701873,-3.989588,-4.897534,-4.148247,-5.411687,-5.865431,-5.221248,-3.406162,-4.005877,-3.630097,-5.695843,-4.193620,-5.182105,-4.163274,-6.455421,-3.625954,-4.979580,-4.848085,-5.482722,-4.242137,-4.847774,-4.279084,-4.871782,-5.155615,-4.953736,-4.173928,-4.422278,-3.879749,-5.071174,-4.117119,-4.428910,-4.930677,-2.848711,-1.177514,-2.556141,-2.677568,-1.625951,-1.652792,-3.183424,-2.203051,-9.715866,-4.946203,-4.612488,-3.283906,-5.314438,-3.935548,-5.279199,-4.144894,-6.359056,-3.936474,-4.067334,-4.104750,-2.575834,-2.834948,-4.124738,-3.861806,-4.932272,-2.957848,0,0};

// OLD VALUES
//float m1R[72];//={16762.800000,16848.200000,16756.700000,16976.300000,16982.400000,16793.300000,16768.900000,17043.400000,16976.300000,17000.700000,17000.700000,16823.800000,16775.000000,17025.100000,16982.400000,16799.400000,16988.500000,16939.700000,17031.200000,17116.600000,17049.500000,16836.000000,16793.300000,16927.500000,17012.900000,16750.600000,16744.500000,16756.700000,16768.900000,16805.500000,16915.300000,16732.300000,16823.800000,16805.500000,16890.900000,17037.300000,16799.400000,16829.900000,16732.300000,16848.200000,16732.300000,16823.800000,16994.600000,16982.400000,16756.700000,16903.100000,16811.600000,16720.100000,17025.100000,16805.500000,16976.300000,17012.900000,16805.500000,16762.800000,16927.500000,16805.500000,16817.700000,16805.500000,16811.600000,16945.800000,17025.100000,17049.500000,16836.000000,16799.400000,16689.600000,16988.500000,16842.100000,16805.500000,16756.700000,16848.200000,16970.200000,16927.500000};
//float m2[72];//={1.065207,1.077715,1.068520,1.067020,1.079879,1.078069,1.067397,1.065857,1.066726,1.068980,1.067889,1.068341,1.080335,1.064980,1.076275,1.068050,1.076324,1.066160,1.077915,1.069939,1.078121,1.070577,1.070158,1.077265,1.075982,1.081535,1.079456,1.068495,1.067135,1.071194,1.068422,1.079664,1.079569,1.078705,1.070071,1.081164,1.079845,1.070440,1.068069,1.080996,1.079384,1.078709,1.079597,1.083275,1.068069,1.081029,1.081224,1.069721,1.080939,1.079244,1.080271,1.081722,1.080099,1.069945,1.072056,1.071014,1.069699,1.069374,1.079250,1.080099,1.083582,1.070049,1.069766,1.071975,1.069421,1.068449,1.067249,1.067506,1.067897,1.069190,1.079479,1.069126 };

// From D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\028_SystemMAC_DryRuns\CURRENTMIRRORCalibPattern\OUT_m1R_m2.txt
float m2[72];//={9.905075e-01,1.004051e+00,1.006072e+00,1.007702e+00,9.963650e-01,9.954237e-01,1.005740e+00,1.004358e+00,9.951638e-01,1.005147e+00,1.005230e+00,1.004242e+00,9.957325e-01,1.008023e+00,1.004486e+00,1.005183e+00,1.004540e+00,1.003749e+00,9.926400e-01,1.006001e+00,1.006879e+00,1.007251e+00,1.006494e+00,1.006549e+00,1.004736e+00,9.953325e-01,9.959387e-01,1.009842e+00,1.003496e+00,1.006701e+00,1.005139e+00,1.007471e+00,1.007449e+00,1.007749e+00,1.006356e+00,1.009576e+00,1.006749e+00,1.009795e+00,9.957350e-01,9.950537e-01,1.008472e+00,1.006977e+00,1.007274e+00,1.010709e+00,9.977712e-01,1.007605e+00,9.958625e-01,9.971312e-01,1.009086e+00,9.937512e-01,9.978738e-01,9.950700e-01,1.008000e+00,1.005079e+00,1.007355e+00,9.992163e-01,1.009735e+00,1.006119e+00,9.935412e-01,1.008091e+00,9.990012e-01,1.006797e+00,9.975650e-01,9.994850e-01,1.007245e+00,1.004920e+00,9.952962e-01,1.006035e+00,9.974787e-01,9.952412e-01,9.932212e-01,1.004687e+00, };
float m1R_1X[72];//={8.372250e+03,8.277700e+03,8.396650e+03,8.289900e+03,8.387500e+03,8.384450e+03,8.311250e+03,8.225850e+03,8.360050e+03,8.247200e+03,8.299050e+03,8.393600e+03,8.405800e+03,8.408850e+03,8.274650e+03,8.378350e+03,8.271600e+03,8.277700e+03,8.265500e+03,8.424100e+03,8.329550e+03,8.268550e+03,8.256350e+03,8.308200e+03,8.292950e+03,8.289900e+03,8.262450e+03,8.299050e+03,8.289900e+03,8.265500e+03,8.311250e+03,8.268550e+03,8.390550e+03,8.399700e+03,8.268550e+03,8.390550e+03,8.399700e+03,8.384450e+03,8.384450e+03,8.390550e+03,8.414950e+03,8.314300e+03,8.292950e+03,8.299050e+03,8.378350e+03,8.299050e+03,8.408850e+03,8.347850e+03,8.302100e+03,8.308200e+03,8.268550e+03,8.283800e+03,8.399700e+03,8.289900e+03,8.198400e+03,8.405800e+03,8.311250e+03,8.262450e+03,8.296000e+03,8.292950e+03,8.399700e+03,8.384450e+03,8.250250e+03,8.299050e+03,8.299050e+03,8.402750e+03,8.286850e+03,8.299050e+03,8.408850e+03,8.396650e+03,8.314300e+03,8.436300e+03 };
float m1R_2X[72];//={1.676280e+04,1.684820e+04,1.675670e+04,1.697630e+04,1.698240e+04,1.679330e+04,1.676890e+04,1.704340e+04,1.697630e+04,1.700070e+04,1.700070e+04,1.682380e+04,16775,1.702510e+04,1.698240e+04,1.679940e+04,1.698850e+04,1.693970e+04,1.703120e+04,1.711660e+04,1.704950e+04,1.683600e+04,1.679330e+04,1.692750e+04,1.701290e+04,1.675060e+04,1.674450e+04,1.675670e+04,1.676890e+04,1.680550e+04,1.691530e+04,1.673230e+04,1.682380e+04,1.680550e+04,1.689090e+04,1.703730e+04,1.679940e+04,1.682990e+04,1.673230e+04,1.684820e+04,1.673230e+04,1.682380e+04,1.699460e+04,1.698240e+04,1.675670e+04,1.690310e+04,1.681160e+04,1.672010e+04,1.702510e+04,1.680550e+04,1.697630e+04,1.701290e+04,1.680550e+04,1.676280e+04,1.692750e+04,1.680550e+04,1.681770e+04,1.680550e+04,1.681160e+04,1.694580e+04,1.702510e+04,1.704950e+04,16836,1.679940e+04,1.668960e+04,1.698850e+04,1.684210e+04,1.680550e+04,1.675670e+04,1.684820e+04,1.697020e+04,1.692750e+04 };
float m1R_4X[72];//={3.406240e+04,3.354651e+04,3.369989e+04,3.397874e+04,3.364411e+04,3.414606e+04,3.363017e+04,3.369989e+04,3.407634e+04,3.364411e+04,3.418789e+04,3.363017e+04,3.403451e+04,3.389509e+04,3.342103e+04,3.396480e+04,3.406240e+04,3.393691e+04,3.356046e+04,3.356046e+04,3.360229e+04,3.372777e+04,3.360229e+04,34160,3.364411e+04,3.414606e+04,3.395086e+04,3.376960e+04,3.361623e+04,3.396480e+04,3.365806e+04,3.409029e+04,3.360229e+04,3.363017e+04,3.406240e+04,3.361623e+04,3.402057e+04,3.413211e+04,3.418789e+04,34160,3.388114e+04,3.365806e+04,3.403451e+04,3.400663e+04,3.360229e+04,3.360229e+04,3.406240e+04,3.410423e+04,3.400663e+04,3.407634e+04,3.365806e+04,3.365806e+04,3.357440e+04,3.364411e+04,3.350469e+04,3.369989e+04,3.360229e+04,3.356046e+04,3.374171e+04,3.413211e+04,3.360229e+04,3.356046e+04,3.378354e+04,3.375566e+04,33672,3.356046e+04,3.351863e+04,3.383931e+04,3.411817e+04,3.399269e+04,3.393691e+04,3.416000e+04 };
float LT[72];
u8 ADC_outs_total[72];

float voltage_xadc_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
curr_obj goldenGate_XADC(float**** IMC_Out_XADC,float**** IMC_Out, mode_type mode, u8 slopeSetting, float* RefMax, int c, curr_obj currObj)
{
	scan_out_internal scanoutInternal[2];
	read_out_XADCvolt read_out_XADCvoltage[2];
	u8 ImgBank;


// TIME MEASURING CODE STARTS
//	XTime tStart_innerLoop;
//	XTime tStop_innerLoop;
//	XTime tElapsed_innerLoop;
//	double tElapsed_innerLoop_msec;
// TIME MEASURING CODE ENDS

	//7. Hybrid mode in SARTIMING_IN_SC scan chain
	{
		u8 scanChain=SARTIMING_IN_SC;
		int len_vecScanIn=scan_len_SARTIMING_IN_SC;
		//6-Bit ADC Set in Internal Mode
		u8 vecScanIn[scan_len_SARTIMING_IN_SC]={0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0};
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//3. FLOATADC_SC
	{
		u8 scanChain=FLOATADC_SC;
		int len_vecScanIn=scan_len_FLOATADC_SC;
		u8 vecScanIn[scan_len_FLOATADC_SC];
		memset(vecScanIn,1,scan_len_FLOATADC_SC*sizeof(u8));
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}

	//8. Adder Tree Enable
	{
	u8 signals[1]={ADCSTART_EXT};
	int signals_len=1;
	u8 values_bytes[1]={0};
	currObj=bitsSet(currObj, signals, signals_len, values_bytes);
	}


	ImgBank=0;
	read_out_XADCvoltage[ImgBank]=perform_XADC_MAC(currObj, ImgBank);
	ImgBank=1;
	read_out_XADCvoltage[ImgBank]=perform_XADC_MAC(currObj, ImgBank);

//3. FLOATADC_SC
	{
		u8 scanChain=FLOATADC_SC;
		int len_vecScanIn=scan_len_FLOATADC_SC;
		u8 vecScanIn[scan_len_FLOATADC_SC];
		memset(vecScanIn,1,scan_len_FLOATADC_SC*sizeof(u8));
		for(int i=0;i<WDL_HW;i++)
		{
			vecScanIn[ADC_outs_total[map_to_72[i]]]=0;
		}
		currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
	}


	//8. Adder Tree Enable
	{
	u8 signals[1]={ADCSTART_EXT};
	int signals_len=1;
	u8 values_bytes[1]={1};
	currObj=bitsSet(currObj, signals, signals_len, values_bytes);
	}

	ImgBank=0;
	scanoutInternal[ImgBank]=perform_IMC_Internal_pyTorch(currObj, ImgBank);
	ImgBank=1;
	scanoutInternal[ImgBank]=perform_IMC_Internal_pyTorch(currObj, ImgBank);
//A
//	// TIME MEASURING CODE STARTS
////		XTime_GetTime(&tStart_innerLoop);
//	// TIME MEASURING CODE ENDS
//
//	//currObj=init_internal(currObj);//293ms
//
//	// TIME MEASURING CODE STARTS
////		XTime_GetTime(&tStop_innerLoop);
////		tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
////		tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
//	// TIME MEASURING CODE ENDS
//
////		read_out_XADCvoltage[ImgBank]=perform_XADC_MAC(currObj, ImgBank);
////	scanoutInternal[ImgBank]=perform_IMC_Internal_pyTorch(currObj, ImgBank);

	// ADC binary to ADC bin conversion
	for(int xdl=0;xdl<XDL_HW;xdl++)//IMAGE
	{
		ImgBank=(xdl>=4);
		for(int xbl=0;xbl<XBL_HW;xbl++)
		{
			for( int wbl=0 ; wbl<WBL_HW ; wbl++ )//Bitline
			{
				for( int wdl=0;wdl<WDL_HW;wdl++ )
					{

							float ADC_bin_see;
							u8 *ADCbits;
							ADCbits       = &scanoutInternal[ImgBank].scanOutBits_ADC_SC_IMGBL[ 0 + ADC_outs_total[map_to_72[wdl]] * ADC_nbits + scan_len_ADC_SC * (wbl + (xdl%4) * 4)];
							ADC_bin_see  =   scanoutInternal[ImgBank].scanOutBits_ADC_SC_IMGBL[ 0 + ADC_outs_total[map_to_72[wdl]] * ADC_nbits + scan_len_ADC_SC * (wbl + (xdl%4) * 4)] ;//
						for( int b=1;b<ADC_nbits;b++)
						{
							ADC_bin_see += ((scanoutInternal[ImgBank].scanOutBits_ADC_SC_IMGBL[ b + ADC_outs_total[map_to_72[wdl]] * ADC_nbits + scan_len_ADC_SC * (wbl + (xdl%4) * 4)])<<b);
						}
						ADC_bins[xdl][xbl][wbl][wdl]=ADC_bin_see;


					}
				if(mode.mode==mode_ICQ){
					Addertree_dec_ADC[xdl][wbl]=0;
					for( int b=0;b<Addertree_output_nbits;b++)
						Addertree_dec_ADC[xdl][wbl]+=(scanoutInternal[ImgBank].scanOutBits_AdderTree_SC[ b + wbl * ( Addertree_output_nbits ) + (xdl%4) * ( WBL_HW * Addertree_output_nbits ) ]<<b);
				}
			}
		}
	}

	float *ADC_bins_see;
	ADC_bins_see=ADC_bins;

	float ADC_mapped_bin;
	float voltage,voltage_xadc;
	float MAC,MAC_xadc;
	float ADC_bin;
//	u8 mode=2;
	// ADC bin->mapped bin->voltage->MAC

	if(mode.mode==mode_ICQ){
		// getting the central reference for a chunk
		for(int xdl=0;xdl<XDL_HW;xdl++)//XDL - number of images stored in SRAM
			{
			central_ref[xdl]=0;
			for( int wbl=0 ; wbl<WBL_HW ; wbl++ )//Bitline
				{
					central_ref[xdl]+=(Addertree_dec_ADC[xdl][wbl]<<wbl);
				}
			central_ref[xdl]=RefMax[c]*central_ref[xdl]/16;//corresponding to an LSB of xm*2^(-FL)
			}
	}

	float *m1R ;
		if(mode.mode==mode_ICQ){
			switch( slopeSetting ){
			case 1:{
						m1R = m1R_1X ;
						break;
					}
			case 2:{
						m1R = m1R_2X ;
						break;
					}
			case 4: {
					m1R = m1R_4X ;
					break;
					}
			}
		}

	for(int xdl=0;xdl<XDL_HW;xdl++)//XDL - number of images stored in SRAM
	{
		ImgBank=(xdl>=4);
		for(int xbl=0;xbl<XBL_HW;xbl++)//XBL - #bits to DAC
		{
			for(int wbl=0;wbl<WBL_HW;wbl++)//WBL - #BLs in chip
			{
				for(int wdl=0;wdl<WDL_HW;wdl++)//WDL - #usable DLs in chip
				{
					ADC_bin=ADC_bins[xdl][xbl][wbl][wdl];

					// SLOPE AND OFFSET SETTINGS #1
//					ADC_mapped_bin=ADC_bin * m_ADC[ADC_outs_total[wdl]] + c_ADC[ADC_outs_total[wdl]];
//					voltage = ( ADC_mapped_bin / 127 ) * 0.9 ; // *** MUST BE MADE / 128

					// SLOPE AND OFFSET SETTINGS #2
//					ADC_mapped_bin = (ADC_bin-1)/2 * m_ADC[ADC_outs_total[wdl]] + c_ADC[ADC_outs_total[wdl]];
//					voltage = ( ADC_mapped_bin / 63 ) * 0.9 ;// *** MUST BE MADE / 64

					// SLOPE AND OFFSET SETTINGS #3
					ADC_mapped_bin=ADC_bin * m_ADC[ADC_outs_total[map_to_72[wdl]]] + c_ADC[ADC_outs_total[map_to_72[wdl]]];
					voltage = ( ADC_mapped_bin / 128 ) * 0.9 ;

					voltage_xadc = read_out_XADCvoltage[ImgBank].scanOutVolts_XADC[  ADC_outs_total[map_to_72[wdl]] + len_XADCVolts * (wbl + (xdl%4) * 4) ] ;
					// extra added by ashwin on 20-Sep-2023 starts
					voltage_see[xdl][xbl][wbl][wdl]  =  voltage;
					voltage_xadc_see[xdl][xbl][wbl][wdl]  =  voltage_xadc;
					// extra added by ashwin on 20-Sep-2023 ends
//					OLD mode_OCC code starts here
//					if( mode.mode == mode_OCC ){
//						//MAC = 0.86*( voltage / 0.9 ) * 160 ;
//						MAC 	 = 0.97 * ( voltage      / 0.9 ) * 166.5 ;
//						MAC_xadc = 1.06 * 0.97 * ( voltage_xadc / 0.9 ) * 166.5 ;
//					}
//					OLD mode_OCC code ends here
					if( mode.mode == mode_OCC ){
						float LSB=mode.LSB;
						float nbit_ADC=mode.nbit_ADC;
						float slopecorrect=mode.slopecorrect_chip;
						float vdd=mode.vdd;

						float OCC=mode.LSB*(pow(2,nbit_ADC)-1);
						MAC = slopecorrect * ( voltage / vdd ) * OCC ;
					}
					if( mode.mode==mode_ICQ ){
						float deltaMAC;
//						float LSB=1.59;
//						float LC=-26;
//						float k=0.5222E-6;
//						float slopecorrect_chip=0.86;
//						float slopecorrect_XADC=0.55;

						float LSB=mode.LSB;
//						float LC=mode.LC;
						float LC=-LT[map_to_72[wdl]];
						float I_mean=mode.I_mean;
						float slopecorrect_chip=mode.slopecorrect_chip;
						float slopecorrect_XADC=mode.slopecorrect_XADC;

						LC = ( ( central_ref[xdl] + LC ) > 0 ) ? LC : ( -central_ref[xdl] ) ;
						// one gets 0 V to 0.9 V for deltaMAC of LC to LC+63*LSB
						// y = y1 + (x-x1)*( y2 - y1 )/( x2 - x1 )
						// deltaMAC =  LC + ( ( voltage - 0 ) * ( ( ( 63*LSB + LC ) - ( LC ) ) / ( ( 0.9 ) - ( 0 ) ) ) ) ;
//						deltaMAC =  LC + ( voltage * 63*LSB / 0.9 ) ;
//						MAC = deltaMAC + central_ref[xdl] ;
						MAC =  ( m2[map_to_72[wdl]]*( central_ref[xdl] + LC ) ) + ( voltage  / ( m1R[map_to_72[wdl]] * I_mean ) ) ;
						MAC = ( MAC >= 0 ) ? MAC : 0 ; // MAC = min( MAC, 0 )
						MAC = slopecorrect_chip * MAC ;

						MAC_xadc =  ( m2[map_to_72[wdl]]*( central_ref[xdl] + LC ) ) + ( voltage_xadc  / ( m1R[map_to_72[wdl]] * I_mean ) ) ;
//						deltaMAC =  LC + ( voltage * 63*LSB / 0.9 ) ;
//						MAC_xadc = deltaMAC + central_ref[xdl] ;
						MAC_xadc = ( MAC_xadc >= 0 ) ? MAC_xadc : 0 ; // MAC = min( MAC, 0 )
						MAC_xadc = slopecorrect_XADC * MAC_xadc ;

					}
					IMC_Out[xdl][wdl][xbl][wbl]      =  (float) MAC;
					IMC_Out_XADC[xdl][wdl][xbl][wbl]      =  (float) MAC_xadc;
//					IMC_Out_see[xdl][xbl][wbl][wdl]  =  (uint16_t) MAC;
				}
			}
		}
	}
	return currObj;
}

//float ADCbin_to_mapped_bin( float ADC_bin, int wdl )
//{
////	float ADC_mapped_bin = ADC_bin * m[ADC_outs_total[wdl]] + c_ADC[ADC_outs_total[wdl]];
//	float ADC_mapped_bin = ADC_bin ;
//	return ADC_mapped_bin;
//}
//float mapped_bin_to_voltage( float ADC_mapped_bin )
//{
//	float voltage = ( ADC_mapped_bin / 127 ) * 0.9 ;
//	return voltage ;
//}
//float voltage_to_MAC( float voltage, u8 mode )
//{
//	float MAC;
//	if( mode == 2 ){
//		MAC = ( voltage / 0.9 ) * 160 ;
//	}
//	return MAC ;
//}
