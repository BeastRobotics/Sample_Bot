#include <CompressorControl.cpp>
#include <Grabber.cpp>
#include <IControl.h>
#include <IterativeRobot.h>
#include <LifterBrake.h>
#include <LiveWindow/LiveWindow.h>
#include <NewXboxController.h>
#include <RobotBase.h>
#include <stddef.h>
#include "MecanumDrive.cpp"

#define NUM_CONTROLLERS 5

class Robot: public IterativeRobot {
	IControl *controllers[NUM_CONTROLLERS];

public:
	Robot() :
			lw(NULL) {

		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i] = NULL;
		}

		controllers[0] = NewXboxController::getInstance();
		controllers[1] = new LifterControl();
		//controllers[1] = new LifterBrake();
		controllers[2] = new CompressorControl();
		//controllers[3] = new ArcadeDrive();
		controllers[3] = new MecanumDrive();
		controllers[4] = new GrabberControl();
		//controllers[5] = new CameraControl();
	}
private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			SmartDashboard::PutNumber("State 2", i);
			if (controllers[i] != NULL)
				controllers[i]->RobotInit();
		}
		SmartDashboard::PutString("State", "Robot Init");
	}

	void AutonomousInit() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
			controllers[i]->AutonomousInit();
		}

	}

	void AutonomousPeriodic() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
			controllers[i]->AutonomousPeriodic();
		}

	}

	void TeleopInit() {
		SmartDashboard::PutString("State", "Tele Init");
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			SmartDashboard::PutNumber("State 2", i);
			if (controllers[i] != NULL)
			controllers[i]->TeleopInit();
		}

	}

	void TeleopPeriodic() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
			controllers[i]->TeleopPeriodic();
		}
		SmartDashboard::PutString("State", "Tele Periodic");
	}

	void TestPeriodic() {
		lw->Run();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
			controllers[i]->TestPeriodic();
		}
	}
};

START_ROBOT_CLASS(Robot);
