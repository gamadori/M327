/*
 * Slave.c
 *
 *  Created on: Dec 11, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Bits.h"
#include "Drive.h"
#include "Slave.h"
#include "Timer.h"

long slvPosReal[SLAVES_NUM_AXIS];	// Posizione Reale degli assi
long slvPosVirt[SLAVES_NUM_AXIS];	// Posizione Virtuale degli assi

//Invio Posizione Assi
long bus_AxisPos[SLAVES_NUM_AXIS];
long bus_AxisVel[SLAVES_NUM_AXIS];
long bus_AxisAcc[SLAVES_NUM_AXIS];

long destPos[SLAVES_NUM_AXIS];
char ChkTimeOut[SLAVES_NUM_AXIS];

//Status assi
word bus_AxisStatusWord[SLAVES_NUM_AXIS];
word bus_AxisControlWord[SLAVES_NUM_AXIS];
byte bus_AxisOperation[SLAVES_NUM_AXIS];
byte bus_AxisModeOperDisplay[SLAVES_NUM_AXIS];
word bus_AxisProfile[SLAVES_NUM_AXIS];



//Gestione Schede Input/output CAN-OPEN
word bus_IO_Input[NUM_SLAVES];
word bus_IO_InputForz[NUM_SLAVES];
word bus_IO_InputForzVal[NUM_SLAVES];
word bus_IO_InputVal[NUM_SLAVES];

word bus_IO_Output[NUM_SLAVES]; 
word bus_IO_OutputForz[NUM_SLAVES];
word bus_IO_OutputForzVal[NUM_SLAVES];
word bus_IO_OutputVal[NUM_SLAVES];

//TABELLA Identificazione Schede sul CAN-OPEN
byte slvIDBoards[NUM_SLAVES];// = { 1, 2};

// Slaves Command
byte slvCmdPos[SLAVES_NUM_AXIS];
byte slvStepCmdPos[SLAVES_NUM_AXIS];
byte slvCmdJog[SLAVES_NUM_AXIS];

dword slvStatusRegister[SLAVES_NUM_AXIS];

void SlvInitIDBoards()
{
	short i;
	
	for (i = 0; i < NUM_SLAVES; ++i)
	{
		slvIDBoards[i] = i + 1;
	}
}

void SlvInitCommands()
{
	short i;
	
	for (i = 0; i < SLAVES_NUM_AXIS; ++i)
	{
		slvCmdPos[i] = 0;		
		slvStepCmdPos[i] = 0;
		slvCmdJog[i] = 0;
	}
}

/*======================
 *
 *	Ciclo principale 
 *
 *======================*/
void SlvServer()
{    
	char asse;
		
	SlvPositionsServer();
	
	SlvJogServer();
	
	for (asse = 0;asse < SLAVES_NUM_AXIS; asse++)
	{
		//Verifica arrivo in posizione o fine operazione homing
		if (CHECK_StatusWordBit(asse, StatusWord_ACK))
		{
			
			if (CHECK_MODE_OPERATION(asse, moHomingMode) && !SlvAxisHomed())
			{
				RESET_ControlWordBit(asse, CtrlWord_HOME);
					
				SlvAxeHomed(asse, TRUE);
			}	
		}
		else if (CHECK_StatusWordBit(asse, StatusWord_Error))
		{
			if (CHECK_MODE_OPERATION(asse, moHomingMode))
			{		
				RESET_ControlWordBit(asse, CtrlWord_HOME);					
				SlvAxeHomed(asse, FALSE);
			}
		}

		if (CHECK_StatusWordBit(asse, StatusWord_InPos))
		{
						
			if (CHECK_MODE_OPERATION(asse, moProfilePositionMode) && 
				!SlvIsAxeInPos(asse) &&
				(slvPosVirt[asse] == bus_AxisPos[asse]))
			{
				SlvAxeInPos(asse, TRUE);

	 		}			
		}				
	}
}


void SlvPositionsServer()
{
	short asse;
	
	for (asse = 0; asse < SLAVES_NUM_AXIS; ++asse)
	{
		if (slvCmdPos[asse])
		{
			slvStepCmdPos[asse] = 1;
			slvCmdPos[asse] = 0;
		}
		
		switch (slvStepCmdPos[asse])
		{
			
			case 1://Set command
				SET_MODE_OPERATION(asse, moProfilePositionMode);
				RESET_ControlWordBit(asse, CtrlWord_STOP);
				RESET_ControlWordBit(asse, CtrlWord_MOVE);
				
				tmrTestPDO = MSEC(5);
				
				slvStepCmdPos[asse]++;
				
				break;
				
			case 2://Command received
				if (!tmrTestPDO && CHECK_MODE_OPERATION(asse, moProfilePositionMode) && !CHECK_StatusWordBit(asse, StatusWord_ACK))
				{
					SET_ControlWordBit(asse, CtrlWord_MOVE);
					slvStepCmdPos[asse]++;
				}
				break;
				
			case 3://Command Completed
				if (CHECK_StatusWordBit(asse, StatusWord_ACK))
				{
					
					RESET_ControlWordBit(asse, CtrlWord_MOVE);
					slvStepCmdPos[asse] = 0;
				}
				break;							
		}	
	}
}

