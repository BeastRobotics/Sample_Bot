#include "WPILib.h"

class TaskController
{
public:
	const char* name;

	bool init(const char* name); //Makes the Thread but does not initilase
	//true on success false on faliure or already existing string

	bool Start(); //Starts A Thread

	static bool StopAll(); //stops all ongoing threads

	bool Stop(); //stops only the thread in this class

	bool ConfirmDestruction(); //Confirms weither or not the class has been destroyed

	bool Run(...); //Starts the thread
}
