/*
 * bldcUser.h
 *
 *  Created on: Feb 8, 2016
 *      Author: gianluca
 */

#ifndef BLDCUSER_H_
#define BLDCUSER_H_

void BldcInit(void);

void ISRQTimer1();

void BldcPWMOut(byte axe, bool status);

void BldcUpdateSpeed(byte axe, long speed);

void BldcTimer();

void BldcGetHall();

void BldcSetStatus(byte axe, bool status);

extern byte bldcSensoresState[];

#endif /* BLDCUSER_H_ */
