/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */


#include "application_layer.h"


extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;


Queue signalQueue;
Semaphore signalSemaphore(0);


InputDriver inputDriver(&signalQueue, &signalSemaphore, LL_GPIO_PIN_0, LL_GPIO_PIN_1);
DAC_Driver outputDriver(&signalQueue);


SignalSynthesis::SignalSynthesis(Queue* outputQueue, Semaphore* semaphore) : shape{0, 1000, 1}, followerMode(false), delay(0),
		queue(outputQueue), sem(semaphore) {}


void SignalSynthesis::setWaveChoice(int choice) {
	if(choice >= 1 && choice <= 4) {
		shape.wave_choice = choice;
	}
	else {
		throw std::out_of_range("Wave choice must be between 1 and 4.");
	}
}


void SignalSynthesis::setFrequency(float freq) {
    if (freq >= 1.0f && freq <= 1000.0f) {
        shape.frequency = static_cast<int32_t>(freq);
    }
    else {
    	throw std::out_of_range("Frequency must be between 1.0 and 1000.0.");
    }
}


void SignalSynthesis::setAmplitude(float amp) {
	if(amp >= 0) {
		shape.amplitude = static_cast<int32_t>(amp);
	}
	else {
		throw std::out_of_range("Amplitude must be non-negative.");
	}
}


void SignalSynthesis::update() {
	inputDriver.update();
	OutputData waveData{shape.wave_choice, shape.frequency, shape.amplitude};
	queue -> enqueue(waveData);
	outputDriver.generate_wave();
}


void SignalSynthesis::enableFollowerMode(bool enable) {
	followerMode = enable;
}


void SignalSynthesis::setDelay(int step) {
	if(step >= 0 && step <= 7) {
		delay = step;
	}
	else {
		throw std::out_of_range("Delay step must be between 0 and 7.");
	}
}


Semaphore::Semaphore(int count) : count(count) {};


void Semaphore::post() {
	count++;
}


void Semaphore::wait() {
	while(count <= 0) {}
	count--;
}


bool Semaphore::tryWait() {
	if(count > 0) {
		count--;
		return true;
	}
	else {
		return false;
	}
}


int Semaphore::getCount() {
	return count;
}
