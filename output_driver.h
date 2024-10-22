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
#define PI 3.14159265358979

class DAC_Driver{
public:
	enum wave_choice {SIN = 1, SQUARE = 2, TRIANGLE = 3, empty = 4};

private:
    int32_t peak_amplitude;
    int32_t offset;
	uint32_t Wave_LUT[NS], Sin_Wave_LUT[NS], Square_Wave_LUT[NS], Triangle_Wave_LUT[NS];
	Queue* queue;

public:
//Constructor
	DAC_Driver(Queue*);

	void generate_wave();
};

class DisplayDriver{

private:

public:
	void update();
};


#endif /* PROJECTB_OUTPUT_DRIVER_H_ */
