/*
 * BMCycle.h
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#ifndef BMCYCLE_H_
#define BMCYCLE_H_

#ifdef _IN_SIMULAZIONE                  
	#define SIMULA_PWR  1
	#define SIMULA_AZZ	1
#else
	#define SIMULA_PWR  0
	#define SIMULA_AZZ	0
#endif                   


/************************
	Variabili 
*/

typedef struct
{
	dword statusMac: 		4;	// Stato della macchina
	dword PwrOffAxeX:		1;
	dword PwrOffProtection:	1;
	dword InPosAxeX:		1;
	dword InPosProtection:	1;
									
}t_status_mac;

typedef struct 
{
	word HomedAxe1:				1;	// 0
	word HomedProtection:		1;	// 1
	word HomedProbe:			1;	// 2
	word AlarmAxe1: 			3;	// 3,4,5
	word AlarmProtection: 		3;	// 6,7,8
	word AlarmProbe:			2;	// 9,10
	word AlarmAxe1SwMin:		1;	// 11
	word AlarmAxe1SwMax:		1;	// 12
	word nmtSlave1:				1;	// 13
	word nmtSlave2:				1;	// 14
	word eepromError:			1;	// 15
	
}t_alarm_mac;

typedef struct 
{
	word Probe	:		1;
	word Protection:	1;
	
}OptionsMachine;

enum AxeName
{
	Axe1,
	AxeProtection
	
};




enum StatusMachine
{
	MacInPowerOff,				//0000	0
	MacInHoming,				//0001	1
	MacInAlarm,					//0010	2
	MacInStartProduction,		//0011	3
	MacInProduction,			//0100	4
	MacInProdTerminate,			//0101	5
	MacInPowerOn,				//0110	6
	MacInSearchMin,				//0111	7
	MacInFoundMin,				//1000	8
	MacInCalibration,			//1001	9
	MacInCalTerminate,			//1010	10
	MacInSelfLearn,				//1011	11
	MacInSelfLearnTerminate		//1100	12
};


enum CycleCommands
{
	eCmdNone,			//0
	eCmdStopProd,		//1
	eCmdStartProd,		//2
	eCmdHoming,			//3
	eCmdSearchMin,		//4
	eCmdCalibration,	//5
	eCmdProbeSelfLearn,	//6
	eCmdProbleHoming,	//7
	eCmdStopAll,		//8
	eCmdStartProdRec	//9
};



extern OptionsMachine macOptions;			// Opzioni della macchina

extern t_status_mac macStatus;				// Contiene gli stati della macchina

extern short iHomeSeq;			//cambia la sequenza di azzeramento = 1 --> BM popi PR

extern char stato_stop_mac;

extern char versionCycle[];


//extern short inpIrq[2];
extern short iSpeed;			//Set delle uscite per la velocita' di discesa delle lame
extern short iVertPress;		//Set dei pressori verticali

/**********************************
	Funzioni
*/

void ResetCiclo();

void IniCiclo();

void CicloServer();

void CmdMacHoming();

void ResetCycle();

void MainCycle();

void vGestioneVelocitaDiscesaLame();

void vGestionePressoriVerticali();


#endif /* BMCYCLE_H_ */
