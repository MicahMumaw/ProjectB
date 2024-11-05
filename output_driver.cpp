/*
 * output_driver.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

#include "output_driver.h"

extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;

DAC_Driver::DAC_Driver(Queue* OutputQueue, Queue3* DisplayQueue)
{
	queue = OutputQueue;
	displayQueue = DisplayQueue;
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
    for (int i = 0; i < NS / 2; ++i)
    {
        Triangle_Wave_LUT[i] = (uint32_t)((amplitude_peak*2 * i) / (NS / 2));
    }
    for (int i = NS / 2; i < NS; ++i)
    {
        Triangle_Wave_LUT[i] = (uint32_t)((amplitude_peak*2 * (NS - i)) / (NS / 2));
    }
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim7);
}

void DAC_Driver::generate_wave()
{
    OutputData input;
    DisplayData output;

    if (queue->dequeue(&input))
    {
        processInput(input);

        if (input.wave_num == 1)
        {
            setupWaveLUT(Wave1_LUT, input.wave_choice, input.amplitude);
            wave_choice1 = input.wave_choice;
            frequency1 = input.frequency;
            amplitude1 = input.amplitude;
        }
        else if (input.wave_num == 2)
        {
            setupWaveLUT(Wave2_LUT, input.wave_choice, input.amplitude);
            wave_choice2 = input.wave_choice;
            frequency2 = input.frequency;
            amplitude2 = input.amplitude;
        }

        if (follow_mode)
        {
            applyFollowMode();
            output.wave_choice = wave_choice1;
            output.frequency = frequency1;
            output.amplitude = amplitude1;
        }
        else
        {
            output.wave_choice = input.wave_choice;
            output.frequency = input.frequency;
            output.amplitude = input.amplitude;
        }

        output.follow_mode = follow_mode;
        output.phase_shift = input.phase_shift;
        displayQueue->enqueue(output);

        setupTimers();
        startDAC();
    }
}

void DAC_Driver::processInput(const OutputData& input)
{
    phase_shift = input.phase_shift;
    //assert(phase_shift <= 256);
    follow_mode = input.follow_mode;

    //assert(input.frequency != 0 && input.frequency <= 1000);
    //assert(input.amplitude != 0 && input.amplitude <= 100);

    if (input.wave_num == 1)
    {
        //assert(input.wave_choice != 0 && input.wave_choice <= 3);
    }
}

void DAC_Driver::setupWaveLUT(uint32_t* waveLUT, int wave_choice, int amplitude)
{
    switch (wave_choice)
    {
        case 1: // Sine wave
            for (int i = 0; i < NS; ++i) {
                waveLUT[i] = (Sin_Wave_LUT[i] * amplitude) / 100;
            }
            break;
        case 2: // Square wave
            for (int i = 0; i < NS; ++i) {
                waveLUT[i] = (Square_Wave_LUT[i] * amplitude) / 100;
            }
            break;
        case 3: // Triangle wave
            for (int i = 0; i < NS; ++i) {
                waveLUT[i] = (Triangle_Wave_LUT[i] * amplitude) / 100;
            }
            break;
        default:
            break;
    }
}

void DAC_Driver::applyFollowMode()
{
    frequency2 = frequency1;
    for (int i = 0; i < NS; ++i)
    {
        if (i + phase_shift < NS)
        {
            Wave2_LUT[i + phase_shift] = Wave1_LUT[i];
        }
        else
        {
            Wave2_LUT[i + phase_shift - NS] = Wave1_LUT[i];
        }
    }
}

void DAC_Driver::setupTimers()
{
    __HAL_TIM_SET_AUTORELOAD(&htim2, (80 * (1e6) / (NS * frequency1 * 5)) - 1);
    __HAL_TIM_SET_AUTORELOAD(&htim7, (80 * (1e6) / (NS * frequency2 * 5)) - 1);
}

void DAC_Driver::startDAC()
{
    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, Wave1_LUT, NS, DAC_ALIGN_12B_R);

    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, Wave2_LUT, NS, DAC_ALIGN_12B_R);
}


//DISPLAY SECTION

DisplayDriver::DisplayDriver(SPI_HandleTypeDef* spiHandle, Queue3* DisplayQueue)
{
    hspi = spiHandle;
    displayQueue = DisplayQueue;
    degree_phase_shift = 0;

    //initialization sequence
	reset_display();
	write_command_array();
    clear_display();
    write_statics();
}

void DisplayDriver::writeCommand(uint8_t command)
{
	GPIOB->ODR &= ~(1 << 6);
	byteArray[0] = 0x1F;
	byteArray[1] = command & 0x0F;
	byteArray[2] = command >> 4;
    HAL_SPI_Transmit(hspi, (uint8_t*)byteArray, 3, 1000);
    GPIOB->ODR |= 1 << 6;
}

void DisplayDriver::writeData(uint8_t data)
{
	GPIOB->ODR &= ~(1 << 6);
	byteArray[0] = 0x5F;
	byteArray[1] = data & 0x0F;
	byteArray[2] = data >> 4;
    HAL_SPI_Transmit(hspi, (uint8_t*)byteArray, 3, 1000);
    GPIOB->ODR |= 1 << 6;
}

void DisplayDriver::reset_display()
{
    GPIOB->ODR &= ~(1 << 5);
    for(int i = 0; i < 1820; i++);
    GPIOB->ODR |= 1 << 5;
}

void DisplayDriver::write_command_array()
{
    for(int i = 0; i < 11; i++)
    {
    	writeCommand(initCommandArray[i]);
    }
}

void DisplayDriver::clear_display()
{
	writeCommand(0x80 | 0x00);
	for(int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			writeCommand(0x80 | (x + y * 0x20));
			writeData(0x20);
		}
	}
}


void DisplayDriver::write_statics()
{
    for(int i = 0; i < 4; i++)
    {
    	writeCommand(0x80 | (0x20 * i));
    	for(int j = 0; j < 12; j++)
    	{
    		writeData(parameters_LUT_hex[i][j]);
    	}
    }
}

void DisplayDriver::writeValues()
{
    DisplayData input;
    if (displayQueue->dequeue(&input))
    {
        writeWaveType(input.wave_choice);
        writeFrequency(input.frequency);
        writeAmplitude(input.amplitude);
        writePhaseShift(input.phase_shift);

        if (input.follow_mode == 1)
        {
            writeFollowModeIndicator();
        }
    }
}

void DisplayDriver::writeWaveType(int wave_choice)
{
	//assert(wave_choice != 0 && wave_choice <= 3);
    writeCommand(0x80 | (0x00 + 0x05));
    for(int i = 0; i < 11; i++)
    {
        writeData(waveTypeArray_hex[wave_choice - 1][i]);
    }
}

void DisplayDriver::writeFrequency(int frequency)
{
	//assert(frequency != 0 && frequency <= 1000);
	for (int i = 0; i < 4; ++i)
	{
	    numberArray[i] = 0x20;
	}
    numberStr = std::to_string(frequency);
    for (size_t i = 0; i < numberStr.size() && i < 4; ++i)
    {
        numberArray[i] = static_cast<uint16_t>(numberStr[i]);
    }

    writeCommand(0x80 | (0x20 + 0x0A));
    for(int i = 0; i < 4; i++)
    {
        writeData(numberArray[i]);
    }
    writeData(0x68);
    writeData(0x7A);
}

void DisplayDriver::writeAmplitude(int amplitude)
{
	//assert(amplitude >= 0 && amplitude <= 100);
	for (int i = 0; i < 4; ++i)
	{
	    numberArray[i] = 0x20;
	}
    numberStr = std::to_string(amplitude);
    for (size_t i = 0; i < numberStr.size() && i < 3; ++i)
    {
        numberArray[i] = static_cast<uint16_t>(numberStr[i]);
    }

    writeCommand(0x80 | (0x40 + 0x0A));
    for(int i = 0; i < 3; i++)
    {
        writeData(numberArray[i]);
    }
    writeData(0x25);
}

void DisplayDriver::writePhaseShift(int phase_shift)
{
	//assert(phase_shift >= 0 && phase_shift <= 256);
	for (int i = 0; i < 4; ++i)
	{
	    numberArray[i] = 0x20;
	}
    degree_phase_shift = phase_shift * 360 / NS;
    numberStr = std::to_string(degree_phase_shift);
    for (size_t i = 0; i < numberStr.size() && i < 3; ++i)
    {
        numberArray[i] = static_cast<uint16_t>(numberStr[i]);
    }

    writeCommand(0x80 | (0x60 + 0x0C));
    for(int i = 0; i < 3; i++)
    {
        writeData(numberArray[i]);
    }
    writeData(0xDF);
}

void DisplayDriver::writeFollowModeIndicator()
{
	writeCommand(0x80 | (0x00 + 0x0F));
	writeData(0x46);
}



