/*
 * Ramp.c
 *
 *  Created on: Feb 15, 2016
 *      Author: info
 */

#include "cpu.h"
#include "settings.h"
#include "Pid.h"
#include "Encoder.h"
#include "Utility.h"
#include "Tuning.h"
#include "drive.h"
#include "Slave.h"
#include "Ramp.h"


#define RAMP_MIN_DEC	5

#define RAMP_MAX_DEC	100

#define RAMP_CONST		128l

#define RAMP_CONST_MOD	0x7F

#define RAMP_SHIFT		7


long vpos[MAC_NUM_AXIS];						// Virtual Position


long rvel[NUM_AXES];	// Actual Speed
long dvel[NUM_AXES];	// Target Speed
short dacc[NUM_AXES];	// acceleration
long dpos[NUM_AXES];	//Target Position

short maxAcc[NUM_AXES];

long kvel[NUM_AXES];		// Per la gestione dei decimali

short command[NUM_AXES];	// Per ciascun asse contiene il comando corrente

short cmdStatus[NUM_AXES];						// Ultimo comando eseguito

// Servo Error
long srvErrorStatic[NUM_AXES];			// Static Servo Error
long srvErrorDynamic[NUM_AXES];			// Dynamic Servo Error
word srvErrorTimeOut[NUM_AXES];			// Time Out Static Servo Error

word srvTimer[NUM_AXES];
word srvStep[NUM_AXES];

long swLimitPos[NUM_AXES];
long swLimitNeg[NUM_AXES];

// Dichiarazioni di funzioni
void RampJogPos(byte axe);

void RampJogNeg(byte axe);

void RampStop(byte axe, word dec);

void RampStopRapid(byte axe);

void RampNextPosition(byte axe);

void RampServerCommand(byte axe);

void RampOnOff(byte axe);

void RampProfilator(byte axe);

void RampInit()
{
	byte i;
	
	for (i = 0; i < NUM_AXES; ++i)
	{
		dpos[i] = 0;
		rvel[i] = dvel[i] = 0;
		vpos[i] = rpos[i] = 0;
		kvel[i] = 0;
		command[i] = cSTOP;
		cmdStatus[i] = cNULL;
	}
}

void RampReset(byte axe)
{
	dpos[axe] = vpos[axe] = rpos[axe];
	kvel[axe] = 0;
	dvel[axe] = rvel[axe] = 0;
	
}
#pragma interrupt called
void RampServer()
{
	byte i;
	
	for (i = 0; i < NUM_AXES; ++i)
	{
		RampServerCommand(i);
		RampNextPosition(i);
	}
}

/*************************************************************************************
 * void PidServerCommand(word axe)
 *
 * Description
 *	Funzione Server dei comandi agli assi
 *
 * Return: None
 *
 * Arguments:
 *word axe: indice dell'asse
 *
 *************************************************************************************/
#pragma interrupt called
void RampServerCommand(byte axe)
{
		
	switch (command[axe])
	{
		case cJOG_POS:
			// Comando di JOG Positivo
			RampJogPos(axe);
			break;
			
		case cJOG_NEG:
			// Comando di JOG Positivo
			RampJogNeg(axe);
			break;
			
		case cMOVE:
			// Comando di Posizionamento
			RampProfilator(axe);
			break;
			
		case cSTOP:
			// Comando di STOP in rampa
			
			RampStop(axe, dacc[axe]);
			break;
			
		case cSTOP_RAPID:
			// Comando di STOP in rampa rapido
			RampStopRapid(axe);
			break;
		
		case cONOFF:
			RampOnOff(axe);
			break;
		
	}
	
	
}

bool RampInMove(byte axe)
{
	return (command[axe] == cMOVE);
}
/*************************************************************************************
 * void PidServerCommand(word axe)
 *
 * Description
 *	Muove l'asse in Jog direzione positiva, eseguendo una rampa di
 *	accelerazione
 *
 * Return: None
 *
 * Arguments:
 *	word axe: indice dell'asse
 *
 *************************************************************************************/
#pragma interrupt called	
void RampJogPos(byte axe)
{
	if (rvel[axe] > dvel[axe])
	{
		// Sto andanto troppo forte devo rallentare
		rvel[axe] -= (long)dacc[axe];
		if (rvel[axe] < dvel[axe])
			rvel[axe] = dvel[axe];
	}
	else if (rvel[axe] < dvel[axe])
	{
		// Devo accellerare
		rvel[axe] += (long)dacc[axe];
		if (rvel[axe] > dvel[axe])
			rvel[axe] = dvel[axe];
	}
	
	cmdStatus[axe] = cJOG_POS;
	
	
}
#pragma interrupt called
void RampJogNeg(byte axe)
{
	if (rvel[axe] < -dvel[axe])
	{
		// Sto andanto troppo forte devo rallentare
		rvel[axe] += (long)dacc[axe];
		if (rvel[axe] > dvel[axe])
			rvel[axe] = dvel[axe];
	}
	else if (rvel[axe] > -dvel[axe])
	{
		// Devo accellerare
		rvel[axe] -= (long)dacc[axe];
		if (rvel[axe] < -dvel[axe])
			rvel[axe] = -dvel[axe];
	}
	
	cmdStatus[axe] = cJOG_NEG;
}

