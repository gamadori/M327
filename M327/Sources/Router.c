/*
 * Router.c
 *
 *  Created on: Jan 13, 2017
 *      Author: info
 */


/*
 * Router.c
 *
 *  Created on: Dec 14, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Utility.h"
#include "Sdo.h"
#include "Router.h"


#define IDX_BLASE	-1
typedef struct
{
	short cmd;
	word cmdOffeset;
	word cmdIndex;
	word sdo;
	word sdoIndex;
	word cmdAlfacol;
		
}MapRouter;

MapRouter sdoTableRouterFunc[] = 
{
//		cmd			Off		cmdIndex,		Sdo,		Sdo Index,		Frame Alfacol				
		
		{0x0007,	2,		IDX_BLASE,		0x1021,		2,				0},
		{0,			0,		0,				0,			0,				0},
};

MapRouter sdoTableRouter[] =
{
//		cmd			Off		cmdIndex,		Sdo,		Sdo Index,		Frame Alfacol					
		{0x0001,	0x0,	IDX_BLASE,		0x6063,		0,				0},			// Real Position
		{0x0002,	0x0,	IDX_BLASE,		0x6064,		0,				0},			// Virtual Position
		
		{0x0021,	0x0,	IDX_BLASE,		0x1021,		1,				0},			// Scrittura sulla slave
		{0x0030,	0x0,	IDX_BLASE,		0x6065,		0,				0},			// Lettura Allarme Assi
		{0x0040,	0x4,	IDX_BLASE,		0x60F9,		1,				0},			// Proportional Gain/Integral Gain/Derived Gain/Upper Limit/Lower Limit
		{0x0050,	0x2,	IDX_BLASE,		0x60F9,		6,				0},			// Static Servo Error, Dynamic Servo Error, Time Out Servo Error
		{0x0055,	0x0,	IDX_BLASE,		0x6087,		0,				0},
		{0x0060,	0xA,	IDX_BLASE,		0x60F9,		9,				0},			// Nominal Current, Max Current, Time Out Current, Time Out Fault
		{0x0070,	0x5,	IDX_BLASE,		0x607C,		1,				0},			// Homing Offsets
		{0x0076,	0x1,	IDX_BLASE,		0x6099,		1,				0},			// Homing Speeds
		{0x0078,	0x0,	IDX_BLASE,		0x609A,		0,				0},			// Homing Acceleration
		{0x0079,	0x1,	IDX_BLASE,		0x6098,		1,				0},			// Homing Dir and Method
		
		{0,			0,		0,				0,			0,				0}
};

bool RouteCheckFrame(MapRouter *map, word frame, word index);

byte RouteGetCmds(MapRouter *map, word frame, word index, word *pFrame, word *pIdx);



/*
 * 
 */
byte RouterFunction(byte numStation, char *buff, word* frame)
{
	word index;
	long value;
	word idx;
	long size;
	
	Alfasscanfs(&buff, (short*)&index);
	
	switch (RouteGetCmds(sdoTableRouterFunc, *frame, index, &currSDOFrame, &idx))
	{
	case cmdSDO:
		size = Alfasscanfl(&buff, &value);
		SDOCommandDownload(numStation, (byte *)&value,  currSDOFrame, idx, 4);
		return cmdSDO;
		
	case cmdAlfacol:
		*frame = currSDOFrame;
		return cmdAlfacol;
	}
	
	return cmdError;
}

byte RouterVariable(byte numStation, char *buff, word* frame, byte cmdType)
{
	word index;
	long value;
	word subIndex;
	long size;
		
	Alfasscanfs(&buff, (short*)&index);
	
	switch (RouteGetCmds(sdoTableRouter, *frame, index, &currSDOFrame, &subIndex))
	{
	case cmdSDO:							
		if (cmdType == opGet)
		{
			SDOCommandUpload(numStation, currSDOFrame, subIndex);
			
		}
		else if (cmdType == opSet)
		{				
			size = Alfasscanfl(&buff, &value);
			SDOCommandDownload(numStation, (byte *)&value, currSDOFrame, subIndex, 4);									
		}
		return cmdSDO;
					
	case cmdAlfacol:
		*frame = currSDOFrame;	
		return cmdAlfacol;
	}	
	return cmdError;
}
/*
byte RouterVariable2(word frame, word index, word *pFrame, word *pIdx)
{
	return RouteGetCmds(sdoTableRouter, frame, index, pFrame, pIdx);
}

byte RouterFunction2(word frame, word index, word *pFrame, word *pIdx)
{
	return RouteGetCmds(sdoTableRouterFunc, frame, index, pFrame, pIdx);
}
*/
byte RouteGetCmds(MapRouter *map, word frame, word index, word *pFrame, word *pIdx)
{
	short i;
	word offset;
	for (i = 0; map[i].cmd; ++i )
	{
		if (RouteCheckFrame(map + i, frame, index))
		{
			if (map[i].sdo)
			{
				offset = frame - map[i].cmd;
				*pFrame = map[i].sdo;
				*pIdx = map[i].sdoIndex + offset;
				return cmdSDO;
			}
			else
			{
				*pFrame = map[i].cmdAlfacol;
				*pIdx = index;
				
				return cmdAlfacol;
			}
		}
	}
	return cmdError;
}


bool RouteCheckFrame(MapRouter *map, word frame, word index)
{
	return (frame >= map->cmd && frame <= map->cmd + map->cmdOffeset && 
			(map->cmdIndex == index || map->cmdIndex == IDX_BLASE));
}

