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

    OutputData test1 = {3, 1000, 75}; //FOR TESTING ONLY

	OutputQueue.enqueue(test1);
	DAC_D.generate_wave();

    while(1)
    {

    }
}
