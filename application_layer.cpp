/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */
#include <cstdint>

class SignalSynthesis {
	public:
		SignalSynthesis();
		enum Shape {SINE, SQUARE, PULSE10};

		void setShape(Shape shape); //sets waveform shape
		void setFrequency(float freq); //sets frequency in Hz
		void setAmplitude(float amp); //sets amplitude
		void update(); //updates method to output signal
		void enableFollowerMode(bool enable); //enables or disables follower mode
		void setDelay(); //sets delays for follower mode

	private:
		Shape shape;
		float frequency;
		float amplitude;
		bool followerMode;
		uint8_t delay;
};


class Semaphore {
	public:
		Semaphore();

		void post(); //signals (posts) the semaphore
		void wait(); //waits (decrements) the semaphore
		bool tryWait(); //tries to wait without blocking
		int getCount(); //gets the current count of the semaphore

	private:
		int count;
};
