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
#include "Dsp402ObjDict.h"
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
		{COMM_PROFILE_1_FIRST, 	COMM_PROFILE_1_LAST, 		(td_indexTable *)CommunicationProfileArea},		
		{0x1021,				0x1021,						(td_indexTable *)CommunicationProfileArea1},
		{0x1800,				TRANSMIT_PDO_LAST,			(td_indexTable *)transmitPDOParameter},
		{0x1400,				RECEIVE_PDO_LAST,			(td_indexTable *)receivePDOParameter},
		{0x1600,				RECEIVE_PDO_MAPPING_LAST,	(td_indexTable *)RxPDOMappingTable},
		{0x1A00,				TRANSMIT_PDO_MAPPING_LAST,	(td_indexTable *)TxPDOMappingTable},
		{0x6040,				DSP_40_MAPPING_LAST,		(td_indexTable *)DSP402MappingTable4},
		{0x6060,				DSP_60_MAPPING_LAST,		(td_indexTable *)DSP402MappingTable6},
		{DSP_70_MAPPING_FIRST,	DSP_70_MAPPING_LAST,		(td_indexTable *)DSP402MappingTable7},
		{DSP_80_MAPPING_FIRST,	DSP_80_MAPPING_LAST,		(td_indexTable *)DSP402MappingTable8},
		{DSP_C0_MAPPING_FIRST,	DSP_C0_MAPPING_LAST,		(td_indexTable *)DSP402MappingTableC},
		{DSP_F0_MAPPING_FIRST,	DSP_F0_MAPPING_LAST,		(td_indexTable *)DSP402MappingTableF},
		{DSP_686_MAPPING_FIRST,	DSP_686_MAPPING_FIRST,		(td_indexTable *)DSP402MappingTable686},
		{0,						0,							NULL}
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

