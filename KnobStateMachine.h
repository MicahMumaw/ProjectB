/*
 * KnobStateMachine.h
 *
 *  Created on: Oct 28, 2024
 *      Author: eegge
 */

/**
 * @file KnobStateMachine.h
 * @brief Header file defining the KnobStateMachine class for handling knob input state transitions.
 *
 * This file contains the definition of the `KnobStateMachine` class, which manages the state
 * transitions for a knob input. It includes a state machine with defined transitions for detecting
 * changes in knob input, such as rising and falling edges.
 *
 * @date Created on: Oct 28, 2024
 * @author eegge
 */

#ifndef GIT_PROJECTB_KNOBSTATEMACHINE_H_
#define GIT_PROJECTB_KNOBSTATEMACHINE_H_

#include <stdint.h>

/**
 * @class KnobStateMachine
 * @brief Implements a finite state machine to detect edge transitions for knob input.
 *
 * The `KnobStateMachine` class manages state transitions based on knob input signals, using a
 * transition table to handle rising and falling edges. It provides an output to indicate
 * changes in the knob's state.
 */
class KnobStateMachine
{
public:
    /**
     * @brief Constructs a KnobStateMachine object.
     *
     * Initializes the state machine in the inactive state with an initial output of 0.
     */
    KnobStateMachine();

    /**
     * @brief Retrieves the current output of the state machine.
     *
     * The output indicates the state of the knob input: -1 for a falling edge, 1 for a rising
     * edge, and 0 for no change.
     *
     * @return The current output as an 8-bit signed integer.
     */
    int8_t Output();

    /**
     * @brief Advances the state of the state machine based on the input.
     *
     * Changes the current state based on the input value, using the transition table to detect
     * edge transitions.
     *
     * @param input The input value as an 8-bit unsigned integer, representing the knob signal.
     */
    void AdvanceState(uint8_t input);

private:
    /// Enum representing possible states of the knob state machine.
    typedef enum { Inactive, Falling, Active, Rising } State;

    State currentState = Inactive; ///< Current state of the knob state machine.
    int8_t currentOutput = 0; ///< Current output value representing edge transitions.

    /**
     * @struct Transition
     * @brief Structure representing a state transition for the knob state machine.
     *
     * Each transition defines a starting state, an input condition, a resulting next state,
     * and an output value indicating the type of edge transition.
     */
    struct Transition {
        State currentState; ///< The current state before the transition.
        uint8_t input; ///< The input value triggering the transition.
        State nextState; ///< The state to transition to based on the input.
        int8_t output; ///< Output value: -1 for falling edge, 1 for rising edge, 0 for no change.
    };

    /// Transition table containing all possible transitions for the knob state machine.
    Transition transitionTable[8] = {
        { Inactive, 1, Inactive, 0 },      // Input high (inactive)
        { Inactive, 0, Falling, -1 },      // Falling edge detected, goes to Falling
        { Falling, 1, Inactive, 0 },       // Rising edge resets to Inactive
        { Falling, 0, Active, 1 },         // Transition to Active on low input
        { Active, 1, Inactive, 0 },        // Goes inactive if input is high
        { Active, 0, Active, 1 },          // Stay Active on continuous low input
        { Rising, 1, Inactive, 0 },        // Reset to Inactive on rising edge
        { Rising, 0, Active, 1 }           // Continue Active on falling edge
    };

    const uint8_t TOTAL_TRANSITIONS = 8; ///< Total number of transitions in the table.
};

#endif /* GIT_PROJECTB_KNOBSTATEMACHINE_H_ */

