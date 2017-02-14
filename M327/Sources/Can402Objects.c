/*
 * Can402Object.c
 *
 *  Created on: Dec 11, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "Settings.h"
#include "CanDic.h"
#include "CanObjects.h"
#include "Can402Objects.h"
#include "Drive.h"
#include "Slave.h"
#include "Encoder.h"
#include "Ramp.h"

td_subIndex Index6040[] = 
{
	{dicRW, tUNSIGNED16, SIZEOFW_WORD, (void *)bus_AxisControlWord}
};

td_subIndex Index6041[] = 
{
	{dicRO, tUNSIGNED16, SIZEOFW_WORD, (void *)bus_AxisStatusWord}
};


td_subIndex Index6060[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (void *)bus_AxisOperation}
};

td_subIndex Index6061[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (void *)bus_AxisModeOperDisplay}
};

td_subIndex Index6063[] = 
{
	{dicRO, tINTEGER32, SIZEOFW_DWORD, (void *)(rpos + NUM_AXES)}
};


td_subIndex Index6064[] = 
{
	{dicRO, tINTEGER32, SIZEOFW_DWORD, (void *)(vpos + NUM_AXES)}
};

td_subIndex Index6065[] = 
{
	{dicRO, tINTEGER16, SIZEOFW_WORD, (void *)(drvAxeRegState + NUM_AXES)}
};

td_subIndex Index607A[] = 
{

	{dicRW, tINTEGER32, SIZEOFW_DWORD, (void *)bus_AxisPos}
};

td_subIndex Index6081[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(void *)bus_AxisVel}
};

td_subIndex Index6083[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(void *)bus_AxisAcc}
};


td_subIndex Index6086[] = 
{
	{dicRW,	tINTEGER16,		SIZEOFW_WORD,	(void *)bus_AxisProfile}
};



td_subIndex Index60FD[] = 
{
	{dicRO, tINTEGER16, SIZEOFW_WORD, (void *)(bus_IO_InputVal)}
};

td_subIndex Index60FE[] = 
{
	{dicRO, tINTEGER16, SIZEOFW_WORD, (void *)(bus_IO_Output)}
};





const td_indexTable DSP402MappingTable4[] =
{
	DeclareIndexTableEntry(Index6040, VAR),
	DeclareIndexTableEntry(Index6041, VAR),
	
	
};


const td_indexTable DSP402MappingTable6[] =
{

	DeclareIndexTableEntry(Index6060, VAR),
	DeclareIndexTableEntry(Index6061, VAR),
	{NULL, 0, VAR},							// 6062
	DeclareIndexTableEntry(Index6063, VAR),	// 6063
	DeclareIndexTableEntry(Index6064, VAR),	// 6064
	DeclareIndexTableEntry(Index6065, VAR)	// 6065

};

const td_indexTable DSP402MappingTable7[] = 
{
	DeclareIndexTableEntry(Index607A, VAR),
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index607B, RECORD),
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index607C, VAR),						
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index607D, RECORD),
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index607E, VAR),
	{NULL, 0, VAR},							// 607F	
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index6080, VAR),
	
	DeclareIndexTableEntry(Index6081, VAR),
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index6082, VAR),
	DeclareIndexTableEntry(Index6083, VAR),
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index6084, VAR),
	{NULL, 0, VAR},//DeclareIndexTableEntry(Index6085, VAR),
	DeclareIndexTableEntry(Index6086, VAR),
	
	
};

const td_indexTable DSP402MappingTableF[] =
{	
	DeclareIndexTableEntry(Index60FD, VAR),	// 60FD
	DeclareIndexTableEntry(Index60FE, VAR)	// 60FD
};

