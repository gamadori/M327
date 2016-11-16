/*
 * Current.c
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */

#include "cpu.h"
#include "utility.h"
#include "ds1855.h"
#include "Current.h"
#include "plc.h"
#include "pid.h"
#include "Timer.h"
#include "Bits.h"
#include "Homing.h"



#define AddNominalCurr	0xf8

#define AddMaxCurr	0xf9

#define DELTA_TEMP	300

#define TIME_MIN_FAN		60	//secondi

void EnableFault0();

void DisableFault0();

word cmdNominalCurr;
word cmdMaxCurr;
	
word currentNominal;	// Corrente nominale

word currentMax;		// Corrente massima

word timeOutCurrent;	// Time Out per la corrente superiore alla soglia

word timerOutFaultDisable;	// Time Out per fault disabilitato

word currentMaxHoming;

word currentNominalHoming;

word currentMaxActual;

bool currentInHoming = TRUE;

short stepCurr = 0;

short stepFan = 0;

short tempAllarm = 2107;	// Temperatura aletta massima 70 gradi il valore 
							// a 100 gradi il valore analogico 3571
							// a 70 gradi il valore analogico 2950  misurata
							// a 60 gradi il valore analogico 2528
							// a 50 gradi il valore analogico 2107
	

bool faultOccors = FALSE;



void CurrInit()
{
	cmdNominalCurr = FALSE;
	cmdMaxCurr = FALSE;
	currentInHoming = FALSE;
		
}
void CmdNominalCurr()
{
	cmdNominalCurr = TRUE;
}
\
void CmdMaxCurr()
{
	cmdMaxCurr = TRUE;
}


void EnableFault0()
{
	
	setReg16(PWMA_SM0DISMAP0, 0xFFFFU); 
	setReg16(PWMA_SM1DISMAP0, 0xFFFFU); 
	setReg16(PWMA_SM2DISMAP0, 0xFFFFU); 
	
	
  	        
}

void DisableFault0()
{
	
	setReg16(PWMA_SM0DISMAP0, 0xFFEEU);
	setReg16(PWMA_SM1DISMAP0, 0xFFEEU);
	setReg16(PWMA_SM2DISMAP0, 0xFFEEU);
	
	        
}




/*
 * Funzione Server per la gestione delle correnti
 */

word reg[3];
void CurrServer()
{
	byte status;

	
	
	reg[0] = getReg16(PWMA_SM0DISMAP0);
	reg[1] = getReg16(PWMA_SM1DISMAP0);
	reg[2] = getReg16(PWMA_SM2DISMAP0);
	
	
	if (IsInHoming)
	{
		if (cmdNominalCurr || !currentInHoming)
		{
			status = ds1855Write(0x00, AddNominalCurr, currentNominalHoming);		
			stepCurr = 0;
			cmdNominalCurr = FALSE;
		}
		
		if (cmdMaxCurr || !currentInHoming)
		{
			status = ds1855Write(0x00, AddMaxCurr, currentMaxHoming);		
			cmdMaxCurr = FALSE;
		}
		
		currentInHoming = TRUE;
	}
	else
	{
		
		if (cmdNominalCurr || currentInHoming)
		{
			status = ds1855Write(0x00, AddNominalCurr, currentNominal);		
			stepCurr = 0;
			cmdNominalCurr = FALSE;
		}
		
		if (cmdMaxCurr || currentInHoming) 
		{
			status = ds1855Write(0x00, AddMaxCurr, currentMax);		
			cmdMaxCurr = FALSE;
		}
		
		currentInHoming = FALSE;
	
	}
	
	
	reg[0] = getReg(PWMA_FSTS0);
	
	faultOccors = (getRegBits(PWMA_FSTS0, PWMA_FSTS0_FFPIN0_MASK) == PWMA_FSTS0_FFPIN0_MASK);
	
	
		
	switch (stepCurr)
	{
		case 0:
			
			DisableFault0();
			
			if (faultOccors)
			{
				TMR_CURRENT = timeOutCurrent;
				stepCurr++;
			}
			break;
			
		case 1:
			
			if (faultOccors)
			{
				TMR_FAULT = MSEC(100);
			}
			else if (!TMR_FAULT)
				stepCurr = 0;
			
			if (!TMR_CURRENT)
			{
				// Corrente nominale limite minimo
				//ds1855Write(0x00, AddNominalCurr, currentNominal[0]);
				EnableFault0();
				
				
			//	pid_axe_status[0] = cMotorAlarm;				
			//	pid_axe_reg_state[0].alarm = AxeDriveAlarm;
				
			//	stepCurr = 0;
				stepCurr++;
			}
			
			
			break;
			
		case 2:
			// Attendo che la corrente nominale ritorno sotto al limite inferiore
			if (!faultOccors)
			{
				
				TMR_CURRENT = timerOutFaultDisable;
				stepCurr++;
			}
			break;
			
		case 3:
			
			
			if (faultOccors)
				stepCurr = 2;
			
			else if (!TMR_CURRENT)
			{
				// Corrente nominale impostata al limite massimo
				//ds1855Write(0x00, AddNominalCurr, currentNominal[1]);
				stepCurr = 0;
			}
			
			
			break;		
			
	}
}

