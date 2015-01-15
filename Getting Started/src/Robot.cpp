#include "WPILib.h"
#include "XboxController.h"

class Robot: public IterativeRobot {

	RobotDrive myRobot; // robot drive system
	//Joystick stick; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;
	bool highGearActivated = false;
	int counter = 0;
	XboxController *xbox;
	Compressor *c = new Compressor(0);
	DoubleSolenoid *ex1 = new DoubleSolenoid(0, 1);
	DoubleSolenoid *ex2 = new DoubleSolenoid(2, 3);
	DriverStation *DS = DriverStation::GetInstance();
	Encoder *en1 = new Encoder(0,1,true,Encoder::k2X); //Motors 1 and 2
	Encoder *en2 = new Encoder(2,3,false,Encoder::k2X); //Motors 3 and 4
	CameraServer *c1 = CameraServer::GetInstance();
	Talon *tal1 = new Talon(0);
	Talon *tal2 = new Talon(1);
	Talon *tal3 = new Talon(2);
	Talon *tal4 = new Talon(3);



public:
	Robot() :
			myRobot(0, 1, 2, 3),// these must be initialized in the same order
			//stick(0),		// as they are declared above.
			lw(NULL), autoLoopCounter(0) {
		myRobot.SetExpiration(0.1);
		xbox = XboxController::getInstance();
		c->SetClosedLoopControl(true); //Turns compressor on
		// No Dash board in Constructor
		c1->StartAutomaticCapture();

	}

private:
	void RobotInit() {
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit() {
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

	void TeleopInit() {
		SmartDashboard::PutBoolean("Increment Counter", false);
		en1->Reset();
		en2->Reset();
	}

	void TeleopPeriodic() {
		myRobot.ArcadeDrive(xbox->getLeftStick());
		SmartDashboard::PutBoolean("High Gear", highGearActivated);

		SmartDashboard::PutNumber("Counter", xbox->getAxisLeftX());

		//Greater
		SmartDashboard::PutString("Greet", "Hello World!");

		//Graph Value
		SmartDashboard::PutNumber("Right Motor", en1->Get());
		SmartDashboard::PutNumber("Left Motor", en2->Get());

		//Numerical Value
		SmartDashboard::PutNumber("Right Motor Count", en1->Get());
		SmartDashboard::PutNumber("Left Motor Count", en2->Get());

		SmartDashboard::PutNumber("Stik it UP", xbox->getLeftStick()->GetX()); //X-Value of Joystick

		SmartDashboard::PutBoolean("X", xbox->isXHeld());
		SmartDashboard::PutBoolean("Y", xbox->isYHeld());

		if (xbox->isAPressed()) {  // if A is pressed, toggle the gear
			if (highGearActivated) {
				ex1->Set(ex1->kReverse);
				ex2->Set(ex2->kReverse);
			}
			else {
				ex1->Set(ex1->kForward);
				ex2->Set(ex2->kForward);
			}
			highGearActivated=!highGearActivated;
		}
	}

	void TestPeriodic() {
		lw->Run();
	}


};

START_ROBOT_CLASS(Robot);
