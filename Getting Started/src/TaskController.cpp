#include "WPILib.h"

class TaskController
{

	const char* name;
	bool run = false;
	Task *aTask = NULL;
	bool debug = false;
	FUNCPTR myTask = NULL;

public:
	bool init(const char* newname)
	{
		//TODO implement
		//set it to turn debug on based on a preference
		name = newname;
		return false;
	}

	bool Start()
	{
		//TODO implement
		aTask->Start((uint32_t)this);
		//TODO if(debug)SmartDashboard::PutBoolean("Running:"+name, true);
		return false;
	}

	static bool StopAll()
	{
		//TODO make this work in C somehow don't know pointers well
		//return TaskController::Stop();
		return false;
	}

	bool Stop()
	{
		run = false;
		if(aTask->Stop())
		{
			return true;
			if(debug)
			{
				//TODO SmartDashboard::PutBoolean("Running:"+name, false);
			}
		}
		return false;
	}

	bool ConfirmDestruction() //Checks if it is not stopped : Will stop it if not
	{
		if(aTask!=NULL)
		{
			if(!aTask->Verify())
			{
				delete aTask;
				aTask = NULL;
				if(debug){
					SmartDashboard::PutBoolean("Stopped", true);
					SmartDashboard::PutString("State", "Disable Stopped");
				}
				return true;
			}
			else return true;
		}
		else return true;
	}

	bool Run(...)
	{
		run = true;
		return false;
	}
};

