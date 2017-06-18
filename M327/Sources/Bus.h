/*
 * Bus.h
 *
 *  Created on: Dec 25, 2016
 *      Author: info
 */

#ifndef BUS_H_
#define BUS_H_

extern short busDec[];

extern short busDec[];

void BusInit();

void BusServer();

void BusEmergency();

bool BusJog(byte axe, long speed, short acc);

bool BusMove(byte axe, long pos, long speed, short acc, bool tuning);

bool BusStop(byte axe, short acc);

bool BusHome(byte axe);

bool BusRSJog(char *param, short index);

bool BusRSMove(char *param, short index, bool tuning);

bool BusRSStop(char *param, short index);

bool BusRSHome(char *param, short index);

void BusStopAll();

void BusSearchMin(byte axe);

#endif /* BUS_H_ */
