/*
 * Alfacol.c
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */


#include <string.h>

#include "cpu.h"
#include "AS1.h"
#include "IFsh1.h"

#include "Bits.h"
#include "Utility.h"

#include "Version.h"
#include "Serial.h"

#include "Timer.h"
#include "Plc.h"

#include "Settings.h"

#include "plc.h"
#include "Alfacol.h"
#include "bldcUser.h"
#include "pid.h"
#include "EEprom.h"
#include "Encoder.h"
#include "Ramp.h"
#include "Drive.h"
#include "Tuning.h"
#include "Current.h"
#include "Sdo.h"
#include "Slave.h"
#include "Bus.h"
#include "Homing.h"
#include "CanOpen.h"
#include "Router.h"

#define ALFACOL_NMAX_PARAM	16

#define ALFACOL_NMAX_CMDS	16


typedef struct 
{
	void *pAddress;				// pointer to the memory area of the variable
	short type;					// Variable data type
	short size;					// Variable Length
	bool readOnly;				// Read Only variable
	bool eeprom;				// Save to EEprom
	word CMD;					// Frame's Value in in hexadecimal
	
}PtlRoadMap;


PtlRoadMap AlfaRoadMap[] =
{
	//	Address,							Type,	Size,				ReadOnly, EEprom, CMD
		{&rpos,								Long,	MAC_NUM_AXIS,		FALSE,	FALSE,	0x0001},
		{&vpos,								Long,	MAC_NUM_AXIS,		FALSE,	FALSE,	0x0002},
		{slvPosReal,						Long,	SLAVES_NUM_AXIS,	TRUE, 	FALSE, 	0x0003},		
		{slvPosVirt,						Long,	SLAVES_NUM_AXIS,	TRUE, 	FALSE, 	0x0004},
		
		{&flashCmdVar,						Short,	1,					FALSE,	FALSE,	0x0021}, 
		{bus_IO_Output,						Short,	NUM_SLAVES,			FALSE,	FALSE,	0x0022},
		{&outputBuffer,						Long,	1,					TRUE,	FALSE,	0x0024},
		{&outValue,							Long,	1,					TRUE,	FALSE,	0x0025},	
		{&outFrz,							Long,	1,					FALSE,	FALSE,	0x0026},
		{&outFrzValue,						Long,	1,					FALSE,	FALSE,	0x0027},
			
		{&inputBuffer,						Long,	1,					TRUE,	FALSE,	0x0028},
		{&inpValue,							Long,	1,					TRUE,	FALSE,	0x0029},
		{&inpFrz,							Long,	1,					FALSE,	FALSE,	0x002A},
		{&inpFrzValue,						Long,	1,					FALSE,	FALSE,	0x002B},		
		{bldcSensoresState,					Byte,	NUM_AXES,			TRUE,	FALSE,	0x002F},
		
		{drvAxeRegState,					Short,	MAC_NUM_AXIS,		TRUE,	FALSE,	0x0030},
		{drvAxeStatus,						Byte,	NUM_AXES,			TRUE,	FALSE,	0x0031},
		
		{ADCValues,							Short,	NUM_ANALOGIC,		FALSE,	FALSE,	0x003F},
		
		{&pidParam[0].a32PGain,				Long,	1,					FALSE,	TRUE,	0x0040},
		{&pidParam[0].a32IGain,				Long,	1,					FALSE,	TRUE,	0x0041},
		{&pidParam[0].a32DGain,				Long,	1,					FALSE,	TRUE,	0x0042},
		{&pidParam[0].f16UpperLim,			Short,	1,					FALSE,	TRUE,	0x0043},
		{&pidParam[0].f16LowerLim ,			Short,	1,					FALSE,	TRUE,	0x0044},
				
		{srvErrorStatic,					Long,	NUM_AXES,			FALSE,	TRUE,	0x0050},
		{srvErrorDynamic,					Long,	NUM_AXES,			FALSE,	TRUE,	0x0051},
		{srvErrorTimeOut,					Short,	NUM_AXES,			FALSE,	TRUE,	0x0052},
		{maxAcc,							Short,	NUM_AXES,			FALSE,	TRUE,	0x0055},
		
		{&currentNominal,					Short,	1,					FALSE,	TRUE,	0x0060},
		{&currentMax,						Short,	1,					FALSE,	TRUE,	0x0061},
		{&timeOutCurrent,					Short,	1,					FALSE,	TRUE,	0x0062},
		{&timerOutFaultDisable,				Short,	1,					FALSE,	TRUE,	0x0063},
		
		{&pidCurrentParam[0].a32PGain,		Long,	1,					FALSE,	TRUE,	0x0064},
		{&pidCurrentParam[0].a32IGain,		Long,	1,					FALSE,	TRUE,	0x0065},		
		{&pidCurrentParam[0].f16UpperLim,	Short,	1,					FALSE,	TRUE,	0x0066},
		{&pidCurrentParam[0].f16LowerLim,	Short,	1,					FALSE,	TRUE,	0x0067},
		{thresholdCurrMaxInf,				Short,	NUM_AXES,			FALSE,	TRUE,	0x0068},
		{thresholdCurrMaxSup,				Short,	NUM_AXES,			FALSE,	TRUE,	0x0069},
		{pidCurrTimeOut,					Short,	NUM_AXES,			FALSE,	TRUE,	0x006A},
		
		{reg,								Short,	3,					FALSE,	FALSE,	0x006E},
		{&faultOccors,						Byte,	1,					FALSE,	FALSE,	0x006F},
		
		{hmDir,								Short,	NUM_AXES,			FALSE,	TRUE,	0x0070},
		{hmOffset,							Long,	NUM_AXES,			FALSE,	TRUE,	0x0071},
		{hmSrvErrBatt,						Long,	NUM_AXES,			FALSE,	TRUE,	0x0072},
		{hmMaxSrvErrBatt,					Long,	NUM_AXES,			FALSE,	TRUE,	0x0073},
		{hmMaxErrBatt,						Long,	NUM_AXES,			FALSE,	TRUE,	0x0074},
		{hmDistBatt,						Long,	NUM_AXES,			FALSE,	TRUE,	0x0075},
		{hmPosAfterHome,					Long,	NUM_AXES,			FALSE,	TRUE,	0x0076},
		{hmSpeedH,							Long,	NUM_AXES,			FALSE,	TRUE,	0x0077},
		{hmSpeedL,							Long,	NUM_AXES,			FALSE,	TRUE,	0x0078},
		{hmAcc,								Short,	NUM_AXES,			FALSE,	TRUE,	0x0079},
		
		{busDec,							Short,	MAC_NUM_AXIS,		FALSE,	TRUE,	0x007F},
		
		{rvel,								Long,	NUM_AXES,			TRUE,	FALSE,	0x0100},
		{dvel,								Long,	NUM_AXES,			TRUE,	FALSE,	0x0101},
		{dacc,								Short,	NUM_AXES,			TRUE,	FALSE,	0x0102},
		{dpos,								Long,	NUM_AXES,			TRUE,	FALSE,	0x0103},
		{kvel,								Long,	NUM_AXES,			TRUE,	FALSE,	0x0104},
		{f16Result,							Short,	NUM_AXES,			TRUE,	FALSE,	0x0105},
		{f16CurrentResult,					Short,	NUM_AXES,			TRUE,	FALSE,	0x0106},
		{f16CurrentInErr,					Short,	NUM_AXES,			TRUE,	FALSE,	0x0107},
		{&debugMachine,						Short,	1,					FALSE,	FALSE,	0x0108},
		
		
		{&tngTime,							Short,	1,					FALSE,	TRUE,	0x0110},		
		{&tngFrq,							Short,	1,					FALSE,	TRUE,	0x0111},
		
		{NULL,								0,		0,					0,		0,		0}	
};

