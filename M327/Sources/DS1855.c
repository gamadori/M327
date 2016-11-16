/*
 * DS1855.c
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */



#include "cpu.h"
#include "utility.h"
#include "ds1855.h"
#include "SDA.h"
#include "SCL.h"
#include "I2C.h"

#define ds1855_TIME_OUT 10000



short ds1855_cmd = 0;

byte livel_current[] = 
{
	0x00,
	0x00
};

/*
 * Inizializza lo stato dei Pin dei Processore
 */
void ds1855Initial(void)
{
	SDA_SetDir(1);
	SCL_SetVal();
	SDA_SetVal();


}


/*
 * Lettura di un dato dal dispositivo
 */
byte ds1855Read(byte device, byte address, byte *data, byte count)
{
	return I2CRead(ds1855DeviceAddress(device), address, data, count);

}

/*
 * Scrittura di un dato nel dispositivo
 */
byte ds1855Write(byte device, byte address, byte data)
{
	return I2CWrite(ds1855DeviceAddress(device), address, data);
}

void ds1855Cmd()
{
	ds1855_cmd = 1;
}

/*==================================================
 *
 * Funzione Server per la regolazione della corrente
 *
 *==================================================*/
void ds1855Server(void)
{
	
}

/****************************************************************

/*==========================================
 *
 * Invio dell'identificativo del dispositivo
 *
 *==========================================*/
byte ds1855DeviceAddress(byte address)
{
	return (0xA0 | (address << 1));
		
}


