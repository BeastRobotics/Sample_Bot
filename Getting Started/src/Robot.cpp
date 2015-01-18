#include "WPILib.h"
#include "XboxController.h"

class Robot: public IterativeRobot {

	RobotDrive myRobot; // robot drive system
	LiveWindow *lw;
	XboxController *xbox; //Declares the xbox controller
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

public:
	Robot() :
		// No Dash board in Constructo

		myRobot(2, 1), // these must be initialized in the same order  2,1 since the turing was inverted
		lw(NULL), autoLoopCounter(0) {
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance(); //Initializes xbox Controller
		//c->SetClosedLoopControl(true); //Turns compressor on
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
		SmartDashboard::PutString("State", "Teleop");
		SmartDashboard::PutBoolean("Increment Counter", false);
		//en1->Reset(); //Clears the Encoder
		//en2->Reset(); //Clears the Encoder
		endTask();
		run = true;
		SmartDashboard::PutNumber("Print Hundred",0);
		FUNCPTR myTask = (FUNCPTR)Robot::CountToHundred;
		aTask = new Task("Counter", myTask);
		aTask->Start((uint32_t)this);
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

		//Starts Basic Drive
		myRobot.ArcadeDrive(xbox->getLeftStick());

		//High gear / needs fancying / changes to set value when it restarts
		SmartDashboard::PutBoolean("High Gear", highGearActivated);

		//Basic Dial
		SmartDashboard::PutNumber("Counter", xbox->getAxisLeftX());

		//Greater
		SmartDashboard::PutString("Greet", "Hello World!");

		//Graph Value
		//SmartDashboard::PutNumber("Right Motor", en1->Get());
		//SmartDashboard::PutNumber("Left Motor", en2->Get());

		//Numerical Value
		//SmartDashboard::PutNumber("Right Motor Count", en1->Get());
		//SmartDashboard::PutNumber("Left Motor Count", en2->Get());

		//Controller X Drift
		SmartDashboard::PutNumber("Stik it UP", xbox->getLeftStick()->GetX()); //X-Value of Joystick

		SmartDashboard::PutBoolean("X", xbox->isXHeld());
		SmartDashboard::PutBoolean("Y", xbox->isYHeld());

		//Access Keys
		double x = pref->GetDouble("Shooting_Speed", 10.0); //Grabs the Shooting speed key from prefereces

		SmartDashboard::PutNumber("User", x);

		if (xbox->isAPressed()) {  // if A is pressed, toggle the gear
			if (highGearActivated) {
				ex1->Set(ex1->kReverse);
				ex2->Set(ex2->kReverse);
			} else {
				ex1->Set(ex1->kForward);
				ex2->Set(ex2->kForward);
			}
			highGearActivated = !highGearActivated;
		}
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
