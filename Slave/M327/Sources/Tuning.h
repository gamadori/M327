/*
 * Tuning.h
 *
 *  Created on: Jul 23, 2016
 *      Author: gianluca
 */

#ifndef TUNING_H_
#define TUNING_H_

extern short tngTime;

extern short tngFrq;

extern short tngVar;

void TngInit();

void TngPush();

word TngPop(short *value);

void TngServer();

void TngEnd();



#endif /* TUNING_H_ */
