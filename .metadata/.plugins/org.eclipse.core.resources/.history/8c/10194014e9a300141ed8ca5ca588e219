


#include "WPILib.h"


#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

class TaskController //Designed As A template class to be inherited and overwritten
{
public:
	const char* name;
	bool run = false;

	bool init(const char* name); //Makes the Thread but does not run it
								 //true on success false on failure or already existing string

	bool Start(); //Starts A Thread

	static bool StopAll(); //stops all ongoing threads

	bool Stop(); //stops only the thread in this class

	bool ConfirmDestruction(); //Confirms weither or not the class has been destroyed

	bool Run(...); //Starts the thread
};

#endif TASKCONTROLLER_H
