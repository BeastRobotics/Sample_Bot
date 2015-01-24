#include "WPILib.h"
#include "TaskController.h"


	static bool stopAll = false;


	TaskController::TaskController(const char* newname)
	{
		this->run = false;
		name = newname;
		aTask = NULL;
		debug = false;
		myTask = NULL;
	}

	TaskController::~TaskController(){

	}

	bool TaskController::init()
	{
		//TODO implement
		myTask = (FUNCPTR)(&TaskController::taskRunner);
		aTask = new Task(name, myTask);
		return true;
	}

	bool TaskController::Start(uint32_t param1)
	{
		aTask->Start(param1);
		return true;
	}

	bool TaskController::StopAll()
	{
		//TODO make this work in C somehow don't know pointers well
		stopAll = true;
		return true;
	}

	bool TaskController::StopRequested()
	{
		return (run == false) || (stopAll == true);
	}

	bool TaskController::Stop()
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

	bool TaskController::ConfirmDestruction() //Checks if it is not stopped : Will stop it if not
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


	int TaskController::taskRunner(uint32_t param){
		return Run(param);
	}


