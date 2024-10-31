/*
 * cpp_main.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "main.h"
#include "output_driver.h"
#include "Queue.h"

extern "C" void cpp_main(void);

void cpp_main(void)
{
	Queue OutputQueue;
    DAC_Driver DAC_D(&OutputQueue);

    OutputData test1 = {1, 1, 100, 75}; //FOR TESTING ONLY
    OutputData test2 = {2, 2, 200, 50}; //FOR TESTING ONLY

	OutputQueue.enqueue(test1);
	OutputQueue.enqueue(test2);
	DAC_D.generate_wave();
	DAC_D.generate_wave();

    // Create a DisplayDriver instance
    DisplayDriver display(&hspi1);

    // Initialize the display
    display.init();
    display.update();
    while(1)
    {

    }
}
