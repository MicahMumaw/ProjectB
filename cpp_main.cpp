/*
 * cpp_main.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "main.h"
#include "output_driver.h"
#include "Queue.h"
#include "input_driver.h"
#include "Semaphore.h"
#include "mem_check.h"

extern "C" void cpp_main(void);

static Semaphore semaphore;

void cpp_main(void)
{
	Queue OutputQueue;
	mem_check check1;

	Queue3 DisplayQueue;
	mem_check check2;

    InputQueue inputQueue;
    mem_check check3;

    DAC_Driver DAC_D(&OutputQueue, &DisplayQueue);

    OutputData test1 = {1, 1, 1000, 100, 0, 0}; //wave num(does not change), wave choice(1, 2, 3), freq(0-1000), amp(0-100), phase shift(0-255), follow mode(0-1)
    OutputData test2 = {2, 3, 564, 75, 256, 1}; //FOR TESTING ONLY

	OutputQueue.enqueue(test1);
	OutputQueue.enqueue(test2);

	DAC_D.generate_wave();
	DAC_D.generate_wave();

    // Create a DisplayDriver instance
    DisplayDriver display(&hspi1, &DisplayQueue);

    // Initialize the display
    display.writeValues();
    display.writeValues();

    InputDriver inputDriver(&inputQueue,&semaphore, LL_GPIO_PIN_9, LL_GPIO_PIN_10, LL_GPIO_PIN_12, LL_GPIO_PIN_2,
    		LL_GPIO_PIN_0, LL_GPIO_PIN_3, LL_GPIO_PIN_11, LL_GPIO_PIN_8, LL_GPIO_PIN_6);
    while(1)
    {
    	inputDriver.update();
    	//connect.connect_IO();
    	//check1.check_memory();
    	//check2.check_memory();
    	//check3.check_memory();
    }
}
