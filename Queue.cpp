/*
 * Queue.cpp
 *
 *  Created on: Oct 8, 2024
 *      Author: micah
 */

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "Queue.h"



Queue::Queue(void)
{
		tail = 0;
		for(int32_t i = 0; i < Q_SIZE; i++)
		{
			buffer[i] = {0,0};
		}
	}

bool Queue::enqueue(OutputData msg)//adds msg to the queue at the indicated index by tail. Returns true if successful and false if buffer is full.
{
	if (tail >= Q_SIZE)
	{
		tail--;
		return false;
	}

	else
	{
		buffer[tail] = msg;
		tail++;
		return true;
	}
}

bool Queue::dequeue(OutputData *msg) //removes message at index 0 and shifts everything left (forward) and writes the removed message to *msg. Returns true if successful and false if if the queue is empty.
{
	if (tail == 0)
	{
	    return false;
	}
	else
	{
		*msg = buffer[0];

		for(int32_t i = 1; i < Q_SIZE; i++)
		{
			buffer[i - 1] = buffer[i];
		}

		buffer[Q_SIZE - 1] = {0, 0, 0};

		return true;
	}
	tail--;
}



