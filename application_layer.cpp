/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */
#include <cstdint>
#include <cmath>

#include "application_layer.h"

SignalSynthesis::SignalSynthesis() : shape(SINE), freq(1000.0f), amp(1.0f), followerMode(false), delay(0) {};


enum SignalSynthesis::Shape {SINE, SQUARE, PULSE10};


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
	return;
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
