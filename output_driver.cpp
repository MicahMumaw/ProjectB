/*
 * output1.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "output_driver.h"

extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;



DAC_Driver::DAC_Driver(Queue* OutputQueue)
{
	queue = OutputQueue;
	peak_amplitude = 2047;
	offset = 2048;

	//Sin Wave LUT generation
    for (int i = 0; i < NS; ++i)
    {
        Sin_Wave_LUT[i] = (uint32_t)(peak_amplitude * sin(2 * PI * i / NS) + offset);
    }

    //Square Wave LUT generation
    for (int i = 0; i < NS; ++i)
    {
        if (i < NS / 2)
        {
            Square_Wave_LUT[i] = peak_amplitude + offset;
        }
        else
        {
            Square_Wave_LUT[i] = 0;
        }
    }

    //Triangle Wave LUT generation
    for (int i = 0; i < NS / 2; ++i) {
        Triangle_Wave_LUT[i] = (uint32_t)((peak_amplitude*2 * i) / (NS / 2));
    }

    for (int i = NS / 2; i < NS; ++i) {
        Triangle_Wave_LUT[i] = (uint32_t)((peak_amplitude*2 * (NS - i)) / (NS / 2));
    }

}
void DAC_Driver::generate_wave()
{
	OutputData input;
	if (queue->dequeue(&input))
	{
		switch(input.wave_choice)
		{
			case 1:
			    for (int i = 0; i < NS; ++i)
			    {
			    	Wave_LUT[i] = (Sin_Wave_LUT[i]*input.amplitude)/100;
			    }
			break;

			case 2:
		    	for (int i = 0; i < NS; ++i)
		    	{
		    		Wave_LUT[i] = (Square_Wave_LUT[i]*input.amplitude)/100;
		    	}
			break;

			case 3:
		    	for (int i = 0; i < NS; ++i)
		    	{
		    		Wave_LUT[i] = (Triangle_Wave_LUT[i]*input.amplitude)/100;
		    	}
			break;

			default:

			break;
		}
		if (input.frequency == 0)
		{
			return;
		}
		else
		{
			__HAL_TIM_SET_AUTORELOAD(&htim2, 1 / (input.frequency*NS / (80*(1e6))));
			HAL_TIM_Base_Start(&htim2);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, Wave_LUT, NS, DAC_ALIGN_12B_R);
			return;
		}
	}
}
