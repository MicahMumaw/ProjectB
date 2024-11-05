/*
 * mem_check.h
 *
 *  Created on: Nov 4, 2024
 *      Author: micah
 */

#ifndef PROJECTB_MEM_CHECK_H_
#define PROJECTB_MEM_CHECK_H_

#include <cstdint>
#include <main.h>

#define array_length 10

class mem_check {
private:
	uint8_t checked_array[array_length] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

public:
	mem_check();
	void check_memory();
};

#endif /* PROJECTB_MEM_CHECK_H_ */
