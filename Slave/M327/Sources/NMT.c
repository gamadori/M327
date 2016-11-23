/*
 * NMT.c
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "Can.h"
#include "CanFifo.h"
#include "CanSettings.h"
#include "CanOpen.h"
#include "Timer.h"
#include "NMT.h"

bool nmtGuardingProtocol = FALSE;

word nmtGuardTime = MSEC(2500);

byte nmtToggleBit = 0;

dword nmtOldTimerSys = 0;

byte nmtLifeTimeFactor;

void GuardTimeStop();

void GuardTimeSendResponse();

void GuardTimeStart();

void NMTServer()
{
	CanMessage m;
	
	if (CanReceivedPop(CanNMTChannel, &m))
	{
		if (m.cob_id == 0 || m.cob_id == canDeviceNodeId)
		{
			switch(m.data[0])
			{
				case (byte)START_REMOTE_NODE:
					stateMachine = OPERATIONAL;
					break;
					
				case (byte)STOP_REMOTE_NODE:
					stateMachine = STOPPED;
					break;
					
				case (byte)ENTER_PRE_OPERATIONAL_STATE:
					stateMachine = PRE_OPERATIONAL;
					break;
					
				case (byte)RESET_REMOTE_NODE:
				
					GuardTimeStop();
					stateMachine = INITIALISATION_NODE;
					break;
					
				case (byte)RESET_REMOTE_COMMUNICATION:
					stateMachine = INITIALISATION_COMMUNICATION;
					break;
					
				default:
					stateMachine = STOPPED;
					break;
					
			}
		}
		else
		{
			stateMachine = STOPPED;
		}
	}
}

void GuardTimeStop()
{
	nmtGuardingProtocol = FALSE;
}

void GuardTimeInit()
{
	nmtGuardingProtocol = TRUE;
	TMR_CAN_LIFETIME = 0;
}

void GuardTimeSendResponse()
{
	CanMessage m;
			
	m.cob_id = canDeviceNodeId;
	m.rtr = 0x00;
	m.len = 1;
	m.data[0] = (nmtToggleBit << 7) | stateMachine;
	m.data[1] = 0x00;
	m.data[2] = 0x00;
	m.data[3] = 0x00;
	m.data[4] = 0x00;
	m.data[5] = 0x00;
	m.data[6] = 0x00;
	m.data[7] = 0x00;
	
	CanTransmitPush(CanNMTErrorChannel, &m);	
	
	if (nmtToggleBit == 0)
		nmtToggleBit = 1;
	else
		nmtToggleBit = 0;	
}

void GuardTimeStart()
{

	TMR_CAN_LIFETIME = nmtGuardTime; // * nmtLifeTimeFactor;
}



void GuardTimeService()
{
	if (!TMR_CAN_LIFETIME && nmtGuardingProtocol)
	{		
		// Scaduto il timer life time
		GuardTimeSendResponse();
		GuardTimeStart();
	}
}

