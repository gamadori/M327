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


void CycleServer()
{
	short axe;
	
	if (!inpPwrFiltered)
	{
		BusStopAll();
		BusEmergency();
	}
	
}
	

