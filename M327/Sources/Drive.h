/*
 * Drive.h
 *
 *  Created on: Feb 17, 2016
 *      Author: gianluca
 */

#ifndef DRIVE_H_
#define DRIVE_H_

enum eAxeStatus
{
	cDisable,			// 0
	cWaitAlmFromPwr,	// 1
	cSendEnableToPwr,	// 2
	cWaitRdyFromDrive,	// 3
	cEnableDrive,		// 4
	cEnabled,			// 5
	cWaitDisable,		// 6
	cDriveAlarm,		// 7
	cNotUsed			// 8
};

// 3 bits Axe Alarm Code
enum AxeCodeAlarm
{
	AxeOk,			// 000:	Axe is OK
	AxeSrvStatic,	// 001: Static Servo Error
	AxeSrvDynamic,	// 010: Dynamic Servo Error	
	AxeAlHoming,	// 011: Homing Alarm
	AxeDriveAlarm	// 100: Drive Alarm
};

typedef struct
{
	word in_pos: 	1;		// virtual position = target position
	word homedNeg:	1;		// homing in negative direction has been done
	word homedPos:	1;		// homing in positive direction has been done
	word homed:		1;		// homing has been done	
	word alarm:	 	3;		// Alarm Code
	word limSwMin:	1;		// Minimum Software Limit reached
	word limSwMax:	1;		// Maximum Software Limit reached
	word axeStatus:	3;		// Stato dell'asse
}t_pid_axe_state;


extern byte drvAxeStatus[];				// Axe Status

extern t_pid_axe_state drvAxeRegState[];	// Axe Status Register

extern t_pid_axe_state drvAxeRegState[];	// Axe Status Register


void DriveInit();

void DriveServer();

#endif /* DRIVE_H_ */