byte boardStation = 0x00;		// Serial Station Number


byte cmd_type[NUM_SERIAL];		// Data Type of the received command

byte dec_state[NUM_SERIAL];		// State of decoding of the received command

byte numStation[NUM_SERIAL];	// Station Number of the received command

word frame_val[NUM_SERIAL];		// Frame's Index of the received command

byte buffFrame[NUM_SERIAL][ALFACOL_LEN_FRAME];	// Frame Buffer of the received command

byte text_data_pos[NUM_SERIAL];		// Position of the Data Type field on the received command

byte frame_idx[NUM_SERIAL];		// Frame's Number of bytes received
	


byte AlfacolCheckBCD(byte *buff, byte pos);

byte GetSizeVariable(short idx);

byte AlfacolCalculateBCD(byte * buff, byte pos);

byte AlfacolCalculateBCD2(byte *buff);

bool AlfacolExecute(byte serial);

bool AlfacolCmd(byte serial, byte *param, bool *repy);

bool AlfacolRDWR(byte serial, byte *param);

bool WriteCmd(short frame, char **p);

bool ReadCmd(short frame, char *p);

void AlfacolCreateResponse(byte serial, bool error);

bool ExistIndex(char *p);

void bootExit(void);

void AlfacolCanCommand(byte serial);

