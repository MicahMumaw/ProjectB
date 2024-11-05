/*
 * Queue.h
 *
 *  Created on: Oct 8, 2024
 *      Author: micah
 */

#include <cstdint>

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#define Q_SIZE 5

struct OutputData
{
	uint32_t wave_num;
	uint32_t wave_choice;
    uint32_t frequency;
    uint32_t amplitude;
    uint32_t phase_shift;
    bool follow_mode;
};


class Queue{
private:
	int32_t tail;
	OutputData buffer[Q_SIZE];

public:
	Queue();
	bool enqueue(OutputData);
	bool dequeue(OutputData*);
};


#endif /* INC_QUEUE_H_ */
