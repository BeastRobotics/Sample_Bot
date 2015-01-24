


#include "WPILib.h"


#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

class TaskController //Designed As A template class to be inherited and overwritten
{
public:

	virtual ~TaskController() = 0;


	const char* name;
	bool run = false;

	TaskController(const char* name);

	bool init(); //Makes the Thread but does not run it
								 //true on success false on failure or already existing string

	bool Start(uint32_t param1); //Starts A Thread

	static bool StopAll(); //stops all ongoing threads

	bool StopRequested(); //Activates When stopall has been called

	bool Stop(); //stops only the thread in this class

	bool ConfirmDestruction(); //Confirms wither or not the class has been destroyed

	virtual bool Run(uint32_t) = 0; //Starts the thread

private:
	Task *aTask;
	bool debug;
	FUNCPTR myTask;

	int taskRunner(uint32_t param);

};

#endif
