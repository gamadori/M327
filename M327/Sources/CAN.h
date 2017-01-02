/*
 * CAN.h
 *
 *  Created on: Jan 16, 2015
 *      Author: gianluca
 */

#ifndef CAN_H_
#define CAN_H_

#define CAN_FIFO_NUM_CHANNEL	16 

enum CanBuffDir
{
	MBTransmit,		
	MBReceive		
	
};

enum CANChannel
{
	CanNMTChannel,			//0
	CanSYNCChennel,			//1
	CanTIMESTAMPChannel,	//2
	CanEMERCENGYChannel,	//3
	CanPDO1TxChannel,		//4
	CanPDO1RxChannel,		//5
	CanPDO2TxChannel,		//6
	CanPDO2RxChannel,		//7
	CanPDO3TxChannel,		//8
	CanPDO3RxChannel,		//9
	CanPDO4TxChannel,		//10
	CanPDO4RxChannel,		//11
	CanSDOTxChannel,		//12
	CanSDORxChannel,		//13
	CanNMTErrorChannel,		//14
	CanNMTSendState			//15
};

#define CAN_BUFF_SDO_TX		0x600


/*
 * I messaggi in entrata e in uscita sono caricati in questo datatype
 */
typedef struct
{	
	word cob_id;	// CAN-id of the received/transmitted message

	byte rtr;		// sets rtr flag or not
	
	byte len;		// defines count of valid data in the data-array
	
	byte data[8];	// Dato
	
} CanMessage;

typedef struct td_s_rx_buffer_message
{
	bool valid; // if set to true, the message is valid: so it has been written
				// but not yet read. if false, the message has not been written
				// yet, or is neither valid.
	CanMessage m; // contains the message itself
	
} MessageBuffer;

extern word CanFifoMbMask[];

extern word CanFifoMbDir[];

extern word deviceNodeId;

void CanInit();

void CanServerTrasmit();

void CanTrasmit(byte channel, CanMessage *msg);

bool CanReceivedPop(byte channel, CanMessage *m);

void CanReceivedPush(byte channel, word id, byte len,  byte *buff);

void CanTransmitPush(byte channel, CanMessage *msg);

#endif /* CAN_H_ */
