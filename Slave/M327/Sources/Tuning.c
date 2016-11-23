/*
 * Tuning.c
 *
 *  Created on: Jul 23, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "AS1.h"

#include "Timer.h"
#include "Ramp.h"
#include "Encoder.h"
#include "Pid.h"
#include "Tuning.h"



enum TmgPhases
{
	tngUnknow,
	tngStop,
	tngWorking,
	tngWaitStop,
	tngCompleted	
};

typedef struct
{
	byte *var;
	byte size;
}tTngVar;

tTngVar tngVarMap[] = 
{
		{(byte *)pidErrPos, 4},	// Error Pos
		{(byte *)rpos, 4},		// Real Pos
		{(byte *)vpos, 4},		// Virtual Pos		
		{(byte *)rvel, 4},		// Virtual Speed
		{(byte *)encVel, 2},	// Real Speed
		{(byte *)f16Result, 2}	// Real Speed	
		
};



short tngStatus = tngStop;

short tngFrq;

short tngTime;		// durata oltre l'arrivo

short tngVar = -1;

bool tngSerialStatus = FALSE;

long tngBytes;

void TngInit()
{
	tngBytes = 0;
	tngSerialStatus = FALSE;
	tngStatus = tngWorking;
	
}

void TngEnd()
{
	if (tngStatus == tngWorking)
	{
		tngStatus = tngWaitStop;
		tmrTuning = MSEC(tngTime);
	}
}
#pragma interrupt called
void TngServer()
{
	
	if (tngStatus == tngWorking || (tngStatus == tngWaitStop && tmrTuning))
	{
		if (!tmrTuningFrq)
		{
			TngPush();
			tmrTuningFrq = MSEC(tngFrq);
			
			if (!RampInMove(0) && tngStatus == tngWorking)
				TngEnd();
		}
			
	}
	else if (tngStatus == tngWaitStop)
		tngStatus = tngCompleted;
	
	
}

#pragma interrupt called
void TngPush()
{
	short i;
	byte *p;
	byte *pdest;
	byte size;	
	word n;
	long value = 0;
	
	if (tngVar > -1)
	{	
		p = tngVarMap[tngVar].var;
		size = tngVarMap[tngVar].size;
		pdest = (byte *)&value;
		
		for (i = 0;  i < size; ++i)
			*pdest++ = *p++;
		EnterCritical(); 
		value = tngBytes;
		ExitCritical();
		AS1_SendBlock((byte *)&value, 4, &n);
		
		if (n != 4)
			n = 0;
		
		tngBytes ++;
	}
}
