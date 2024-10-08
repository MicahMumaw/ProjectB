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


class Queue{
private:
	int32_t tail;
	int32_t buffer[Q_SIZE];

public:
	Queue();
	bool enqueue(int32_t);
	bool dequeue(int32_t*);
};


#endif /* INC_QUEUE_H_ */
