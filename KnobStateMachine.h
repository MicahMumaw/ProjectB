/*
 * KnobStateMachine.h
 *
 *  Created on: Oct 28, 2024
 *      Author: eegge
 */

#ifndef GIT_PROJECTB_KNOBSTATEMACHINE_H_
#define GIT_PROJECTB_KNOBSTATEMACHINE_H_

#include <stdint.h>

class KnobStateMachine
{
public:
    KnobStateMachine();
    int8_t Output();
    void AdvanceState(uint8_t input);

private:
    typedef enum { Inactive, Falling, Active, Rising } State;
    State currentState = Inactive;
    int8_t currentOutput = 0;

    struct Transition {
        State currentState;
        uint8_t input;
        State nextState;
        int8_t output;  // -1 for falling edge, 1 for rising edge, 0 for no change
    };

    // Updated transition table to differentiate edge transitions
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


    const uint8_t TOTAL_TRANSITIONS = 8;
};



#endif /* GIT_PROJECTB_KNOBSTATEMACHINE_H_ */
