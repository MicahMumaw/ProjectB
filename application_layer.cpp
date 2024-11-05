/*
 * application_layer.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: micah
 */


#include "application_layer.h"


SignalSynthesis::SignalSynthesis(Queue* outputQueue, Semaphore* semaphore) : shape{0, 1000, 1}, queue(outputQueue), sem(semaphore) {}


void SignalSynthesis::setWaveChoice(int waveType) {
	shape.wave_choice = waveType;
}


void SignalSynthesis::setFrequency(float freq) {
	shape.frequency = freq;

}


void SignalSynthesis::setAmplitude(float amp) {
	shape.amplitude = amp;
}


void SignalSynthesis::update() {
	Shape waveData{shape.wave_choice, shape.frequency, shape.amplitude};
	queue -> enqueue(waveData);
	if(sem) {
		sem -> signal();
	}
}


Application::Application(Queue* outputQueue1, Queue* outputQueue2, Semaphore* semaphore1, Semaphore* semaphore2)
	: channel1(outputQueue1, semaphore1), channel2(outputQueue2, semaphore2), followerModeChannel2(false), delayChannel2(0) {}


void Application::configureChannel1(int waveType, float freq, float amp) {
	assert(waveType >= kMinWaveType && waveType <= kMaxWaveType);
	assert(freq >= kMinFrequency && freq <= kMaxFrequency);
	assert(amp >= 0);
	channel1.setWaveChoice(waveType);
	channel1.setFrequency(freq);
	channel1.setAmplitude(amp);
}


void Application::configureChannel2(int waveType, float freq, float amp) {
	assert(waveType >= kMinWaveType && waveType <= kMaxWaveType);
	assert(freq >= kMinFrequency && freq <= kMaxFrequency);
	assert(amp >= 0);
	channel2.setWaveChoice(waveType);
	channel2.setFrequency(freq);
	channel2.setAmplitude(amp);
}


void Application::updateChannels() {
	channel1.update();
	if(followerModeChannel2) {
		channel2.setWaveChoice(channel1.shape.wave_choice);
		channel2.setFrequency(channel1.shape.frequency);
		channel2.setAmplitude(channel1.shape.amplitude);
		float period = 1.0f / channel1.shape.frequency;
		int delaySamples = static_cast<int>(delayChannel2 * (period / 8.0f) * 20000);
		channel2.setDelay(delaySamples);
	}
	channel2.update();
}


void Application::enableFollowerModeChannel2(bool enable) {
	followerModeChannel2 = enable;
}


void Application::setDelayChannel2(int step) {
	assert(step >= 0 && step <= 7);
	delayChannel2 = static_cast<uint8_t>(step);
}










