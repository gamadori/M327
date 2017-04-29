/*
 * Timer.c
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "CanSettings.h"
#include "Plc.h"
#include "Timer.h"

#define	TIMER_TICK	1

#define NUM_TIMERS	20


dword tmrs[NUM_TIMERS];
dword timerSys = 0;
dword tmrsCan[NUM_CAN_BOARDS];
dword tmrsPlc[NUM_INPUTS];

void TimersInit()
{
	int i;
	
	for (i = 0; i < NUM_TIMERS; ++i)
	{
		tmrs[i] = 0;
	}
	for (i = 0; i < NUM_CAN_BOARDS; ++i)
	{
		tmrsCan[i] = 0;
	}
	
	for (i = 0; i < NUM_INPUTS; ++i)
	{
		tmrsPlc[i] = 0;
	}
}


#pragma interrupt called
void TimerServer()
{
	int i;
	
	for (i = 0; i < NUM_TIMERS; ++i)
	{
		if (tmrs[i] >= TIMER_TICK)
			tmrs[i] -= TIMER_TICK;
		else
			tmrs[i] = 0;
	}
	for (i = 0; i < NUM_CAN_BOARDS; ++i)
	{
		
		
		if (tmrsCan[i] >= TIMER_TICK)
			tmrsCan[i] -= TIMER_TICK;
		else
			tmrsCan[i] = 0;
		
	}
	
	for (i = 0; i < NUM_INPUTS; ++i)
	{
		if (tmrsPlc[i] >= TIMER_TICK)
			tmrsPlc[i] -= TIMER_TICK;
		else
			tmrsPlc[i] = 0;
	}
	
	timerSys++;
}
