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
#include "Timer.h"
#include "NMT.h"

word nmtGuardTime = SEC(5);

short statusSlaves[NUM_CAN_BOARDS];

word nmtAllarmi[NUM_CAN_BOARDS];

bool nmtBootUp[NUM_CAN_BOARDS];

tNmtUltimoAllarme nmtUltimoAllarme;

word canAllarme = nmtOk;

byte nmtLifeTimeFactor;

void NMTInit()
{
	short i;
	
	for (i = 0; i < NUM_CAN_BOARDS; ++i)
	{
		statusSlaves[i] = 0;
		nmtAllarmi[i] = nmtOk;
		nmtBootUp[i] = FALSE;
	}
	nmtUltimoAllarme.scheda = 0;
	nmtUltimoAllarme.allarme = nmtOk;
}

void NMTInitBootUp()
{
	short i;
	
	for (i = 0; i < NUM_CAN_BOARDS; ++i)
	{
		nmtBootUp[i] = FALSE;
	}
}


void NMTServerReceive()
{
	CanMessage msg;
	short id;
	
	
	if (CanReceivedPop((byte)CanNMTErrorChannel, &msg))
	{
		id = ((msg.cob_id) & 0x0F) - 1;
				
		if (id < NUM_CAN_BOARDS)
		{
			if ((statusSlaves[id] = (msg.data[0] & 0x7F)) == 0)
				nmtBootUp[id] = TRUE;
			
			tmrsCan[id] = nmtGuardTime;					
		}
	}
}

void NMTSendState(byte state)
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
	
	CanTransmitPush((byte)CanNMTSendState, &m);
}

/*
 * Check if the slave are in Ready state
 */
bool NMTSlaveRdy(tNmtUltimoAllarme *status)
{
	short i;
	bool errore = FALSE;	
	
	status->allarme = nmtOk;
	
	for (i = FIRST_CAN_BOARD; i < NUM_CAN_BOARDS; ++i)
	{
		if (!nmtBootUp[i])
		{
			if (!errore)
			{
				errore = TRUE;
				status->allarme = nmtBootUpError;
				status->scheda = i;
				canAllarme = nmtBootUpError;
				
			}
			nmtAllarmi[i] = nmtBootUpError;
		}
		else if (statusSlaves[i] != HB_PRE_OPERATIONAL && statusSlaves[i] != HB_OPERATIONAL)
		{
						
			if (!errore)
			{
				errore = TRUE;
				status->allarme = nmtRdyError;
				status->scheda = i;
				canAllarme = nmtRdyError;
				
			}
			nmtAllarmi[i] = nmtRdyError;
		}
		
		else
			nmtAllarmi[i] = nmtOk;
	}
	
	if (!errore)
		canAllarme = nmtOk;

	return !errore;
}

/*
 * Verifica se le slaves sono nello stato Operational
 */
bool NMTSlaveOperational(tNmtUltimoAllarme *status)
{
	short i;
	bool errore = FALSE;
	
	for (i = FIRST_CAN_BOARD; i < NUM_CAN_BOARDS; ++i)
	{
		if (statusSlaves[i] != HB_OPERATIONAL) 
		{
			if (!errore)
			{
				
				errore = TRUE;
				status->allarme = nmtOperationalError;
				status->scheda = i;
				canAllarme = nmtOperationalError;
			}
			
			nmtAllarmi[i] = nmtOperationalError;
		}
		else if (!tmrsCan[i])
		{
			if (!errore)
			{
				
				errore = TRUE;
				status->allarme = nmtOperationalTimeOut;
				status->scheda = i;
				canAllarme =  nmtOperationalTimeOut;
			}
			
			nmtAllarmi[i] = nmtOperationalTimeOut;
		}
		else
			nmtAllarmi[i] = nmtOk;
	}
	
	if (!errore)
		canAllarme = nmtOk;
	
	return !errore;
}

void NMTSalvaUltimoAllarme(word scheda, word allarme)
{
	nmtUltimoAllarme.scheda = scheda;
	nmtUltimoAllarme.allarme = allarme;
}

