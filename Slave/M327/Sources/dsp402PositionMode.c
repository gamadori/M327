/** ###################################################################
**     Filename  : 402PositionMode.C
**     Project   : MP_M307_SLAVE
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 16.16
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE 402PositionMode */

#include "cpu.h"
#include "Dsp402StateMachine.h"
#include "Dsp402PositionMode.h"
#include "Dsp402Homing.h"
#include "Pid.h"
#include "plc.h"
#include "Timer.h"
#include "Ramp.h"

td_pp pp402[NUM_MOTORS];


void PositionReset(td_DSP402 *pDev);

bool ppNewSetPoint(td_DSP402 *pDev);

void ppSetPointAcknowledge(td_DSP402 *pDev, word status);



/*====================================
 * 
 * Inizializza le variabili del modulo
 *
 *====================================*/

void PositionInit(td_DSP402 *pDev, word axe)
{
	pDev->pp = &pp402[axe];		
	pDev->pp->posRangeLimit.count = 2;
	pDev->pp->posLimit.count = 2;
	
	pDev->pp->stepTarget = 0;
}
/*=============================
 *
 * Reset degli stati del modulo
 *
 *=============================*/
void PositionReset(td_DSP402 *pDev)
{
	pDev->pp->stepTarget = 0;
	ppSetPointAcknowledge(pDev, 0);
}

void PositionModeServer(td_DSP402 *pDev)
{
	switch (pDev->pp->stepTarget)
	{
		case 0:			
			ppSetPointAcknowledge(pDev, 0);
			pDev->pp->stepTarget++;
			
			break;
			
		case 1:
			if (ppNewSetPoint(pDev))
			{				
				// Esecuzione del comando di posizione
				SendTargetPosition(pDev);
				
				// Avverto che ho ricevuto il comando di posizione
				ppSetPointAcknowledge(pDev, 1);
			
				pDev->pp->stepTarget++;
				
				TMR_SETPOINT = MSEC(10);//100
			}
			break;
			
		case 2:
			// Attendo che New Set Point sia disabilitato
			if (!ppNewSetPoint(pDev))// || !TMR_SETPOINT)
			{
				ppSetPointAcknowledge(pDev, 0);
				pDev->pp->stepTarget = 0;  
			}
			
			break;
	}
	
	if (pDev->pp->targetPosition == vpos[0])
		pDev->statusWord.pp.targetReached = 1;
	else
		pDev->statusWord.pp.targetReached = 0;
	
	
}

void JogModeServerPos(td_DSP402 *pDev)
{
	if (pDev->pp->profileVelocity < 0)
		pDev->pp->profileVelocity = -pDev->pp->profileVelocity;
	
	RampJogCommand(0, pDev->pp->profileVelocity, (short)pDev->pp->profileAcceleration);
	
}

void JogModeServerNeg(td_DSP402 *pDev)
{
	if (pDev->pp->profileVelocity > 0)
		pDev->pp->profileVelocity = -pDev->pp->profileVelocity;
	
	RampJogCommand(0, pDev->pp->profileVelocity, (short)pDev->pp->profileAcceleration);
}

bool ppNewSetPoint(td_DSP402 *pDev)
{
	return pDev->controlWord.pp.newSetPoint;
}

void ppSetPointAcknowledge(td_DSP402 *pDev, word status)	
{
	pDev->statusWord.pp.SetPointAck = status;
}



void SendTargetPosition(td_DSP402 *pDev)
{

	
	td_pp *pp = pDev->pp;
	td_factorGroup *fg = pDev->fg;
	 
	switch (pp->motionProfileType)
	{
		case 0:
			RampMoveCommand(0, pp->targetPosition, pp->profileVelocity, (short)pp->profileAcceleration);
			break;
			
		default:
			
			RampMoveCommand(0, pp->targetPosition, pp->profileVelocity, (short)pp->profileAcceleration);
			break;
	}
}


/* END 402PositionMode */