bool AlfacolRoute(byte serial);

char AlfacolGetTypeOp(byte cmd);

bool AlfacolSDO(byte serial);

bool AlfacolSDOVariable(byte serial, byte subIndex, long value);


/*
 * Description:
 * 		Initialise the module
 */
void AlfacolInit(byte serial)
{		
	
	dec_state[serial] = 0;	
}

void AlfacolInitVar()
{
	short i;
		
	for (i = 0; i < NUM_SERIAL; ++i)
		dec_state[i] = 0;
}

void AlfacolSetStation()
{
	if (boardStation == 0)
		boardStation = (inpDipSwitchStation);
	}
/*==============================================================
 * Module:
 *	void AlfacolSaveInEeprom()
 *
 * Description : 
 *	Saving variables in Eeprom
 *
 *
 * Inputs:
 *	none
 * Outputs:
 *	none
 *
 * Returns:
 *	none
 *
 *==============================================================*/
void AlfacolSaveInEeprom()
{
	short i;
	
	
	EEpromInit();
	for (i = 0; AlfaRoadMap[i].pAddress != NULL; ++i)
	{
		if (AlfaRoadMap[i].eeprom)
		{
			
			switch (AlfaRoadMap[i].type)
			{
			case Byte:
				EepromSetByteBuff((byte *)AlfaRoadMap[i].pAddress, AlfaRoadMap[i].size);
				break;
				
			case Short:
				EepromSetWordBuff((word *)AlfaRoadMap[i].pAddress, AlfaRoadMap[i].size);
				break;
					
			case Long:
				EepromSetLongBuff((dword *)AlfaRoadMap[i].pAddress, AlfaRoadMap[i].size);
				break;
			}			
		}	
		
		
	}
	
	
}

/*==============================================================
 * Module:
 *	void AlfacolGetFromEeprom()
 *
 * Description : 
 *	Gets the variable's value from the eeprom
 *
 *
 * Inputs:
 *	none
 * Outputs:
 *	none
 *
 * Returns:
 *	none
 *
 *=============================================================*/
void AlfacolGetFromEeprom()
{
	short i;
	
	EEpromInit();
	
	for (i = 0; AlfaRoadMap[i].pAddress != NULL; ++i)
	{
		if (AlfaRoadMap[i].eeprom)
		{
			switch (AlfaRoadMap[i].type)
			{
			case Byte:
				EepromGetByteBuff((byte *)AlfaRoadMap[i].pAddress, AlfaRoadMap[i].size);
				break;
							
			case Short:
				EepromGetWordBuff((word *)AlfaRoadMap[i].pAddress, AlfaRoadMap[i].size);
				break;
								
			case Long:
				EepromGetLongBuff((dword *)AlfaRoadMap[i].pAddress, AlfaRoadMap[i].size);
				break;
			}	
		}
	}	
}

