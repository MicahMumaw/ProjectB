/*
 * input_driver.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */
/**
 * @file InputDriver.cpp
 * @brief Implementation of the InputDriver class.
 *
 * This class is responsible for managing the input from various knobs and buttons.
 * It reads the values of the knobs and buttons, processes the input, and enqueues
 * the data for further processing.
 *
 * @date Created on: Nov 3, 2024
 * @author eegge
 */
#include "input_driver.h"
/**
 * @brief Constructs an InputDriver object.
 *
 * Initializes the InputDriver with the provided parameters for the input queue,
 * semaphore, knob pins, and button pins.
 *
 * @param q Pointer to the InputQueue object for handling input data.
 * @param sem Pointer to the Semaphore object for synchronization.
 * @param pinA1 Pin number for the first knob's A signal.
 * @param pinB1 Pin number for the first knob's B signal.
 * @param pinA2 Pin number for the second knob's A signal.
 * @param pinB2 Pin number for the second knob's B signal.
 * @param pinA3 Pin number for the third knob's A signal.
 * @param pinB3 Pin number for the third knob's B signal.
 * @param pinButton1 Pin number for the first button.
 * @param pinButton2 Pin number for the second button.
 * @param pinButton3 Pin number for the third button.
 * @param pinButton4 Pin number for the fourth button.
 */
InputDriver::InputDriver(InputQueue* q, Semaphore* sem, uint32_t pinA1, uint32_t pinB1, // @suppress("Symbol shadowing")
		uint32_t pinA2, uint32_t pinB2,uint32_t pinA3, uint32_t pinB3,  uint32_t pinButton1,uint32_t pinButton2,uint32_t pinButton3,uint32_t pinButton4) // @suppress("Symbol shadowing")
{
	queue = q;
	semaphore = sem;

    this->pinA1 = pinA1;
    this->pinB1 = pinB1;


    this->pinA2 = pinA2;
    this->pinB2 = pinB2;


    this->pinA3 = pinA3;
    this->pinB3 = pinB3;

    this->pinButton1 = pinButton1;
    this->pinButton2 = pinButton2;
    this->pinButton3 = pinButton3;
    this->pinButton4 = pinButton4;

}
/**
 * @brief Updates the input driver state and processes input from knobs and buttons.
 *
 * This method reads the current state of the knobs and buttons, updates the input data,
 * and enqueues it if there are changes. It utilizes state machines for each button to
 * determine their current states.
 */
void InputDriver::update()
{
	LL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
    if (semaphore->tryWait())
    {
        return;  // exit early if semaphore isn't flagged
    }

    InputData inputData;

    // reads return values and make sure then are as expected. before placing them in struct
    inputData.freq_amp = readKnob(pinA1, pinB1);
    inputData.phase_shift = readKnob(pinA2, pinB2);

    int8_t freqAmpChange = readKnob(pinA1, pinB1);
       if (freqAmpChange >=-1 || freqAmpChange<=1)
       {
           inputData.freq_amp = freqAmpChange;
       }

       int8_t phaseShiftChange = readKnob(pinA2, pinB2);
       if (phaseShiftChange >=-1 || phaseShiftChange<=1)
       {
           inputData.phase_shift = phaseShiftChange;
       }


    // read button values
    button1StateMachine.AdvanceState(LL_GPIO_IsInputPinSet(GPIOA, pinButton1) ? 1 : 0);
    button2StateMachine.AdvanceState(LL_GPIO_IsInputPinSet(GPIOA, pinButton2) ? 1 : 0);
    button3StateMachine.AdvanceState(LL_GPIO_IsInputPinSet(GPIOA, pinButton3) ? 1 : 0);
    button4StateMachine.AdvanceState(LL_GPIO_IsInputPinSet(GPIOA, pinButton4) ? 1 : 0);

    // Store button values in inputData struct
    inputData.wave_choice = button1StateMachine.Output();
    inputData.wave_select = button2StateMachine.Output();
    inputData.echo_mode = button3StateMachine.Output();
    inputData.select_freqamp = button4StateMachine.Output();

    // Assertions
    //assert(inputData.freq_amp >= -1 && inputData.freq_amp <= 1);
    //assert(inputData.phase_shift >= -1 && inputData.phase_shift <= 1);
    //assert(inputData.wave_choice == 0 || inputData.wave_choice == 1);
    //assert(inputData.wave_select == 0 || inputData.wave_select == 1);
    //assert(inputData.echo_mode == 0 || inputData.echo_mode == 1);
    //assert(inputData.select_freqamp == 0 || inputData.select_freqamp == 1);

    // Enqueue the struct with updated input data only if a change occurs
    if (inputData.freq_amp != 0 || inputData.phase_shift != 0 || inputData.select_freqamp ||
        inputData.wave_choice || inputData.wave_select || inputData.echo_mode)
    {
        queue->enqueue(inputData);
    }
}
/**
 * @brief Reads the state of a knob based on its A and B signal pins.
 *
 * This method uses state machines to determine the direction of the knob's rotation.
 *
 * @param pinA The pin number for the knob's A signal.
 * @param pinB The pin number for the knob's B signal.
 * @return
 *   - 1 if the knob is rotated clockwise,
 *   - -1 if rotated counterclockwise,
 *   - 0 if there is no rotation.
 */
int8_t InputDriver::readKnob(int pinA, int pinB)
{
    stateMachineA.AdvanceState(LL_GPIO_IsInputPinSet(GPIOA, pinA) ? 1 : 0);
    stateMachineB.AdvanceState(LL_GPIO_IsInputPinSet(GPIOA, pinB) ? 1 : 0);

    int8_t outputA = stateMachineA.Output();
    int8_t outputB = stateMachineB.Output();

    if (outputA == -1 && outputB == 0)
    {
        return 1; // Clockwise
    }
    else if (outputA == -1 && outputB == -1)
    {
        return -1; // Counterclockwise
    }

    return 0; // No rotation
}











