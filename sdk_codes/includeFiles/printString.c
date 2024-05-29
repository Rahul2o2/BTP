#include <custom_h.h>
#include "xgpio.h"

void printString(curr_obj currObj)
{
	u32 dataWrite=(((u32)currObj.stringCurr[RWL_EN_EXT])<<31)+(((u32)currObj.stringCurr[SC_DIN])<<30)+(((u32)currObj.stringCurr[ScanSel_3])<<29)+(((u32)currObj.stringCurr[ScanSel_2])<<28)+(((u32)currObj.stringCurr[ScanSel_1])<<27)+(((u32)currObj.stringCurr[ScanSel_0])<<26)+(((u32)currObj.stringCurr[CLK_A])<<25)+(((u32)currObj.stringCurr[CLK_B])<<24)+(((u32)currObj.stringCurr[CLK_C])<<23)+(((u32)currObj.stringCurr[OEb_1])<<22)+(((u32)currObj.stringCurr[RBL_PRE_EN_B])<<21)+(((u32)currObj.stringCurr[OEb_2])<<20)+(((u32)currObj.stringCurr[WWL_EN])<<19)+(((u32)currObj.stringCurr[OEb_3])<<18)+(((u32)currObj.stringCurr[IMG_Sel_Dec_2])<<17)+(((u32)currObj.stringCurr[OEb_4])<<16)+(((u32)currObj.stringCurr[IMG_Sel_Dec_1])<<15)+(((u32)currObj.stringCurr[OEb_9])<<14)+(((u32)currObj.stringCurr[IMG_Sel_Dec_0])<<13)+(((u32)currObj.stringCurr[OEb_10])<<12)+(((u32)currObj.stringCurr[RW_SA_EN])<<11)+(((u32)currObj.stringCurr[ADC0_S0])<<10)+(((u32)currObj.stringCurr[SUBVoltPADConnect])<<9)+(((u32)currObj.stringCurr[ADC0_S1])<<8)+(((u32)currObj.stringCurr[MODE_R_WB])<<7)+(((u32)currObj.stringCurr[PRE_DR_EN])<<6)+(((u32)currObj.stringCurr[unused0])<<5)+(((u32)currObj.stringCurr[unused1])<<4)+(((u32)currObj.stringCurr[CS_0])<<3)+(((u32)currObj.stringCurr[CS_1])<<2)+(((u32)currObj.stringCurr[ADC1_S0])<<1)+(((u32)currObj.stringCurr[ADC1_S1])<<0);

	u32 dataWrite1=(((u32)currObj.stringCurr[BL_Sel_Dec_EXT_0])<<31)+(((u32)currObj.stringCurr[BL_Sel_Dec_EXT_1])<<30)+(((u32)currObj.stringCurr[unused2])<<29)+(((u32)currObj.stringCurr[unused3])<<28)+(((u32)currObj.stringCurr[unused4])<<27)+(((u32)currObj.stringCurr[unused5])<<26)+(((u32)currObj.stringCurr[RESET_GLOBALTIMER])<<25)+(((u32)currObj.stringCurr[unused6])<<24)+(((u32)currObj.stringCurr[CLK_DFF_EXT])<<23)+(((u32)currObj.stringCurr[unused7])<<22)+(((u32)currObj.stringCurr[DataCM_EN_EXT])<<21)+(((u32)currObj.stringCurr[OEb_5])<<20)+(((u32)currObj.stringCurr[DataSUB_EN_EXT])<<19)+(((u32)currObj.stringCurr[OEb_6])<<18)+(((u32)currObj.stringCurr[Reset_SARLOGIC_EXT])<<17)+(((u32)currObj.stringCurr[unused8])<<16)+(((u32)currObj.stringCurr[DataResis_EN])<<15)+(((u32)currObj.stringCurr[unused9])<<14)+(((u32)currObj.stringCurr[RefResis_EN])<<13)+(((u32)currObj.stringCurr[unused10])<<12)+(((u32)currObj.stringCurr[CLK_SARLOGIC_IN_EXT])<<11)+(((u32)currObj.stringCurr[unused11])<<10)+(((u32)currObj.stringCurr[unused12])<<9)+(((u32)currObj.stringCurr[SA_EN_EXT])<<8)+(((u32)currObj.stringCurr[AllVIN_n_EXT])<<7)+(((u32)currObj.stringCurr[RefSUB_EN_EXT])<<6)+(((u32)currObj.stringCurr[unused13])<<5)+(((u32)currObj.stringCurr[unused14])<<4)+(((u32)currObj.stringCurr[unused15])<<3)+(((u32)currObj.stringCurr[RefCM_EN_EXT])<<2)+(((u32)currObj.stringCurr[unused16])<<1)+(((u32)currObj.stringCurr[OEb_7])<<0);

	u32 dataWrite2=(((u32)currObj.stringCurr[FPGA_RESET])<<3)+(((u32)currObj.stringCurr[ADCSTART_EXT])<<2)+(((u32)currObj.stringCurr[PRE_VCM_EXT])<<1)+(((u32)currObj.stringCurr[OEb_8])<<0);

	XGpio_DiscreteWrite(&currObj.GPIO_PCB_32INPUTS_Inst,1,dataWrite);
	XGpio_DiscreteWrite(&currObj.GPIO_PCB_32INPUTS_1_Inst,1,dataWrite1);
	XGpio_DiscreteWrite(&currObj.GPIO_PCB_32INPUTS_2_Inst,1,dataWrite2);
}