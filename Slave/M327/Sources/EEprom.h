/*
 * Eeprom.h
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */
#ifndef EEPROM_H_
#define EEPROM_H_


enum eFlashCmds
{
	eFlashNull,
	eFlashRead,
	eFlashWrite,
	eFlashBackRead,
	eFlashBackWrite
};

extern short flashCmdVar;

void EEpromInit();

void FlashVariableServer();

byte EepromGetByte(byte *data);

byte EepromGetWord(word *data);

byte EepromGetLong(dword *data);

byte EepromSetByte(byte data);

byte EepromSetWord(word data);

byte EepromSetLong(dword data);

#endif /* EEPROM_H_ */
