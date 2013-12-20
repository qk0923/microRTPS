/*
*
* msgFIFO.h - message FIFO queue
*
* Copyright (C) 2013         Maciej Szyma�ski <mszymanski90@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

#include "msgFIFO.h"

void MsgQueueInit(MsgQueue* msgQueue)
{
	unsigned portBASE_TYPE i;

	msgQueue->full=0;
	msgQueue->in=0;
	msgQueue->out=0;

	for(i=0; i<MSG_QUEUE_LENGTH; i++)
	{
		msgQueue->queue[i].msgID=0;
		msgQueue->queue[i].tpbufID=MAX_TOPICS;
	}
}

void MsgQueueRead(MsgQueue* msgQueue, unsigned portBASE_TYPE* tpbufID, unsigned portBASE_TYPE* msgID)
{
	msgQueue->out++;
	if(msgQueue->out >= MSG_QUEUE_LENGTH) msgQueue->out = 0;
	if(msgQueue->in == msgQueue->out) msgQueue->full = 0;


	*tpbufID = msgQueue->queue[msgQueue->out - 1].tpbufID;
	*msgID = msgQueue->queue[msgQueue->out - 1].msgID;

	// if this field is read when no message incoming, this will cause error
	msgQueue->queue[msgQueue->out - 1].msgID = 0;
	msgQueue->queue[msgQueue->out - 1].tpbufID = MAX_TOPICS;
}

void MsgQueueWrite(MsgQueue* msgQueue, unsigned portBASE_TYPE tpbufID, unsigned portBASE_TYPE msgID)
{
	if(!msgQueue->full)
	{
		msgQueue->queue[msgQueue->in].tpbufID = tpbufID;
		msgQueue->queue[msgQueue->in].msgID = msgID;
		msgQueue->in++;

		if(msgQueue->in >= MSG_QUEUE_LENGTH) msgQueue->in = 0;
		if(msgQueue->in == msgQueue->out) msgQueue->full = 1;
	}
}

