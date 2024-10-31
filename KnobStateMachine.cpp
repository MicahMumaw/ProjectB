/*
 * KnobStateMachine.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: eegge
 */

#include "KnobStateMachine.h"

KnobStateMachine::KnobStateMachine() : currentState(Inactive), currentOutput(0) {}

int8_t KnobStateMachine::Output() {
    return currentOutput;
}

void KnobStateMachine::AdvanceState(uint8_t input) {
    for (int i = 0; i < TOTAL_TRANSITIONS; i++) {
        if (transitionTable[i].currentState == currentState && transitionTable[i].input == input) {
            currentState = transitionTable[i].nextState;
            currentOutput = transitionTable[i].output;
            return;
        }
    }
    currentOutput = 0; // No change if no transition matched
}


