/*
 * BMAlarms.c
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#include "cpu.h"

#include "Bits.h"
#include "Slave.h"
#include "Drive.h"
#include "Nmt.h"

#include "BM/BMCycle.h"
#include "BM/Probe.h"
#include "BM/BMAlarms.h"


MacAlarms Alarms;

char MAC_HOMED;


void InitStatoMacchina()
{

	*((dword *)&Alarms) = 0;
	
}



void LeggiStatoMacchina()
{  
	char x = !Alarms.HomedAxeX;
	char p = !Alarms.HomedProtection;

	MAC_HOMED = x && p;

  
	macStatus.PwrOffAxeX = CHECK_StatusWordBit(0, StatusWord_OperEnb);     
	macStatus.PwrOffProtection = CHECK_StatusWordBit(1, StatusWord_OperEnb);
	
	macStatus.InPosAxeX = drvAxeRegState[0].in_pos;
	macStatus.InPosProtection = drvAxeRegState[1].in_pos;
	
	Alarms.HomedAxeX = (drvAxeRegState[0].homed == 0);
	
	Alarms.AxeXLimSwPos = (drvAxeRegState[0].limSwMax == 1);
	Alarms.AxeXLimSwNeg = (drvAxeRegState[0].limSwMin == 1);
	
	if (macOptions.Protection)
	{
		Alarms.HomedProtection = (drvAxeRegState[1].homed == 0);
		Alarms.ProtLimSwPos = (drvAxeRegState[1].limSwMax == 1);
		Alarms.ProtLimSwNeg = (drvAxeRegState[1].limSwMin == 1);
	}
	else
	{
		Alarms.HomedProtection = 0;
		Alarms.ProtLimSwPos = 0;
		Alarms.ProtLimSwNeg = 0;
	}
	
	if (macOptions.Probe)
	{		
		Alarms.HomedWidth = (probeStatus == 0);
	}
	else
	{
		Alarms.HomedWidth = 0;
	}
	
	Alarms.AlarmAxeX = drvAxeRegState[0].alarm;
	Alarms.AlarmProtection = drvAxeRegState[1].alarm;
	
	Alarms.AlarmCanBoard1 = nmtAllarmi[0];
	Alarms.AlarmCanBoard2 = nmtAllarmi[1];
	
	

}
