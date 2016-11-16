/*
 * CanFifo.c
 *
 *  Created on: Feb 16, 2015
 *      Author: gianluca
 */

#include "cpu.h"
#include "Can.h"
#include "CanFifo.h"

#define FIFO_ADD_END_BUFF(fifo)		(fifo->buff + fifo->maxSize)

bool CanMsgFifoInitOk(tCanFifo* fifo);

bool CanFifoPutMsg(tCanFifo *fifo, CanMessage *m);

bool CanFifoPutData(tCanFifo *fifo, word id, byte len, byte *buff);

void CanFifoCopyMsg(CanMessage *msgOr, CanMessage *msgDest);

void CanFifoCopyDataToMsg(word id, byte len, byte *buff, CanMessage *msgDest);

void CanFifoNext(tCanFifo* fifo, CanMessage **p);


void CanMsgFifoInit(tCanFifo* fifo, CanMessage *buff, short size)
{
	fifo->buff = buff;
	fifo->maxSize = size;
	
	fifo->head = fifo->tail = fifo->buff;
		
}

bool CanMsgFifoInitOk(tCanFifo* fifo)
{
	return fifo->buff != NULL && fifo->maxSize > 0;
}

/*
 * Inserisce un byte nella fifo
 */
void CanMsgFifoPushOne(tCanFifo *fifo, CanMessage *m)
{
	short i;
	
	if (CanMsgFifoInitOk(fifo))
	{
		if (!CanFifoPutMsg(fifo, m))
		{
			// In Queue there is not a Message with the same id
			// push the message on Queue
			CanFifoCopyMsg(m, fifo->head);
			CanFifoNext(fifo, &fifo->head);
		}		
		
	}
}

void CanFifoPushOne(tCanFifo *fifo, word id, byte len, byte *buff)
{
	short i;
	
	if (len < 8 && CanMsgFifoInitOk(fifo))
	{
		
		if (!CanFifoPutData(fifo, id, len, buff))
		{
			// In Queue there is not a Message with this id
			// push the message on Queue
			
			CanFifoCopyDataToMsg(id, len, buff, fifo->head);
			CanFifoNext(fifo, &fifo->head);
		}		
	}
}
/*
 * Inserisce num byte nella fifo
 */
void CanMsgFifoPush(tCanFifo *fifo, CanMessage *buff, short num)
{
	short i;
	
	for (i = 0; i < num; ++i)
	{
		CanMsgFifoPushOne(fifo, buff + i);
	}
}
/*
 * Preleva un byte dalla Fifo
 */
bool CanMsgFifoPopOne(tCanFifo *fifo, CanMessage *m)
{
	short i;
	
	if (fifo->tail == fifo->head)
		return FALSE;
	m->cob_id = fifo->tail->cob_id;
	m->len = fifo->tail->len;
	for (i = 0; i < 8; ++i)
	{
		m->data[i] = fifo->tail->data[i];
	}
		
	CanFifoNext(fifo, &fifo->tail);
	
	return TRUE;
}

bool CanFifoPutMsg(tCanFifo *fifo, CanMessage *m)
{
	CanMessage *p = fifo->tail;
	while (p != fifo->head)
	{
		if (p->cob_id == m->cob_id)
		{
			CanFifoCopyMsg(m, p);
			return TRUE;
		}
		else
		{
			CanFifoNext(fifo, &p);
		}
	}
	return FALSE;
}

bool CanFifoPutData(tCanFifo *fifo, word id, byte len, byte *buff)
{
	CanMessage *p = fifo->tail;
	
	while (p != fifo->head)
	{
		if (p->cob_id == id)
		{
			CanFifoCopyDataToMsg(id, len, buff, p);
			return TRUE;
		}
		else
		{
			CanFifoNext(fifo, &p);
		}
	}
	return FALSE;
}

void CanFifoCopyMsg(CanMessage *msgOr, CanMessage *msgDest)
{
	short i;
	
	msgDest->cob_id = msgOr->cob_id;
	msgDest->len = msgOr->len;
	for (i = 0; i < msgOr->len; ++i)
	{
		msgDest->data[i] = msgOr->data[i];
	}
}

void CanFifoCopyDataToMsg(word id, byte len, byte *buff, CanMessage *msgDest)
{
	short i;
	
	msgDest->cob_id = id;
	msgDest->len = len;
	for (i = 0; i < len; ++i)
	{
		msgDest->data[i] = buff[i];
	}
}


void CanFifoNext(tCanFifo *fifo, CanMessage **p)
{
	if (++*p >= FIFO_ADD_END_BUFF(fifo))
	{
		*p = fifo->buff;
	}	
}
