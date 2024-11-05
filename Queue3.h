/*
 * DisplayQueue.h
 *
 *  Created on: Nov 3, 2024
 *      Author: micah
 */

#include <cstdint>

#ifndef PROJECTB_DISPLAYQUEUE_H_
#define PROJECTB_DISPLAYQUEUE_H_

#define Q_SIZE 5

struct DisplayData
{
	uint32_t wave_num;
	uint32_t wave_choice;
    uint32_t frequency;
    uint32_t amplitude;
    uint32_t phase_shift;
    bool follow_mode;
};


class Queue3{
private:
	int32_t tail;
	DisplayData buffer[Q_SIZE];

public:
	Queue3();
	bool enqueue(DisplayData);
	bool dequeue(DisplayData*);
};

#endif /* PROJECTB_DISPLAYQUEUE_H_ */
