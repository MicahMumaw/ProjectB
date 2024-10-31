/*
 * Semaphore.h
 *
 *  Created on: Oct 30, 2024
 *      Author: eegge
 */

#ifndef GIT_PROJECTB_SEMAPHORE_H_
#define GIT_PROJECTB_SEMAPHORE_H_

class Semaphore
{
public:
    Semaphore();

    // Signal method to set the flag (called by ISR every 2 ms)
    bool signal();

    bool tryWait();

private:
    bool flag;
};


extern Semaphore mySemaphore;



#endif /* GIT_PROJECTB_SEMAPHORE_H_ */