/*======================================================
 ** 
 ** @Brief
 ** 	Decodes the received serial string
 ** 	
 ** @Return
 ** 	0: if there's format error
 ** 	1: if there isn't the terminator
 ** 	2: if the string is correct
 ** 	
 *=======================================================*/
byte AlfacolDecode(byte c, byte serial)
{
	short i;
	
	serial_buff[serial][dec_state[serial] + 1] = c;
	serial_buff[serial][dec_state[serial] + 2] = 0;
	
	
	switch (dec_state[serial])
	{

		
	case 0:
		// Gets Station Number
		if (IsHexNumber(c))
		{
			++dec_state[serial];
			numStation[serial] = (cVal(c) << sh_exp16[1]);
			
			return 1;	
		}
		else
			return 0;
		
		
	case 1:
		
		if (IsHexNumber(c))
		{
			++dec_state[serial];
			numStation[serial] += cVal(c) << sh_exp16[0];
			
			if ((numStation[serial] & 0xF0) == boardStation || serial == SCI1)
				return 1;			
			
		}
		
		return 0;
		
		
	case 2:
		// Gets Type of Command
		switch (c)
		{
		case eAlfaCommand:
		case eAlfaReply:
		case eAlfaError:
		case eAlfaComNoReply:
		case eAlfaRead:
		case eAlfaWrite:
		
			// Received a Command
			cmd_type[serial] = c;
			
			text_data_pos[serial] = 0;
			frame_val[serial] = 0;			
			
			frame_idx[serial] = 0;
			
			
			dec_state[serial]++;
			
			return 1;
						
		default:
			dec_state[serial] = 0;
			return 0;
		}
		
		
	default:
		
	
		if (!text_data_pos[serial])
		{
			dec_state[serial]++;
			
			// Gets the frame
			if (IsHexNumber(c) && frame_idx[serial] < ALFACOL_LEN_FRAME)	
			{
				buffFrame[serial][frame_idx[serial]++] = c;
				return 1;			
			}
			else if (frame_idx[serial] > 0)
			{
				frame_val[serial] = 0;
				
				// Decodes the received frame
				for (i = frame_idx[serial] - 1; i >= 0; --i)
				{
					frame_val[serial] += cVal(buffFrame[serial][i]) << sh_exp16[frame_idx[serial] - i - 1];
				}
				text_data_pos[serial] = (byte)(dec_state[serial] + 1);
				return 1;
			}
			else
				return 0;			
				
		}	
		else
		{			
			// Gets the Data
			if (c != ALFACOL_END)
			{
				dec_state[serial]++;
				return 1;
			}
			else
			{
				// Received the terminator
				// the two previous characters are the BCD
				 
				if (dec_state[serial] < 6 || !AlfacolCheckBCD(serial_buff[serial], dec_state[serial]))
					return 0;
			
				if (serial == SCI0)
				{					 
					if ((numStation[serial] & 0x0F) == ALFACOL_DEF_STATION)
					{
						// The destination of the received string is the master 						
						serial_buff[serial][dec_state[serial] - 1] = 0;
						return AlfacolExecute(serial);
					}	
					
					else
					{
						// The destination of the received string is the slave 
						//AlfacolSendBuffer(SCI1, SCI0);
						serial_buff[serial][dec_state[serial] -1] = 0;	
							
						AlfacolSDO(serial);							
						
					}									
				}
				else if (serial == SCI1)
				{
					// Dato proviene dalla slave
					if (numStation[serial] & 0xF0)
					{
						// Devo mandarlo all'esterno						
						AlfacolSendBuffer(SCI0, serial);
					}
					
					
				}	
			}			
		}
	}							
	
	return 0;		
}

/* 
 ** @Brief
 ** 	Sends a block of characters to the serial channel.
 ** @Parameters
 ** 	short canal:	serial channel number
 ** 	short serial:	serial channel index
 */
