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

#ifndef MSGFIFO_H_
#define MSGFIFO_H_

#include "microRTPS_config.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct sMsgAddress
{
	unsigned portBASE_TYPE tpbufID;
	unsigned portBASE_TYPE msgID;
} MsgAddress;

typedef struct sMsgQueue
{
	MsgAddress queue[MSG_QUEUE_LENGTH];
	unsigned portBASE_TYPE full;
	unsigned portBASE_TYPE in;
	unsigned portBASE_TYPE out;
	xSemaphoreHandle mutex;
} MsgQueue;

void MsgQueueInit(MsgQueue* msgQueue);
void MsgQueueRead(MsgQueue* msgQueue, unsigned portBASE_TYPE* tpbufID, unsigned portBASE_TYPE* msgID);
void MsgQueueWrite(MsgQueue* msgQueue, unsigned portBASE_TYPE tpbufID, unsigned portBASE_TYPE msgID);

#endif /* MSGFIFO_H_ */
