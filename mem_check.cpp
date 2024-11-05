/*
 * mem_check.cpp
 *
 *  Created on: Nov 4, 2024
 *      Author: micah
 */

#include "mem_check.h"

mem_check::mem_check()
{

}

void mem_check::check_memory()
{
	for(int i = 0; i < array_length; i++)
	{
		if(checked_array[i] != i)
		{
			NVIC_SystemReset();
		}
	}
}
