/*
*
* TopicBuffer.c - topic buffer
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

#include "TopicBuffer.h"

TopicBufferHandle CreateTopicBuffer(unsigned portBASE_TYPE topicID, unsigned portBASE_TYPE msg_length)
{
	TopicBufferHandle pom;
	portBASE_TYPE i;

	pom = pvPortMalloc(sizeof(TopicBuffer));
	pom->messages = pvPortMalloc(TPBUF_LENGTH * msg_length);
	pom->topicID = topicID;
	pom->sem_space_left = xSemaphoreCreateCounting(TPBUF_LENGTH, 0);
	pom->subscribersCount = 0;

	return pom;
}

portBASE_TYPE DestroyTopicBuffer(TopicBufferHandle TBHandle)
{
	vSemaphoreDelete(TBHandle->sem_space_left);
	vPortFree(TBHandle->messages);
	vPortFree(TBHandle);

	return 0;
}

tMsg GetMsgFromTopicBuffer(TopicBufferHandle TBHandle, unsigned portBASE_TYPE msg_index)
{
	return TBHandle->messages[msg_index];
}

void MsgDoneReading(TopicBufferHandle TBHandle, unsigned portBASE_TYPE msg_index)
{
	if(TBHandle->msgPendingReads[msg_index]>0) TBHandle->msgPendingReads[msg_index]--;

	// all subscribing apps have read this message; this slot is now avaible for writing
	if(TBHandle->msgPendingReads[msg_index]==0) xSemaphoreGive(TBHandle->sem_space_left);
}

void WriteTopicBuffer(TopicBufferHandle TBHandle, tMsg msg, unsigned portBASE_TYPE subscribers_count)
{
	int i;

	// block if no space in buffer
	xSemaphoreTake(TBHandle->sem_space_left, (portTickType) 0);

	for(i=0; i<TPBUF_LENGTH; i++)
	{
		if(TBHandle->msgPendingReads[i]<=0)
		{
			// all apps subscribing this topic, read this message
			TBHandle->messages[i] = msg;
			TBHandle->msgPendingReads[i] = subscribers_count;
			break;
		}
	}
}
