/*
 * TestTask.h
 *
 *  Created on: Jan 24, 2015
 *      Author: education
 */



#ifndef SRC_TESTTASK_H_
#define SRC_TESTTASK_H_

#include "TaskController.h"

class TestTask: public TaskController //Designed As A template class to be inherited and overwritten
{
public:

	TestTask(const char* name);

	~TestTask() {}

	bool Run(uint32_t); //Starts the thread
};



#endif /* SRC_TESTTASK_H_ */
