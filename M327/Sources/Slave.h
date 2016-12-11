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
extern word bus_AxisOperation[];
extern word bus_AxisModeOperDisplay[];
extern word bus_AxisProfile[];
extern t_pid_axe_state bus_AxisStatusWordAlarm[];

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

#endif /* SLAVE_H_ */
