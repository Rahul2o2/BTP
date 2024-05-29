#include<custom_h.h>
#define wsi(wl_i, dl_i) ((wl_i * 97) + dl_i)

curr_obj fine_coarseSRAM(curr_obj currObj,u8 Fine_cm[88], u8 Coarse_cm[88], u8 Fine_sub[88], u8 Coarse_sub[88])
{
	u8 writeSRAMdata[97*68];

	int DL_nums_len=44;
	u8 DL_nums[44];
	int ws_i=0,wl_i=0,dl_i=0,i=0;

	//default=0
	for(ws_i=0;ws_i<97*68;ws_i++)
	{
		writeSRAMdata[ws_i]=0;
	}

	//Fine[i]= number of ones, index<Fine[i];index++ --> write ones, write 0s

	for(int l=0;l<44;l++)
	{
		i=2*l;
		dl_i=9+i;
		DL_nums[l]=dl_i;

		wl_i=0;
		//0:28:2 has Fine_CM Calibration for DL9,11...
		for(int j=0;j<Fine_cm[i];j++,wl_i+=2)
		{
			writeSRAMdata[wsi(wl_i,dl_i)]=1;
		}

		wl_i=1;
		//1:29:2 has Fine_CM Calibration for DL 10,12,...
		for(int j=0;j<Fine_cm[i+1];j++,wl_i+=2)
		{
			writeSRAMdata[wsi(wl_i,dl_i)]=1;
		}

		wl_i=30;
		//30:58:2 has Fine_SUB Calibration for DL9,11...
		for(int j=0;j<Fine_sub[i];j++,wl_i+=2)
		{
			writeSRAMdata[wsi(wl_i,dl_i)]=1;
		}

		wl_i=31;
		//31:59:2 has Fine_SUB Calibration for DL 10,12,...
		for(int j=0;j<Fine_sub[i+1];j++,wl_i+=2)
		{
			writeSRAMdata[wsi(wl_i,dl_i)]=1;
		}

		//WL60,62 has Coarse_CM Calibration for DL 9,11,...
		writeSRAMdata[wsi(60,dl_i)]=(Coarse_cm[i]>>1) & 1;
		writeSRAMdata[wsi(62,dl_i)]=Coarse_cm[i] & 1;

		//WL61,63 has Coarse_CM Calibration for DL 10,12,...
		writeSRAMdata[wsi(61,dl_i)]=(Coarse_cm[i+1]>>1) & 1;
		writeSRAMdata[wsi(63,dl_i)]=Coarse_cm[i+1]>>1 & 1;

		//WL64,66 has Coarse_SUB Calibration for DL 9,11,...
		writeSRAMdata[wsi(64,dl_i)]=(Coarse_sub[i]>>1) & 1;
		writeSRAMdata[wsi(66,dl_i)]=Coarse_sub[i] & 1;

		//WL65,67 has Coarse_SUB Calibration for DL 10,12,...
		writeSRAMdata[wsi(65,dl_i)]=(Coarse_sub[i+1]>>1) & 1;
		writeSRAMdata[wsi(67,dl_i)]=Coarse_sub[i+1] & 1;
	}
	writeSRAM_calib(currObj, writeSRAMdata, DL_nums, DL_nums_len);
	return currObj;
}

//if(Coarse[ci+1]==3)
//					writeSRAMdata[wsi]=1;
//
//				else if(Coarse[ci+1]==2&&j==66)
//					writeSRAMdata[wsi]=1;
//
//				else if(Coarse[ci+1]==1&&j==67)
//					writeSRAMdata[wsi]=1;
