/*
 * Alfacol.h
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */

#ifndef ALFACOL_H_
#define ALFACOL_H_

#define ALFACOL_START			'%'

#define BROADCAST_START			'^'


#define ALFACOL_END				13

#define ALFACOL_LEN_STATION		2

#define ALFACOL_LEN_FRAME 		4

#define ALFACOL_POS_COMM_CHAR	3


#define ALFACOL_SEPARATOR		';'


#define ALFACOL_DEF_STATION		0x00

#define ALFACOL_SLV_STATION		0x02

#define STATION_BROADCAST	0x00

typedef struct 
{
	void *pAddress;				// pointer to the memory area of the variable
	short type;					// Variable data type
	short size;					// Variable Length
	bool readOnly;				// Read Only variable
	bool eeprom;				// Save to EEprom
	word CMD;					// Frame's Value in in hexadecimal
	
}PtlRoadMap;

struct AlfaCmd
{
	char *str;	
	byte id;
};

enum eCodeAlfaCmd
{
	eAlfacolCmdNull,
	eAlfacolCmdRead,
	eAlfacolCmdWrite,
	eAlfacolCmdRdFlash,
	eAlfacolCmdWrFlash,
	eAlfacolCmdPlot,
	eAlfacolCmdEvent,
	eAlfacolCmdCmd,
	eAlfacolCmdTest
};


enum eAlfaCmdType
{
	eAlfaCommand = '#',
	eAlfaRead = '?',
	eAlfaWrite = '=',
	eAlfaReply = '$',
	eAlfaError = '!',
	eAlfaComNoReply = '£'
};

enum TypeVariable
{
	Byte,
	Short,
	Long,
	Double,
	String,
	Ack, 
	BroadCast
};

typedef bool (*p_fver)(void *);
typedef bool (*p_fpar)(byte, char *, long *);
typedef bool (*p_fpar_f)(byte);
typedef bool (*p_fcmd)(char *);

extern word frame_val[];

void AlfacolInitVar();

void AlfacolInit(byte serial);

void AlfacolSetStation();

void AlfacolSaveInEeprom();

void AlfacolGetFromEeprom();

byte AlfacolDecode(byte c, byte serial);

void AlfacolComposeCommand(byte serial, byte type, byte station, char *frame, long value);

byte AlfacolSendBuffer(short canal, short serial);

void AlfacolSendResponse(byte serial, dword resp,  bool error, short size);

#endif /* ALFACOL_H_ */
