/*
 * output_driver.h
 *
 *  Created on: Sep 28, 2024
 *      Author: micah
 */

#include "main.h"
#include "math.h"
#include <cstdint>
#include "stm32l4xx_hal.h"
#include "Queue.h"

#ifndef PROJECTB_OUTPUT_DRIVER_H_
#define PROJECTB_OUTPUT_DRIVER_H_

#define NS 128

class DAC_Driver{
private:
	uint32_t Sin_Wave_LUT[NS];
	//Queue* queue;

public:
//Constructor
	//DAC_Driver(Queue*);
	DAC_Driver();

	void generate_sin(int);
	void generate_square();
	void generate_triangle();
	void generate_delaywave();
};

class DisplayDriver{

private:

public:
	void update();
};


#endif /* PROJECTB_OUTPUT_DRIVER_H_ */
