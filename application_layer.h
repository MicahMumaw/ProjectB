/*
 * application_layer.h
 *
 *  Created on: Oct 2, 2024
 *      Author: zeke u
 */

#ifndef GIT_PROJECTB_APPLICATION_LAYER_H_
#define GIT_PROJECTB_APPLICATION_LAYER_H_


#include <cstdint>
#include <stdexcept>
#include <cmath>
#include "application_layer.h"
#include "input_driver.h"
#include "output_driver.h"
#include "Queue.h"
#include "main.h"


class SignalSynthesis {
	public:
		SignalSynthesis();
		struct Shape {
			int32_t wave_choice;
			int32_t frequency;
			int32_t amplitude;
		};
		void setWaveChoice(int choice); //sets wave choice
		void setFrequency(float freq); //sets frequency in Hz
		void setAmplitude(float amp); //sets amplitude
		void update(); //updates method to output signal
		void enableFollowerMode(bool enable); //enables or disables follower mode
		void setDelay(); //sets delays for follower mode

	private:
		Shape shape;
		bool followerMode;
		uint8_t delay;
		Queue* queue;
		Semaphore* sem;
};


class Semaphore {
	public:
		Semaphore();

		void post(); //signals (posts) the semaphore
		void wait(); //waits (decrements) the semaphore
		bool tryWait(); //tries to wait without blocking
		int getCount(); //gets the current count of the semaphore

	private:
		volatile int count;
};

#endif /* GIT_PROJECTB_APPLICATION_LAYER_H_ */