byte AlfacolSendBuffer(short canal, short serial)
{
	word snd;
	
	if (canal == SCI0)
		return AS1_SendBlock(serial_buff[serial], strlen((char *)serial_buff[serial]), &snd);
	else
	{
		//CanTrasmit(serial_buff[serial], strlen((char *)serial_buff[serial]));
		return TRUE;
	}
}

/*=======================================================================================
 ** @Brief
 ** 	Creates the serial string
 ** @Parameters
 ** 	byte serial:
 ** 		serial channel index
 ** 	byte type:
 ** 		Command's Type
 ** 	byte station:
 ** 		Station Number
 ** 	char *frame
 ** 		pointer to the frame string
 ** 	long value
 ** 		string serial value 
 *=======================================================================================*/
void AlfacolComposeCommand(byte serial, byte type, byte station, char *frame, long value)
{
	short i;
	short j = 0;
	byte bcc;
	byte buff[3];
	byte *p = serial_buff[serial];
	
	if (station != STATION_BROADCAST)
		*p++ = ALFACOL_START;
	else
		*p++ = BROADCAST_START;
	
	ByteToHex(station, p);
	p += 2;
	
	*p++ = type;
	
	for (i = 0;  frame[i] && i < ALFACOL_LEN_FRAME; ++i)
		*p++ = frame[i];
	
	*p++ = ';';
	*p++ = '0';
	*p++ = ';';
	
	p += Alfasprintfl((char *)p, value);	
	*p = 0;
	
	bcc = AlfacolCalculateBCD2(serial_buff[serial]);
	
	ByteToHex(bcc, buff);
	
	*p++ = buff[0];
	*p++ = buff[1];
	
	*p++ = ALFACOL_END;
	*p = 0;
	
}


/*===========================================================
 ** 
 ** @Brief
 ** 	Checks the BCD of the received string
 ** 	Alfacol uses BCC (Block Check Code) calculated using XOR
 ** 	of all characters, from '%' to last data character.
 ** 	Alternatively, you can use two asterisks '**'
 **  @Parameters
 **  	byte *buff: serial string buffer
 **  	byte pos: index of the starting character of the BCD
 *===========================================================*/
byte AlfacolCheckBCD(byte *buff, byte pos)
{
	byte t_bcd = 0;

	
	if (buff[pos - 1] == '*' && buff[pos] == '*')
		return 1;
	
		
	t_bcd = AlfacolCalculateBCD(buff, pos);
	
	if (t_bcd == ((cVal(buff[pos - 1]) << sh_exp16[1]) + (cVal(buff[pos]) << sh_exp16[0])))
		return 1;
	else
		return 0;
	
}

/*********************************************************
 **
 ** @Brief
 ** 	Calculation BCC of the serial string 
 ** 
 ** @Parameters:
 ** 	byte *buff: buffer of the received serial string
 ** 	byte pos: positions of the BCC characters in the string
 ** 	
 ** @Return:
 **		The BCC value
 **
 **********************************************************/
byte AlfacolCalculateBCD(byte * buff, byte pos)
{
	short i;
	byte c;
	byte t_bcd = 0;
	
	for (i = 0; i <= pos - 2; ++i)
	{
		c = buff[i];
		t_bcd ^= c;
	}
	
	return t_bcd;
}

byte AlfacolCalculateBCD2(byte *buff)
{
	byte t_bcd = 0;
	
	while (*buff)
	{
		t_bcd ^= *buff++;
	}
	
	return t_bcd;
}

void bootExit(void)
{
//	WDog1_Enable();
}

/*=============================================
 ** @Brief 
 ** 	Execute the serial received command
 ** @Parameters:
 ** 	byte serial: serial channel index
 ** 	byte *param: function parameters of the 
 ** 		serial command  	 
 *=============================================*/
