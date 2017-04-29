/*
 * Cycle.c
 *
 *  Created on: Apr 28, 2017
 *      Author: info
 */
#include "cpu.h"
#include "Settings.h"
#include "plc.h"
#include "Homing.h"
#include "Bits.h"
#include "Cycle.h"
#include "Slave.h"
#include "Bus.h"
#include "Timer.h"

#define inpPwrOn		((inputBuffer & BIT15) == BIT15)

short cycleStatus;

void CycleServer()
{
	short axe;
	
	switch (cycleStatus)
	{
	case 0:
	
		for (axe = 0; axe < NUM_AXES; ++axe)
		{
			hmStep[axe] = 0;
		}
		
		BusStopAll();
		
		BusEmergency();
		
		
		if (inpPwrOn)
		{
			cycleStatus++;
			TmrPwr = MSEC(100);
		}
		break;

		
	case 1:
		if (!inpPwrOn)
			cycleStatus = 0;
		else if (!TmrPwr)
			cycleStatus++;
		break;
		
	case 2:
		if (!inpPwrOn)
		{
			cycleStatus++;
			TmrPwr = MSEC(100);
		}
		break;

	case 3:
		if (inpPwrOn)
			cycleStatus = 2;
		else if (!TmrPwr)
			cycleStatus = 0;
		break;
	}
}
	

