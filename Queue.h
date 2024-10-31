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
	int32_t wave_num;
	int32_t wave_choice;
    int32_t frequency;
    int32_t amplitude;
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
