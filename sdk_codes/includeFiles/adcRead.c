// 2ADC STARTS HERE
//#include <custom_h.h>
//
//void adcRead(u8* returnBytes, u16 repeatAvg, XAdcPs *XADC_VPVN_Instance)
//{
//	int iter;
//	u16 adcOut_VPVN[200];
//	u16 adcOut_AUX00[200];
//	double check_VPVN, check_AUX00, check_VPVN_map, check_AUX00_map;
//	double p1_VPVN  = 1.004738775718001, p2_VPVN = 0.0004344839776331213 ; // from MATLAB
//	double p1_AUX00 = 1.004732834060259, p2_AUX00 = 0.0002823608132561791 ; // from MATLAB
//	for (iter=0; iter<repeatAvg; iter++)
//			{
//			adcOut_VPVN[iter]=XAdcPs_GetAdcData( XADC_VPVN_Instance, XADCPS_CH_VPVN ) ;
//			adcOut_AUX00[iter]=XAdcPs_GetAdcData( XADC_VPVN_Instance, XADCPS_CH_AUX00 ) ;
//
//			adcOut_VPVN[iter]=adcOut_VPVN[iter]>>4;   // MSB:LSB+4 of the register is the data
//			adcOut_AUX00[iter]=adcOut_AUX00[iter]>>4; // MSB:LSB+4 of the register is the data
//
//			check_VPVN_map = ( ( ( ( (double) adcOut_VPVN[iter] ) * 0.244 * 0.001 ) * ( p1_VPVN ) ) + p2_VPVN ) * 1000 ;
//			check_VPVN = adcOut_VPVN[iter] * 0.244  ;
//			check_AUX00_map = ( ( ( ( (double) adcOut_AUX00[iter] ) * 0.244 * 0.001 ) * ( p1_AUX00 ) ) + p2_AUX00 ) * 1000 ;
//			check_AUX00 = adcOut_AUX00[iter] * 0.244 ;
//			returnBytes[(iter*4)]=(u8)(adcOut_VPVN[iter]>>6);//MSB 6 Bits
//			adcOut_VPVN[iter]=adcOut_VPVN[iter]<<10;
//			returnBytes[(iter*4)+1]=(u8)(adcOut_VPVN[iter]>>10);//LSB 6 Bits
//
//			returnBytes[(iter*4)+2]=(u8)(adcOut_AUX00[iter]>>6);//MSB 6 Bits
//			adcOut_AUX00[iter]=adcOut_AUX00[iter]<<10;
//			returnBytes[(iter*4)+3]=(u8)(adcOut_AUX00[iter]>>10);//LSB 6 Bits
//			}
//}
// 2ADC ENDS HERE

// 1ADC STARTS HERE
// From video https://youtu.be/QfjBP9KT5lo
//#include <custom_h.h>
//
//void adcRead(u8* returnBytes, u16 repeatAvg)
//{
//	int iter;
//	u16 adcOut[200];
//	double check1, check2;
//	double p1 = 0.993657, p2 = -0.332727 ; // from MATLAB
//	for (iter=0; iter<repeatAvg; iter++)
//			{
//			adcOut[iter] = Xil_In32(C_BASEADDR +  0x20C );
//			adcOut[iter]=adcOut[iter]>>4;
//			check1 = adcOut[iter] * 0.244 ;
////			check2 = ( ( adcOut[iter] * 0.244 ) - ( p2 ) ) / p1 ;
//			returnBytes[(iter*2)]=(u8)(adcOut[iter]>>6);//MSB 6 Bits
//			adcOut[iter]=adcOut[iter]<<10;
//			returnBytes[(iter*2)+1]=(u8)(adcOut[iter]>>10);//LSB 6 Bits
//			}
//}
// 1ADC ENDS HERE
// LAB PDF CODE STARTS HERE
#include <custom_h.h>
void adcRead(u8* returnBytes, u16 repeatAvg )
{

		#define SYSMON_DEVICE_ID XPAR_SYSMON_0_DEVICE_ID //ID of xadc_wiz_0
//		#define XSysMon_RawToExtVoltage(AdcData) \
//		((((float)(AdcData))*(1.0f))/65536.0f) //(ADC 16bit result)/16/4096 = (ADC 16bit result)/65536
		// voltage value = (ADC 16bit result)/65536 * 1Volt
		static XSysMon SysMonInst; //a sysmon instance
//		static int SysMonFractionToInt(float FloatNum);
		double check_VPVN, check_AUX00, check_VPVN_map, check_AUX00_map;
		u16 adcOut_VPVN[200];
		u16 adcOut_AUX00[200];
		u8 SeqMode;
		u16 ExtVolRawData;
		float TempData,VccIntData,ExtVolData;
		int xStatus;
		XSysMon_Config *SysMonConfigPtr;
		XSysMon *SysMonInstPtr = &SysMonInst;
//		init_platform();
//		print("Hello World\n\r");
		//----------------------------------------------------------------------- SysMon Initialize
		SysMonConfigPtr = XSysMon_LookupConfig(SYSMON_DEVICE_ID);
		if(SysMonConfigPtr == NULL) print("LookupConfig FAILURE\n\r");
		xStatus = XSysMon_CfgInitialize(SysMonInstPtr, SysMonConfigPtr,SysMonConfigPtr->BaseAddress);
		if(XST_SUCCESS != xStatus) print("CfgInitialize FAILED\r\n");
		//-----------------------------------------------------------------------------------------
		XSysMon_GetStatus(SysMonInstPtr); // Clear the old status
//		while(1)
//		{ //wait until EOS activated
		while ((XSysMon_GetStatus(SysMonInstPtr) & XSM_SR_EOS_MASK) != XSM_SR_EOS_MASK);
		for ( int iter=0; iter<repeatAvg; iter++)
		{

			ExtVolRawData = XSysMon_GetAdcData(SysMonInstPtr,XSM_CH_VPVN); //Read the external Vpn Data
			ExtVolRawData=ExtVolRawData>>4;
			adcOut_VPVN[iter]=ExtVolRawData;
			check_VPVN = ExtVolRawData * 0.244 ;

			ExtVolRawData = XSysMon_GetAdcData(SysMonInstPtr,XSM_CH_AUX_MIN); //Read the external Vaux0 Data
			ExtVolRawData=ExtVolRawData>>4;
			adcOut_AUX00[iter]=ExtVolRawData;
			check_AUX00 = ExtVolRawData * 0.244 ;

	//		ExtVolRawData = XSysMon_GetAdcData(SysMonInstPtr,XSM_CH_AUX_MIN+8);//Read the external Vaux8 Data
	//		ExtVolRawData=ExtVolRawData>>4;
	//		checkaux8 = ExtVolRawData * 0.244 ;

			returnBytes[(iter*4)]=(u8)(adcOut_VPVN[iter]>>6);//MSB 6 Bits
			adcOut_VPVN[iter]=adcOut_VPVN[iter]<<10;
			returnBytes[(iter*4)+1]=(u8)(adcOut_VPVN[iter]>>10);//LSB 6 Bits

			returnBytes[(iter*4)+2]=(u8)(adcOut_AUX00[iter]>>6);//MSB 6 Bits
			adcOut_AUX00[iter]=adcOut_AUX00[iter]<<10;
			returnBytes[(iter*4)+3]=(u8)(adcOut_AUX00[iter]>>10);//LSB 6 Bits
		}
}
// LAB PDF CODE ENDS HERE
