/** ###################################################################
**     Filename  : 402Homing.C
**     Project   : BattutaMobile
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 16.20
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE 402Homing */


#include "cpu.h"
#include "Dsp402StateMachine.h"
#include "Dsp402homing.h"
#include "Dsp402PositionMode.h"
#include "Bits.h"
#include "Ramp.h"
#include "plc.h"
#include "Timer.h"
#include "CanOpen.h"
#include "Encoder.h"
#include "Drive.h"


td_hm hm402[NUM_MOTORS];

short hmDir = 1;


void HomingServerSensor(td_DSP402 *pDev);

#pragma define_section codedsp402Homing "user.text"  RWX
#pragma section codedsp402Homing begin

/*====================================
 * 
 * Inizializza le variabili del modulo
 *
 *====================================*/
void HomingInit(td_DSP402 *pDev, word axe)
{	
	pDev->hm = &hm402[axe];
		
	pDev->hm->step = 0;
	pDev->hm->speed.count = 2;
	
//	pDev->hm->offset.count = 7;
}

/*=============================
 *
 * Reset degli stati del modulo
 *
 *=============================*/
void HomingReset(td_DSP402 *pDev)
{
	pDev->hm->step = 0;
	
	
}



/*=========================================
 *
 * Module: void HomingBatt()
 * Funzione di esecuzione di Azzeramento
 * in doppia battuta
 *
 *=========================================*/
void HomingServer(td_DSP402 *pDev)
{
	long err, errpos;
	
	td_hm *hm = pDev->hm;
	
	if (pDev->controlWord.hm.homingOperationStart == 0)
	{
		if (hm->step > 0)
		{
			RampStopRapidCommand(0);
			
			hm->step = 0;
		}
	}
	
	switch (hm->step)
	{
		case 0:
			pDev->statusWord.hm.homingAtteined = 0;
			
			if (pDev->controlWord.hm.homingOperationStart == 1)
			{
				// Avvio dell'azzeramentop				
				
				hmDir = 1;
				
				drvAxeRegState[0].homed = 0;
				hm->numBatt = 0;				
				hm->step++;
				//hm->step = 20; //Per provare a impostare la quota ZERO
			}
			
			break;
			
		case 1:
			hm->numBatt = 0;				
			//LoadKPidHoming();
			
			drvAxeRegState[0].alarm = 0;
			
			hm->step++;
			
		case 2:
			// Provo a cercare la battuta
			
			if (!TMR_HOMING)
			{
				RampJogCommand(0, - (hmDir * hm->speed.speedsH), (word)hm->acceleration);
				hm->step++;
			}
			break;
			
		case 3:
			errpos = (vpos[0] - rpos[0]);
			
			if (errpos >= hm->off.homSrvErrBatt || errpos <= -hm->off.homSrvErrBatt)
			{
				RampStopRapidCommand(0);
				
				hm->step++;
				
			}
			break;
			
		case 4:
			if (RampCommandExecuted(0, cSTOP_RAPID))
			{
				
				if (!hm->numBatt)
				{
					hm->posBatt = rpos[0];
					
					RampMoveCommand(0, rpos[0] + hm->off.homDistBatt, hm->speed.speedsH, (word)hm->acceleration);
				
					hm->step++;
				}
				else
				{
					// Verifico che le due posizioni di battuta individuata
					// Coincidono
					err = rpos[0] - hm->posBatt;
					
					if (err < -hm->off.homMaxErrBatt || err > hm->off.homMaxErrBatt)
					{
						// Non coincidono la posizione delle 2 battute
						// Provo ad aumentare il Srv error in battuta
						if (hm->off.homSrvErrBatt < hm->off.homMaxSrvErrBatt)
						{
							hm->off.homSrvErrBatt += 50;//hm->off.stepSrv;
							
							if (hm->off.homSrvErrBatt > hm->off.homMaxSrvErrBatt)
							{
								hm->off.homSrvErrBatt = hm->off.homMaxSrvErrBatt;
								hm->step = 1;
							}
								
						}
						else
						{
							hm->step = 10;
								
						}
						
					}
					else
					{
						RampMoveCommand(0, rpos[0] + hm->off.offset, hm->speed.speedsH, (word)hm->acceleration);

						hm->step = 15;
					}
				}
			}
			break;
			
		case 5:
			if (dpos[0] == vpos[0])
			{
				// Riprovo un'altra volta cercare la battuta			
				TMR_HOMING = MSEC(200);
				hm->step = 2;
				hm->numBatt++;				
				
			}
			break;
		
		case 10:
			// Errore durante l'azzeramento
			drvAxeRegState[0].alarm = AxeAlHoming;
			DriveCommandEnable(0, 0);
			break;
			
		case 15:
			if (dpos[0] == vpos[0])
			{
				
				vpos[0] = rpos[0];
				
				//UpLoadKPidHoming();
				
				hm->step = 20;
				
				TMR_HOMING = MSEC(100);
			}
			
			break;
		
		case 20:
			if (!TMR_HOMING)
			{
				
				rpos[0] = vpos[0] = dpos[0] = hm402[0].off.homPosAfterHome;
				
				drvAxeRegState[0].homed = 1;
				pDev->statusWord.hm.homingAtteined = 1;	
				pDev->statusWord.hm.targetReached = 1;	
				hm->step = 25;
			}
			
			break;
			
		case 25:
			if (pDev->controlWord.hm.homingOperationStart == 0)
			{
					pDev->statusWord.hm.homingError = 0;
					RampStopRapidCommand(0);
					hm->step = 0;
			}
	}
}


