/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */
#include <cstdint>

#include "application_layer.h"

SignalSynthesis::SignalSynthesis() {};


enum SignalSynthesis::Shape {SINE, SQUARE, PULSE10};


void SignalSynthesis::setShape(Shape shape) {
	return;
}


void SignalSynthesis::setFrequency(float freq) {
	return;
}


void SignalSynthesis::setAmplitude(float amp) {
	return;
}


void SignalSynthesis::update() {
	return;
}


void SignalSynthesis::enableFollowerMode(bool enable) {
	return;
}


void SignalSynthesis::setDelay() {
	return;
}


Semaphore::Semaphore(int count = 0) : count(count) {};


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


int Sempahore::getCount() {
	return count;
}
