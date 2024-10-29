/*
 * StateMachine.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: eegge
 */

#include "statemachine.h"

StateMachine::StateMachine()
{

}

uint8_t StateMachine::Output()
{
    return currentOutput;
}


void StateMachine::AdvanceState(uint8_t input)
{
    for(uint8_t i = 0; i < TOTAL_STATES; i++)
    {
        if(transitionTable[i].currentState == currentState && transitionTable[i].input == input)
        {
            currentState = transitionTable[i].nextState;
            currentOutput = transitionTable[i].output;
            break;
        }
    }
}


