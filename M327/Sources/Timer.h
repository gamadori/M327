/*
 * Timer.h
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#ifndef TIMER_H_
#define TIMER_H_



#define MIN(x)		(dword)(x * (60000))

#define SEC(x)		(dword)(x * (1000))

#define DSEC(x)		(dword)(x * (100))

#define MSEC(x)		(dword)(x)

#define TMR_CAN_CYCLE		tmrs[0]

#define TMR_CAN_SDO			tmrs[1]

#define TMR_PLC				tmrs[2]

#define tmrTuning			tmrs[3]

#define tmrTuningOut		tmrs[4]

#define tmrTuningFrq		tmrs[5]

#define TMR_CURRENT			tmrs[6]

#define TMR_FAULT			tmrs[7]

#define tmrSDO				tmrs[8]

#define tmrTestPDO			tmrs[9]

#define TmrHome				tmrs[10]

#define TmrPwr				tmrs[11]

#define TmrPidCurr(axe)		tmrs[12 + axe]


extern dword tmrs[];

extern dword timerSys;

extern dword tmrsCan[];

extern dword tmrsPlc[];

extern dword tmrsCycle[];

void TimersInit();
		
void TimerServer();

#endif /* TIMER_H_ */
