/*
 * Copyright (C) 2009 - 2018 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include <custom_h.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xuartps.h"
#include "xparameters.h"
#include "sleep.h"

#include "xgpio.h"

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

#define BUFFER_SIZE 200000

struct tcp_pcb *globalTCP_pcb;

u8_t *write_head;

static u8 abcd[BUFFER_SIZE];
static int copy_head=0;

u8 function_index;
u8 scanChain;
u16_t scan_len;
u16_t bytes_to_send;
u16_t bytes_to_send2;
curr_obj currObj;

scan_out scanOut;
scan_out_internal scanoutInternal;

int transfer_data() {

	//tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
}

void print_app_header()
{
//#if (LWIP_IPV6==0)
//	xil_printf("\n\r\n\r-----lwIP TCP echo server ------\n\r");
//#else
//	xil_printf("\n\r\n\r-----lwIPv6 TCP echo server ------\n\r");
//#endif
//	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

err_t push_data(struct tcp_pcb *tpcb)
{

	u16_t packet_size = bytes_to_send2;
	u16_t max_bytes=tcp_sndbuf(tpcb);
	err_t status;

	if(bytes_to_send == 0)
	{
		return ERR_OK;
	}

	if(packet_size > max_bytes)
	{
		packet_size=max_bytes;
	}

	status = tcp_write(tpcb, (void*)write_head, packet_size, 1);

	//xil_printf("push_data: Asked to add %d bytes to send buffer, adding %d bytes\r\n",bytes_to_send, packet_size);

	if(packet_size > bytes_to_send2)
	{
		bytes_to_send=0;
	}
	else
	{
		bytes_to_send2-=packet_size;
	}

	write_head+=packet_size;

	return status;
}


err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}
	u8 rtvalue[1]={127};
//	xil_printf("entered recv_callback\r\n");
	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);
	memcpy(&abcd[copy_head],p->payload,p->len);

	function_index=abcd[0];
	if(function_index==105)
	{
		copy_head+=1446;
	}
	else
	{
		copy_head=0;
	}

	bytes_to_send=p->len;

	u8 signals_len;

	currObj=initialisation_code(currObj);

	//cases
	switch(function_index)
	{
		case 64:
		{		//bitSet
			signals_len=(bytes_to_send-1)/2;
			u8 signals[signals_len];
			u8 values_bytes[signals_len];
			for(int k=0;k<signals_len;k++)
			{
				signals[k]=abcd[k+1];
				values_bytes[k]=abcd[signals_len+1+k];
			}

			currObj=bitsSet(currObj, signals, signals_len, values_bytes);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);

			break;
		}
		case 73:
		{
			//defaultState
			currObj=defaultState(currObj);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 67:
		{//scanSelect
			scanChain=abcd[1];
			currObj=scanSelect(currObj,scanChain);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 68:
			{//scanInVector
				int len_vecScanIn=bytes_to_send-2;
				u8 vecScanIn[len_vecScanIn];
				scanChain=abcd[1];
				for(int k=0;k<len_vecScanIn;k++)
				{
					vecScanIn[k]=abcd[k+2];
				}
				currObj=scanInVector_uncompressed_FAST(currObj,vecScanIn, len_vecScanIn,scanChain);
				tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
				break;
			}
		case 69:
		{
			//scanOutVector
			scanChain=abcd[1];
			scanOut=scanOutVector(currObj,scanChain);//MARKER
			currObj=scanOut.currObj;
			switch(scanChain)
			{
				case SA_SC:
				{
					scan_len=97;
					break;
				}
				case ADC_SC:
				{
					scan_len=644;
					break;
				}
				case AdderTree_SC:
				{
					scan_len=176;
					break;
				}
				case SARTIMING_OUT_SC:
				{
					scan_len=26;
					break;
				}
				case WRITE_2_SC:
				{
					scan_len=97;
					break;
				}
				case WRITE_1_SC:
				{
					scan_len=97;
					break;
				}
				case WRITE_0_SC:
				{
					scan_len=97;
					break;
				}
				case IMC_RBL_SC:
				{
					scan_len=88;
					break;
				}
				case IMC_VOut_SC:
				{
					scan_len=88;
					break;
				}
				case WWL_SC:
				{
					scan_len=1160;
					break;
				}
				case FLOATADC_SC:
				{
					scan_len=92;
					break;
				}
				case SARTIMING_IN_SC:
				{
					scan_len=110;
					break;
				}
				case ControlPath_SC:
				{
					scan_len=17;
					break;
				}
			}
			u8 returnBytes[scan_len];
			u8 len_send[2]={((scan_len>>8)&0xFF),(scan_len&0xFF)};
			tcp_write(globalTCP_pcb,&len_send,2,1);

			switch(scanChain)
			{
				int k;
				case SA_SC:
				{
					for(k=0;k<scan_len;k++)
					{
						returnBytes[k]=scanOut.scanOutBits_SA_SC[k];
					}
					break;
				}
				case ADC_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
					break;
				}
				case AdderTree_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_AdderTree_SC[k];
					break;
				}
				case SARTIMING_OUT_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_SARTIMING_OUT_SC[k];
					break;
				}
				case WRITE_2_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WRITE_2_SC[k];
					break;
				}
				case WRITE_1_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WRITE_1_SC[k];
					break;
				}
				case WRITE_0_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WRITE_0_SC[k];
					break;
				}
				case IMC_RBL_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_IMC_RBL_SC[k];
					break;
				}
				case IMC_VOut_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_IMC_VOut_SC[k];
					break;
				}
				case WWL_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WWL_SC[k];
					break;
				}
				case FLOATADC_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_FLOATADC_SC[k];
					break;
				}
				case SARTIMING_IN_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_SARTIMING_IN_SC[k];
					break;
				}
				case ControlPath_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_ControlPath_SC[k];
					break;
				}

			}

			tcp_write(globalTCP_pcb,&returnBytes,sizeof(returnBytes),1);
			break;
		}
		case 70:
		{
			//bitLineSelect
			u8 bitLine=abcd[1];
			currObj=bitLineSelect(currObj,bitLine);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 72:
		{	//pulse
			abcd[0]=27;

			int signals_len=(bytes_to_send-1)/2;
			u8 signals[signals_len];
			u8 currentValues[signals_len];
			for(int k=0;k<signals_len;k++)
			{
				signals[k]=abcd[k+1];
				currentValues[k]=abcd[k+signals_len+1];
			}
			currObj=pulse(currObj, signals, currentValues, signals_len);

			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 74:
			//scanInWriteScanChains_dataOnly
			abcd[0]=47;
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		case 75:
			//scanInWriteScanChains_maskOnly
			abcd[0]=57;
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		case 76:
			//scanInWriteChains_new
		{
			int data_len=(bytes_to_send-1)/2;
			u8 data[data_len];
			for(int k=0;k<data_len;k++)
			{
				data[k]=abcd[1+k];
			}
			int mask_len=data_len;
			u8 mask[mask_len];
			for(int k=0;k<mask_len;k++)
			{
				mask[k]=abcd[data_len+1+k];
			}
			currObj=scanInWriteScanChains_new(currObj,data,data_len,mask,mask_len);

			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 77:
		{
			//loadBitlinetoCell_FPGA
			int bitLines_len=bytes_to_send-1;
			u8 bitLines[bitLines_len];
			for(int k=0;k<bitLines_len;k++)
			{
				bitLines[k]=abcd[k+1];
			}

			currObj=loadBitlinetoCell_FPGA( currObj, bitLines, bitLines_len );
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 78:
		{	//loadCelltoSACtr_FPGA
			int bitLines_len=(bytes_to_send-1);
			u8 bitLines[bitLines_len];
			for(int k=0;k<bitLines_len;k++)
			{
				bitLines[k]=abcd[k+1];
			}
			currObj=loadCelltoSACtr_FPGA( currObj, bitLines, bitLines_len );
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 80:
			//write_onto_datalines
		{
			int len_data_DL = 1092 ;
			u8 data_DL[1092];
			for(int i=0 ; i<len_data_DL ; i++ )
			{
				data_DL[i] = abcd[1+i];
			}

			int mark = len_data_DL+1 ;
			int i = mark ;

			u8 colSel[4];

			do
			{
				colSel[ i - mark ] = abcd[i];

				i++;
			}while(abcd[i]!=126); //i holds index of parameterMarkerByte
			int len_colSel = i - mark;
			int j = i+1;
			int k = 0;
			u8 DL_nums[97];
			do
			{
				DL_nums[k] = abcd[j];
				j++;
				k++;
			}while(abcd[j]!=127);

			int len_DL_nums = k;

			int dl_row=0;
			u8 data_DLbitwiseWithUnused[1160];
			int len_data_DLbitwiseWithUnused=1160;
			//INITIALISE UNUSED SCAN BITS TO ZERO HERE
			int unusedBits[68]={32,33,66,67,100,101,134,135,168,169,202,203,236,237,270,271,304,305,338,339,372,373,406,407,440,441,474,475,508,509,542,543,576,577,610,611,644,645,678,679,712,713,746,747,780,781,814,815,848,849,882,883,916,917,950,951,984,985,1018,1019,1052,1053,1086,1087,1088,1089,1158,1159};
			int len_unusedBits=68;
			for (i=0;i<len_unusedBits;i++)
			{
				data_DLbitwiseWithUnused[unusedBits[i]]=0;//MAKING ALL UNUSED BITS AS ZERO
			}
			//POPULATING USEFUL DATA
			for(i=0;i<len_data_DL;i++)
			{
				if(data_DL[i]==1)
				{
					data_DLbitwiseWithUnused[dl_row]=1;
				}
				else
				{
					data_DLbitwiseWithUnused[dl_row]=0;
				}
				// if dlRow is in the unusedbits then do dlrow++
				if (dl_row%34 == 31 && dl_row != 1085 && dl_row < 1089)
				{
					dl_row += 3;
				}
				else if (dl_row == 1085)
				{
				   dl_row += 5;
				}
				else if (dl_row != 1157)
				{
					dl_row++;
				}
			}
			i=0,j=0,k=0,dl_row=0;
			int data_1_i=0,data_0_i=0;
			int data_1_row_indices[1160];
			int data_0_row_indices[1160];

			for(i=0;i<len_data_DLbitwiseWithUnused;i++)
			{
				if(data_DLbitwiseWithUnused[i]==1)
				{
					data_1_row_indices[data_1_i] = dl_row;
					data_1_i++;
				}
				else
				{
					data_0_row_indices[data_0_i] = dl_row;
					data_0_i++;
				}
				dl_row++;
			}

			int len_data_1_row_indices = data_1_i;
			int len_data_0_row_indices = data_0_i;



			currObj = write_onto_datalines(currObj,data_1_row_indices,len_data_1_row_indices,data_0_row_indices,len_data_0_row_indices,colSel,len_colSel,DL_nums,len_DL_nums);



			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 83:
		{	//adcRead
			u16 repeatAvg;
			repeatAvg=(u16)(abcd[1]);
			u8 returnBytes[200];
			int fileSize_return=repeatAvg*4;
			adcRead( returnBytes, repeatAvg );
			tcp_write(globalTCP_pcb,&returnBytes,fileSize_return,1);
			break;
		}
		case 90:
		{	//perform_RxOnRowColSel_FPGA_2
			// get the data to the read scan chain
			u16 WLnum=abcd[1]*100+abcd[2];
			u8 BL=abcd[3];
			scanOut=perform_RxOnRowColSel_FPGA(currObj, &BL, WLnum) ;

			currObj=scanOut.currObj;
			scan_len=97;
			u8 returnBytes[97];

			for(int k=0;k<scan_len;k++)
			{
				returnBytes[k]=scanOut.scanOutBits_SA_SC[k];
			}
			tcp_write(globalTCP_pcb,&returnBytes,97,1);
			break;
		}
		case 91:
		{	//performWxOnRowColSel_FPGA_2
			int data_len=(bytes_to_send-4)/2;
			u8 data[data_len];
			for(int k=0;k<data_len;k++)
			{
				data[k]=abcd[4+k];
			}
			int mask_len=data_len;
			u8 mask[mask_len];
			for(int k=0;k<mask_len;k++)
			{
				mask[k]=abcd[data_len+4+k];
			}
			u16 WLnum=abcd[1]*100+abcd[2];
			u8 BL=abcd[3];
			currObj=perform_WxOnRowColSel_FPGA_2_new(currObj, data, data_len, mask, mask_len, &BL, WLnum) ;

			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 94:
		{	//adcRead_BLs
			abcd[0]=49;
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 84:
		{	//wait
			u8 waitTime;
			waitTime=abcd[1];// in 100s of microseconds, i.e. max wait is 12700 microseconds
			u16 actualWaitTime = ( (u16) (waitTime) ) * 100 ;

			usleep( actualWaitTime );//MARKER
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 104:
		{	//readSRAM
			u8 bitLine=abcd[1];

			SRAM_Store SRAMStore;
			SRAMStore = readSRAM(currObj,bitLine);
			currObj=SRAMStore.currObj;
			int lenSRAMdata=97*1092;
			u8 returnBytes[lenSRAMdata];
			for(int k=0;k<lenSRAMdata;k++)
			{
				returnBytes[k]=SRAMStore.SRAMdata[k];
			}
			tcp_write(globalTCP_pcb,&returnBytes,lenSRAMdata,1);
			break;
		}
		case 105:
		{	//writeSRAM
			if(copy_head>1024*97)
			{
				int len_SRAMdata = 1024*97 ;
				 u8 writeSRAMdata[len_SRAMdata];
				for(int i=0 ; i<len_SRAMdata ; i++ )
				{
					writeSRAMdata[i] = abcd[1+i];
				}


				u8 colSel=abcd[len_SRAMdata+1];

				int j = len_SRAMdata+2;
				int k = 0;
				u8 DL_nums[97];

				do
				{
					DL_nums[k] = abcd[j];//handled by twoByteSplit in MATLAB
					j++;
					k++;
				}while(abcd[j]!=127);

				int len_DL_nums = k;

				int bitLine=colSel;

				currObj=writeSRAM_1024_FAST_NEW(currObj,bitLine, writeSRAMdata,  DL_nums, len_DL_nums);
				rtvalue[0]=129;
				tcp_write(globalTCP_pcb,&rtvalue,1,1);
				copy_head=0;
			}
			else
			tcp_write(globalTCP_pcb,&rtvalue,1,1);
			break;
		}
		case 106:
		{	//fine_coarseSRAM
			u8 Fine_cm[88],Coarse_cm[88],Fine_sub[88],Coarse_sub[88];
			for(int i=0;i<88;i++)
			{
				Fine_cm[i]=abcd[1+i];
				Fine_sub[i]=abcd[89+i];
				Coarse_cm[i]=abcd[177+i];
				Coarse_sub[i]=abcd[264+i];
			}
			currObj=fine_coarseSRAM(currObj,Fine_cm,Coarse_cm,Fine_sub,Coarse_sub);

			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 107:
		{	//IMGSelect
			u8 IMG=abcd[1];
			currObj=IMGSelect(currObj,IMG);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 108:
		{	//perform_hybridIMC_BLIMG_FPGA
			u8 BL=abcd[1];

			u8 IMG=abcd[2];
			scanOut=perform_hybridIMC_BLIMG_FPGA(currObj, BL, IMG) ;
			currObj=scanOut.currObj;

			// loading returnBytes array
			scan_len=644;
			u8 returnBytes[644];

			for(int k=0;k<scan_len;k++)
			{
				returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
			}
			bytes_to_send2=scan_len;
			write_head = returnBytes;

			if(bytes_to_send2>0)
			{
				err=push_data(tpcb);
			}
			break;
		}
		case 109:
		{	//perform_internalIMC_ImgBankFPGA
			u8 ImgBank=abcd[1];//ImgBank takes 0 or 1
			scanoutInternal=perform_internalIMC_ImgBankFPGA(currObj,  ImgBank) ;
			//scanoutInternal=perform_IMC_Internal_pyTorch(currObj,  ImgBank) ;//Comment THIS STATEMENT
			currObj=scanoutInternal.currObj;
			scan_len=644*4*4;
			u8 returnBytes[644*4*4];


			for(int k=0;k<scan_len;k++)
			{
				returnBytes[k]=scanoutInternal.scanOutBits_ADC_SC_IMGBL[k];
			}

			tcp_write(globalTCP_pcb,&returnBytes,scan_len,1);
			break;
		}
		case 110:
		{//LoadwriteSRAM_SDCard
			break;
		}
		case 111:
		{//RBL_IMC_Select

			u8 RBL=abcd[1];
			currObj=RBL_IMC_Select(currObj, RBL);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 112:
		{//writeSRAM_1024_FAST_NEW
			break;
		}
		case 114:
		{//scanOutVector_FAST
			scanChain=abcd[1];
			scanOut=scanOutVector_FAST(currObj,scanChain);//MARKER
			//back to main
			currObj=scanOut.currObj;
			switch(scanChain)
			{
				case SA_SC:
				{
					scan_len=97;
					break;
				}
				case ADC_SC:
				{
					scan_len=644;
					break;
				}
				case AdderTree_SC:
				{
					scan_len=176;
					break;
				}
				case SARTIMING_OUT_SC:
				{
					scan_len=26;
					break;
				}
				case WRITE_2_SC:
				{
					scan_len=97;
					break;
				}
				case WRITE_1_SC:
				{
					scan_len=97;
					break;
				}
				case WRITE_0_SC:
				{
					scan_len=97;
					break;
				}
				case IMC_RBL_SC:
				{
					scan_len=88;
					break;
				}
				case IMC_VOut_SC:
				{
					scan_len=88;
					break;
				}
				case WWL_SC:
				{
					scan_len=1160;
					break;
				}
				case FLOATADC_SC:
				{
					scan_len=92;
					break;
				}
				case SARTIMING_IN_SC:
				{
					scan_len=110;
					break;
				}
				case ControlPath_SC:
				{
					scan_len=17;
					break;
				}
			}
			u8 returnBytes[scan_len];
			u8 len_send[2]={((scan_len>>8)&0xFF),(scan_len&0xFF)};
			tcp_write(globalTCP_pcb,&len_send,2,1);

			switch(scanChain)
			{
				int k;
				case SA_SC:
				{
					for(k=0;k<scan_len;k++)
					{
						returnBytes[k]=scanOut.scanOutBits_SA_SC[k];
					}
					break;
				}
				case ADC_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
					break;
				}
				case AdderTree_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_AdderTree_SC[k];
					break;
				}
				case SARTIMING_OUT_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_SARTIMING_OUT_SC[k];
					break;
				}
				case WRITE_2_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WRITE_2_SC[k];
					break;
				}
				case WRITE_1_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WRITE_1_SC[k];
					break;
				}
				case WRITE_0_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WRITE_0_SC[k];
					break;
				}
				case IMC_RBL_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_IMC_RBL_SC[k];
					break;
				}
				case IMC_VOut_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_IMC_VOut_SC[k];
					break;
				}
				case WWL_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_WWL_SC[k];
					break;
				}
				case FLOATADC_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_FLOATADC_SC[k];
					break;
				}
				case SARTIMING_IN_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_SARTIMING_IN_SC[k];
					break;
				}
				case ControlPath_SC:
				{
					for(k=0;k<scan_len;k++)
						returnBytes[k]=scanOut.scanOutBits_ControlPath_SC[k];
					break;
				}

			}

			tcp_write(globalTCP_pcb,&returnBytes,scan_len,1);
			break;
		}
		case 115:
		{//readSRAMSliced
			u8 ***Read_out_weight = (u8***)calloc(68,sizeof(u8**));
			for(uint16_t wd = 0; wd < 68; wd++){
				Read_out_weight[wd] = (u8**)calloc(1024,sizeof(u8*));
				for(uint16_t wl = 0; wl < 1024; wl++){
					Read_out_weight[wd][wl] = (u8*)calloc(4,sizeof(u8));
				}
			}


			// Read_out_reference->2x8x1024x4
			u8 ****Read_out_reference = (u8****)calloc(2,sizeof(u8***));
			for(uint16_t lh = 0; lh < 2; lh++){
				Read_out_reference[lh] = (u8***)calloc(8,sizeof(u8**));
				for(uint16_t rd = 0; rd < 8; rd++){
					Read_out_reference[lh][rd] = (u8**)calloc(1024,sizeof(u8*));
					for(uint16_t wl = 0; wl < 1024; wl++){
						Read_out_reference[lh][rd][wl] = (u8*)calloc(4,sizeof(u8));
					}
				}
			}


			// Read_out_referenceAdderTree->1024x4
			u8 **Read_out_referenceAdderTree = (u8**)calloc(1024,sizeof(u8*));
			for(uint16_t wl = 0; wl < 1024; wl++){
				Read_out_referenceAdderTree[wl] = (u8*)calloc(4,sizeof(u8));
			}


			// Read_out_image->8x1024
			u8 **Read_out_image = (u8**)calloc(8,sizeof(u8*));
			for(uint16_t rd = 0; rd < 8; rd++){
				Read_out_image[rd] = (u8*)calloc(1024,sizeof(u8));
			}


			readSRAMSliced(currObj, Read_out_image, Read_out_weight, Read_out_reference, Read_out_referenceAdderTree);



			{// Read_out_weight->68x1024x4
				u8 Read_out_weight_FLAT[68*1024*4];
				for(int i = 0; i < 4; i++)
				{
					for(int j = 0; j < 1024; j++)
					{
						for(int k = 0; k < 68; k++)
						{
								Read_out_weight_FLAT[k+j*(68)+i*(68*1024)]=Read_out_weight[k][j][i];
						}
					}
				}



				char FileName_Rwt[]="Rwt.bin";
				u32 size=(68*1024*4)*sizeof(u8);
				int rc=WriteFile_custom(FileName_Rwt, size, (void *) Read_out_weight_FLAT);
			}





			{// Read_out_ref->2x8x1024x4
				u8 Read_out_ref_FLAT[2*8*1024*4];
				for(int i = 0; i < 4; i++)
				{
					for(int j = 0; j < 1024; j++)
					{
						for(int k = 0; k < 8; k++)
						{
							for(int l = 0; l < 2; l++)
							{
								Read_out_ref_FLAT[l+k*2+j*(2*8)+i*(2*8*1024)]=Read_out_reference[l][k][j][i];
							}
						}
					}
				}



				char FileName_RRef[]="RRef.bin";
				u32 size=(2*8*1024*4)*sizeof(u8);
				int rc=WriteFile_custom(FileName_RRef, size, (void *) Read_out_ref_FLAT);
			}


			{// Read_out_referenceAdderTree->1024x4
				u8 Read_out_referenceAdderTree_FLAT[1024*4];
				for(int i = 0; i < 4; i++){
					for(int j = 0; j < 1024; j++){
						Read_out_referenceAdderTree_FLAT[j+i*1024]=Read_out_referenceAdderTree[j][i];
					}
				}

				char FileName_RATr[]="RATr.bin";
				u32 size=(1024*4)*sizeof(u8);
				int rc=WriteFile_custom(FileName_RATr, size, (void *) Read_out_referenceAdderTree_FLAT);
			}

			{// Read_out_image->8x1024
				u8 Read_out_image_FLAT[8*1024];
				for(int i = 0; i < 1024; i++){
					for(int j = 0; j < 8; j++){
						Read_out_image_FLAT[j+i*8]=Read_out_image[j][i];
					}
				}

				char FileName_RIMG[]="RIMG.bin";
				u32 size=(8*1024)*sizeof(u8);

				int rc=WriteFile_custom(FileName_RIMG, size, (void *) Read_out_image_FLAT);
			}
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 116:
		{//perform_hybridIMC_ADCSWEEP_FPGA
			u8 BL=abcd[1];
			u8 IMG=abcd[2];
			scanOut=perform_hybridIMC_ADCSWEEP_FPGA(currObj, BL, IMG) ;

			currObj=scanOut.currObj;
		// loading returnBytes array
			scan_len=644;
			u8 returnBytes[644];
			for(int k=0;k<scan_len;k++)
			{
				returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
			}
			tcp_write(globalTCP_pcb,&returnBytes,scan_len,1);
			break;
		}
		case 117:
		{//perform_internalIMC_TOPSW_ImgBankFPGA
			u8 ImgBank=abcd[1];//ImgBank takes 0 or 1
			scanoutInternal=perform_internalIMC_TOPSW_ImgBankFPGA(currObj,  ImgBank) ;
			//scanoutInternal=perform_IMC_Internal_pyTorch(currObj,  ImgBank) ;//Comment THIS STATEMENT
			currObj=scanoutInternal.currObj;
			scan_len=644*4*4;
			u8 returnBytes[644*4*4];

			for(int k=0;k<scan_len;k++)
			{
				returnBytes[k]=scanoutInternal.scanOutBits_ADC_SC_IMGBL[k];
			}
			tcp_write(globalTCP_pcb,&returnBytes,scan_len,1);
			break;
		}
		case 118:
		{
		//perform_hybridIMC_TOPSW_FPGA
			u8 BL=abcd[1];
			u8 IMG=abcd[2];
			scanOut=perform_hybridIMC_TOPSW_FPGA(currObj, BL, IMG) ;

			currObj=scanOut.currObj;

		// loading returnBytes array
			scan_len=644;
			u8 returnBytes[644];

			for(int k=0;k<scan_len;k++)
			{
				returnBytes[k]=scanOut.scanOutBits_ADC_SC[k];
			}
			tcp_write(globalTCP_pcb,&returnBytes,scan_len,1);
			break;
		}
		case 119:
		{//store_images_SRAM_MultiBitX
			u8 X[1024],IMGnum,mode;
			IMGnum=abcd[1];
			mode=abcd[2];
			for(int i=0;i<1024;i++)
			{
				X[i]=abcd[3+i];
			}
			currObj=store_images_SRAM_MultiBitX(X,mode,IMGnum ,currObj);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 120:
		{//set_modeInternalHybridTiming
			u8 internal_hyb_B=abcd[1];
			u16 IMG_TIME_GPIO_DATA=(abcd[2]<<6)+abcd[3];
			u8 RWL_EN_DELAY=abcd[4];
			u8 CM_EN_DELAY=abcd[5];
			u8 ADCSTART_DELAY=abcd[6];
			u8 SAMPLING_DELAY=abcd[7];
			u8 EVALUATE_DELAY=abcd[8];
			u32 FPGA_RESET_REPEAT_UB_hybrid=(u32)((abcd[9]<<6)+abcd[10]);

			currObj= set_modeInternalHybridTiming(currObj,internal_hyb_B,IMG_TIME_GPIO_DATA,RWL_EN_DELAY,CM_EN_DELAY,ADCSTART_DELAY,SAMPLING_DELAY,EVALUATE_DELAY,FPGA_RESET_REPEAT_UB_hybrid);
			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
		case 121:
		{//set_ADCbitModeInt_HybB
			u8 internal_hyb_B=abcd[1];
			u8 ADC_bitMode=abcd[2];
			currObj= set_ADCbitModeInt_HybB(currObj,internal_hyb_B,ADC_bitMode);

			tcp_write(globalTCP_pcb,&abcd,bytes_to_send,1);
			break;
		}
	}

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

err_t sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
//	xil_printf("entered sent_callback");
//	xil_printf(" bytes_to_send = %d\r\n", bytes_to_send);
//	xil_printf(" len = %d\r\n",len);
//	xil_printf(" free space = %d\r\n", tcp_sndbuf(tpcb));

	if(bytes_to_send <=0)
		return ERR_OK;

	return push_data(tpcb);
}
err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	//tcp_sent(newpcb, sent_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	globalTCP_pcb = newpcb;

	return ERR_OK;
}

int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	//xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}
