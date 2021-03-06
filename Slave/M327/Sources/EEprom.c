/*
 * EEprom.c
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "IFsh1.h"
#include "Alfacol.h"
#include "Eeprom.h"


word eepromIndex;


short flashCmdVar = eFlashNull;


void EEpromInit()
{
	eepromIndex = 0x8000;
	
}


/*====================================================================
 * Module:
 *		void FlashVariableServer()
 * Description:
 * 		Funzione server per la lettura e la scrittura delle variabili
 *		dalla Flash 
 * Inputs:
 *		none
 * Return:
 *		none
 *
 *=====================================================================*/

void FlashVariableServer()
{
	
	switch (flashCmdVar)
	{
		case eFlashRead:
			AlfacolGetFromEeprom();
			flashCmdVar = eFlashNull;
			break;
			
		case eFlashWrite:
			AlfacolSaveInEeprom();
			flashCmdVar = eFlashNull;
			break;				
						
			
	}	
}



/*
 ** @brief
 ** 	Gets a byte from the virtual page.
 */
byte EepromGetByte(byte *data)
{	
	byte Err;
	
	Err = IFsh1_GetByteFlash(eepromIndex << 1, data);
	eepromIndex++;
	
	return Err;
}

/*
 ** @brief
 ** 	Gets a word from the virtual page.
 */
byte EepromGetWord(word *data)
{

	return IFsh1_GetWordFlash(eepromIndex++, data);
	
}

/*
 ** @brief
 ** 	Gets a dword from the virtual page
 */
byte EepromGetLong(dword *data)
{
	
	byte Err = IFsh1_GetLongFlash(eepromIndex, data);
	eepromIndex += 2;
	
	return Err;
}

/*
 ** @brief
 ** 	Writes a byte in the virtual page.
 */
byte EepromSetByte(byte data)
{
	byte Err;
	Err = IFsh1_SetByteFlash(eepromIndex << 1, data);
	eepromIndex++;
			
	return Err;
}

/*
 ** @brief
 ** 	Writes word in the virtual page.
 */
byte EepromSetWord(word data)
{
	
	return IFsh1_SetWordFlash(eepromIndex++, data);
			
}


/*
 ** @brief
 ** 	Writes a dword in the virtual page.
 */
byte EepromSetLong(dword data)
{
	byte Err;
	
	Err = IFsh1_SetLongFlash(eepromIndex, data);
	eepromIndex += 2;
			
}
