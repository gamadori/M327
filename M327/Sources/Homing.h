/*
 * Homing.h
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */

#ifndef HOMING_H_
#define HOMING_H_

extern short hmStep[];

extern short hmDir[];

extern long hmOffset[]; 

extern long hmSrvErrBatt[];

extern long hmMaxSrvErrBatt[];

extern long hmMaxErrBatt[];	

extern long hmDistBatt[];

extern long hmPosAfterHome[];

extern long hmSpeedH[];

extern long hmSpeedL[];

extern short hmAcc[];

extern byte hmMethod[];

bool IsInHoming(byte axe);

void HomeInit(byte axe);

void HomeStart(byte axe);

void HomeServer(byte axe);

void HomeSearchMinServer(byte axe, short dir);

void HomeSearchMinStart(byte axe);
		
#endif /* HOMING_H_ */
