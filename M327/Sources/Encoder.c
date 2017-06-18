/*
 * Encoder.c
 *
 *  Created on: Jun 23, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Enc1.h"
#include "Enc2.h"
#include "Settings.h"
#include "Slave.h"
#include "Encoder.h"


long rpos[MAC_NUM_AXIS];						// Actual Position

long encPos[NUM_ENCODERS];

long encPos0[NUM_ENCODERS];						// Posizione di zero

long encPosRel[NUM_ENCODERS];					// Posizione Relativa allo zero

short encCount[NUM_ENCODERS];					// Valore del contatore

short encVel[NUM_ENCODERS];

void InitEncoder()
{
	byte id;
	
	for (id = 0; id < NUM_ENCODERS; ++id)
	{
		rpos[id] = 0;
		encCount[id] = 0;
	}
}

#pragma interrupt called
void GetEncoders()
{
	byte id;
	Enc1_TCounterValue count;
	short tmp;
	
	for (id = 0; id < NUM_ENCODERS; ++id)
	{
		switch(id)
		{
		case 0:
			Enc1_GetCounterValue(&count);
			break;
			
		case 1:
			Enc2_GetCounterValue(&count);
		}
		tmp = count - encCount[id];
		encCount[id] = (short)count;
		encPos[id] -= tmp;
		encVel[id] = -tmp;
	}
	
	for (id = 0; id < NUM_AXES; ++id)
	{
		rpos[id] = encPos[id];
	}
}
