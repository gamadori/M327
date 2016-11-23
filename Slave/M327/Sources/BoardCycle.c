/*
 * Settings.c
 *
 *  Created on: Nov 17, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "CAN1.h"

#include "plc.h"
#include "Can.h"
#include "CanOpen.h"
#include "Serial.h"
#include "BoardCycle.h"


byte boardStation;

short boardState;

void BoardSettings();

void BoardRun();

void BoardInit()
{
	boardState = boardInit;
}

void BoardServer()
{
	switch (boardState)
	{
	case boardInit:
		if (ioWorked)
			boardState = boardSettings;
		break;
		
	case boardSettings:
		BoardSettings();
		boardState = boardRun;
		break;
		
	case boardRun:
		BoardRun();
		break;
	}
	
}

/*
 * Board Settings
 */
void BoardSettings()
{
	if (boardStation == 0)
		boardStation = (inpDipSwitchStation);
	
	SerialSetIDBoard(SCI0, boardStation);
	CanOpenSetID(boardStation);	
	CanInit();
	CAN1_Enable();
}

/*
 * Board in Run Status: execute all function of main cycle
 */
void BoardRun()
{
	CanServerTrasmit();
	SerialReceve();
	CANOPENServer();
}


