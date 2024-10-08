/*
 * output1.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "output_driver.h"

extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;


DAC_Driver::DAC_Driver()//Queue* OutputQueue
{
    uint32_t wave[NS] =
    {
        2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355,
        3431, 3504, 3574, 3639, 3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076,
        4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026, 3998, 3965, 3927, 3884, 3837, 3786, 3730,
        3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790, 2695, 2598, 2500,
        2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031,
        944, 860, 779, 701, 627, 556, 488, 424, 365, 309, 258, 211, 168, 130, 97,
        69, 45, 26, 13, 4, 0, 1, 8, 19, 35, 56, 82, 113, 149, 189,
        234, 283, 336, 394, 456, 521, 591, 664, 740, 820, 902, 987, 1075, 1166, 1258,
        1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047
    };
    for (int i = 0; i < NS; ++i) {
    	Sin_Wave_LUT[i] = wave[i];
    }

    //queue = OutputQueue;

//	for (int i = 0; i < NS; i++)
//	{
//	    Wave_LUT[i] = (uint32_t)(2048 + 2047 * sin((2 * M_PI * i) / NS));
//	}
}

void DAC_Driver::generate_sin(int freq)
{
	//int32_t freq;
	//queue->dequeue(&freq);
//	if (freq == 0)
//	{
//		return;
//	}
//	else
//	{
//		return;
//	}

	__HAL_TIM_SET_AUTORELOAD(&htim2, 1 / (freq*NS / (80*(1e6))));
	//HAL_TIM_Base_Init(&htim2);
	HAL_TIM_Base_Start(&htim2);
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, Sin_Wave_LUT, NS, DAC_ALIGN_12B_R);

}

void generate_square()
{
	return;
}

void generate_triangle()
{
	return;
}

void generate_delay()
{
	return;
}