bool AlfacolCmd(byte serial, byte *param, bool *reply)
{
	short index;
	
	Alfasscanfs((char**)&param, (short*)&index);
	
	
	
	switch (frame_val[serial])
	{
		
			
		case 0x0003:
			// Stop Command
			return BusRSStop((char *)param, index);
			
			
		case 0x0004:
			// Jog Command
			return BusRSJog((char *)param, index);
			
		case 0x0005:
			// Move Command
			return BusRSMove((char *)param, index, FALSE);
			
		case 0x0007:
			CmdNominalCurr();
			return TRUE;
			
		case 0x0008:
			CmdMaxCurr();
			return TRUE;
			
		case 0x0009:		
			return BusRSHome((char *)param, index);
				
		case 0x0010:
			// Move Command Tuning
			*reply = FALSE;
			return BusRSMove((char *)param, index, TRUE);
			
			
		case 0xFF:
			bootExit();		
			*reply = FALSE;
			return TRUE;
			
		default:			
			return FALSE;
	}
	return FALSE;
}

/*===================================================
 *
 ** @Brief
 ** 	Get or Set the value of a variable
** 	@Parameters
** 		byte serial: index serial channel
** 		byte *param: pointer to serial buffer
 *
 *===================================================*/
bool AlfacolRDWR(byte serial, byte *param)
{

	byte *p;
	short i;
	short idxFound;
	
	p = param;
	
	i=0;
	idxFound = -1;
	
	for (i = 0; AlfaRoadMap[i].pAddress != NULL && idxFound < 0; ++i)
	{
		if (AlfaRoadMap[i].CMD == frame_val[serial])
			idxFound = i;
	}
	
	
	if (idxFound >= 0)
	{
		if (cmd_type[serial] == eAlfaWrite && !AlfaRoadMap[idxFound].readOnly)
		{
			return WriteCmd(idxFound, (char **)&p);
		}
		else
			return ReadCmd(idxFound, (char *)p);		
	}
	else
		return FALSE;
	
}


/*===========================================
 **
 ** @Brief 
 **		Sets the value of a variable
 **	@Parameters
 **		short frame: frame value
 **		char *p: pointer to serial buffer
 **	@Return
 **		true if the parameters are correct
 **		false otherwise
 *=============================================*/
bool WriteCmd(short frame, char **p)
{
	char *pbValue;
	long *plValue;
	short *pValue;
	double *pdValue;
	char *pString;
	short index = 0;

	
	
	if (ExistIndex(*p))
	{
		// Gets the Index
		Alfasscanfs((char **)p, &index);
	}
	else
		index = 0;
	
		
	if (index > AlfaRoadMap[frame].size)
		return FALSE;	// Error Index		
					
	switch (AlfaRoadMap[frame].type)
	{
		case Byte:		
			pbValue = (char *)AlfaRoadMap[frame].pAddress + index;
						
			Alfasscanfb((char **)p, pbValue);			
		
			break;
			
		case Short:
			pValue = (short *)AlfaRoadMap[frame].pAddress + index;
						
			index = Alfasscanfs((char **)p, pValue);			
			
			break;
			
		case Long:
			plValue = (long *)AlfaRoadMap[frame].pAddress + index;
			
			Alfasscanfl((char **)p, plValue);			
			
			break;
			
		case Double:
			pdValue = (double *)AlfaRoadMap[frame].pAddress + index;
			Alfasscanfd((char **)p, pdValue);
			break;
			
		case String:
			pString = (char *)AlfaRoadMap[frame].pAddress + index;
			
			Alfasprints(*p, pString, AlfaRoadMap[frame].size);
						
			break;
			
	}
			

}

/*===========================================
 **
 ** @Brief 
 **		Gets the value of a variable
 **	@Parameters
 **		short frame: frame value
 **		char *p: pointer to serial buffer
 **	@Return
 **		true if the parameters are correct
 **		false otherwise
 *=============================================*/
