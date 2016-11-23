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

static const bool drvEnabled[] = {TRUE, FALSE};


bool drvCmdEnable[NUM_AXES];				// Richiesta di attivazione dell'asse

byte drvAxeStatus[NUM_AXES];				// Axe Status

byte drvOldAxeStatus[NUM_AXES];				// Previous axe state

t_pid_axe_state drvAxeRegState[NUM_AXES];	// Axe Status Register

void DrvEventAxeInEnable(byte axe);

void DrvEventAxeEnabledEachStep(byte axe);

void DrvEventAxeDisableEachStep(byte axe);

void DrvEventAxeInDisable(byte axe);

void DrvEventAxeInAlarmEachStep(byte axe);

/*
 * Drive Initialisation
 */
void DriveInit()
{
	int i;
	for (i = 0; i < NUM_AXES; ++i)
	{
		drvCmdEnable[i] = FALSE;
		drvAxeStatus[i] = cDisable;
	}
}

/*
 * Drive Server
 */
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
		
	}
	TngServer();
}

/*
 * Drive in enable Event
 */
void DrvEventAxeInEnable(byte axe)
{
	drvAxeRegState[axe].alarm = AxeOk;	// Reset alarms
	PidReset(axe);
	RampReset(axe);
}

/*
 * Drive in disable Event
 */
void DrvEventAxeInDisable(byte axe)
{
	PidReset(axe);
	RampReset(axe);
}

/*
 * Event Called at each cycle of drive enabled
 */
#pragma interrupt called
void DrvEventAxeEnabledEachStep(byte axe)
{
	RampSrvError(axe);
	RampServer(axe);
	PidServer(axe);	
	
	if (!drvCmdEnable[axe])
	{
		DrvEventAxeInDisable(axe);
		drvAxeStatus[axe] = cDisable;
	}
}

/*
 * 
 * Event Called at each step of drive disabled
 */
#pragma interrupt called
void DrvEventAxeDisableEachStep(byte axe)
{
	PwmUpdateOutput(axe, 0);
	
	if (drvCmdEnable[axe])
	{
		DrvEventAxeInEnable(axe);
		drvAxeStatus[axe] = cEnabled;
	}
	else
	{
		RampReset(axe);
	}
	
}

/*
 * Event Called at each cycle of drive in alarm
 */
#pragma interrupt called
void DrvEventAxeInAlarmEachStep(byte axe)
{
	PwmUpdateOutput(axe, 0);
	
	if (!drvCmdEnable[axe])
	{
		drvAxeStatus[axe] = cDisable;
	}
}

/*
 * Drive Enable/Disable Control
 */
void DriveCommandEnable(byte axe, bool state)
{
	drvCmdEnable[axe] = state;
}
