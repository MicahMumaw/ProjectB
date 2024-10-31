/*
 * StateMachine.h
 *
 *  Created on: Oct 28, 2024
 *      Author: eegge
 */

#ifndef GIT_PROJECTB_STATEMACHINE_H_
#define GIT_PROJECTB_STATEMACHINE_H_

#include <stdint.h>

#include <stdint.h>

class StateMachine
{
public:
    StateMachine();
    uint8_t Output();
    void AdvanceState(uint8_t input);

private:
    typedef enum {Inactive, Falling, Active, Rising} State;
    State currentState = Inactive;
    uint8_t currentOutput = 0;

    struct Transition {
        State currentState;
        uint8_t input;
        State nextState;
        uint8_t output;
    };

    // Updated transition table to detect falling edge for button press
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

    const uint8_t TOTAL_STATES = 8;
};




#endif /* GIT_PROJECTB_STATEMACHINE_H_ */
