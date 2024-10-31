/*
 * input_driver.h
 *
 *  Created on: Oct 1, 2024
 *      Author: eegge
 */

#include "stm32l4xx_hal.h"
#include "Queue.h"
#include "application_layer.h"

class InputDriver
{
private:
    Queue* queue;
    int counter;
    Semaphore* semaphore;
    int pinA;
    int pinB;
    int lastB;
    int lastButtonState;

    void handleButtonPress();


public:

    InputDriver(Queue* q, Semaphore* sem, int pinA, int pinB, int pinButton);

    void update();

    void incrementCounter();
    void decrementCounter();

    void getCounter(int *counterValue);
};

#endif /* GIT_PROJECTB_INPUT_DRIVER_H_ */