/*############################################################################
 * Funzione di stop dell'asse 
 * Ferma l'asse eseguento una rampa in base alla decellerazione che gli viene
 * passata come parametro
 *###########################################################################*/
#pragma interrupt saveall
void RampStop(byte axe, word dec)
{
	if (rvel[axe] < 0)
	{
		rvel[axe] += dec;
		if (rvel[axe] >= 0)
		{
			rvel[axe] = 0;
			
			kvel[axe] = 0;
			
			cmdStatus[axe] = cSTOP;
		}
	}
	else
	{
		rvel[axe] -= dec;
		
		if (rvel[axe] <= 0)
		{
			rvel[axe] = 0;
			
			kvel[axe] = 0;
			
			cmdStatus[axe] = cSTOP;	
		}
	}
	
			
}

#pragma interrupt called
void RampOnOff(byte axe)
{
	rvel[axe] = dvel[axe];
}


/*####################################################################### 
 * Funzione di Stop Rapido dell'asse
 * ferma l'asse immediatamente senza eseguire la rampa di decellerazione
 * deve essere usato solo nei casi di emergenza
 #######################################################################*/
#pragma interrupt called
void RampStopRapid(byte axe)
{
	kvel[axe] = 0;
	rvel[axe] = 0;
	dpos[axe] = vpos[axe];
	
	cmdStatus[axe] = cSTOP_RAPID;

}


/*************************************************************************************
 * void PidProfilator(word axe)
 *
 * Description
 *	Posiziona i due assi in modo sincrono
 *
 * Return: None
 *
 * Arguments: None
 *	
 *
 *************************************************************************************/
#pragma interrupt saveall
void RampProfilator(byte axe)
{
	long delta_pos;
	long spa_arr;
	long temp;
	
	cmdStatus[axe] = cMOVE;
	
	// Calcolo dello spazio che mi rimane prima di arrivare a destinazione
	delta_pos = dpos[axe] - vpos[axe];
	
	
	// Calcolo dello spazio di arresto alla velocita' che ci troviamo
	if (rvel[axe] >= 0)
		temp = (rvel[axe] + dacc[axe]);
		
	else
		temp = rvel[axe] - dacc[axe];
		
	
	
	spa_arr = (temp * temp) / (2 * dacc[axe] * RAMP_CONST);
	
	if (delta_pos > spa_arr)
	{
		// Non sono ancora in fase di decellerazione
		if (rvel[axe] < dvel[axe])
		{
			// Procedura di accelerazione
			rvel[axe] += dacc[axe];
			if (rvel[axe] > dvel[axe])
				rvel[axe] = dvel[axe];
		}
		else if (rvel[axe] > dvel[axe])
		{
						
			// Procedura di accelerazione	
			rvel[axe] -= dacc[axe];
			if (rvel[axe] < dvel[axe])
				rvel[axe] = dvel[axe];
		}
		
	}
	else if (delta_pos < -spa_arr)
	{
		// Non sono ancora in fase di decellerazione
		if (rvel[axe] > -dvel[axe])
		{			
			
			// Procedura di accelerazione
			rvel[axe] -= dacc[axe];
			if (rvel[axe] < -dvel[axe])
				rvel[axe] = -dvel[axe];
		}
		else if (rvel[axe] < -dvel[axe])
		{			
			// Procedura di decellerazione
			rvel[axe] += dacc[axe];
			if (rvel[axe] > -dvel[axe])
				rvel[axe] = -dvel[axe];
		}
		
	}
	else 
	{
		/* Escludo il calcolo dei decimali */
		if (rvel[axe] > RAMP_CONST || rvel[axe] < -RAMP_CONST)
			kvel[axe] = 0;
		
		/* Calcolo lo spazio di arresto senza accelerare*/
		spa_arr = (rvel[axe] * rvel[axe]) / (2 * dacc[axe] * RAMP_CONST);
		
		if (delta_pos < -spa_arr || delta_pos > spa_arr)
		{
			
		}
		else //if (delta_pos >= -spa_arr && delta_pos <= spa_arr)
		{
			

			// Arresto in Rampa
			if (rvel[axe] > dacc[axe])
				rvel[axe] -= dacc[axe];
			
			else if (rvel[axe] < -dacc[axe])
				rvel[axe] += dacc[axe];
			
			else
			{
				
				rvel[axe] = 0;
				vpos[axe] = dpos[axe];
				kvel[axe] = 0;
			}
				
			
			if (delta_pos >= -1 && delta_pos <= 1)
			{
				
				rvel[axe] = 0;
				vpos[axe] = dpos[axe];
				kvel[axe] = 0;
				
				TngEnd();
								
			}
		}
	}
}


