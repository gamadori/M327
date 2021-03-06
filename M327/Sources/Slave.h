/*
 * Slave.h
 *
 *  Created on: Dec 11, 2016
 *      Author: gianluca
 */

#ifndef SLAVE_H_
#define SLAVE_H_

#define NUM_SLAVES			1
#define SLAVES_NUM_AXIS		1
#define MAC_NUM_AXIS		NUM_AXES + SLAVES_NUM_AXIS

#include "Drive.h"

//Invio Posizone Assi
extern long bus_AxisPos[];
extern long bus_AxisVel[];
extern long bus_AxisAcc[];

extern long destPos[];
extern char ChkTimeOut[];

extern long slvPosReal[];
extern long slvPosVirt[];

extern word bus_AxisStatusWord[];
extern word bus_AxisControlWord[];
extern byte bus_AxisOperation[];
extern byte bus_AxisModeOperDisplay[];
extern word bus_AxisProfile[];


extern word bus_IO_Input[];
extern word bus_IO_InputForz[];
extern word bus_IO_InputForzVal[];
extern word bus_IO_InputVal[];

extern word bus_IO_Output[];
extern word bus_IO_OutputForz[];
extern word bus_IO_OutputForzVal[];
extern word bus_IO_OutputVal[];

extern byte slvIDBoards[];

void SlaveInitIDBoards();

#define CmdStop		1
#define CmdJog		2
#define CmdMove		3
#define CmdHome		4


enum ModesOperation
{
	moJogModeNeg = -2,
	moJogModePos = -1,
	moReserved1 = 0,
	moProfilePositionMode = 1,
	moVelocityMode = 2,
	moProfileVelocityMode = 3,
	moTorqueProfileMode = 4,
	moReserved2 = 5,
	moHomingMode = 6,
	moInterpolatedPositionMode = 7,
	moResetEncoder2 = 8,
	moHomingSearch = 9
};
	
#define CtrlWord_HOME	BIT4
#define CtrlWord_MOVE	BIT4
#define CtrlWord_STOP	BIT8

#define CtrlWord_DISABLE_AXIS	BIT2 + BIT1 + BIT0
#define CtrlWord_ENABLE_AXIS	BIT3 + BIT2 + BIT1 + BIT0

#define StatusWord_OperEnb		BIT2
#define StatusWord_ACK			BIT12
#define StatusWord_InPos		BIT10
#define StatusWord_Error		BIT13

#define SlvBitAxeInPos			BIT0
#define SlvBitAxeHomed			BIT1
#define SlvBitAxeStopped		BIT2

#define SlvAxeInPos(axe, state)		(state) ? (slvStatusRegister[axe] |= SlvBitAxeInPos) : (slvStatusRegister[axe] &= ~SlvBitAxeInPos)

#define SlvAxeHomed(axe, state)		(state) ? (slvStatusRegister[axe] |= SlvBitAxeHomed) : (slvStatusRegister[axe] &= ~SlvBitAxeHomed)

#define SlvAxeStopped(axe, state)	(state) ? (slvStatusRegister[axe] |= SlvBitAxeStopped) : (slvStatusRegister[axe] &= ~SlvBitAxeStopped)


#define SlvIsAxeInPos(axe)			((slvStatusRegister[axe] & SlvBitAxeInPos) == SlvBitAxeInPos)

#define SlvIsAxeHomed(axe)			((slvStatusRegister[axe] & SlvBitAxeHomed) == SlvBitAxeHomed)

#define SlvIsAxeStopped(axe)		((slvStatusRegister[axe] & SlvBitAxeStopped) == SlvBitAxeStopped)


void SlvServer();

void SlvInitIDBoards();

void SlvInitCommands();

void SlvPositionsServer();

void SlvJogServer();

void SlvCmdPos(short asse);

void SlvJogNeg(short asse);

void SlvCmdJogPos(short asse);

void SlvCmdJogNeg(short asse);

void SlvPosAsse(byte asse, long vel, long pos, short acc, byte profile);

void SlvJogAsse(short asse, long vel, short acc);

void SlvJogPos(short asse);

void SlvStopAsse(short asse, short acc);

void SlvStopAllAxes();

bool SlvHomeAsse(short asse);
void SET_MODE_OPERATION(int asse, byte valore);

word CHECK_MODE_OPERATION(int asse, int valore);

void SET_ControlWordBit(int asse, int bit);

void RESET_ControlWordBit(int asse, int bit);

void RESET_StatusWordBit(int asse, int bit);

bool CHECK_StatusWordBit(int asse, int bit);

bool SlvAxisHomed();

#endif /* SLAVE_H_ */
