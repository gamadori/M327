/*
 * Drive.c
 *
 *  Created on: Feb 17, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "PWMC1.h"
#include "settings.h"
#include "Pid.h"
#include "Ramp.h"
#include "pwm.h"
#include "Bits.h"
#include "Plc.h"
#include "BldcUser.h"
#include "Tuning.h"

#include "Drive.h"
#include "Slave.h"

static const bool drvEnabled[] = {TRUE, FALSE};
static const long drvInpBitEnable[] = {BIT15, BIT1};

#define drvInpEnable(axe)		inpPwrFiltered

byte drvAxeStatus[NUM_AXES];				// Axe Status
byte drvOldAxeStatus[NUM_AXES];				// Previous axe state
t_pid_axe_state drvAxeRegState[MAC_NUM_AXIS];	// Axe Status Register

void DrvEventAxeInEnable(byte axe);

void DrvEventAxeEnabledEachStep(byte axe);

void DrvEventAxeDisableEachStep(byte axe);

void DrvEventAxeInDisable(byte axe);

void DrvEventAxeInAlarmEachStep(byte axe);

void DriveInit()
{
	int i;
	for (i = 0; i < NUM_AXES; ++i)
	{
		drvAxeStatus[i] = cDisable;
	}
}
#pragma interrupt called
void DriveServer()
{
	byte i;
	
	for (i = 0; i < NUM_AXES; ++i)
	{
		if (drvEnabled[i])
		{
			switch (drvAxeStatus[i])
			{
			case cEnabled:
				// The motor is enabled				
				DrvEventAxeEnabledEachStep(i);
				break;
				
			case cDriveAlarm:
				DrvEventAxeInAlarmEachStep(i);
				break;
				
			default:
				// Axe Disabled
				DrvEventAxeDisableEachStep(i);
				
				
			}			
		}
		else
			drvAxeStatus[i] = cNotUsed;
		
		BldcSetStatus(i, drvAxeStatus[i] == cEnabled);
		drvAxeRegState[i].axeStatus = drvAxeStatus[i];
		
	}
	TngServer();
}

/*
 * Event Axe goes in Enable State
 */
void DrvEventAxeInEnable(byte axe)
{
	drvAxeRegState[axe].alarm = AxeOk;	// Reset alarms
	PidReset(axe);
	RampReset(axe);
}

void DrvEventAxeInDisable(byte axe)
{
	PidReset(axe);
	RampReset(axe);
}

/*
 * Event Axe is Enabled each step
 */
#pragma interrupt called
void DrvEventAxeEnabledEachStep(byte axe)
{
	RampSrvError(axe);
	RampSwLimit(axe);
	RampServer(axe);
	PidServer(axe);	
	
	if (!drvInpEnable(axe))
	{
		DrvEventAxeInDisable(axe);
		drvAxeStatus[axe] = cDisable;
	}
}

#pragma interrupt called
void DrvEventAxeDisableEachStep(byte axe)
{
	PwmUpdateOutput(axe, 0);
	
	if (drvInpEnable(axe))
	{
		DrvEventAxeInEnable(axe);
		drvAxeStatus[axe] = cEnabled;
	}
	else
	{
		RampReset(axe);
	}
	
}

#pragma interrupt called
void DrvEventAxeInAlarmEachStep(byte axe)
{
	PwmUpdateOutput(axe, 0);
	
	if (!drvInpEnable(axe))
	{
		drvAxeStatus[axe] = cDisable;
	}
}

