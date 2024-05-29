/*
 * loadBitlinetoCell_FPGA.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Ashwin Balagopal
 */


#include <custom_h.h>

curr_obj loadBitlinetoCell_FPGA(curr_obj currObj,u8 *bitLines, int bitLines_len)
{
	int k;
	u8 columnSelect;
	for(k=0;k<bitLines_len;k++)
	{
		columnSelect=bitLines[k];
		currObj=bitLineSelect(currObj,columnSelect);
		{
			//SELECT MODE WRITE
			u8 GPIO_MODE_READ_WRITE_DATA=1;//0=C-INPUT 1=MODE_WRITE; 2=MODE_READ; >2 = RESTRICTED-DO NOT USE
			XGpio_DiscreteWrite(&currObj.GPIO_MODE_READ_WRITE_Inst,1,GPIO_MODE_READ_WRITE_DATA);
		}
		{
			pulse01FPGA_RESET_handshake( currObj );
		}
		{
			//DESELECT MODE WRITE
			u8 GPIO_MODE_READ_WRITE_DATA=0;//0=C-INPUT 1=MODE_WRITE; 2=MODE_READ; >2 = RESTRICTED-DO NOT USE
			XGpio_DiscreteWrite(&currObj.GPIO_MODE_READ_WRITE_Inst,1,GPIO_MODE_READ_WRITE_DATA);
		}
	}
	columnSelect=0;
	currObj=bitLineSelect(currObj,columnSelect);//Default Setting
	return currObj;
}

