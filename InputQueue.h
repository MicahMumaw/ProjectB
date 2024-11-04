/*
 * InputQueue.h
 *
 *  Created on: Oct 23, 2024
 *      Author: eegge
 */

#ifndef GIT_PROJECTB_INPUTQUEUE_H_
#define GIT_PROJECTB_INPUTQUEUE_H_

#include <cstdint>
#define Q_SIZE 5


struct InputData
{
	int8_t freq_amp; // Knob 1
	//int8_t amplitude; // Knob 2
	int8_t phase_shift; // Knob 3
	bool wave_choice; //Wave shapes (share,triangle,...)
	bool wave_select; // Controlling wave 1 or follow wave
	bool echo_mode; // change if waves are a copy of one another or different
	bool select_freqamp;   // Selects frequency/amplitude knob
};

class InputQueue
{
private:
    InputData buffer[Q_SIZE];
    int32_t tail;

public:
    InputQueue();

    bool enqueue(const InputData &msg);
    bool dequeue(InputData *msg);
};




#endif /* GIT_PROJECTB_INPUTQUEUE_H_ */
