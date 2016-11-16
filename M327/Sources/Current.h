/*
 * Current.h
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */

#ifndef CURRENT_H_
#define CURRENT_H_

typedef struct 
{
	byte count;
	word flashCmdVar;
	word cmdNominalCurr;
	word cmdMaxCurr;
	
}td_canCommands;

extern word currentNominal;	// Corrente nominale

extern word currentMax;			// Corrente massima

extern word currentMaxHoming;

extern word currentNominalHoming;

extern word currentMaxActual;

extern word timeOutCurrent;		// Time Out per la corrente superiore alla soglia

extern word timerOutFaultDisable;

extern bool faultOccors;

extern word reg[];

extern short tempAllarm;

void CurrInit();

void CurrServer();

void CmdNominalCurr();

void CmdMaxCurr();


#endif /* CURRENT_H_ */
