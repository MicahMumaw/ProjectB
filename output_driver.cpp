/*
 * output1.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "output_driver.h"

extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;

DAC_Driver::DAC_Driver(Queue* OutputQueue)
{
	queue = OutputQueue;
	amplitude_peak = 2047;
	offset = 2048;
	phase_shift = 0;
	follow_mode = false;
	wave_choice1 = 1, wave_choice2 = 1;
	frequency1 = 500, frequency2 = 500;
	amplitude1 = 100, amplitude2 = 100;

	//Sin Wave LUT generation
    for (int i = 0; i < NS; ++i)
    {
        Sin_Wave_LUT[i] = (uint32_t)(amplitude_peak * sin(2 * PI * i / NS) + amplitude_peak);
    }

    //Square Wave LUT generation
    for (int i = 0; i < NS; ++i)
    {
        if (i < NS / 2)
        {
            Square_Wave_LUT[i] = amplitude_peak + amplitude_peak;
        }
        else
        {
            Square_Wave_LUT[i] = 0;
        }
    }

    //Triangle Wave LUT generation
    for (int i = 0; i < NS / 2; ++i) {
        Triangle_Wave_LUT[i] = (uint32_t)((amplitude_peak*2 * i) / (NS / 2));
    }

    for (int i = NS / 2; i < NS; ++i) {
        Triangle_Wave_LUT[i] = (uint32_t)((amplitude_peak*2 * (NS - i)) / (NS / 2));
    }

}
void DAC_Driver::generate_wave()
{
	OutputData input;
	if (queue->dequeue(&input))
	{
		if(input.wave_num == 1)
		{
			wave_choice1 = input.wave_choice;
			frequency1 = input.frequency;
			amplitude1 = input.amplitude;
			switch(input.wave_choice)
			{
				case 1:
					for (int i = 0; i < NS; ++i)
					{
						Wave1_LUT[i] = (Sin_Wave_LUT[i]*input.amplitude)/100;
					}
				break;

				case 2:
					for (int i = 0; i < NS; ++i)
					{
						Wave1_LUT[i] = (Square_Wave_LUT[i]*input.amplitude)/100;
					}
				break;

				case 3:
					for (int i = 0; i < NS; ++i)
					{
						Wave1_LUT[i] = (Triangle_Wave_LUT[i]*input.amplitude)/100;
					}
				break;

				default:
				break;
			}
		}
		else if (input.wave_num == 2)
		{
			wave_choice2 = input.wave_choice;
			frequency2 = input.frequency;
			amplitude2 = input.amplitude;
			switch(input.wave_choice)
			{
				case 1:
					for (int i = 0; i < NS; ++i)
					{
						Wave2_LUT[i] = (Sin_Wave_LUT[i]*input.amplitude)/100;
					}
				break;

				case 2:
					for (int i = 0; i < NS; ++i)
					{
						Wave2_LUT[i] = (Square_Wave_LUT[i]*input.amplitude)/100;
					}
				break;

				case 3:
					for (int i = 0; i < NS; ++i)
					{
						Wave2_LUT[i] = (Triangle_Wave_LUT[i]*input.amplitude)/100;
					}
				break;

				default:
				break;
			}

		}

		if(follow_mode)//true
		{
			for(int i = 0; i < NS; i++)
			{
				Wave2_LUT[i] = Wave1_LUT[i];
				frequency2 = frequency1;
			}
		}
		else if(!follow_mode && phase_shift != 0)//false
		{
			for(int i = 0; i < NS; i++)
			{
				if(i + phase_shift <= 128)
				{
					Wave2_LUT[i + phase_shift] = Wave1_LUT[i];
				}
				else
				{
					Wave2_LUT[i + phase_shift - NS] = Wave1_LUT[i];
				}
			}
		}
		if (input.frequency == 0)
		{
			return;
		}

		//Start timers with updated periods and DACs with updated tables
		if(input.wave_num == 1)
		{
			__HAL_TIM_SET_AUTORELOAD(&htim2, 1 / ((frequency1*NS) / (80*(1e6))));
			HAL_TIM_Base_Start(&htim2);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, Wave1_LUT, NS, DAC_ALIGN_12B_R);
		}
		else if(input.wave_num == 2)
		{
			__HAL_TIM_SET_AUTORELOAD(&htim7, 1 / ((frequency2*NS) / (80*(1e6))));
			HAL_TIM_Base_Start(&htim7);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, Wave2_LUT, NS, DAC_ALIGN_12B_R);
		}
	}
}

DisplayDriver::DisplayDriver(SPI_HandleTypeDef* spiHandle)
{
    hspi = spiHandle;

}

// Initialize the EA DOGS164W-A display
void DisplayDriver::init()
{
    uint8_t initCommandNum = 11;
    uint8_t initCommandArray[initCommandNum] =
    {
    	0x3A,
    	0x09,
		0x06,
		0x1E,
		0x39,
		0x1B,
		0x6C,
		0x56,
		0x7A,
		0x38,
		0x0F, //display on, cursor on, cursor blink
    };

    GPIOB->ODR |= 1 << 6;
	HAL_GPIO_WritePin(RST_Port, RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
    HAL_GPIO_WritePin(RST_Port, RST_Pin, GPIO_PIN_RESET);
    for(int i = 0; i < 720; i++);
    HAL_GPIO_WritePin(RST_Port, RST_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    //Display initialization sequence
    for(int i = 0; i < 11; i++)
    {
    	writeCommand(initCommandArray[i]);
    }
}

// Write a command to the display
void DisplayDriver::writeCommand(uint8_t command)
{
	GPIOB->ODR &= ~(1 << 6);	// Select the display
	byteArray[0] = 0x1F;
	byteArray[1] = command & 0x0F;
	byteArray[2] = command >> 4;
    HAL_SPI_Transmit(hspi, (uint8_t*)byteArray, 3, HAL_MAX_DELAY);    	// Send the command
    GPIOB->ODR |= 1 << 6; 												//Deselect the display
}

// Write data to the display
void DisplayDriver::writeData(uint8_t data)
{
	GPIOB->ODR &= ~(1 << 6);	// Select the display
	byteArray[0] = 0x5F;
	byteArray[1] = data & 0x0F;
	byteArray[2] = data >> 4;
    HAL_SPI_Transmit(hspi, (uint8_t*)byteArray, 3, HAL_MAX_DELAY);    	// Send the command
    GPIOB->ODR |= 1 << 6;
}

// Write a string to the display
void DisplayDriver::writeString(const char* str)
{
    while (*str)
    {
        writeData(*str++);  // Send each character as data
    }
}

void DisplayDriver::update()
{
	for(int i = 0; i < 4; i++)
	{
		writeCommand(lineSelect[i]);
		writeData(0x30);
		writeData(0x30);
		writeData(0x30);
		writeData(0x30);
	}
}
