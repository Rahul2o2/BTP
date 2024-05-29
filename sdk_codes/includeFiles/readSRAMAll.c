/*
 * readSRAMAll.c
 */

#include <custom_h.h>
curr_obj readSRAMAll(curr_obj currObj, u8 ***readSRAMTotal)
{
	//readSRAMTotal=>97*1092*4
	int numDLs=97;
	int numWWLs=1092;
	int numBLs=4;
	//u8 readSRAMTotal[numDLs][numWWLs][numBLs];
	u8 readSRAMAll_unroll[numDLs*1092];
	SRAM_Store SRAMStore;
	int lenSRAMdata=numWWLs*numDLs;
	int bitLine,WWL,DL;

	for(bitLine=0;bitLine<numBLs;bitLine++)
	{
		SRAMStore = readSRAM(currObj,bitLine);
		currObj=SRAMStore.currObj;
		memcpy(readSRAMAll_unroll,SRAMStore.SRAMdata,lenSRAMdata);

		for(WWL=0;WWL<numWWLs;WWL++)
		{

			for(DL=0;DL<numDLs;DL++)
			{
				readSRAMTotal[DL][WWL][bitLine]=readSRAMAll_unroll[DL+(WWL*numDLs)];
			}
		}
	}
	return currObj;
}