bool ReadCmd(short frame, char *p)
{
	double dvalue;
	long value;
	short index;
	char *buff = p;
	short size = 0;
	short idx = 0;
	
	if (AlfaRoadMap[frame].size > 1)
	{
		// Gets Index 	
		Alfasscanfs((char **)&buff, &index);
			
		if (index > AlfaRoadMap[frame].size)
			return FALSE;	// Index Error
	}
	else
		index = 0;
			
		switch (AlfaRoadMap[frame].type)
		{
		
			case Byte:
				value = (long) *((char *)AlfaRoadMap[frame].pAddress + index);
				break;
				
				
			case Short:
				value = (long) *((short *)AlfaRoadMap[frame].pAddress + index);						
						
				break;
				
			case Long:
				value = *((long *)AlfaRoadMap[frame].pAddress + index);
							
				break;
				
			case Double:
				dvalue = *((double *)AlfaRoadMap[frame].pAddress + index);
				break;
				
			case String:
				buff = (char *)((char *)AlfaRoadMap[frame].pAddress + index);
				break;
				
		}
		
		if (idx >= 1)
			*p++ = ALFACOL_SEPARATOR;
		
		switch (AlfaRoadMap[frame].type)
		{
			case Double:
				Alfasprintfd((char *)p, dvalue);
				break;
				
			case String:
				Alfasprints(buff, (char *)p, AlfaRoadMap[frame].size);
				break;
				
			default:
				Alfasprintfl((char *)p, value);	
				break;
		}
		
		return TRUE;
}

/*=========================================
 **
 ** @Brief
 ** 	Executes the received command
 ** @Parameters
 ** 	byte serial: index serial channel
 *=========================================*/

bool AlfacolExecute(byte serial)
{
	bool st;
	bool reply = TRUE;
	
	switch (cmd_type[serial])
	{
		case eAlfaWrite:
		case eAlfaRead:
		case eAlfaComNoReply:
			st =  AlfacolRDWR(serial, (serial_buff[serial] + text_data_pos[serial]));	
			break;
			
		case eAlfaCommand:
			st = AlfacolCmd(serial, (serial_buff[serial] + text_data_pos[serial]), &reply);
			break;
	}
	if (reply)
		AlfacolCreateResponse(serial, !st);
	
	return st;
}

/*======================================================
 **
 ** @Brief
 ** 	Creates the Response to received serial command
 ** @Parameters
 ** 	byte serial: serial channel index
 ** 	bool error: creates a error response
 ** 	 
 *======================================================*/
void AlfacolCreateResponse(byte serial, bool error)
{
	short i;
	word snd;
	byte bcc;
	byte buff[3];
	
	if (error)
		serial_buff[serial][ALFACOL_POS_COMM_CHAR] = eAlfaError;
	else		
		serial_buff[serial][ALFACOL_POS_COMM_CHAR] = eAlfaReply;
	
	
	for (i = 0; serial_buff[serial][i] && i < SERIAL_BUFF_SIZE - 4; ++i)
	;
	
	if (i < 0 || i > SERIAL_BUFF_SIZE - 4)
	{
		i = SERIAL_BUFF_SIZE - 4;
	}
	
	
	serial_buff[serial][i] = 0;
	bcc = AlfacolCalculateBCD2(serial_buff[serial]);
	
	ByteToHex(bcc, buff);
	
	serial_buff[serial][i++] = buff[0];
	serial_buff[serial][i++] = buff[1];
	
	serial_buff[serial][i++] = ALFACOL_END;
	serial_buff[serial][i] = 0;
	
	
	AS1_SendBlock(serial_buff[serial], strlen((char *)serial_buff[SCI0]), &snd);
			
}



/*========================================================
 ** 
 ** @Brief
 ** 	Verifies if exist the index in the serial command.
 ** @Parameters
 ** 	char *p: received serial string	
 **
 **
 *========================================================*/
bool ExistIndex(char *p)
{
	short count;
		
	for (count = 0; *p != 0 && count < 1; p++)
	{
		if (*p == ';')
			count++;
	}
	
	return count >= 1;
	
}




/*==============================================================
 * Module:
 *	short GetSizeVariable(short type)
 *
 * Description : 
 *	byte size of a variable
 *
 *
 * Inputs:
 *	short type: variable type
 * Outputs:
 *	none
 *
 * Returns:
 *	size in bytes of the variable
 *
 *==============================================================*/

