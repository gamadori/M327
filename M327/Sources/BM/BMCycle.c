/*
 * BMCycle.c
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#include "cpu.h"

#include "Bus.h"
#include "plc.h"
#include "Drive.h"

#include "BM/BMSettings.h"
#include "BM/BMPlc.h"
#include "BM/Probe.h"
#include "BM/BMProd.h"
#include "BM/BMAlarms.h"
#include "BM/BMPos.h"
#include "BM/BMCycle.h"

#define BM_DEC	20

#define AXIS_AG 0
#define AXIS_PR 1

//*************************************

OptionsMachine macOptions;			// Opzioni della macchina

t_status_mac macStatus;				// Contiene gli stati della macchina

char stato_stop_mac = 0;

OptionsMachine macOptions;			// Opzioni della macchina

short cmdCycle = 0;	 				// Comando ricevuto

short macStatusOld = 0;

char versionCycle[] = 		"M327 3.0.0.0 18-08-2017";

/********************************************/

short iSpeed;			//Set delle uscite per la velocita' di discesa delle lame
short iVertPress;		//Set dei pressori verticali

short iStepHoming;		//indice server homing

short iHomeSeq;			//cambia la sequenza di azzeramento = 1 --> BM poi PR
						//									= 0 --> BM e PR

void IniCiclo(void);

bool AxesReadyToHoming();

void StatusServer();

void HomingSever();



void IniCiclo()
{
	*((word *)&macStatus) = 0;
	iStepHoming = 0;
}
              


void ResetCiclo()
{ 
	short axe;
	
	cmdCycle = eCmdNone;
	
	for (axe = 0; axe < BM_NUM_AXES; ++axe)
	{
		BusStop(axe, BM_DEC);
	}	
	
	ResetProbe();
}        
/*-------------------------------*/
void CicloServer()
{
	ProbeHoming();
	ProbeMeasure();
	
}



/*===================================
 * Module: void CmdMacHoming()
 *
 * Description:
 *	Comando di azzeramento degli assi 
 *	della macchina
 *
 *===================================*/
void CmdMacHoming()
{
	iStepHoming = 1;//attivazione ciclo di homing in sequenza
	
/*	
	HomeAsse(0);
	
	if (macOptions.Protection)
		HomeAsse(1);
		
	if (macOptions.Probe && !probeStatus)
		CmdProbeHome();
*/	
					
}

/*------------------------------- */

void MainCycle()
{

	PositionServer();
	
	if (!inpPwrFiltered)
	{
		ResetCycle();
		macStatus.statusMac = MacInPowerOff;
	}
	
	else if (cmdCycle == eCmdStopAll)
	{
		ResetCycle();
		macStatus.statusMac = MacInPowerOff;
	}
	
	
	
	if (macStatus.statusMac > MacInPowerOff)
	{
		CicloServer();
		
		switch (cmdCycle)
		{
			case eCmdStopProd:
			case eCmdStopAll:
						
				macStatus.statusMac = MacInPowerOff;
				cmdCycle = eCmdNone;
			
					
			break;	
				
			case eCmdHoming:
				CmdMacHoming();
				cmdCycle = eCmdNone;
				
				break;
				
			case eCmdSearchMin:
				BusSearchMin(Axe1);
				
				cmdCycle = eCmdNone;
				macStatus.statusMac = MacInSearchMin;				
				break;
				
				
			case eCmdCalibration:
				CmdProbeSearchMin();
				macStatus.statusMac = MacInCalibration;
				cmdCycle = eCmdNone;				
				break;
				
			case eCmdProbeSelfLearn:
			
				CmdProbeMeasure();
				macStatus.statusMac = MacInSelfLearn;
				cmdCycle = eCmdNone;
				
				break;
				
			case eCmdProbleHoming:
				CmdProbeHome();
				cmdCycle = eCmdNone;
				break;
			
			
			case eCmdStartProd:
				
				if (macStatus.statusMac == MacInProdTerminate)
				{
					ResetProd();
				}
				macStatus.statusMac = MacInStartProduction;	
				cmdCycle = eCmdStartProdRec;
			
				break;
				
			case eCmdStartProdRec:
				if (MAC_HOMED)
				{
					macStatus.statusMac = MacInStartProduction;	
					cmdCycle = eCmdNone;
				}
				break;	
				
		}		
	}
	
	switch (macStatus.statusMac)
	{
		case MacInPowerOff:
		
			
			cmdCycle = eCmdNone;
			stepCycle = 0;
			iStepHoming = 0;
			
			// Stop agli assi
			BusStop(Axe1, BM_DEC);
			BusStop(AxeProtection, BM_DEC);
			
			if (macStatusOld != macStatus.statusMac)
				ResetCycle();
			
			if (inpPwrFiltered)
			{
				// Power On	
				macStatus.statusMac = MacInPowerOn;
			
			}			
			
			break;
		
		case MacInAlarm:
			//StopAxes();
			
			break;
			
		case MacInSelfLearnTerminate:
		case MacInCalTerminate:
		case MacInFoundMin:
		case MacInPowerOn:
		
			//CheckMacAlarm();
		
			break;
			
			
		case MacInHoming:
			// Macchina in azzeramento
			//CheckMacAlarm();
			
			if (MAC_HOMED)
			{
				macStatus.statusMac = MacInPowerOn;								
			}
			break;
		
		case MacInSearchMin:
			// Ricerca della minima asta
			if (drvAxeRegState[Axe1].homedNeg)
			{
				macStatus.statusMac = MacInFoundMin;
			}
			
			break;
		
		case MacInProduction:
		case MacInStartProduction:
		case MacInProdTerminate:
	
			//CheckMacAlarm();
			
			// Verifico se gli assi sono stati azzerati
			if (!MAC_HOMED)
			{
				CmdMacHoming();
				
			}
			else 
			{
				if (cmdCycle == eCmdStartProd || cmdCycle == eCmdStartProdRec)
				{
					stepCycle = 0;
	 				cmdCycle = eCmdNone;
				}				
				else if (cmdCycle == eCmdStopProd)
				{
					
					macStatus.statusMac = MacInPowerOn;
					cmdCycle = eCmdNone;
				
				}
				// Produzione
				ProductionServer();
			}
		
		
			break;
			
		
		case MacInCalibration:
			//CheckMacAlarm();
			break;
			
		
		case MacInSelfLearn:
			// Macchina in Self Learn
			//CheckMacAlarm();
			break;
			
		
			
				
	}
	
	macStatusOld = macStatus.statusMac;
}


