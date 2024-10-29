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
}


void SignalSynthesis::setFrequency(float freq) {
    if (freq >= 1.0f && freq <= 1000.0f) {
        shape.frequency = static_cast<int32_t>(freq);
    }
}


void SignalSynthesis::setAmplitude(float amp) {
    shape.amplitude = static_cast<int32_t>(amp);
}


void SignalSynthesis::update() {
	inputDriver.update();
	outputDriver.generate_wave();
}


void SignalSynthesis::enableFollowerMode(bool enable) {
	followerMode = enable;
}


void SignalSynthesis::setDelay(int step) {
	if(step >= 0 && step <= 7) {
		delay = step;
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
