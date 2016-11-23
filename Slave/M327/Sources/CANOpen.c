/*
 * CANOpen.c
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Timer.h"
#include "CanSettings.h"
#include "Can.h"
#include "CanOpen.h"
#include "NMT.h"
#include "SDO.h"
#include "PDO.h"
#include "Drive.h"

byte stateMachine = INITIALISATION_COMMUNICATION;

byte canDeviceNodeId;

bool canFirstTime = FALSE;

void Initialisation();

void PreOperational();

void Operational();

void Stopped();

void SendBootUp();

void CANOpenSendState(byte state);

//----------------------------------------------------------------------------
// SetupCanbus(void)
//
//----------------------------------------------------------------------------

void CANOPENServer()
{
	NMTServer();
	
	switch( stateMachine )
	{
		case INITIALISATION_COMMUNICATION:			
			// change automatically into initialisation of the node
			stateMachine = INITIALISATION_NODE;
			break;
			
		case INITIALISATION_NODE:
		// because sending of heartbeat must start just after bootup, lets do that...
		// the first message has to be the boot-up message (with first data-byte
		// set to 0, therefore we have to send this heartbeat NOW.
			Initialisation();
			
			// create the list with all heartbeat-entries...
			
			//GuardTimeInit();
			SendBootUp();
			
			//FCMB13_Control |= 0x0040;		// MB13 receive
			
			stateMachine = PRE_OPERATIONAL;
			GuardTimeInit();

			GuardTimeService();
			
			SDOServer();
			break;
			
		case PRE_OPERATIONAL:
			
			canFirstTime = TRUE;
			PreOperational();
		//	EmgcyServer();
			GuardTimeService();
			
			break;
			
		case OPERATIONAL:
			Operational();
			GuardTimeService();
			//EmgcyServer();
	
	
			break;
			
		case STOPPED:
			Stopped();
			break;
	}
	
	if (stateMachine != OPERATIONAL)
		DriveCommandEnable(0, 0);
	//NMTErrorServer();
	
	
}

/*
 * Inizializzazione del CANOPEN
 */
void SetupCanBus()
{
}


void Initialisation()
{
	SDOServer();
	PDOInit(canDeviceNodeId);		
}



void PreOperational()
{	
	SDOServer();
}

void Operational()
{
	
	PDOReceiveServer();
	
	SDOServer();
	
	PDOTransmitServer();
	
	
}

void Stopped()
{
}

/*
 * Invio BootUp
 * Tramite questo servizio, la NMT slave indica che è avvenuta la transizione di stato da 
 * INITIALISING a PRE-OPERATIONAL
 */
void SendBootUp()
{
	CanMessage m;
	
	m.cob_id = canDeviceNodeId;
	m.rtr = 0x00;
	m.len = 1;
	m.data[0] = 0x00;
	m.data[1] = 0x00;
	m.data[2] = 0x00;
	m.data[3] = 0x00;
	m.data[4] = 0x00;
	m.data[5] = 0x00;
	m.data[6] = 0x00;
	m.data[7] = 0x00;
	
	CanTransmitPush(CanNMTErrorChannel, &m);	
	
}

void CanOpenSetID(byte id)
{
	canDeviceNodeId = id; 
}
