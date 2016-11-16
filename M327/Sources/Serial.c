/*
 * Serial.c
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "AS1.h"
#include "Alfacol.h"
#include "Serial.h"



byte ptc_current[NUM_SERIAL];

byte serial_buff[NUM_SERIAL][SERIAL_BUFF_SIZE];		// Buffer di appoggio per la decodifica

bool serialBusy[NUM_SERIAL];

byte SerialRecvChar(byte nSerial, byte *c);

void SerialDecode(byte n_serial, byte c);

/*
 **	Initialise the variable
 **	of the module
 **	
 */
void SerialInit(void)
{
	register byte i;
	
	for (i = 0; i < NUM_SERIAL; ++i)
	{

		serialBusy[i] = FALSE;
	}
	
}



void SerialReceve()
{
	byte c;
	
	while (SerialRecvChar(SCI0, &c) == ERR_OK)
	{
		SerialDecode(SCI0, c);
	}
}
/*
 ** Decode the received commands
 */
void SerialDecode(byte n_serial, byte c)
{
			
	switch (c)
	{
	case ALFACOL_START:
		AlfacolInit(n_serial);
		ptc_current[n_serial] = PTC_ALFACOL;
		serial_buff[n_serial][0] = c;
		
		break;
		
	
		
		
	default:
		
		switch(ptc_current[n_serial])
		{
		case PTC_ALFACOL:				
			if (!AlfacolDecode(c, n_serial))
			{
				ptc_current[n_serial] = PTC_NULL;
			}
			
			break;
			
		
			
		default:
			
			break;	
				
		}
	}
	
}

/*
**	Description:
**		If any data is received, this method returns one character,
**  	otherwise it returns an error code (it does not wait for
**  	data). This method is enabled only if the receiver property
**  	is enabled.
**         
** Parameters:
** 		byte nSerial: 
** 			Serial Number.
**		byte *c:	character 
** 			Pointer to a received character
** Returns:
**		- Error code, possible codes:
**      	ERR_OK - OK
**          Error Code otherwise	 
 */
byte SerialRecvChar(byte nSerial, byte *c)
{
	switch (nSerial)
	{
	case SCI0:
		return AS1_RecvChar(c);
		
	default:
		return ERR_RXEMPTY;
	}
}


