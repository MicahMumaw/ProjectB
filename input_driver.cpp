/*
 * input_driver.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */
#include "input_driver.h"

InputDriver::InputDriver(Queue* q, Semaphore* sem, int pinA, int pinB)
{
	queue=q;
	semaphore=sem;
	counter=0;
	this->pinA =pinA;
	this->pinB =pinB;
}



void InputDriver::update()
{

    if (!semaphore->tryWait()) //waits for post (debouncing) returns right away if no flag
    {
        return;
    }

    int currentState = (HAL_GPIO_ReadPin(GPIOA, pinA) << 1) | HAL_GPIO_ReadPin(GPIOA, pinB);

    int direction=0;

    if (lastState == 0b00 && currentState == 0b01)
    {
    	direction = 1;  // cw
    }
    else if (lastState == 0b01 && currentState == 0b00)
    {
        direction = -1;  // counter cw
    }

    lastState = currentState;

    if (direction == 1)
    {
    	incrementCounter(); //cw
    }

    else if (direction == -1)
    {
    	decrementCounter(); //counter cw
    }
}


void InputDriver::incrementCounter()
{
    counter++;
    queue->enqueue(counter);
}


void InputDriver::decrementCounter()
{
    counter--;
    queue->enqueue(counter);
}


void InputDriver::getCounter(int *counterValue)
{
    *counterValue = counter;

}







