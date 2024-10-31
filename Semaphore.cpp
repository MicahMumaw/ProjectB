/*
 * Semaphore.cpp
 *
 *  Created on: Oct 30, 2024
 *      Author: eegge
 */

#include "Semaphore.h"


Semaphore::Semaphore() : flag(false) {}

bool Semaphore::signal()
{
    if (flag)
    {
        return false;
    }
    else
    {
        flag = true;
        return true;
    }
}

bool Semaphore::tryWait()
{
    if (!flag)
    {
        return false;
    }
    else
    {
        flag = false;
        return true;
    }
}


