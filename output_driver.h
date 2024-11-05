/**
 * @file output1.h
 * @brief Output driver implementation for DAC and display control on Nucleo board.
 *
 * This file provides the functionality for generating and displaying waveforms using
 * the DAC and SPI display drivers.
 *
 * @author Micah
 * @date September 24, 2024
 */

#include "main.h"
#include "math.h"
#include "stm32l4xx_hal.h"
#include "Queue.h"
#include "Queue3.h"
#include "string"
#include "iomanip"
#include "sstream"
#include "cassert"

#ifndef PROJECTB_OUTPUT_DRIVER_H_
#define PROJECTB_OUTPUT_DRIVER_H_

#define NS 256
#define PI 3.14159265358979
#define command_num 11

class DAC_Driver{ // @suppress("Miss copy constructor or assignment operator")
public:
	/**
	 * @brief Constructor for the DAC_Driver class.
	 *
	 * Initializes the DAC driver, setting initial values for wave parameters,
	 * generating lookup tables (LUT) for sine, square, and triangle waves, and
	 * starting timers for waveform generation.
	 *
	 * @param OutputQueue Queue for output data to be processed.
	 * @param DisplayQueue Queue for displaying output data.
	 */
	DAC_Driver(Queue*, Queue3*);

	/**
	 * @brief Generates the waveform based on input parameters and updates display.
	 *
	 * This function retrieves data from the queue, processes the input parameters,
	 * selects and configures the appropriate waveform LUT based on the wave type,
	 * and sets up timers and DAC channels to output the waveform. It also updates
	 * the display queue with the generated wave information.
	 */
	void generate_wave();

private:
	Queue* queue;
    Queue3* displayQueue;

    uint32_t amplitude_peak,
			 phase_shift,
			 offset,
			 wave_choice1, wave_choice2,
			 frequency1, frequency2,
			 amplitude1, amplitude2,
			 Wave1_LUT[NS], Wave2_LUT[NS],
			 Sin_Wave_LUT[NS], Square_Wave_LUT[NS], Triangle_Wave_LUT[NS], Follow_Wave_LUT[NS],
			 temp[NS];

    bool follow_mode;

    enum wave_choice {SIN = 1, SQUARE = 2, TRIANGLE = 3};

    /**
     * @brief Processes the input data and validates wave parameters.
     *
     * Sets the phase shift and follow mode from the input data, and performs
     * checks to ensure frequency, amplitude, and wave type are within expected
     * ranges.
     *
     * @param input Structure containing waveform parameters such as frequency,
     * amplitude, and phase shift.
     */
    void processInput(const OutputData& input);

    /**
     * @brief Configures a lookup table (LUT) based on wave type and amplitude.
     *
     * Adjusts the values of a specified LUT to create a waveform with the given
     * type (sine, square, or triangle) and amplitude. The LUT is scaled according
     * to the specified amplitude.
     *
     * @param waveLUT Pointer to the LUT array for storing wave values.
     * @param wave_choice Wave type: 1 for sine, 2 for square, 3 for triangle.
     * @param amplitude Amplitude of the waveform, in percentage (0-100).
     */
    void setupWaveLUT(uint32_t* waveLUT, int wave_choice, int amplitude);

    /**
     * @brief Applies follow mode, syncing Wave2 to Wave1 with phase shift.
     *
     * Sets the frequency of Wave2 to match Wave1, and applies the specified
     * phase shift to Wave2 to align it with Wave1. The LUT for Wave2 is
     * generated based on Wave1 with the applied phase shift.
     */
    void applyFollowMode();

    /**
     * @brief Configures the timers based on the selected wave frequencies.
     *
     * Sets the auto-reload values for the timers controlling DAC output rates.
     * The timer frequencies are adjusted based on the selected frequencies of
     * Wave1 and Wave2 to control the waveform's output rate.
     */
    void setupTimers();

