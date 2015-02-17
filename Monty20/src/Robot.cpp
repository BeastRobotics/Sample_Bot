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

	void driveStraight(){
		addCommand(MOVE,3000);
	}
	void driveBack(){
		addCommand(MOVE,-3000);
	}

	Robot() :
			lw(NULL) {
		head = NULL;
		currentCommand = NULL;
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i] = NULL;
		}

		int chooser = Preferences::GetInstance()->GetInt("AutoChooser",1);
		switch(chooser){
		case 1:
			break;
		case 2:
			driveStraight();
			break;
		case 3:
			driveBack();
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
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
	int automonusCommand;


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
			SmartDashboard::PutNumber("Command Operation", currentCommand->operation);
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

};

START_ROBOT_CLASS(Robot);
