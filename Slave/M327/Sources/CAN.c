/*
 * CAN.c
 *
 *  Created on: Jan 16, 2015
 *      Author: gianluca
 */

#include "cpu.h"
#include "CAN1.h"
#include "CAN.h"
#include "CanFifo.h"
#include "plc.h"

#define MB0ID	0x100

#define MB1ID	0x200

#define CAN_BUFF_SIZE		512

#define CAN_NUM_TX_CHANNEL	1

#define CAN_NUM_CHANNEL		2

#define CAN_FIFO_BUFF_SIZE	5




tCanFifo canMsgFifo[CAN_FIFO_NUM_CHANNEL];		// Manager FIFO 

CanMessage canFifoQueue[CAN_FIFO_NUM_CHANNEL][CAN_FIFO_BUFF_SIZE];	// Buffer FIFO

word deviceNodeId = 0;


word CanFifoMbMask[] = 
{
		CAN_MB0_MASK,
		CAN_MB1_MASK,
		CAN_MB2_MASK,
		CAN_MB3_MASK,
		CAN_MB4_MASK,
		CAN_MB5_MASK,
		CAN_MB6_MASK,
		CAN_MB7_MASK,
		CAN_MB8_MASK,
		CAN_MB9_MASK,
		CAN_MB10_MASK,
		CAN_MB11_MASK,
		CAN_MB12_MASK,
		CAN_MB13_MASK,
		CAN_MB14_MASK,
		CAN_MB15_MASK
		
};

word CanFifoMbDir[] =
{
		MBReceive,		// MB0
		MBReceive,		// MB1
		MBReceive,		// MB2
		MBTransmit,		// MB3
		MBTransmit,		// MB4
		MBReceive,		// MB5
		MBTransmit,		// MB6
		MBReceive,		// MB7
		MBTransmit,		// MB8
		MBReceive,		// MB9
		MBTransmit,		// MB10
		MBReceive,		// MB11
		MBTransmit,		// MB12
		MBReceive,		// MB13
		MBTransmit,		// MB14
		MBTransmit		// MB15
};
byte CanBufferCode[] = 
{
		0x000,
		0x080,
		0x100,
		0x080,
		0x180,
		0x200,
		0x280,
		0x300,
		0x380,
		0x400,
		0x480,
		0x500,
		0x580,
		0x600,
		0x700,
		0x780
};

void CanMSBInit();

void CanInit()
{
	short i;
	
	//deviceNodeId = inpDipSwitchStation;
	CanMSBInit();
	for (i = 0; i < CAN_FIFO_NUM_CHANNEL; ++i)
	{
		CanMsgFifoInit(&canMsgFifo[i], canFifoQueue[i], CAN_FIFO_BUFF_SIZE);
	}
}

void CanMSBInit()
{
	short i;
	
	for (i = 0; i < CAN_FIFO_NUM_CHANNEL; ++i)
	{
		CAN1_SetBufferCode(i, CanBufferCode[i]);
	}
}

void CanServerTrasmit()
{

	word state;
	short i;
	byte c;
	short n;
	word delay;
	CanMessage m;
		
	
	for (i = 0; i < CAN_FIFO_NUM_CHANNEL; ++i)
	{
		if (CanFifoMbDir[i] == MBTransmit && CanMsgFifoPopOne(&canMsgFifo[i], &m))	
		{
			CAN1_SendFrame(i, m.cob_id, DATA_FRAME, m.len, m.data, FALSE);
						
			delay = 1000;
			do
			{
				state = CAN1_GetStateTX();
				
			}while (!(state & CanFifoMbMask[i]) && delay--);
		}
	}	
}

void CanFlush(byte channel)
{
	CanFifoFlush(canMsgFifo + channel);
}
/*
 * Push the data on Received Can Queue 
 */
void CanReceivedPush(byte channel, word id, byte len,  byte *buff)
{
	if (CanFifoMbDir[channel] == MBReceive)
	{
		CanFifoPushOne(canMsgFifo + channel, id, len, buff);
	}
}

bool CanReceivedPop(byte channel, CanMessage *m)
{
	if (CanFifoMbDir[channel] == MBReceive)
	{
		return CanMsgFifoPopOne(canMsgFifo + channel, m);
	}
	else
		return FALSE;
}

void CanTransmitPush(byte channel, CanMessage *msg)
{
	if (CanFifoMbDir[channel] == MBTransmit)
	{
		CanMsgFifoPushOne(canMsgFifo + channel, msg);
	}
}


