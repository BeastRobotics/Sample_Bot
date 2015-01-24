#include "WPILib.h"
#include "TestTask.h"


TestTask::TestTask(const char* name): TaskController(name)
	{

	}


bool TestTask::Run(uint32_t param)
{
	while(!StopRequested())
	{
		Wait(0.05);
	}
	return false;
}
