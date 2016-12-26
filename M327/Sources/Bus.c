/*
 * Bus.c
 *
 *  Created on: Dec 25, 2016
 *      Author: info
 */

#include "cpu.h"
#include "Utility.h"
#include "Settings.h"
#include "Ramp.h"
#include "Slave.h"
#include "Tuning.h"
#include "Homing.h"
#include "Bus.h"

enum MacCmds
{
	MacCmdNull,
	MacCmdStop,
	MacCmdHome,
	MacCmdRun
};

enum MacStatus
{
	MacEmergency,
	MacPowerOn,
	MacHoming,
	MacRun,
	MacAlarm
};

short busCmd[MAC_NUM_AXIS];
short busState[MAC_NUM_AXIS];

short busDec[MAC_NUM_AXIS];	// Decellerazione in fase di stop

void BusInit()
{
	short i;
	for (i = 0; i < MAC_NUM_AXIS; ++i)
		busCmd[i] = MacCmdNull;
	
}
void BusServer()
{
	short i;
	
	for (i = 0; i < MAC_NUM_AXIS; ++i)
	{
		switch (busCmd[i])
		{
		case MacCmdStop:
			BusStop(i, busDec[i]);
			busCmd[i] = MacCmdNull;
			break;
			
		case MacCmdHome:
			HomeServer(i);
			break;
		}
	}
}


bool BusJog(byte axe, long speed, short acc)
{
	if (axe < NUM_AXES)
	{							
		RampJogCommand((byte)axe, speed, acc);
			
		return TRUE;
	}
	else if (axe < MAC_NUM_AXIS)
	{
		SlvJogAsse(axe - NUM_AXES, speed, acc);
		return TRUE;
	}
	else
		return FALSE;	
}

bool BusMove(byte axe, long pos, long speed, short acc, bool tuning)
{
	if (axe < NUM_AXES)
	{							
		RampMoveCommand((byte)axe, pos, speed, acc, tuning);
			
		return TRUE;
	}
	else if (axe < MAC_NUM_AXIS)
	{
		 SlvPosAsse(axe - NUM_AXES, speed, pos, acc, 0);
	}
	else
		return FALSE;
}

bool BusStop(byte axe, short acc)
{
	if (axe < NUM_AXES)
	{							
		RampStopCommand((byte)axe, acc);
			
		return TRUE;
	}
	else if (axe < MAC_NUM_AXIS)
	{
		SlvStopAsse(axe, acc);
		return TRUE;
	}
	else
		return FALSE;
}

void BusStopAll()
{
	byte axe;
	
	for (axe = 0; axe < MAC_NUM_AXIS; ++axe)
	{
		BusStop(axe, busDec[axe]);
	}
}

bool BusHome(byte axe)
{
	if (axe < NUM_AXES)
	{
		HomeStart(axe);
		busCmd[axe] = MacCmdHome;
		return TRUE;
	}
	else if (axe < MAC_NUM_AXIS)
	{
		return SlvHomeAsse(axe);
	}
	else
		return FALSE;
	
}
/*
 * Comando di Jog dalla seriale
 */
bool BusRSJog(char *param, short index)
{
	long speed;
	short acc;
	
	if (index == -1)
		Alfasscanfs((char **)&param, &index);
	
	Alfasscanfl((char **)&param, &speed);
	Alfasscanfs((char **)&param, &acc);
				
	return BusJog(index, speed, acc);
}

bool BusRSMove(char *param, short index, bool tuning)
{
	long pos;
	long speed;
	short acc;
	
	if (index == -1)
		Alfasscanfs((char **)&param, &index);
		
	Alfasscanfl((char **)&param, &pos);
	Alfasscanfl((char **)&param, &speed);
	Alfasscanfs((char **)&param, &acc);
	
	if (tuning)
	{
		Alfasscanfs((char **)&param, &tngVar);
		Alfasscanfs((char **)&param, &tngFrq);
		Alfasscanfs((char **)&param, &tngTime);
	}
	return BusMove(index, pos, speed, acc, tuning);
	
	
}

bool BusRSHome(char *param, short index)
{
	if (index == -1)
			Alfasscanfs((char **)&param, &index);
		
	return BusHome(index);
}
/*
 * Comando di Stop dalla seriale
 */
bool BusRSStop(char *param, short index)
{
	
	short acc;
	
	if (index == -1)
		Alfasscanfs((char **)&param, &index);
		
	Alfasscanfs((char **)&param, &acc);
				
	return BusStop(index, acc);
}
