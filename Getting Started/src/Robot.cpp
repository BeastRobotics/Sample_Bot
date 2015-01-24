#include "WPILib.h"
#include "NewXboxController.h"

class Robot: public IterativeRobot {

	RobotDrive myRobot; // robot drive system
	LiveWindow *lw;
	Compressor *c = new Compressor(0); //Declasres the compressor
	DoubleSolenoid *ex1 = new DoubleSolenoid(0, 1); //First solenoid pair
	DoubleSolenoid *ex2 = new DoubleSolenoid(2, 3); //Second solenoid pair
	DriverStation *DS = DriverStation::GetInstance(); //Declares the driver station
	//Encoder *en1 = new Encoder(0, 1, true, Encoder::k2X); //Motors 1 and 2
	//Encoder *en2 = new Encoder(2, 3, false, Encoder::k2X); //Motors 3 and 4
	CameraServer *c1 = CameraServer::GetInstance(); //Declares camera
	Preferences *pref = Preferences::GetInstance(); //Declares preferences on SmartDashboard
	Task *aTask = NULL;
	int autoLoopCounter;
	bool highGearActivated = false; //False for low gear, true for high gear
	int counter = 0;
	bool run = false;

	NewXboxController* controller=NULL;

public:
	Robot() :
			// No Dash board in Constructo

			myRobot(2, 1), // these must be initialized in the same order  2,1 since the turing was inverted
			lw(NULL), autoLoopCounter(0) {
		myRobot.SetExpiration(0.1);
		c1->StartAutomaticCapture();

	}

private:
	void RobotInit() {
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit() {
		SmartDashboard::PutString("State", "Autonomous");
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic() {
		if (autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
				{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
		} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}
	}
	static int CountToHundred(uint32_t param)
	{
		Robot *myClass = (Robot*)param;

		for (int i = 0; i < 100 && myClass->run; i++) {
			SmartDashboard::PutNumber("Print Hundred", i + 1);
			Wait(5);
		}
		return 0;
	}

	void TeleopInit() {
		controller=NewXboxController::getInstance();
	}

	void endTask()
	{
		run = false;
		if (aTask != NULL) {
					bool stopped = aTask->Stop();
					SmartDashboard::PutBoolean("Task Value", stopped);
					SmartDashboard::PutBoolean("Stopped", false);
					//SmartDashboard::PutNumber("Print Hundred",-999);
				}
	}

	void DisabledPeriodic()
	{
		if(aTask!=NULL)
		{
			if(!aTask->Verify())
			{
				delete aTask;
				aTask = NULL;
				SmartDashboard::PutBoolean("Stopped", true);
				SmartDashboard::PutString("State", "Disable Stopped");
			}
		}
	}

	void TeleopPeriodic() {

		controller->update();

		SmartDashboard::PutBoolean("New Xbox: X", controller->getXHeld());
		SmartDashboard::PutBoolean("New Xbox: Y", controller->getYHeld());
		SmartDashboard::PutBoolean("New Xbox: A", controller->getAHeld());
		SmartDashboard::PutBoolean("New Xbox: B", controller->getBHeld());

		SmartDashboard::PutBoolean("New Xbox: L3", controller->getL3Held());
		SmartDashboard::PutBoolean("New Xbox: R3", controller->getR3Held());
	}

	void DisabledInit()
	{
		SmartDashboard::PutString("State", "Disabled");
		endTask();
	}

	void TestPeriodic() {
		lw->Run();
	}

};

START_ROBOT_CLASS(Robot);
