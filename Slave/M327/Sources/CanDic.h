/*
 * CanDic.h
 *
 *  Created on: Jan 3, 2016
 *      Author: gianluca
 */

#ifndef CANDIC_H_
#define CANDIC_H_
#include "cpu.h"

/* MODULE Dictionary */

#define NO_SUCH_INDEX 0x11

#define COMM_PROFILE_1_FIRST 	0x1000
#define COMM_PROFILE_1_LAST 	0x1018


enum ObjectCode
{
	DicNull = 0,
	DicDomain = 2,
	DicDefType = 5,
	DicDefStruct = 6,
	DicVar = 7,
	DicArray = 8,
	DicRecord = 9
};

enum DicType
{
	tBOOLEAN 			= 0x001,
	tINTEGER8			= 0x002,
	tINTEGER16			= 0x003,
	tINTEGER32 			= 0x004,
	tUNSIGNED8			= 0x005,
	tUNSIGNED16			= 0x006,
	tUNSIGNED32 		= 0x007,
	tREAL32				= 0x008,
	tVISIBLE_STRING 	= 0x009,
	tOCTET_STRING		= 0x00A,
	tUNICODE_STRING		= 0x00B,
 	tTIME_OF_DAY		= 0x00C,
 	tTIME_DIFFERENCE	= 0x00D,
	tDOMAIN				= 0x00F,
	tINTEGER24			= 0x010,
	tREAL64 			= 0x011,
	tINTEGER40			= 0x012,
	tINTEGER48			= 0x013,
	tINTEGER56			= 0x014,
	tINTEGER64			= 0x015,
	tUNSIGNED24			= 0x016,
	tUNSIGNED40			= 0x018,
	tUNSIGNED48			= 0x019,
	tUNSIGNED56			= 0x01A,
	tUNSIGNED64			= 0x01B
};

enum DicAttribute
{
	
	dicRW,
	dicWO,
	dicRO,
	dicConst
};

/** Each entry of the object dictionary can be a different object. this objects
 * are defined by the CANopen standard. see standard for more information.
 * At the moment the object dictionary does not really care about this
 * different types. this is topic to change!
*/
enum objectDefinition { DOMAIN=2, DEFTYPE=5, DEFSTRUCT=6, VAR=7, ARRAY=8, RECORD=9 };

typedef struct 
{
	word index;
	enum ObjectCode object;
	char *name;
	word type;
	byte attribute;
	bool optional;	
}tdDictionary;

 
typedef struct td_s_subIndex
{
	byte attribute;
	byte type;
	dword size;
	byte *pObject;
	
}td_subIndex;


typedef struct 
{	
	td_subIndex *subIndex;
	byte subCount;
	enum objectDefinition object;

		
} td_indexTable;





typedef struct td_DeviceType
{
	word deviceProfile;
	word dev_type_mode;	
}DeviceType;



byte getODEntry(word index, byte subIndex, byte** pData, dword* pdwSize, dword *numByte);

byte setODEntry(word index, byte subIndex, byte *pbData, dword dwSize);



#endif /* CANDIC_H_ */
