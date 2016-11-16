/*
 * I2C.c
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */


#include "cpu.h"
#include "utility.h"
#include "SDA.h"
#include "SCL.h"
#include "Bits.h"
#include "I2C.h"

#define I2C_TIME_OUT 10000

void I2CStart();

void I2CStop();

void I2CWrite8(byte value);

byte I2CRead8();

void I2CDeviceAddress(byte address, byte wr);

byte I2CWaitAck();

void I2CSendAck();

short I2C_cmd = 0;



/*
 * Inizializza lo stato dei Pin dei Processore
 */
void I2CInitial(void)
{
	SDA_SetDir(1);
	SCL_SetVal();
	SDA_SetVal();


}


void I2CCmd()
{
	I2C_cmd = 1;
}



/****************************************************************
 * Lettura di un registro del I2C
 *
 *	Inputs:
 *		byte device:	indirizzo del dispositivo
 *		byte address:	indirizzo del registro
 *		byte *data:		buffer del dato da leggere
 *		byte count:		numero di letture
 *
 *	Return
 *		1: sel lettura e' terminata correttamente
 *		0: false
 *****************************************************************/
byte I2CRead(byte device, byte address, byte *data, byte count)
{

	I2CStart();
	I2CDeviceAddress(device, 0);
	
	
	if (!I2CWaitAck())
		return 0;
	
	I2CWrite8(address);
	
	if (!I2CWaitAck())
		return 0;
	
	I2CStart();
	
	I2CDeviceAddress(device, 1);
	
	if (!I2CWaitAck())
		return 0;
	
	while (count-- > 0)
	{
		*data++ = I2CRead8();
		I2CSendAck();
		
	}
	
	I2CStop();
	
	return 1;
}


byte I2CWrite(byte device, byte address, byte data)
{
	I2CStart();
	I2CDeviceAddress(device, 0);
	
	if (!I2CWaitAck())
		return 0;
	
	I2CWrite8(address);
	
	if (!I2CWaitAck())
		return 0;
	
	I2CWrite8(data);
	
	if (!I2CWaitAck())
		return 0;
	
	I2CStop();
	
	return 1;
	
}

/*====================================
 * Comando di START:
 *	SDA va basso quando il SCL e' alto
 *
 *====================================*/
void I2CStart()
{
	SDA_SetDir(1);
	TmrDelay(60000);
	
	// Alzo sia il clock che il dato
	SDA_SetVal();
	SCL_SetVal();
	
	TmrDelay(50000);
	
	// Abbasso SDA
	SDA_ClrVal();
	TmrDelay(5000);
	
	SCL_ClrVal();
	
	TmrDelay(5000);
	
}


/*====================================
 * Comando di STOP
 *	SDA va alto quando il SCL e' alto
 *====================================*/
void I2CStop()
{
	// Mi assicuro che i SCL e SDA siano bassi
	SCL_ClrVal();
	SDA_ClrVal();
	
	TmrDelay(10000);
	
	SCL_SetVal();
	
	TmrDelay(1000);
	
	SDA_SetVal();
	
	TmrDelay(1000);

}


/*=====================================
 *
 * Scrittura di un byte al dispositivo
 *
 *=====================================*/
void I2CWrite8(byte value)
{
		byte idx;
	
	SCL_ClrVal();	// Dovrebbe gia' essere bassa
	
	for (idx = 0; idx < 8; ++idx)
	{
		TmrDelay(10000);
		
		if (value & Bits[7 - idx])
			SDA_SetVal();
		else
			SDA_ClrVal();
		
		TmrDelay(10000);
		
		SCL_SetVal();
		
		TmrDelay(20000);
		
		SCL_ClrVal();
	}

	
}



/*===================================
 *
 * Lettura di un byte dal dispositivo
 *
 *===================================*/
byte I2CRead8()
{
	byte idx, value = 0;
	
	SCL_ClrVal();
	SDA_ClrVal();
	SDA_SetDir(0);
	TmrDelay(2000);
	
	for (idx = 0; idx < 8; ++idx)
	{
		SCL_SetVal();
		
		TmrDelay(2000);
		
		value &= ~Bits[7 - idx];
		
		TmrDelay(2000);
		
		
		TmrDelay(20000);
		
		if (SDA_GetVal())
			value |= Bits[7 - idx];
		
		TmrDelay(2000);
		
	//	SDA_SetDir(1);
		
		SCL_ClrVal();
		
		TmrDelay(20000);
		

	}
	SDA_ClrVal();
	SDA_SetDir(1);
	
	return value;
}

/*==========================================
 *
 * Invio dell'identificativo del dispositivo
 *
 *==========================================*/
void I2CDeviceAddress(byte address, byte wr)
{
	 address = (byte) (address | wr);
	
	I2CWrite8(address);
		
}


/*================
 * Attesa dell'ack
 *
 *================*/
byte I2CWaitAck()
{
	byte tmp;
	short conta = 0;
	
	
	SCL_ClrVal();
	
	TmrDelay(5000);
	
	SDA_SetDir(0);
	
	TmrDelay(5000);
	
	
	SCL_SetVal();
	
	TmrDelay(25000);
	
	
	while ((tmp = SDA_GetVal()) && conta++ < I2C_TIME_OUT)
	;
	

	
	// Attendo stabilizzazione SCL

	
	TmrDelay(2000);
	
	// Abbasso SCL
	SCL_ClrVal();
	
	TmrDelay(2000);
	
	SDA_ClrVal();
	SDA_SetDir(1);
	
	TmrDelay(2000);
	
	return !tmp;
}


/*===============
 *
 * Invio dell'ack
 *
 *==============*/
void I2CSendAck()
{
	// Abbasso il segnale SDA
	SDA_ClrVal();
	
	TmrDelay(2000);
	
	// Alzo il SCL
	SCL_SetVal();

	TmrDelay(1000);
	
	// abbasso SCL
	SCL_ClrVal();
	
	TmrDelay(2000);
	
	SDA_ClrVal();



}