/***********************************************
 
 ***********************************************/

void HomingServerSensor(td_DSP402 *pDev)
{
	
	td_hm *hm = pDev->hm;
	
	if (pDev->controlWord.hm.homingOperationStart == 0)
	{
		if (hm->step > 0)
		{
			RampStopRapidCommand(0);			
			hm->step = 0;
		}
	}
	
	switch (hm->step)
	{
		case 0:
			pDev->statusWord.hm.homingAtteined = 0;
			
			if (pDev->controlWord.hm.homingOperationStart == 1)
			{
				// Avvio dell'azzeramentop				
				
				hmDir = 1;
				
				drvAxeRegState[0].homed = 0;
				hm->numBatt = 0;				
				hm->step++;
			}
			
			break;
		case 1:
			hm->numBatt = 0;
			//LoadKPidHoming();
			
			drvAxeRegState[0].alarm = 0;
									
			hm->step++;
			
		case 2:
			// Provo a cercare la battuta
			
			if (!inpHome)
			{
							
				RampJogCommand(0, - (hmDir * hm->speed.speedsH), (word)hm->acceleration);
				
				hm->step++;
			}
			else 
				hm->step = 3;
			break;
			
		case 3:
			if (inpHome)
			{
				RampStopRapidCommand(0);
				
				hm->step++;
				
			}
			break;
			
		case 4:
												
			RampJogCommand(0, hmDir * hm->speed.speedsL, (word)hm->acceleration);
			hm->step++;
					
			break;
			
		case 5:
			if (!inpHome)
			{
				RampStopRapidCommand(0);
				hm->step = 15;
			}
				
			break;
			
			
		case 15:
			if (RampCommandExecuted(0, cSTOP_RAPID))
			{
				
				vpos[0] = rpos[0];
				
				//UpLoadKPidHoming();
				
				hm->step = 20;
				
				TMR_HOMING = MSEC(200);
			}
			
			break;
		
		case 20:
			if (!TMR_HOMING)
			{
				
				rpos[0] = vpos[0] = dpos[0] = 0;
				drvAxeRegState[0].homed = 1;
				pDev->statusWord.hm.homingAtteined = 1;	
				pDev->statusWord.hm.targetReached = 1;	
				hm->step = 25;
			}
			
			break;
			
		case 25:
			if (pDev->controlWord.hm.homingOperationStart == 0)
			{
					pDev->statusWord.hm.homingError = 0;
					RampStopRapidCommand(0);
					hm->step = 0;
			}
							
			
	}
}

bool IsInHoming()
{
	return (devices402[0].modeOperation == moHomingMode && devices402[0].hm->step > 0 && 
		devices402[0].hm->step < 10);
}


#pragma section codedsp402Homing end

/* END 402Homing */
