/*
 * CanDic.c
 *
 *  Created on: Jan 3, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "CanDic.h"
#include "CanObjects.h"
#include "CanOpen.h"
#include "Utility.h"

typedef struct
{
	word addAreaFirst;
	word addAreaLast;
	
	td_indexTable *table;
	
} td_CommProfileArea;

word SizeDicType[] =
{
	0x00,
	0x01,	//tBOOLEAN
	0x01,	//tINTEGER8
	0x02,	//tINTEGER16
	0x04,	//tINTEGER32
	0x01,	//tUNSIGNED8
	0x02,	//tUNSIGNED16
	0x04,	//tUNSIGNED32
	0x04,	//tREAL32 		
	0x00,	//tVISIBLE_STRING				
	0x00,	//tOCTET_STRING		= 0x00A,
	0x00,	//tUNICODE_STRING		= 0x00B,
 	0x06,	//tTIME_OF_DAY		= 0x00C,
 	0x06,	//tTIME_DIFFERENCE	= 0x00D,
	0x00,	//tDOMAIN				= 0x00F,
	0x03,	//tINTEGER24			= 0x010,
	0x08,	//tREAL64 			= 0x011,
	0x05,	//tINTEGER40			= 0x012,
	0x06,	//tINTEGER48			= 0x013,
	0x07,	//tINTEGER56			= 0x014,
	0x08,	//tINTEGER64			= 0x015,
	0x03,	//tUNSIGNED24			= 0x016,
	0x05,	//tUNSIGNED40			= 0x018,
	0x06,	//tUNSIGNED48			= 0x019,
	0x07,	//tUNSIGNED56			= 0x01A,
	0x08	//tUNSIGNED64			= 0x01B
};

td_CommProfileArea DicAreaMapping[] =
{
		{0x1000,	0x1018,	(td_indexTable *)CommunicationProfileArea},
		{0x1400,	0x1406, (td_indexTable *)receivePDOParameter},
		{0x1600,	0x1603,	(td_indexTable *)RxPDOMappingTable},
		{0x1800, 	0x1803, (td_indexTable *)transmitPDOParameter},	
		{0x1A00,	0x1A03,	(td_indexTable *)TxPDOMappingTable},	
		{0x4000,	0x4001,	(td_indexTable *)IO_MappingTable},	
		{0,			0,		NULL}
};

 
/*======================================================================
 *
 * byte getODEntry(word index, byte subIndex, void* pData, word* pdwSize)
 *
 *	Ricerca nel dizionario la parola ricercata
 *
 *=======================================================================*/
byte getODEntry(word index, byte subIndex, byte** pData, dword* pdwSize, dword *numByte)
{
	word offset;

	short i;
	bool trovato;
	
	for (trovato = FALSE, i = 0; !trovato && DicAreaMapping[i].addAreaFirst > 0; ++i)
	{
		if (index >= DicAreaMapping[i].addAreaFirst && 
						index <= DicAreaMapping[i].addAreaLast)
		{
			offset = (index - DicAreaMapping[i].addAreaFirst);
			
			if (DicAreaMapping[i].table[offset].subCount < subIndex)
			{
				return NO_SUCH_INDEX;
			}
			
			*pdwSize = DicAreaMapping[i].table[offset].subIndex[subIndex].size;
			*pData = (byte *)DicAreaMapping[i].table[offset].subIndex[subIndex].pObject;			
			*numByte = SizeDicType[DicAreaMapping[i].table[offset].subIndex[subIndex].type];
			return SUCCESSFUL;
			
		}
	}
	
	return NO_SUCH_INDEX;
}


byte setODEntry(word index, byte subIndex, byte *pbData, dword dwSize)
{
	word offset;
	short i;
	bool trovato;
	word size;
	
	
	for (trovato = FALSE, i = 0; !trovato && DicAreaMapping[i].addAreaFirst > 0; ++i)
	{
		if (index >= DicAreaMapping[i].addAreaFirst && 
						index <= DicAreaMapping[i].addAreaLast)
		{
			offset = (index - DicAreaMapping[i].addAreaFirst);
			
			if (DicAreaMapping[i].table[offset].subCount < subIndex)
				return NO_SUCH_INDEX;
			
			
			if (dwSize > (DicAreaMapping[i].table[offset].subIndex[subIndex].size << 1))
				dwSize = (DicAreaMapping[i].table[offset].subIndex[subIndex].size << 1);;
			
			Move(pbData, DicAreaMapping[i].table[offset].subIndex[subIndex].pObject, dwSize);
			//CopyDataFromCAN(pbData, (word *)DicAreaMapping[i].table[offset].subIndex[subIndex].pObject, dwSize);
			return SUCCESSFUL;
		}
	}
		
	return NO_SUCH_INDEX;
	
}

