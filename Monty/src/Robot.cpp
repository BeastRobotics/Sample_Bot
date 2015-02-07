#include "WPILib.h"
#include "LifterControl.cpp"
#include "IControl.h"

#define NUM_CONTROLLERS 1

class Robot: public IterativeRobot {
	IControl *controllers[NUM_CONTROLLERS];

public:
	Robot() :
			lw(NULL) {
		controllers[0] = new LifterControl();
	}
private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i]->RobotInit();
		}
	}

	void AutonomousInit() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i]->AutonomousInit();
		}

	}

	void AutonomousPeriodic() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i]->AutonomousPeriodic();
		}

	}

	void TeleopInit() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i]->TeleopInit();
		}

	}

	void TeleopPeriodic() {
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i]->TeleopPeriodic();
		}

	}

	void TestPeriodic() {
		lw->Run();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i]->TestPeriodic();
		}
	}
};

START_ROBOT_CLASS(Robot);
