/*
 * NMT.h
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#ifndef NMT_H_
#define NMT_H_

enum NmtErrors
{
	nmtOk,
	nmtBootUpError,
	nmtRdyError,
	nmtOperationalError,
	nmtOperationalTimeOut
};


typedef struct 
{
	word scheda;
	word allarme;
	
}tNmtUltimoAllarme;


#define NMTCodeStartRemoteNode		0

#define NMTCodeStopRemoteNode		2
	
#define NMTCodeEnterPreOperationl	128

#define NMTCodeResetNode			129

#define NMTCodeResetCommunication	130
	

#define HB_BOOTUP					0

#define HB_STOPPED					4

#define HB_OPERATIONAL				5

#define HB_PRE_OPERATIONAL			127

extern word nmtGuardTime;

extern byte nmtLifeTimeFactor;

/*	Functions' declare */

void NMTInit();

void NMTServerReceive();

void NMTInitBootUp();

bool NMTSlaveRdy(tNmtUltimoAllarme *status);

bool NMTSlaveOperational(tNmtUltimoAllarme *status);

void NMTSendState(byte state);

bool NMTSlaveRdy(tNmtUltimoAllarme *status);

void NMTSalvaUltimoAllarme(word scheda, word allarme);

#endif /* NMT_H_ */
