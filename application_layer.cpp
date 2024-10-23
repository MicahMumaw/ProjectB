/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */


#include "application_layer.h"
#include "input_driver.h"
#include "output_driver.h"
#include "Queue.h"
#include "main.h"


extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;


Queue signalQueue;
Semaphore signalSemaphore(0);


InputDriver inputDriver(&signalQueue, &signalSemaphore, LL_GPIO_PIN_0, LL_GPIO_PIN_1);
DAC_Driver outputDriver(&signalQueue);


SignalSynthesis::SignalSynthesis(Queue* outputQueue, Semaphore* sempahore)
	: shape(SINE), freq(1000.0f), amp(1.0f), followerMode(false), delay(0), queue(outputQueue), sem(semaphore)) {};


void SignalSynthesis::setShape(Shape shape) {
	this -> shape = shape;
	return;
}


void SignalSynthesis::setFrequency(float freq) {
	if(freq >= 1.0f && freq <= 1000.0f){
		this -> freq = freq;
	}
	return;
}


void SignalSynthesis::setAmplitude(float amp) {
	this -> amp = amp;
	return;
}


void SignalSynthesis::update() {
	inputDriver.update();
	outputDriver.generate_wave();
}


void SignalSynthesis::enableFollowerMode(bool enable) {
	followerMode = enable;
	return;
}


void SignalSynthesis::setDelay(int step) {
	if(step >= 0 && step <= 7) {
		delay = step * (1.0f / freq) / 8;
	}
	return;
}


Semaphore::Semaphore(int count) : count(count) {};


void Semaphore::post() {
	count++;
	return;
}


void Semaphore::wait() {
	while(count <= 0) {}
	return;
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
