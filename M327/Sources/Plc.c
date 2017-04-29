/*
 * Plc.c
 *
 *  Created on: Jan 3, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "SM1.h"
#include "CSInput.h"
#include "Bits.h"
#include "Utility.h"
#include "Timer.h"
#include "Plc.h"
#include "Out0.h"
#include "Out1.h"
#include "Out2.h"
#include "Out3.h"
#include "Out4.h"
#include "Out5.h"
#include "Out6.h"
#include "Out7.h"
#include "Out8.h"
#include "Out9.h"
#include "Out10.h"
#include "Out11.h"
#include "Out12.h"
#include "Out13.h"
#include "Out14.h"
#include "Out15.h"
#include "AD1.h"
#include "CanSettings.h"

dword inputBuffer;
dword outputBuffer;

dword inputPrevious;

dword inputFiltered;

dword outValue;
dword outFrz;
dword outFrzValue;

dword inpValue;
dword inpFrz;
dword inpFrzValue;

word busInputs[NUM_IO_BOARD];
word busOutputs[NUM_IO_BOARD];

short plcStatus = stIONull;
bool ioWorked = FALSE;

word ADCValues[NUM_ANALOGIC];

word inpFilter[NUM_INPUTS];

void PlcSetOutpts();

void PlcInit()
{
	inputPrevious = 0;
	inputBuffer = 0;
	outputBuffer = 0;
	outValue = 0;
	outFrz = 0;
	outFrzValue = 0;
	inpValue = 0;
	inpFrz = 0;
	inpFrzValue = 0;
	CSInput_ClrVal();
	plcStatus = stCmdInput;
}


void PlcServer()
{
	word ioNumSended;
	word buff;
	
	outputBuffer = (outValue & ~outFrz) | (outFrzValue & outFrz);
	
	inputBuffer = (inpValue & ~inpFrz) | (inpFrzValue & inpFrz);
	
	switch (plcStatus)
	{
		case stCmdInput:					
			
			if (!TMR_PLC)
			{										
				CSInput_SetVal();
				
				SM1_ClearRxBuf();
				
				plcStatus = stWaitReceve;
				SM1_SendBlock((byte *)&inputBuffer, NUM_INPUTS_BLOCK, &ioNumSended);				
				
				TMR_PLC = MSEC(10);
			}
			break;
			
		case stWaitReceve:
			
			ioNumSended = SM1_GetCharsInRxBuf();
			if (SM1_GetCharsInRxBuf() >= NUM_INPUTS_BLOCK)
			{
				
				
				SM1_RecvBlock((byte *)&buff, NUM_INPUTS_BLOCK, &ioNumSended);
				
				if (ioNumSended == NUM_INPUTS_BLOCK)
				{
					inpValue = buff;
					//inputBuffer = (buff & ~inpFrz) | (inpFrzValue & inpFrz);					
					ioWorked = TRUE;
				}
				
				plcStatus = stCmdInput;
				CSInput_ClrVal();
				
			}	
			else if (!TMR_PLC)
			{
				
				plcStatus = stCmdInput;
				CSInput_ClrVal();
			}
			break;
	}
	PlcSetOutpts();
	
}

void PlcSetOutpts()
{
	Out0_PutVal(bitSelected(outputBuffer, 0));
	Out1_PutVal(bitSelected(outputBuffer, 1));
	Out2_PutVal(bitSelected(outputBuffer, 2));
	Out3_PutVal(bitSelected(outputBuffer, 3));
	Out4_PutVal(bitSelected(outputBuffer, 4));
	Out5_PutVal(bitSelected(outputBuffer, 5));
	Out6_PutVal(bitSelected(outputBuffer, 6));
	Out7_PutVal(bitSelected(outputBuffer, 7));
	Out8_PutVal(bitSelected(outputBuffer, 8));
	Out9_PutVal(bitSelected(outputBuffer, 9));
	Out10_PutVal(bitSelected(outputBuffer, 10));
	Out11_PutVal(bitSelected(outputBuffer, 11));
	Out12_PutVal(bitSelected(outputBuffer, 12));
	Out13_PutVal(bitSelected(outputBuffer, 13));
	Out14_PutVal(bitSelected(outputBuffer, 14));
	Out15_PutVal(bitSelected(outputBuffer, 15));
	
}

bool bitSelected(dword field, byte bit)		
{
	return ((field & Bits[bit]) == Bits[bit]);
}

/*
 * Inizializza gli ingressi
 * analogici
 */
void AnalogicInit()
{
	short i;
	
	for (i = 0; i < NUM_ANALOGIC; ++i)
	{
		ADCValues[i] = 0;
	}
	
	AD1_Measure(TRUE);

}


void AnalogicServer()
{	
	if (AD1_GetValue(ADCValues) != ERR_NOTAVAIL)
	{
		AD1_Measure(FALSE);
	
	
	}
}

void FilterInputs()
{ 
	word tmp;
	short i;
	
	tmp = (word)~(inputPrevious ^ inputBuffer);
	
	
	for (i = 0; i < NUM_INPUTS; ++i)
	{
		
		if (inpFilter[i])
		{
			if (!(tmp & Bits[i]))
			{
				tmrsPlc[i] = MSEC(inpFilter[i]);
			}
			else if (!tmrsPlc[i])
			{
				
				if (inputBuffer & Bits[i])
					inputFiltered |= Bits[i];
				
				else
					inputFiltered &= ~Bits[i];
				
			}
		}
		else 
		{
			if ((inputBuffer & Bits[i]) == Bits[i])
				inputFiltered |= Bits[i];
			
			else
				inputFiltered &= ~Bits[i];
			
		}
			
	}
	
	inputPrevious = inputBuffer;
}