/*===================================
 * Module: void ResetCycle()
 *
 * Description:
 *	Comando di Reset del ciclo
 *
 *===================================*/
void ResetCycle()
{
//	stepCycle = 0;
	
	ResetProbe();
	
	cmdCycle = eCmdNone;
	
	BusStopAll();	
	
	outProbe(0);
	
}


/***********************
Sequenza di Homing degli assi:
iHomeSeq = 0 --> AG + PR
iHomeSeq = 1 --> AG --> PR
iHomeSeq = 2 --> PR --> AG
************************/
void HomingSever()
{

	switch(iStepHoming)
	{
		//----------------------------------
		case 1://la scelta
			if (iHomeSeq == 1)
			{
				BusHome(AXIS_AG);		//Avvio homing battuta mobile
				
				iStepHoming = 10;			//passo successivo
			}
			else if (iHomeSeq == 2)
			{
				if (macOptions.Protection)	//protezione abilitata
					BusHome(AXIS_PR);		//Avvio homing Protezione
					
				iStepHoming = 20;			//passo successivo
			}
			else
			{
				BusHome(AXIS_AG);		//Avvio homing battuta mobile
				
				if (macOptions.Protection)	//protezione abilitata
					BusHome(AXIS_PR);	//Avvio Homing Protezione
				
				iStepHoming = 99;
			}
			break;
		//----------------------------------
		case 10://attesa AG in homing
			if (macOptions.Protection)	//protezione abilitata
			{
				if (drvAxeRegState[AXIS_AG].homed == 1)//attesa homing BM
				{
					BusHome(AXIS_PR);	//Avvio Homing Protezione
						
					iStepHoming = 99;		//passo successivo
				}
			}
			else
				iStepHoming = 99;
			
			break;
		//----------------------------------
		case 20://Attesa PR in Homing
			if (macOptions.Protection)	//protezione abilitata
			{
					
				if (drvAxeRegState[AXIS_PR].homed == 1)//attesa homing BM
				{
					BusHome(AXIS_AG);	//Avvio Homing AG
					
					iStepHoming = 99;	//passo successivo
				}
				else
					iStepHoming = 99;
				
				break;
			}
			else
			{
				BusHome(AXIS_AG);	//Avvio Homing AG
				
				iStepHoming = 99;	//passo successivo
				
			}
				
	}

	
}

/**************************************
 * 
 * Module void StatusServer()
 *
 * Description:
 *	Lettura dello Stato della macchina
 *
 ***************************************/

void StatusServer()
{

}

void vGestioneVelocitaDiscesaLame()
{
short velocita;

	if (iSpeed<0)
		iSpeed = 0;
	
	if (iSpeed>5)
		iSpeed = 5;
	
	if (inpPwrFiltered)
	{
		velocita = iSpeed;		
	}
	else
	{
		velocita = 0;
	}
	
	switch (velocita)
	{
		case 0:
			disOut(BIT6 | BIT7 | BIT14 | BIT15);
			break;
		case 1:
			setOut(BIT6);
			disOut(BIT7 | BIT14 | BIT15);
			break;
		case 2:
			setOut(BIT6 | BIT7);
			disOut(BIT14 | BIT15);
			break;
		case 3:
			setOut(BIT6 | BIT7 | BIT14);
			disOut(BIT15);
			break;
		case 4:
			setOut(BIT6 | BIT14 | BIT7 | BIT15);
			break;
	}
}

void vGestionePressoriVerticali()
{
short PressVert;

	if (inpPwrFiltered)
	{
		PressVert = iVertPress;		
	}
	else
	{
		PressVert = 0;
	}

	//Pressore vicino alla GUIDA FISSA LIVELLO 0	
	if (PressVert & BIT0)
	{
		outPressVert0(1);
		
	}
	else
	{
		outPressVert0(0);		
	}

	//Pressore LIVELLO 1	
	if (PressVert & BIT1)
	{
		outPressVert1(1);
	}
	else
	{
		outPressVert1(0);
	}
	//Pressore LIVELLO 2	
	if (PressVert & BIT2)
	{
		outPressVert2(1);
	}
	else
	{
		outPressVert2(0);
	}
	//Pressore LIVELLO 3	
	if (PressVert & BIT3)
	{
		outPressVert3(1);
	}
	else
	{
		outPressVert3(0);
	}
	//Pressore LIVELLO 4	
	
	
	if (PressVert & BIT4)
	{
		outPressVert4(1);
	}
	else
	{
		outPressVert4(0);
	}
	//Pressore LIVELLO 5	
	if (PressVert & BIT5)
	{
		outPressVert5(1);
	}
	else
	{
		outPressVert5(0);
	}

	
}
