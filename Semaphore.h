/*
 * Semaphore.h
 *
 *  Created on: Oct 30, 2024
 *      Author: eegge
 */

/**
 * @file Semaphore.h
 * @brief Header file defining the Semaphore class for managing signaling and waiting mechanisms.
 *
 * This file contains the definition of the `Semaphore` class, which provides a simple mechanism
 * for signaling and waiting in a multi-threaded or interrupt-driven environment.
 *
 * @date Created on: Oct 30, 2024
 * @author eegge
 */

#ifndef GIT_PROJECTB_SEMAPHORE_H_
#define GIT_PROJECTB_SEMAPHORE_H_

/**
 * @class Semaphore
 * @brief A simple semaphore class for managing a binary flag.
 *
 * The `Semaphore` class provides methods for setting and checking a binary flag, enabling
 * synchronization between an ISR (Interrupt Service Routine) and the main application. The
 * semaphore can be signaled by an ISR and checked by the main program.
 */
class Semaphore
{
public:
    /**
     * @brief Constructs a Semaphore object.
     *
     * Initializes the semaphore with the flag set to false.
     */
    Semaphore();

    /**
     * @brief Signals the semaphore by setting the flag.
     *
     * This method is typically called by an ISR every 2 ms to set the flag.
     *
     * @return Returns true if the flag was successfully set, indicating the semaphore was signaled.
     */
    bool signal();

    /**
     * @brief Attempts to wait for the semaphore by checking the flag.
     *
     * Checks if the flag is set and, if so, clears the flag to indicate the wait operation succeeded.
     *
     * @return Returns true if the flag was set and cleared, indicating a successful wait; false otherwise.
     */
    bool tryWait();

private:
    bool flag; ///< The binary flag indicating the semaphore state.
};

/**
 * @brief External semaphore instance for global use.
 *
 * An instance of the `Semaphore` class, intended for use by other parts of the application to
 * facilitate signaling and waiting functionality.
 */
extern Semaphore mySemaphore;

#endif /* GIT_PROJECTB_SEMAPHORE_H_ */

