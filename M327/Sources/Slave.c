/*
 * Slave.c
 *
 *  Created on: Dec 11, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Drive.h"
#include "Slave.h"


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
word bus_AxisOperation[SLAVES_NUM_AXIS];
word bus_AxisModeOperDisplay[SLAVES_NUM_AXIS];
word bus_AxisProfile[SLAVES_NUM_AXIS];
t_pid_axe_state bus_AxisStatusWordAlarm[SLAVES_NUM_AXIS];


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

void SlaveInitIDBoards()
{
	short i;
	
	for (i = 0; i < NUM_SLAVES; ++i)
	{
		slvIDBoards[i] = i + 1;
	}
}