/*#################################################################
 * Calcola in base alla velocita' richiesta la posizione successiva 
 * dell'asse virtuale
 *#################################################################*/
#pragma interrupt called
void RampNextPosition(byte axe)
{
	long tmp;
	
	kvel[axe] += rvel[axe];
	
	if (kvel[axe] >= 0)
		tmp = kvel[axe];
	else
		tmp = -kvel[axe];
	
	
	
	if (kvel[axe] >= 0)
	{
		vpos[axe] += (tmp >> RAMP_SHIFT); 
	
	}
	else
	{
		vpos[axe] -= (tmp >> RAMP_SHIFT); 
	
	}
	
	tmp = tmp & RAMP_CONST_MOD;
	
	if (kvel[axe] < 0)
	{
		
		kvel[axe] = - tmp;
		
	}
	else
		kvel[axe] = tmp;
		
		
}

/*****************************
 * Description:
 * 	Stop with Ramp
 * Return: 
 * 	None
 * Arguments:
 * 	byte axe: Axe's id
 * 	word dec: deceleration 
 ****************************/
void RampStopCommand(byte axe, word dec)
{
	if (!dec)
	{
		PidStopRapidCommand(axe);
	}
	else
	{
		if (dec < RAMP_MIN_DEC)
			dec = RAMP_MIN_DEC;
		
		dacc[axe] = dec;
		command[axe] = cSTOP;
	}
}

/****************************
 * Description:
 * 	Stop without Ramp
 * Return: 
 * 	None
 * Arguments:
 * 	byte axe: Axe' index
 ***************************/
#pragma interrupt called
void PidStopRapidCommand(byte axe)
{		
	command[axe] = cSTOP_RAPID;
}

void RampMoveCommand(byte axe, long pos, long speed, short acc, bool tuning)
{
	dpos[axe] = pos;
			
	dacc[axe] = acc;
	dvel[axe] = speed;
	command[axe] = cMOVE;
	
	if (tuning)
		TngInit();
	
}

void RampJogCommand(byte axe, long speed, short acc)
{
	if (speed >= 0)
	{
		
		dacc[axe] = acc;
		dvel[axe] = speed;
		command[axe] = cJOG_POS;
	}
	else if (speed < 0)
	{
		dacc[axe] = acc;
		dvel[axe] = speed;
		command[axe] = cJOG_POS;
	}
	
}
	


/*
 * Check Servo Error
 */
void RampSrvError(byte axe)
{
	long err = vpos[axe] - rpos[axe];
	
	if (!rvel[axe])
	{
		// axis is stopped
		
		if (srvErrorStatic[axe] && (err < -srvErrorStatic[axe] || err > srvErrorStatic[axe]))
		{
			if (!srvStep[axe])
			{
				srvTimer[axe] = srvTimer[axe];
				srvStep[axe]++;
			}
			else if (!srvTimer[axe]--)
			{
				// Static Servo Error
				drvAxeRegState[axe].alarm = AxeSrvStatic;
				drvAxeStatus[axe] = cDriveAlarm;
			}
		}
		else 
		{
			srvStep[axe] = 0;
			
		}
	}
	else if (srvErrorDynamic[axe] && (err < -srvErrorDynamic[axe] || err > srvErrorDynamic[axe]))
	{
		// Dynamic Servo Error
		drvAxeRegState[axe].alarm = AxeSrvDynamic;
		drvAxeStatus[axe] = cDriveAlarm;
	}
}

void RampSwLimit(byte axe)
{
	if (drvAxeRegState[axe].homed)
	{
		drvAxeRegState[axe].limSwMin = (vpos[axe] < swLimitNeg[axe]);
		
		if (drvAxeRegState[axe].limSwMin && rvel[axe] < 0)	// Minimum Software Limit reached
		{
			
			RampStopCommand(axe, 0);
		}
		drvAxeRegState[axe].limSwMax = (vpos[axe] > swLimitPos[axe]);
		
		if (drvAxeRegState[axe].limSwMax && rvel[axe] > 0)	// Maximun Software Limit reached
		{
			RampStopCommand(axe, 0);
		}
	}
	else
	{
		drvAxeRegState[axe].limSwMin = 0;	// Minimum Software Limit reached
		drvAxeRegState[axe].limSwMax = 0;	// Maximun Software Limit reached
	}
}

