/*
 * DS185.h
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */

#ifndef DS185_H_
#define DS185_H_

#define FAULT_LIMIT_1	0x1A // 0x10 // 3 Ampere	///0x2F

#define FAULT_LIMIT_2	0xc5

void ds1855Initial(void);

byte ds1855Read(byte device, byte address, byte *data, byte count);

byte ds1855Write(byte device, byte address, byte data);

byte ds1855DeviceAddress(byte address);

void ds1855Server(void);

void ds1855Cmd();



enum LivelsCurrent
{
	currNominal,
	currAlarm,
	currBooster
};

#define NAMXA_LIVEL_CURRENT		3
/* END ds1855 */

#endif /* DS185_H_ */
