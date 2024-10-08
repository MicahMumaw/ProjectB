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
	//Queue OutputQueue;
    DAC_Driver DAC_D;//&OutputQueue


    //OutputQueue.enqueue(1000);
    while(1)
    {
    DAC_D.generate_sin(1000); //624999 for 1hz 625 for 1kHz
    }
}
