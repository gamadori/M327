/*
 * Probe.c
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */
#include "cpu.h"

#include "Encoder.h"
#include "Eeprom.h"

#include "BM/BMSettings.h"
#include "BM/BMPlc.h"
#include "BM/BMCycle.h"
#include "BM/BMTimers.h"
#include "BM/BMProd.h"
#include "BM/BMPos.h"
#include "BM/BMAlarms.h"
#include "BM/Probe.h"

long probePos0 = 0;

word probeNumBatt;

long probePos;

long probeBatt;

short probeHomMaxErrBatt;

word probeTimeOut = 200;

word probeStatus;

word stepProbe;

void InitProbe()
{
	stepProbe = 0;
	probeStatus = 0;
}

void ResetProbe()
{
	stepProbe = 0;
	outProbe(0);

}

void CmdProbeHome()
{
	stepProbe = 1;
}

void CmdProbeSearchMin()
{
	stepProbe = 20;
}

void CmdProbeMeasure()
{
	stepProbe = 100;
}

void ProbeMeasure()
{
	switch (stepProbe)
	{
		case 100:
			outProbe(1);
			TMR_PROBE = MSEC(500);
			probePos = encPosRel[ENC_PROBE];
			stepProbe++;
			break;
			
		case 101:
			if (!TMR_PROBE)
			{
				
				if (probePos == encPosRel[ENC_PROBE])
				{
					switch (macStatus.statusMac)
					{
						case MacInSelfLearn:
							// Calcolo della dimensione del bordo dell'asta	
							mouldDepthImp[DepthSideB] = encPos[ENC_PROBE];
							mmMouldDept[DepthSideB] = encMmPos[ENC_PROBE];					
							EepromSave();
							stepProbe++;
							
							break;
							
						case MacInCalibration:
							// Calcolo del minimo
							encImpMinPos[ENC_PROBE] = encPos[ENC_PROBE];
						
							EepromSave();
							stepProbe++;
							break;
							
						default:
							stepProbe += 2;
							break;
					}
										
					
				}
				TMR_PROBE = MSEC(probeTimeOut);	
				
				probePos = encPosRel[ENC_PROBE];
				
			}
			break;
			
		case 102:
			if (flashCmdVar == eFlashNull)
			{
				switch (macStatus.statusMac)
				{
					case MacInSelfLearn:						
						macStatus.statusMac = MacInSelfLearnTerminate;							
						break;
							
					case MacInCalibration:							
						macStatus.statusMac = MacInCalTerminate;
						break;
				}
				stepProbe++;
			}
			break;
			
	}
}
/*=========================================
 *
 * Module: void HomingBatt()
 * Description: 
 *  Funzione di esecuzione di Azzeramento
 *  del tastatore
 *
 *=========================================*/
void ProbeHoming()
{
	long err;
	long a;
	
	switch (stepProbe)
	{
		
		case 1:
			probeStatus = 0;
	
		case 20:
			outProbe(1);
			stepProbe++;
			
			probeNumBatt = 0;
			probePos = encPos[ENC_PROBE];
			
			
			
			Alarms.AlarmWidth = AxeOk;
					
			TMR_PROBE = MSEC(probeTimeOut);
			
						
			break;
			
		case 21:
		case 2:
			if (!TMR_PROBE)
			{
				
				if (probePos == encPos[ENC_PROBE])
				{
					
					
					
					if (!probeNumBatt)
					{
						probeBatt = probePos;
						stepProbe++;
						outProbe(0);
						probeNumBatt++;
					}
					else
					{
						err = encPos[ENC_PROBE] - probeBatt;
					
						if (err < -probeHomMaxErrBatt || err > probeHomMaxErrBatt)
						{
							// Errore 
							stepProbe = 50;
							Alarms.AlarmWidth = AxeAlHoming;
							outProbe(0);	
						}
						else
						{
							//SET_MODE_OPERATION(ENC_PROBE, moResetEncoder2);	// Azzeramento encoder
							if (stepProbe == 2)			
								encPos0[ENC_PROBE] = encPos[ENC_PROBE]; 
							else
							{
								encImpMinPos[ENC_PROBE] = encPosRel[ENC_PROBE];
							}
							stepProbe = 60;
							Alarms.AlarmWidth = AxeOk;
							
						}
						
						
					}
				}
				
				
				TMR_PROBE = MSEC(probeTimeOut);	
				
				probePos = encPos[ENC_PROBE];
				
			}
			break;
		case 23:
		case 3:
			if (!TMR_PROBE)
			{
				if (encPos[ENC_PROBE] >= probePos)
				{
					
					outProbe(1);
					TMR_PROBE = MSEC(probeTimeOut);
					
					
					stepProbe--;	
				}
			}
			break;
			
		case 60:
			if (!TMR_PROBE)
			{
				outProbe(0);
				probeStatus = 1;
			}
			break;
	}
}