void SlvJogServer()
{
	short asse;
	
	
	for (asse = 0; asse < SLAVES_NUM_AXIS; ++asse)
	{
		switch (slvCmdJog[asse])
		{
			case 1:
				// Attendo scrittuta velocità accelerazione sul bus
				slvCmdJog[asse]++;
				break;
				
			case 2:
				SET_MODE_OPERATION(asse, moJogModePos);
				RESET_ControlWordBit(asse, CtrlWord_STOP);
				slvCmdJog[asse] = 0;
				
			break;
			
			case 3:
				// Attendo scrittuta velocità accelerazione sul bus
				slvCmdJog[asse]++;
				break;
				
			case 4:
				SET_MODE_OPERATION(asse, moJogModeNeg);
				RESET_ControlWordBit(asse, CtrlWord_STOP);
				slvCmdJog[asse] = 0;
				break;
		}
	}
}


void SlvCmdPos(short asse)
{
	
	SlvAxeInPos(asse, FALSE);
	
	slvCmdPos[asse] = 1;
	
}

void SlvCmdJogPos(short asse)
{
	slvCmdJog[asse] = 1;
}

void SlvCmdJogNeg(short asse)
{
	slvCmdJog[asse] = 3;
}

/*
 * Muove un Asse in una data posizione 
 */
void SlvPosAsse(byte asse, long vel, long pos, short acc, byte profile)
{
	bus_AxisPos[asse] = pos;
	bus_AxisVel[asse] = vel;
	bus_AxisAcc[asse] = acc;
	bus_AxisProfile[asse] = profile;
	
	SlvCmdPos(asse);
	
}           

void SlvJogAsse(short asse, long vel, short acc)
{

	bus_AxisAcc[asse] = acc;
	
	if(vel>0)
		SlvJogPos(asse);
	else
	{
		vel = -vel;
		SlvJogNeg(asse);
	}
	
	bus_AxisVel[asse] = vel;

}


void SlvJogPos(short asse)
{
		
	SET_MODE_OPERATION(asse, moJogModePos);
	RESET_ControlWordBit(asse, CtrlWord_STOP);
	
	SlvCmdJogPos(asse);
}

void SlvJogNeg(short asse)
{
	SET_MODE_OPERATION(asse, moJogModeNeg);
	RESET_ControlWordBit(asse, CtrlWord_STOP);
	
	SlvCmdJogNeg(asse);
}



void SlvStopAsse(short asse, short acc)
{
	bus_AxisVel[asse] = 0;
	bus_AxisAcc[asse] = acc;
	RESET_ControlWordBit(asse, CtrlWord_MOVE);
	RESET_ControlWordBit(asse, CtrlWord_HOME);
	
	RESET_StatusWordBit(asse, StatusWord_ACK);
	
	SET_ControlWordBit(asse, CtrlWord_STOP);
}

void SlvStopAllAxes()
{
	short i;
	
	for (i = 0; i < SLAVES_NUM_AXIS; ++i)
	{
		SlvStopAsse(i, 0);
	}
}
/*
 * Azzeramento dell'asse
 */
bool SlvHomeAsse(short asse)
{
	if (asse < SLAVES_NUM_AXIS)
	{
		
		if (CHECK_StatusWordBit(asse, StatusWord_ACK))
		{
			RESET_ControlWordBit(asse,CtrlWord_HOME);
		}
		
		SlvAxeHomed(asse, FALSE);
		
		SET_MODE_OPERATION(asse, moHomingMode);
		RESET_ControlWordBit(asse, CtrlWord_STOP);
		SET_ControlWordBit(asse, CtrlWord_HOME);
		
		return TRUE;
	}
	else
		return FALSE;
}

//------------------------------------------
void SET_MODE_OPERATION(int asse, byte valore)
{
	if (asse < SLAVES_NUM_AXIS)
		bus_AxisOperation[asse] = (byte)valore;
}

word CHECK_MODE_OPERATION(int asse, int valore)
{
	return (bus_AxisModeOperDisplay[asse] == valore);
}

//------------------------------------------

void SET_ControlWordBit(int asse, int bit)
{
	bus_AxisControlWord[asse] |= bit; 	
}


void RESET_ControlWordBit(int asse, int bit)
{
	bus_AxisControlWord[asse] &= ~bit; 	
	
}

//------------------------------------------

void RESET_StatusWordBit(int asse, int bit)
{
	bus_AxisStatusWord[asse] &= ~bit; 	
	
}

bool CHECK_StatusWordBit(int asse, int bit)
{
	return (bus_AxisStatusWord[asse] & bit) == bit; 	
	
}

bool SlvAxisHomed()
{
	short i;
	
	for (i = 0; i < SLAVES_NUM_AXIS; ++i)
	{
		if (!SlvIsAxeHomed(i))
			return FALSE;
	}
	return TRUE;
}


