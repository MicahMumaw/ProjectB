/*
 * output1.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "main.h"
#include "output_driver.h"

DAC_Driver::DAC_Driver()
{
	void get_sineval(uint32_t sine_val[]);
}

void get_sineval(uint32_t sine_val[])
{
	for (int i = 0; i < MAX_SAMPLES; i++)
	{
		sine_val[i] = ((sin(i * 2 * pi / MAX_SAMPLES) + 1)) * res_12b / 2;
	}
}

void DAC_Driver::test(){

}
