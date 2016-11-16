/*
 * PID.h
 *
 *  Created on: Feb 15, 2016
 *      Author: info
 */

#ifndef PID_H_
#define PID_H_

#include "gflib.h"





extern GFLIB_CTRL_PID_P_AW_T_A32 pidParam[];	// PID Parameters

extern frac16_t f16Result[];		// PID Output value

extern frac16_t f16CurrentInErr[];

extern frac16_t f16CurrentResult[];


extern long pidErrPos[];

extern GFLIB_CTRL_PI_P_AW_T_A32 pidCurrentParam[];



extern word thresholdCurrMaxSup[];

extern word thresholdCurrMaxInf[];

extern short pidCurrTimeOut[];

void PidInit();

void PidReset(byte axe);

void PidServer(byte axe);

#endif /* PID_H_ */
