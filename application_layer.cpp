/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */

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


Semaphore::Semaphore() {};


void Semaphore::post() {
	return;
}


void Semaphore::wait() {
	return;
}


bool Semaphore::tryWait() {
	return true;
}


int Sempahore::getCount() {
	return 0;
}
