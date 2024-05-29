/*
 * set_fastScan_Control_GPIO.c
 *
 *  Created on: Aug 22, 2023
 *      Author: ashlo
 */
#include <custom_h.h>
/* SPLITTER CODE IN VERILOG
   // ASHWIN ADDED FOR MULTIPLE FPGA_RESET STARTS
    parameter NR=2;// copied from PCB_CONTROL.v
    parameter NF=13;// copied from PCB_CONTROL.v

    output [(NR):0]FPGA_RESET_REPEAT_NUMREPEAT;
    output [(NF-1):0]FPGA_RESET_REPEAT_LB;
    output [(NF-1):0]FPGA_RESET_REPEAT_UB;

    assign {FPGA_RESET_REPEAT_NUMREPEAT[(NR):0],FPGA_RESET_REPEAT_UB[(NF-1):0],FPGA_RESET_REPEAT_LB[(NF-1):0]} =  gpio_io_o ;
 */
void set_TOPSW_GPIO_bank1(curr_obj currObj, u32 FPGA_RESET_REPEAT_LB, u32 FPGA_RESET_REPEAT_UB, u32 FPGA_RESET_REPEAT_NUMREPEAT)
{
	u32 dataWrite;
	dataWrite=0;
	int NR=2;
	int NF=13;
	dataWrite+=( ((u32)FPGA_RESET_REPEAT_LB)<<0 )+
	   ( ((u32)FPGA_RESET_REPEAT_UB)<<NF )+
	   ( ((u32)FPGA_RESET_REPEAT_NUMREPEAT)<<(NF+NF) );
	XGpio_DiscreteWrite(&currObj.GPIO_TOPSW_Inst,1,dataWrite);
}
void set_TOPSW_GPIO_bank2(curr_obj currObj)
{
}
