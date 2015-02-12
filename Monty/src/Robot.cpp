#include <ArcadeDrive.cpp>
#include <CameraControl.cpp>
#include <CompressorControl.cpp>
#include <Grabber.cpp>
#include <IControl.h>
#include <IterativeRobot.h>
#include <LifterBrake.h>
#include <LiveWindow/LiveWindow.h>
#include <NewXboxController.h>
#include <RobotBase.h>
#include <stddef.h>

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
		//controllers[1] = new LifterControl();
		controllers[1] = new LifterBrake();
		controllers[2] = new CompressorControl();
		controllers[3] = new ArcadeDrive();
		controllers[4] = new GrabberControl();
		//controllers[5] = new MecanumDrive();
		//controllers[6] = new CameraControl();
	}
private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
				controllers[i]->RobotInit();


		}
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
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
			controllers[i]->TeleopInit();
		}

	}

	void TeleopPeriodic() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
			controllers[i]->TeleopPeriodic();
		}

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