    /**
     * @brief Starts the DAC output using DMA with the configured wave LUTs.
     *
     * Initiates DMA-based output of Wave1 and Wave2 on the DAC channels. Stops
     * any ongoing DMA operation before restarting it with the latest LUT data.
     */
    void startDAC();
};

class DisplayDriver{ // @suppress("Miss copy constructor or assignment operator")

private:
    SPI_HandleTypeDef* hspi;             // SPI handler

    uint8_t
		byteArray[3],
		numberArray[4],
		waveTypeArray_hex[3][12] =
		{
			{0x53, 0x69, 0x6E, 0x75, 0x73, 0x6F, 0x69, 0x64, 0x20, 0x20, 0x20, 0x20},  // Sinusoid
			{0x54, 0x72, 0x69, 0x61, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x20, 0x20, 0x20},  // Triangle
			{0x53, 0x71, 0x75, 0x61, 0x72, 0x65, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20}   // Square
		},
		parameters_LUT_hex[4][12] =
		{
			{0x57, 0x61, 0x76, 0x65, 0x3A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},  // wave_choice_LUT_hex
			{0x46, 0x72, 0x65, 0x71, 0x75, 0x65, 0x6E, 0x63, 0x79, 0x3A, 0x20, 0x20},  // frequency_LUT_hex
			{0x41, 0x6D, 0x70, 0x6C, 0x69, 0x74, 0x75, 0x64, 0x65, 0x3A, 0x20, 0x20},  // amplitude_LUT_hex
			{0x50, 0x68, 0x61, 0x73, 0x65, 0x20, 0x53, 0x68, 0x69, 0x66, 0x74, 0x3A}   // phase_shift_LUT_hex
		},
		initCommandArray[command_num] =
			{0x3A, 0x09, 0x06, 0x1E, 0x39, 0x1B, 0x6C, 0x56, 0x7A, 0x38, 0x0C}; // command list for initialization

    uint16_t degree_phase_shift;

    std::string numberStr;

    Queue3* displayQueue;

    /**
     * @brief Resets the display using the reset pin.
     */
    void reset_display();

    /**
     * @brief Writes an array of initialization commands to the display.
     */
    void write_command_array();

    /**
     * @brief Clears the display by writing spaces to all positions.
     */
    void clear_display();

    /**
     * @brief Writes static display content that does not change.
     */
    void write_statics();

    /**
     * @brief Writes data to the display over SPI.
     *
     * @param data Data byte to write to the display.
     */
    void writeData(uint8_t data);

    /**
     * @brief Writes a command to the display over SPI.
     *
     * @param command Command byte to write to the display.
     */
    void writeCommand(uint8_t command);

    /**
     * @brief Writes the wave type (sine, triangle, or square) to the display.
     *
     * @param wave_choice Wave choice as an integer (1 for sine, 2 for square, 3 for triangle).
     */
    void writeWaveType(int wave_choice);

    /**
     * @brief Writes the frequency of the waveform to the display.
     *
     * @param frequency Frequency value to display.
     */
    void writeFrequency(int frequency);

    /**
     * @brief Writes the amplitude of the waveform to the display.
     *
     * @param amplitude Amplitude value to display.
     */
    void writeAmplitude(int amplitude);

    /**
     * @brief Writes the phase shift of the waveform to the display in degrees.
     *
     * @param phase_shift Phase shift value in degrees.
     */
    void writePhaseShift(int phase_shift);

    /**
     * @brief Indicates if follow mode is enabled on the display.
     */
    void writeFollowModeIndicator();

public:
    /**
     * @brief Constructs a DisplayDriver object and initializes the display.
     *
     * @param spiHandle Pointer to the SPI handle used for communication with the display.
     * @param DisplayQueue Pointer to the queue storing display data.
     */
    DisplayDriver(SPI_HandleTypeDef* spiHandle, Queue3*);

    /**
     * @brief Updates the display with the latest waveform values from the queue.
     *
     * Writes wave type, frequency, amplitude, phase shift, and follow mode indicator to the display.
     */
    void writeValues();
};


#endif /* PROJECTB_OUTPUT_DRIVER_H_ */
