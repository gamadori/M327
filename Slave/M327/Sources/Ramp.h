/*
 * Ramp.h
 *
 *  Created on: Feb 15, 2016
 *      Author: info
 */

#ifndef RAMP_H_
#define RAMP_H_

/* Comandi per gli assi */
enum eAxeCommands
{              
	cNULL,
	cJOG_POS,			// Comando di JOG Positivo
	cJOG_NEG,			// Comando di JOG Negativo
	cMOVE,				// Comando di Posizionamento
	cSTOP,				// Comando di Stop in Rampa
	cSTOP_RAPID,			// Comando di Stop Rapido, no Rampa
	cONOFF
};

extern long vpos[];						// Virtual Position

extern long rvel[];

extern long dvel[];

extern short dacc[];

extern long dpos[];	

extern long kvel[];	

extern long srvErrorStatic[];

extern long srvErrorDynamic[];

extern word srvErrorTimeOut[];



void RampInit();

void RampReset(byte axe);

void RampServer();

void RampJogPos(byte axe);

void RampJogNeg(byte axe);

void RampStopRapidCommand(byte axe);

void RampStopCommand(byte axe, word dec);

void RampJogCommand(byte axe, long speed, short acc);

void RampMoveCommand(byte axe, long pos, long speed, short acc);

void RampMoveCommandTuning(byte axe, long pos, long speed, short acc, bool tuning);

bool RampRSJog(char *param, short index);

bool RampRSMove(char *param, short index, bool tuning);

bool RampRSStop(char *param, short index);

void RampSrvError(byte axe);

bool RampInMove(byte axe);

bool RampCommandExecuted(byte axe, byte cmd);

#endif /* RAMP_H_ */
