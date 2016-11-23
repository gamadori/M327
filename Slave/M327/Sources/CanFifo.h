/*
 * CanFifo.h
 *
 *  Created on: Feb 16, 2015
 *      Author: gianluca
 */

#ifndef CANFIFO_H_
#define CANFIFO_H_



typedef struct
{
	CanMessage *buff;
	
	CanMessage *tail;
	
	CanMessage *head;
	
	short maxSize;
	
	
	
} tCanFifo;

void CanMsgFifoInit(tCanFifo* fifo, CanMessage *buff, short size);

void CanFifoFlush(tCanFifo *fifo);

void CanMsgFifoPushOne(tCanFifo *fifo, CanMessage *m);

void CanMsgFifoPush(tCanFifo *fifo, CanMessage *buff, short num);

bool CanMsgFifoPopOne(tCanFifo *fifo, CanMessage *m);

void CanFifoPushOne(tCanFifo *fifo, word id, byte len, byte *buff);

#endif /* CANFIFO_H_ */