byte GetSizeVariable(short idx)
{
	short size;
	
	switch (AlfaRoadMap[idx].type)
	{
		case Byte:
			size = 1;
			break;
			
		case Short:
			size = 2;
			break;
			
		case Long:
			size = 4;
			break;
			
		default:
			size = 4;
			break;
	}
	
	return (word)size * (word)AlfaRoadMap[idx].size;
}

/*
 * Creazione di una frame di risposta con dato un numero
 */
void AlfacolSendResponse(byte serial, dword resp,  bool error, short size)
{
	word snd;
	byte bcc;
	byte buff[2];
	byte *p;
	
	//Sostituzione del carattere = o ? con lo stato della risposta
	if (error)
		serial_buff[serial][ALFACOL_POS_COMM_CHAR] = eAlfaError;
	else		
		serial_buff[serial][ALFACOL_POS_COMM_CHAR] = eAlfaReply;
	
	p = serial_buff[serial] + text_data_pos[serial];
	
	
//	p += Alfasprintfl((char *)p, resp);
	
	switch (size)
	{
		case 2:
			p += Alfasprintfl((char *)p, (short)resp);
			break;
					
		
		default:
			p += Alfasprintfl((char *)p, resp);
			break;
	}
	
	//Calcolo del BDC2
	bcc = AlfacolCalculateBCD2(serial_buff[serial]);
	
	ByteToHex(bcc, buff);
	
	*p++ = buff[0];
	*p++ = buff[1];
	
	*p++ = ALFACOL_END;
	*p = 0;
	
	
	AS1_SendBlock(serial_buff[serial], strlen((char *)serial_buff[SCI0]), &snd);
			
}

bool AlfacolSDO(byte serial)
{
	char subIndex;
	long value;
	char *p;
	p = (char *)serial_buff[serial] + text_data_pos[serial];
	
	Alfasscanfb(&p, &subIndex);
	Alfasscanfl(&p, &value);
	
	if (frame_val[serial] > 0x0FFF)
	{
		currSDOFrame = frame_val[serial];
	
		AlfacolSDOVariable(serial, subIndex, value);
		
		
	}
	else
		AlfacolRoute(serial);		
	
	return TRUE;
	
}

bool AlfacolSDOVariable(byte serial, byte subIndex, long value)
{
	long size;
	
	
	switch (cmd_type[serial])
	{
	case eAlfaRead:
		SDOCommandUpload(numStation[serial], currSDOFrame, subIndex);
		break;
	
	default:
		SDOCommandDownload(numStation[serial], (byte *)&value, currSDOFrame, subIndex, 4);									
		break;
		
	}
}

bool AlfacolRoute(byte serial)
{
	char *p;
	byte resp;
	
	p = (char *)serial_buff[serial] + text_data_pos[serial];
	if (cmd_type[serial] == eAlfaCommand)
	{
		resp = RouterFunction(numStation[serial], p, &frame_val[serial]);
	}
	else
	{
		resp = RouterVariable(numStation[serial], p, &frame_val[serial], AlfacolGetTypeOp(cmd_type[serial]));		
	}
	
	switch (resp)
	{
	case cmdAlfacol:
		return AlfacolExecute(serial);
		
	case cmdSDO:
		return TRUE;
		
	default:
		return FALSE;
	}	
}

void AlfacolCanCommand(byte serial)
{
	char subIndex;
	long lValue;
	char *p = (char *)(serial_buff[serial] + text_data_pos[serial]);
	
	currSDOFrame = frame_val[serial];
	Alfasscanfb(&p, &subIndex);
	Alfasscanfl(&p, &lValue);
	SDOCommandDownload(numStation[serial], (byte *)lValue, currSDOFrame, subIndex, 4);
}

/*
 * Determina se si tratta di un comando di lettura (get) o scrittura (set)
 */
char AlfacolGetTypeOp(byte cmd)
{
	switch (cmd)
	{
		
	case eAlfaWrite:
		return opSet;
	default:
		return opGet;
	}		
}

