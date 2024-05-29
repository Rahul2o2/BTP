/* store_images_SRAM_MultiBitX.c*/

#include<utilsinclude.h>
#include<custom_h.h>
//u8 IMG_DL_nums[8]={1,2,3,4,5,6,7,8};
curr_obj store_images_SRAM_MultiBitX(uint8_t* X,u8 mode,u8 IMGnum ,curr_obj currObj)
{
	u8 writeSRAMdata[9*1024];
    int bitLine=0;

	memset(writeSRAMdata,0,9*1024*sizeof(writeSRAMdata[0]));


	if(mode==1)
	{
		//DAC in 1b MODE where IMG_Select_DL_nums = 1,3,5,7 for IMG=0-3; IMG_Select_DL_nums=2,4,6,8 for IMG=4-7;
		//Will select 1,2=MSB DLs and 7,8= LSB DLs to populate 1/0;  other DLs are written with 0
		//NOTE: 1,2=MSB DLs 3,4=MSB-1 DLs 5,6=LSB+1 DLs 7,8= LSB DLs
		for(int i=0;i<1024;i++)
		{
			if(IMGnum<4)
			{
				if(X[i])
				{
					writeSRAMdata[9*i+1]=1;
				}
				else
				{
					writeSRAMdata[9*i+7]=1;
				}
			}
			else
			{
				if(X[i])
				{
					writeSRAMdata[9*i+2]=1;
				}
				else
				{
					writeSRAMdata[9*i+8]=1;
				}
			}

		}
	}
	else//mode>=2
	{
		//DAC in 2b MODE where IMG_Select_DL_nums = 1,3,5,7 for IMG=0-3; IMG_Select_DL_nums=2,4,6,8 for IMG=4-7;
		//Will select 1,2=MSB DLs; 3,4=MSB-1; 5,6=LSB+1 and 7,8= LSB DLs to populate 1/0;  other DLs are written with 0
		//NOTE: 1,2=MSB DLs 3,4=MSB-1 DLs 5,6=LSB+1 DLs 7,8= LSB DLs
		for(int i=0;i<1024;i++)
		{
			if(IMGnum<4)
			{
				if(X[i]==3)
				{
					writeSRAMdata[9*i+1]=1;
				}
				else if(X[i]==2)
				{
					writeSRAMdata[9*i+3]=1;
				}
				else if(X[i]==1)
				{
					writeSRAMdata[9*i+5]=1;
				}
				else
				{
					writeSRAMdata[9*i+7]=1;
				}
			}
			else
			{
				if(X[i]==3)
				{
					writeSRAMdata[9*i+2]=1;
				}
				else if(X[i]==2)
				{
					writeSRAMdata[9*i+4]=1;
				}
				else if(X[i]==1)
				{
					writeSRAMdata[9*i+6]=1;
				}
				else
				{
					writeSRAMdata[9*i+8]=1;
				}
			}

		}
	}
	// Selectively write into the necessary Image Data-Lines for given IMGnum COMMON for 1b and 2b MODE
	//Image Selection is a combination of bitline and IMG_Select_DL_nums;
	//Example-1 IMG_num=6=> bitline=6-4=2; IMG_Select_DL_nums=2,4,6,8 [Since (IMGnum<4)==0]
	//Example-2 IMG_num=3=> bitline=3; IMG_Select_DL_nums=1,3,5,7[Since (IMGnum<4)==1]
	int DL_nums_len=4;
	u8 IMG_Select_DL_nums[4];
	if(IMGnum<4)
	{
		bitLine=IMGnum;
		IMG_Select_DL_nums[0]=1;
		IMG_Select_DL_nums[1]=3;
		IMG_Select_DL_nums[2]=5;
		IMG_Select_DL_nums[3]=7;
	}
	else
	{
		bitLine=IMGnum-4;
		IMG_Select_DL_nums[0]=2;
		IMG_Select_DL_nums[1]=4;
		IMG_Select_DL_nums[2]=6;
		IMG_Select_DL_nums[3]=8;
	}
	currObj=writeSRAM_1024_Images_FAST_NEW(currObj,bitLine,writeSRAMdata,IMG_Select_DL_nums,DL_nums_len);

	return currObj;
}
