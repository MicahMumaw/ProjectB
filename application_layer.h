/*
 * application_layer.h
 *
 *  Created on: Oct 2, 2024
 *      Author: zeke u
 */

#ifndef GIT_PROJECTB_APPLICATION_LAYER_H_
#define GIT_PROJECTB_APPLICATION_LAYER_H_


#include <cstdint>
#include <cmath>
#include <casserts>
#include "input_driver.h"
#include "output_driver.h"
#include "Queue.h"
#include "Semaphore.h"
#include "main.h"


class SignalSynthesis {
	public:
		SignalSynthesis(Queue* outputQueue, Semaphore* semaphore);
		struct Shape {
			int32_t wave_choice;
			int32_t frequency;
			int32_t amplitude;
		};
		void setWaveChoice(int waveType); //sets wave choice
		void setFrequency(float freq); //sets frequency in Hz
		void setAmplitude(float amp); //sets amplitude
		void update(); //updates method to output signal

	private:
		Shape shape;
		Queue* queue;
		Semaphore* sem;
};


class Application {
public:
	Application(Queue* outputQueue1, Queue* outputQueue2, Semaphore* semaphore1, Semaphore* semaphore2);
	void configureChannel1(int waveType, float freq, float amp);
	void configureChannel2(int waveType, float freq, float amp);
	void updateChannels();
	void enableFollowerModeChannel2(bool enable);
	void setDelayChannel2(int step);

private:
	SignalSynthesis channel1;
	SignalSynthesis channel2;
	bool followerModeChannel2;
	uint8_t delayChannel2;
	float kMaxFrequency = 1000.0f;
	float kMinFrequency = 1.0f;
	int kMinWaveType = 1;
	int kMaxWaveType = 4;
};













};


#endif /* GIT_PROJECTB_APPLICATION_LAYER_H_ */
