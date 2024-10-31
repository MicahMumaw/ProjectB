///*
// * input_driver.cpp
// *
// *  Created on: Sep 24, 2024
// *      Author: micah
// */
//#include "input_driver.h"
//
//InputDriver::InputDriver(Queue* q, Semaphore* sem, int pinA, int pinB)
//{
//	queue=q;
//	semaphore=sem;
//	counter=0;
//	this->pinA =pinA;
//	this->pinB =pinB;
//}
//
//void InputDriver::update()
//{
//
//    if (!semaphore->tryWait()) //waits for post (debouncing) returns right away if no flag
//    {
//        return;
//    }
//
//    bool A_state = LL_GPIO_ISInputPinSet(GPIOA, pinA);
//    bool B_state = LL_GPIO_ISInputPinSet(GPIOA, pinB);
//
//    int direction = 0;
//
//    // clockwise: rising edge of B with A high
//    if (!lastB && B_state && A_state)
//    {
//        direction = 1;
//    }
//    // counterclockwise: rising edge of B with A low
//    else if (!lastB && B_state && !A_state)
//    {
//        direction = -1;
//    }
//
//    lastB = B_state; // updating last b allows to check for edges
//
//    if (direction == 1)
//    {
//        incrementCounter(); // cw
//    }
//    else if (direction == -1)
//    {
//        decrementCounter(); // ccw
//    }
//
//    bool currentButtonState = LL_GPIO_ISInputPinSet(GPIOA, pinButton);
//
//    if (!lastButtonState && currentButtonState)  // edge detection
//    {
//         handleButtonPress();
//    }
//
//    lastButtonState = currentButtonState;
//}
//
//
//
//void InputDriver::incrementCounter()
//{
//    counter++;
//    queue->enqueue(counter);
//}
//
//
//void InputDriver::decrementCounter()
//{
//    counter--;
//    queue->enqueue(counter);
//}
//
//
//void InputDriver::getCounter(int *counterValue)
//{
//    *counterValue = counter;
//
//}
//
//void InputDriver::handleButtonPress()
//{
//
//    int buttonPressFlag = 999;  // example value whatever teamates want in the queue for button press can change
//    queue->enqueue(buttonPressFlag);
//}
//
//
//
//
//
//
