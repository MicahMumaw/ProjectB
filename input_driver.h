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
#include "StateMachine.h"
#include "Semaphore.h"
#include "KnobStateMachine.h"
#include "stm32l4xx_hal.h"
#include <cassert>

class InputDriver
{
private:
    InputQueue* queue;
    Semaphore* semaphore;
    int pinA1, pinB1;  // knob 1 pins
    int pinA2, pinB2;  // knob 2 pins
    int pinA3, pinB3;  // knob 3 pins
    int pinButton1, pinButton2, pinButton3, pinButton4;  // button pins

    bool lastB1, lastB2, lastB3;  // last states for edge detection (knobs)


    StateMachine button1StateMachine;
    StateMachine button2StateMachine;
    StateMachine button3StateMachine;
    StateMachine button4StateMachine;

    KnobStateMachine stateMachineA;  // state machine for Pin A
    KnobStateMachine stateMachineB;  // state machine for Pin B


    const uint16_t phase_shiftA = LL_GPIO_PIN_0;
    const uint16_t phase_shiftB = LL_GPIO_PIN_0;
    GPIO_TypeDef* phase_shift_port = GPIOA;

    const uint16_t F_knobA = LL_GPIO_PIN_9;
    const uint16_t F_knobB = LL_GPIO_PIN_10;
    GPIO_TypeDef* F_knob_port = GPIOA;

    const uint16_t A_knobA = LL_GPIO_PIN_12;
    const uint16_t A_knobB = LL_GPIO_PIN_2;
    GPIO_TypeDef* A_knob_port = GPIOA;

public:
    InputDriver(InputQueue* q, Semaphore* sem,
    		uint32_t pinA1, uint32_t pinB1,
			uint32_t pinA2, uint32_t pinB2,
			uint32_t pinA3, uint32_t pinB3,
			uint32_t pinButton1,uint32_t pinButton2,
			uint32_t pinButton3,uint32_t pinButton4);

    void update();
    int8_t readKnob(int pinA, int pinB);
};



#endif /* GIT_PROJECTB_INPUT_DRIVER_H_ */
