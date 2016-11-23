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

// state-change commands
/** change state machine state into: operational
*/
#define START_REMOTE_NODE 			0x01

/** change state machine state into: stopped
 */
#define STOP_REMOTE_NODE 			0x02

/** change state machine state into: pre-operational
 */
#define ENTER_PRE_OPERATIONAL_STATE 0x80

/** change state machine state into: reset node
 */
#define RESET_REMOTE_NODE 			0x81


/** change state machine state into: reset communication
 */
#define RESET_REMOTE_COMMUNICATION 0x82

// possible state machine states
// values are choosen so, that they can be sent directly for
// heartbeat messages...

/** state machine is in state: operational
 */
#define OPERATIONAL (byte)0x05

/** state machine is in state: stopped
 */
#define STOPPED (byte)0x04

/** state machine is in state: pre-operational
 */
#define PRE_OPERATIONAL (byte)0x7F

/** state machine is in state: initialisation of the node
 */
#define INITIALISATION_NODE (byte)0x00

/** state machine is in state: initialisation of the communication
 */
#define INITIALISATION_COMMUNICATION (byte)0x10

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

void NMTServer();

void GuardTimeService();

void GuardTimeInit();


#endif /* NMT_H_ */
