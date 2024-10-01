/*
 * output_driver.h
 *
 *  Created on: Sep 28, 2024
 *      Author: micah
 */

#include "main.h"
#include "math.h"

#ifndef PROJECTB_OUTPUT_DRIVER_H_
#define PROJECTB_OUTPUT_DRIVER_H_

#define pi 3.1415926579
#define MAX_SAMPLES 10
#define res_8b 256
#define res_12b 4096


class DAC_Driver{
private:
	uint32_t sine_val[MAX_SAMPLES];


public:
//Constructor
	DAC_Driver();
//Assign sine_val values;
	void get_sineval(uint32_t);

	void test();

};

class DisplayDriver{

private:

public:

};


#endif /* PROJECTB_OUTPUT_DRIVER_H_ */
