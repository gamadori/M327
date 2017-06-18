/*
 * plc.h
 *
 *  Created on: Jan 4, 2016
 *      Author: gianluca
 */

#ifndef PLC_H_
#define PLC_H_

enum StatusIO
{
	stIONull,
	stCmdOutput,
	stWaitTrasmit,
	stCmdInput,
	stWaitReceve
};

#define NUM_OUTPUT		16

#define NUM_INPUTS		16

#define NUM_ANALOGIC	8

#define NUM_INPUTS_BLOCK	2

#define NUM_OUTPUTS_BLOCK	2

#define inpDipSwitchStation			((inputBuffer & 0xFF00) >> 8) 

#define inpPwrFiltered				((inputFiltered & BIT15) == BIT15)


bool bitSelected(dword field, byte bit);

extern dword inputBuffer;
extern dword outputBuffer;
extern dword inputFiltered;

extern dword outValue;
extern dword outFrz;
extern dword outFrzValue;

extern dword inpValue;
extern dword inpFrz;
extern dword inpFrzValue;

extern word busInputs[];

extern word busOutputs[];

extern word ADCValues[];

extern word inpFilter[];

void PlcInit();

void PlcServer();

void AnalogicInit();

void AnalogicServer();

void FilterInputs();

#endif /* PLC_H_ */
