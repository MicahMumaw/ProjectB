/*
 * StateMachine.h
 *
 *  Created on: Oct 28, 2024
 *      Author: eegge
 */

/**
 * @file StateMachine.h
 * @brief Header file defining the StateMachine class for managing state transitions based on input.
 *
 * This file contains the definition of the `StateMachine` class, which implements a simple
 * finite state machine with states and transitions for handling button press events.
 *
 * @date Created on: Oct 28, 2024
 * @author eegge
 */

#ifndef GIT_PROJECTB_STATEMACHINE_H_
#define GIT_PROJECTB_STATEMACHINE_H_

#include <stdint.h>

/**
 * @class StateMachine
 * @brief Implements a finite state machine to detect button press events and handle transitions.
 *
 * The `StateMachine` class manages state transitions based on input signals. It includes an
 * output function to retrieve the current output and a method to advance the state based on
 * input values, using an internal transition table.
 */
class StateMachine
{
public:
    /**
     * @brief Constructs a StateMachine object.
     *
     * Initializes the state machine to the inactive state with an initial output of 0.
     */
    StateMachine();

    /**
     * @brief Retrieves the current output of the state machine.
     *
     * @return The current output value as an 8-bit unsigned integer.
     */
    uint8_t Output();

    /**
     * @brief Advances the state of the state machine based on the input.
     *
     * Changes the current state based on the input value, using the predefined transition table.
     * @param input The input value as an 8-bit unsigned integer, which affects the state transition.
     */
    void AdvanceState(uint8_t input);

private:
    /// Enum representing possible states of the state machine.
    typedef enum {Inactive, Falling, Active, Rising} State;

    State currentState = Inactive; ///< Current state of the state machine.
    uint8_t currentOutput = 0; ///< Current output value of the state machine.

    /**
     * @struct Transition
     * @brief Structure representing a transition in the state machine.
     *
     * Each transition defines a starting state, an input condition, a resulting next state,
     * and an output value.
     */
    struct Transition {
        State currentState; ///< The current state before the transition.
        uint8_t input; ///< The input value triggering the transition.
        State nextState; ///< The state to transition to based on the input.
        uint8_t output; ///< The output value after the transition.
    };

    /// Transition table containing all possible transitions for the state machine.
    Transition transitionTable[8] = {
        {Inactive, 1, Inactive, 0},
        {Inactive, 0, Falling, 0},
        {Falling, 1, Inactive, 0},
        {Falling, 0, Active, 1},
        {Active, 1, Rising, 0},
        {Active, 0, Active, 1},
        {Rising, 1, Inactive, 0},
        {Rising, 0, Active, 1}
    };

    const uint8_t TOTAL_STATES = 8; ///< Total number of states in the transition table.
};

#endif /* GIT_PROJECTB_STATEMACHINE_H_ */
