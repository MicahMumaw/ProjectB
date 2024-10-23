/*
 * input_driver.h
 *
 *  Created on: Oct 1, 2024
 *      Author: eegge
 */
#ifndef INPUT_DRIVER_H_
#define INPUT_DRIVER_H_

#include "InputQueue.h"
#include "stm32l4xx_ll_gpio.h"
//#include "Semaphore.h"

enum ButtonState
{
    RELEASED,
    PRESSED,
    DEBOUNCING
};

class InputDriver
{
private:
    InputQueue* queue;
    Semaphore* semaphore;  //
    int pinA1, pinB1;  // knob 1 pins
    int pinA2, pinB2;  // knob 2 pins
    int pinA3, pinB3;  // knob 3 pins
    int pinButton1, pinButton2, pinButton3;  // button pins

    bool lastB1, lastB2, lastB3;  // last states for edge detection (knobs)
    bool lastButton1, lastButton2, lastButton3;  // last states for buttons

    ButtonState stateButton1, stateButton2, stateButton3;

public:
    InputDriver(InputQueue* q, Semaphore* sem,
                int pinA1, int pinB1, int pinButton1,
                int pinA2, int pinB2, int pinButton2,
                int pinA3, int pinB3, int pinButton3);

    void update();  // main update function to read knob and button inputs

    int readKnob(int pinA, int pinB, bool &lastB);
    bool readButton(int pinButton, bool &lastButton);
    void updateButtonState(int pinButton, bool &lastButton, ButtonState &state, bool &pressed);
};

#endif /* GIT_PROJECTB_INPUT_DRIVER_H_ */
