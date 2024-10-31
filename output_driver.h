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

class DAC_Driver{ // @suppress("Miss copy constructor or assignment operator")
public:
	enum wave_choice {SIN = 1, SQUARE = 2, TRIANGLE = 3, empty = 4};

private:
    int32_t amplitude_peak, phase_shift, offset, wave_choice1, wave_choice2, frequency1, frequency2, amplitude1, amplitude2;
    bool follow_mode;
	uint32_t Wave1_LUT[NS], Wave2_LUT[NS], Sin_Wave_LUT[NS], Square_Wave_LUT[NS], Triangle_Wave_LUT[NS], Follow_Wave_LUT[NS], temp[NS];
	Queue* queue;


public:
//Constructor
	DAC_Driver(Queue*);

	void generate_wave();
};

class DisplayDriver{

private:
//    const uint16_t A0_Pin = GPIO_PIN_4;  // Command/Data
//    GPIO_TypeDef* A0_Port = GPIOB;

    const uint16_t RST_Pin = GPIO_PIN_5; // Reset
    GPIO_TypeDef* RST_Port = GPIOB;

    const uint16_t CS_Pin = GPIO_PIN_6;  // Chip Select
    GPIO_TypeDef* CS_Port = GPIOB;

    SPI_HandleTypeDef* hspi;             // SPI handler

    uint8_t byteArray[3];

    uint8_t lineSelect[4] =
    {
    	0x08 | 0x00,
		0x80 | 0x20,
		0x80 | 0x40,
		0x80 | 0x60
    };

public:
    // Constructor
    DisplayDriver(SPI_HandleTypeDef* spiHandle);

    // Public methods
    void init();                         // Initialize the display
    void writeCommand(uint8_t command);  // Write command to display
    void writeData(uint8_t data);        // Write data to display
    void writeString(const char* str);   // Write string to display
    void update();                       // Placeholder function (can be used for further display updates)
};


#endif /* PROJECTB_OUTPUT_DRIVER_H_ */
