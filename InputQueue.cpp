/*
 * InputQueue.cpp
 *
 *  Created on: Oct 23, 2024
 *      Author: eegge
 */

#include "InputQueue.h"


InputQueue::InputQueue(void)
{
    tail = 0;
    for (int32_t i = 0; i < Q_SIZE; i++)
    {
        buffer[i] = {0, 0, 0, false, false, false};
    }

}


bool InputQueue::enqueue(const InputData &msg)
{
    if (tail >= Q_SIZE)
    {
        return false;
    }
    else
    {
        buffer[tail] = msg;
        tail++;
        return true;
    }
}


bool InputQueue::dequeue(InputData *msg) {
    if (tail == 0)
    {
        return false;
    }
    else
    {
        *msg = buffer[0];

        for (int32_t i = 1; i < tail; i++)
        {
            buffer[i - 1] = buffer[i];
        }

        buffer[tail - 1] = {0, 0, 0, false, false, false};
        tail--;
        return true;
    }
}


