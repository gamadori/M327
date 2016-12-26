/*
 * Homing.c
 *
 *  Created on: Dec 25, 2016
 *      Author: info
 */

#include "cpu.h"
#include "Settings.h"
#include "Drive.h"
#include "Pid.h"
#include "Bus.h"
#include "Ramp.h"
#include "Encoder.h"
#include "Homing.h"


short hmDir[NUM_AXES];				// Direzione per cercare la battuta 1 Positiva 0 Negativa
long hmOffset[NUM_AXES];			// Offset dalla battuta 
long hmSrvErrBatt[NUM_AXES];		// Servo Error in battuta
long hmMaxSrvErrBatt[NUM_AXES];		// Massimo servo Error in battuta
long hmMaxErrBatt[NUM_AXES];		// Massima differenza delle posizioni delle battute trovate	
long hmDistBatt[NUM_AXES];
long hmPosAfterHome[NUM_AXES];
long hmSpeedH[NUM_AXES];
long hmSpeedL[NUM_AXES];
short hmAcc[NUM_AXES];

short hmBattNum[NUM_AXES];	// Numero di battute finora eseguite con successo
short hmStep[NUM_AXES];
long hmBattPos[NUM_AXES];	// Posizione in cui si e' trovata la battuta

void HomeIncSrvBatt(byte axe);

void HomeInit(byte axe)
{
	hmStep[axe] = 0;
}

void HomeStart(byte axe)
{
	hmStep[axe] = 0;
}

void HomeServer(byte axe)
{
	long errPos;
	switch (hmStep[axe])
	{
	case 0:
		drvAxeRegState[axe].homed = 0;
		PidLoadHomingParam(axe);
		hmBattNum[axe] = 0;
		hmStep[axe]++;
		// muovo alla ricerca della battuta
	
	case 1:
		
		BusJog(axe, hmDir[axe] * hmSpeedH[axe], hmAcc[axe]);
		hmStep[axe]++;
		break;
		
	case 2:
				
		errPos = (vpos[axe] - rpos[axe]);
				
		if (errPos >= hmSrvErrBatt[axe] || errPos <= -hmSrvErrBatt[axe])
		{
			BusStop(axe, 0);
			hmStep[axe]++;
					
		}
		break;
		
	case 3:
		if (RampAxeGetCmd(axe, cSTOP_RAPID))
		{
			if (!hmBattNum[axe])
			{
				// E' la prima volta che trovo la battuta devo provarci un'altra volta
				hmBattPos[axe] = rpos[axe];
				BusMove(axe, rpos[axe] - hmDir[axe] * hmDistBatt[axe], hmSpeedH[axe], hmAcc[axe], 0);				
			
				hmStep[axe]++;
			}
			else
			{
				// Verifico che le due posizioni di battuta individuata
				// Coincidono
				errPos = rpos[axe] - hmBattPos[axe];
				
				if (errPos < -hmMaxErrBatt[axe] || errPos > hmMaxErrBatt[axe])
				{
					// Le posizioni non coincidono provo ad aumentare il srvError
					if (hmSrvErrBatt[axe] < hmMaxSrvErrBatt[axe])
					{
						HomeIncSrvBatt(axe);
					}
					else
					{
						drvAxeRegState[axe].alarm = AxeAlHoming;
						hmStep[axe] = 10;
						PidUploadHomingParam(axe);
					}
				}
				else 
				{
					BusMove(axe, rpos[axe] - hmDir[axe] * hmOffset[axe], hmSpeedH[axe], hmAcc[axe], 0);
					hmStep[axe]++;
				}
			}
		}
		break;
		
	case 4:
		if (vpos[axe] == dpos[axe])
		{
			rpos[axe] = vpos[axe] = dpos[axe] = hmPosAfterHome[axe];							
			drvAxeRegState[axe].homed = 1;
			PidUploadHomingParam(axe);
			hmStep[axe] ++;
		}
		break;
	
	}
}

void HomeIncSrvBatt(byte axe)
{
	hmSrvErrBatt[axe] += 50;//hm->off.stepSrv;
													
	if (hmSrvErrBatt[axe] > hmMaxSrvErrBatt[axe])						
		hmSrvErrBatt[axe] = hmMaxSrvErrBatt[axe];
	hmStep[axe] = 1;
	hmBattNum[axe] = 0;
}

bool IsInHoming(byte axe)
{
	return hmStep[axe] > 0;
}

