/*
 * BMAlarms.h
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#ifndef BMALARMS_H_
#define BMALARMS_H_

typedef struct
{	
	
	dword HomedAxeX:		1;
	dword HomedProtection:	1;
	dword HomedWidth:		1;	
	dword AlarmAxeX:		4;
	dword AlarmProtection:	4;
	dword AlarmWidth:		4;
	dword AlarmCanBoard1:	4;
	dword AlarmCanBoard2:	4;
	dword AxeXLimSwPos:		1;
	dword AxeXLimSwNeg:		1;
	dword ProtLimSwPos:		1;
	dword ProtLimSwNeg:		1;
	
		
}MacAlarms;

extern MacAlarms Alarms;
extern char MAC_HOMED;


void InitStatoMacchina();

void LeggiStatoMacchina();


#endif /* BMALARMS_H_ */
