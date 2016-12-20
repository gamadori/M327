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


byte stateMachine = COMMUNICATION_INITIALIZATION;

short debugMachine = 0;

word canIDBoards[NUM_CAN_BOARDS] = {1};

void PreOperational();

void Operational();

void CANOpenSendState(byte state);

void CANOPENServer()
{
	tNmtUltimoAllarme stato;
		
	switch (stateMachine)
	{
		case COMMUNICATION_INITIALIZATION:
			stateMachine = NODE_INITIALIZATION;
			break;
			
		case NODE_INITIALIZATION:				
			TMR_CAN_CYCLE = SEC(5);		// Time Out waiting Slave bootup
			stateMachine = PRE_OPERATIONAL;
			NMTSendState(RESET_REMOTE_NODE);
			SDOServer();
			break;
	
		case PRE_OPERATIONAL:
			
			if (NMTSlaveRdy(&stato) || debugMachine)
			{
				CANOpenSendState(START_REMOTE_NODE);
				
				stateMachine = OPERATIONAL;
			}
			else if (!TMR_CAN_CYCLE)
			{
				// TimeOut Waiting boot up
				// execute a reset command
				
				NMTSalvaUltimoAllarme(stato.scheda, stato.allarme);
				
				NMTInitBootUp();
				CANOpenSendState(RESET_REMOTE_NODE);
				TMR_CAN_CYCLE = SEC(5);		// Waiting Slave in operational state
				
				stateMachine = NODE_INITIALIZATION;
			}
			PreOperational();
			NMTServerReceive();
			//	EmgcyServer();
			//GuardTimeService();
			
			break;
			
		case OPERATIONAL:
			
			NMTServerReceive();
			
			if (!NMTSlaveOperational(&stato) && !debugMachine)
			{
				// a slave was rest
				if (!TMR_CAN_CYCLE)
				{		
					NMTSalvaUltimoAllarme(stato.scheda, stato.allarme);
					NMTInitBootUp();
					
					//if (!pwr_ok)
					{				
						CANOpenSendState(RESET_REMOTE_NODE);
						stateMachine = NODE_INITIALIZATION;
					}
				}
				//PreOperational();
			}
			
			Operational();
			
			break;	
		case STOPPED:
			NMTServerReceive();
			break;
			
	}
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

void CANOpenSendState(byte state)
{
	CanMessage m;
	m.cob_id = 0;
	m.rtr = 0x00;
	m.len = 1;
	m.data[0] = state;
	m.data[1] = 0x00;
	m.data[2] = 0x00;
	m.data[3] = 0x00;
	m.data[4] = 0x00;
	m.data[5] = 0x00;
	m.data[6] = 0x00;
	m.data[7] = 0x00;
	
	CanTransmitPush(CanNMTSendState, &m);
}

