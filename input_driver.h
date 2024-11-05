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

/**
 * @class InputDriver
 * @brief Manages input state processing for multiple knobs and buttons.
 *
 * The InputDriver class initializes and monitors state changes for a series of rotary knobs and buttons.
 * It uses state machines to handle edge detection for the knobs and button presses, and it queues events
 * for use by the main application.
 */
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


public:
    /**
         * @brief Constructs an InputDriver object with specified pins and shared resources.
         *
         * Initializes the InputDriver, setting up GPIO pins, state machines, and shared resources
         * such as the input queue and semaphore.
         *
         * @param q Pointer to the InputQueue for storing input events.
         * @param sem Pointer to the Semaphore for queue access synchronization.
         * @param pinA1 GPIO pin number for knob 1's A signal.
         * @param pinB1 GPIO pin number for knob 1's B signal.
         * @param pinA2 GPIO pin number for knob 2's A signal.
         * @param pinB2 GPIO pin number for knob 2's B signal.
         * @param pinA3 GPIO pin number for knob 3's A signal.
         * @param pinB3 GPIO pin number for knob 3's B signal.
         * @param pinButton1 GPIO pin number for button 1.
         * @param pinButton2 GPIO pin number for button 2.
         * @param pinButton3 GPIO pin number for button 3.
         * @param pinButton4 GPIO pin number for button 4.
         */
    InputDriver(InputQueue* q, Semaphore* sem,
    		uint32_t pinA1, uint32_t pinB1,
			uint32_t pinA2, uint32_t pinB2,
			uint32_t pinA3, uint32_t pinB3,
			uint32_t pinButton1,uint32_t pinButton2,
			uint32_t pinButton3,uint32_t pinButton4);

    /**
        * @brief Updates the states of all connected inputs (knobs and buttons).
        *
        * This function is called to check for any changes in the inputs' states.
        * It uses the state machines to process transitions for knobs and buttons and
        * enqueues events as necessary.
        */

    void update();

    /**
         * @brief Reads the rotation direction of a rotary knob based on A and B signals.
         *
         * This function analyzes the states of the specified A and B signals to determine
         * the direction of rotation for a rotary knob.
         *
         * @param pinA GPIO pin for the knob's A signal.
         * @param pinB GPIO pin for the knob's B signal.
         * @return An int8_t indicating direction: -1 for counterclockwise, 1 for clockwise, 0 for no movement.
         */

    int8_t readKnob(int pinA, int pinB);
};



#endif /* GIT_PROJECTB_INPUT_DRIVER_H_ */
