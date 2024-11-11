/*
 * InputQueue.h
 *
 *  Created on: Oct 23, 2024
 *      Author: eegge
 */

/**
 * @file InputQueue.h
 * @brief Header file defining the InputQueue class and InputData struct for handling input data.
 *
 * This file contains the definition of the `InputData` structure for storing input parameters
 * from various knobs and buttons, as well as the `InputQueue` class, which implements a fixed-size
 * circular queue for managing these input events.
 *
 * @date Created on: Oct 23, 2024
 * @author eegge
 */

#ifndef GIT_PROJECTB_INPUTQUEUE_H_
#define GIT_PROJECTB_INPUTQUEUE_H_

#include <cstdint>

/// Queue size constant
#define Q_SIZE 5

/**
 * @struct InputData
 * @brief Structure to store input data from knobs and buttons.
 *
 * This structure holds data from different inputs such as knobs for frequency, phase shift,
 * and amplitude, as well as buttons for selecting wave shape and mode options.
 */
struct InputData
{
    int8_t freq_amp; ///< Frequency or amplitude input from Knob 1.
    int8_t phase_shift; ///< Phase shift input from Knob 3.
    bool wave_choice; ///< Selection for wave shapes (e.g., sine, triangle).
    bool wave_select; ///< Selects between wave 1 or a following wave.
    bool echo_mode; ///< Toggles echo mode, affecting wave relationship.
    bool select_freqamp; ///< Selects frequency or amplitude control for Knob 1.
};

/**
 * @class InputQueue
 * @brief Implements a circular queue for storing InputData items.
 *
 * The `InputQueue` class provides functionality to enqueue and dequeue `InputData` items
 * within a fixed-size buffer, enabling structured handling of user inputs.
 */
class InputQueue
{
private:
    InputData buffer[Q_SIZE]; ///< Array buffer to store queued input data.
    int32_t tail; ///< Index of the tail of the queue.

public:
    /**
     * @brief Constructs an InputQueue object.
     *
     * Initializes the queue with an empty state.
     */
    InputQueue();

    /**
     * @brief Enqueues a message into the queue.
     *
     * Adds a new `InputData` item to the end of the queue.
     * @param msg The `InputData` item to enqueue.
     * @return `true` if the enqueue operation is successful; `false` if the queue is full.
     */
    bool enqueue(const InputData &msg);

    /**
     * @brief Dequeues a message from the queue.
     *
     * Removes an `InputData` item from the front of the queue.
     * @param msg Pointer to store the dequeued `InputData` item.
     * @return `true` if the dequeue operation is successful; `false` if the queue is empty.
     */
    bool dequeue(InputData *msg);
};

#endif /* GIT_PROJECTB_INPUTQUEUE_H_ */
