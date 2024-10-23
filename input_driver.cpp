/*
 * input_driver.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "input_driver.h"

InputDriver::InputDriver(InputQueue* q, Semaphore* sem, int pinA1, int pinB1, int pinButton1,
                         int pinA2, int pinB2, int pinButton2,int pinA3, int pinB3, int pinButton3)
{
	queue = q;
	semaphore = sem;

    this->pinA1 = pinA1;
    this->pinB1 = pinB1;
    this->pinButton1 = pinButton1;

    this->pinA2 = pinA2;
    this->pinB2 = pinB2;
    this->pinButton2 = pinButton2;

    this->pinA3 = pinA3;
    this->pinB3 = pinB3;
    this->pinButton3 = pinButton3;

    lastB1 = lastB2 = lastB3 = false;
    lastButton1 = lastButton2 = lastButton3 = false;

    stateButton1 = RELEASED;
    stateButton2 = RELEASED;
    stateButton3 = RELEASED;
}

void InputDriver::update()
{

    if (semaphore && !semaphore->tryWait())
    {
        return;  // exit early if semaphore isn't ready
    }

    InputData inputData;

    // read knob values and store in inputData
    inputData.frequency = readKnob(pinA1, pinB1, lastB1);
    inputData.amplitude = readKnob(pinA2, pinB2, lastB2);
    inputData.phase_shift = readKnob(pinA3, pinB3, lastB3);

    // read button values
    bool pressed1 = false, pressed2 = false, pressed3 = false;
    updateButtonState(pinButton1, lastButton1, stateButton1, pressed1);
    updateButtonState(pinButton2, lastButton2, stateButton2, pressed2);
    updateButtonState(pinButton3, lastButton3, stateButton3, pressed3);

    //store button values in inputData
    inputData.wave_choice = pressed1;
    inputData.wave_select = pressed2;
    inputData.echo_mode = pressed3;

    // enqueue the struct with updated input data
    queue->enqueue(inputData);
}

int InputDriver::readKnob(int pinA, int pinB, bool &lastB)
{
    bool A_state = LL_GPIO_IsInputPinSet(GPIOA, pinA);
    bool B_state = LL_GPIO_IsInputPinSet(GPIOA, pinB);

    int direction = 0;

    // detecting whether cw or ccw
    if (!lastB && B_state && A_state)
    {
        direction = 1;  // cw
    }
    else if (!lastB && B_state && !A_state)
    {
        direction = -1;  // ccw
    }

    lastB = B_state;
    return direction;
}

void InputDriver::updateButtonState(int pinButton, bool &lastButton, ButtonState &state, bool &pressed)
{
    bool currentButtonState = LL_GPIO_IsInputPinSet(GPIOA, pinButton);

    switch (state)
    {
    case RELEASED:
        if (currentButtonState)
        {
            state = PRESSED;  // move to PRESSED state
        }
        break;

    case PRESSED:
        if (currentButtonState)
        {
            state = DEBOUNCING;  // move to DEBOUNCING state
        }
        else
        {
            state = RELEASED;  // move to RELEASED state
        }
        break;

    case DEBOUNCING:
        if (!currentButtonState)
        {
            pressed = true;  // button was pressed
            state = RELEASED;  // return back to RELEASED state
        }
        break;
    }

    lastButton = currentButtonState;  // update last state
}










