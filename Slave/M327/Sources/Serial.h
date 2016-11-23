/*
 * Serial.h
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#define NUM_SERIAL			3

#define SERIAL_BUFF_SIZE	64


enum SerialCanal
{	
	SCI0,
	SCI1,
	SCI2
};

enum SerialProtocol
{
	PTC_NULL,
	PTC_ALFACOL,	// Protocollo simil Nais
	PTC_ALFAHEX,	// Protocollo Alfamacchine
	PTC_BETTERCOL,	// Protocollo Better
	PTC_SCANNER,
	PTC_BOOTCOL
};



struct SerialCmd
{
	char *str;	
	byte id;
};

extern byte ptc_current[];

extern byte serial_buff[NUM_SERIAL][SERIAL_BUFF_SIZE];	// buffer della seriale

extern byte rs_trasm_buff[NUM_SERIAL][SERIAL_BUFF_SIZE];

extern bool serialBusy[];

extern byte serialBoardStation[NUM_SERIAL];

void SerialInit();

void SerialSetIDBoard(byte nSerial, byte idSerial);


void SerialPutRecvBuff(byte n_serial, byte c);

byte SerialGetRecvBuff(byte n_serial);

void SerialPutTrasmBuff(byte n_serial, byte c);

byte SerialGetTrasmBuff(byte n_serial);

byte SerialValueTrasmBuff(byte n_serial);

void SerialReceve();

void SerialCanRecv();

//void SerialTrasmit(void);

void SerialSetTrasmit(void);

void SerialTx(byte nserial);

void SerialComTx(byte nserial);

#endif /* SERIAL_H_ */
