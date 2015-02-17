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
#include "LifterControl.cpp"
#include "LifterControlTester.cpp"
#include "CameraControl.cpp"
#include "AutoDelay.h"

#define NUM_CONTROLLERS 7
#define GRABBER 4
#define MOVE 3
#define LIFTER 1
#define DELAY 6
#define G_OPEN 0
#define G_CLOSE 1
#define AUTODRIVE 0
#define GRABTURNLEFT 1
#define GRABTURNRIGHT 2
#define WAIT 1000
#define AUTO_D 3000

struct Command_Node {
	int index;
	int operation;
	Command_Node* nextCommand;
};

class Robot: public IterativeRobot {

	IControl *controllers[NUM_CONTROLLERS];
	int autoReturns[NUM_CONTROLLERS];
	int autoDrive = AUTODRIVE;
	int grabTurnLeft = GRABTURNLEFT;
	int grabTurnRight = GRABTURNRIGHT;

	Command_Node* head;
	Command_Node* currentCommand;
public:

	void nothing(){
		addCommand(DELAY,WAIT);
	}
	void driveStraight() {
		addCommand(MOVE, AUTO_D);
	}
	void driveBack() {
		addCommand(MOVE, -AUTO_D);
	}
	void getContainer() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(MOVE, AUTO_D);
		addCommand(GRABBER, G_OPEN);
	}
	void getContainerBack() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(MOVE, -AUTO_D);
		addCommand(GRABBER, G_OPEN);
	}
	void getTote() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(MOVE, 1);
		addCommand(DELAY, WAIT);
		addCommand(MOVE, AUTO_D);
	}
	void getToteContainer() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 2000);
		addCommand(DELAY, WAIT);
		addCommand(MOVE, 500);
		addCommand(GRABBER, G_OPEN);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, -2000);
		addCommand(DELAY, WAIT);
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(DELAY, WAIT);
		addCommand(MOVE, 1);
		addCommand(DELAY, WAIT);
		addCommand(MOVE, AUTO_D);
		addCommand(DELAY, WAIT);
		addCommand(GRABBER, G_OPEN);
	}

	Robot() :
			lw(NULL) {
		head = NULL;
		currentCommand = NULL;
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
		controllers[5] = new CameraControl();

		//lifter = new LifterControlTester();
		//controllers[5] = new MecanumDrive();
		controllers[6] = new AutoDelay();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			autoReturns[i] = 0;
		}
	}

private:
	LiveWindow *lw;
	//int automonusCommand;

	void addCommand(int index, int operation) {

		Command_Node* toAdd = new Command_Node();
		toAdd->index = index;
		toAdd->nextCommand = NULL;
		toAdd->operation = operation;
		Command_Node* current = head;
		if (head == NULL) {
			head = toAdd;
			return;
		}

		while (current->nextCommand != NULL) {
			current = current->nextCommand;
		}
		current->nextCommand = toAdd;
	}

	void deleteList(Command_Node *bob) {
		if (bob->nextCommand != NULL)
			deleteList(bob->nextCommand);
		delete bob;
	}

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
		int chooser = Preferences::GetInstance()->GetInt("AutoChooser", 1);
		switch (chooser) {
		case 1:
			SmartDashboard::PutString("ChooserValue","You Failed At Life");
			nothing();
			break;
		case 2:
			SmartDashboard::PutString("ChooserValue","You can drive straight!");
			driveStraight();
			break;
		case 3:
			SmartDashboard::PutString("ChooserValue","Driving back");
			driveBack();
			break;
		case 4:
			SmartDashboard::PutString("ChooserValue","Get Container");
			getContainer();
			break;
		case 5:
			SmartDashboard::PutString("ChooserValue","Get Container Backwards");
			getContainerBack();
			break;
		case 6:
			SmartDashboard::PutString("ChooserValue","Get the Tote");
			getTote();
			break;
		case 7:
			SmartDashboard::PutString("ChooserValue","You got everything!!!!!!!!!!");
			getToteContainer();
			break;
		}
		currentCommand = head;
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
				controllers[i]->AutonomousInit();
		}

	}
	void AutonomousPeriodic() {

		SmartDashboard::PutBoolean("DoneAuto", currentCommand == NULL);
		if (currentCommand != NULL) {
			SmartDashboard::PutNumber("Command Index", currentCommand->index);
			SmartDashboard::PutNumber("Command Operation",
					currentCommand->operation);
			int result = controllers[currentCommand->index]->AutonomousPeriodic(
					currentCommand->operation);
			if (result == 1) {
				currentCommand = currentCommand->nextCommand;
			}
		}
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
				controllers[i]->AutonomousExecute();
		}

	}

	void TeleopInit() {
		//SmartDashboard::PutNumber("Tommy Genius", (double)chooser->GetSelected());
		SmartDashboard::PutString("State", "Tele Init");
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			SmartDashboard::PutNumber("State 2", i);
			if (controllers[i] != NULL)
				controllers[i]->TeleopInit();
		}
		//lifter->init();
	}

	void TeleopPeriodic() {
		SmartDashboard::PutString("State", "Tele Periodic");
		for (int i = 0; i < NUM_CONTROLLERS; i++) {

			if (controllers[i] != NULL)
				controllers[i]->TeleopPeriodic();
		}

		//lifter->move();
	}

	void TestPeriodic() {
		lw->Run();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
				controllers[i]->TestPeriodic();
		}
	}

}
;

START_ROBOT_CLASS(Robot);
