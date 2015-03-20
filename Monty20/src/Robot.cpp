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
#include "DriveTest.cpp"
#include "imu.h"
#include "AHRS.h"

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
#define AUTO_D 2000
#define AUTO_BACK -2000
#define AUTO_GCB -2000
#define AUTO_GT 3000
#define AUTO_GTC AUTO_GT
#define LEFT 0
#define RIGHT 1
#define ENABLE_AHRS
#define FrontLeftEncoderStart 2
#define FrontLeftEncoderEnd 3
#define FrontRightEncoderStart 0
#define FrontRightEncoderEnd 1
#define RearLeftEncoderStart 11
#define RearLeftEncoderEnd 10
#define RearRightEncoderStart 4
#define RearRightEncoderEnd 5

struct Command_Node {
	int index;
	int operation;
	Command_Node* nextCommand;
};

class Robot: public IterativeRobot {


    Encoder *FrontRight = new Encoder(FrontRightEncoderStart, FrontRightEncoderEnd);
    Encoder *FrontLeft = new Encoder(FrontLeftEncoderStart, FrontLeftEncoderEnd);
    Encoder *RearRight = new Encoder(RearRightEncoderStart, RearRightEncoderEnd);
    Encoder *RearLeft = new Encoder(RearLeftEncoderStart, RearLeftEncoderEnd);

	//Navx
	NetworkTable *table;
#if defined(ENABLE_AHRS)
	AHRS *imu;
#elif defined(ENABLE_IMU_ADVANCED)
	IMUAdvanced *imu;
#else // ENABLE_IMU
	IMU *imu;
#endif
	SerialPort *serial_port;
	bool first_iteration;
	//NavX

	IControl *controllers[NUM_CONTROLLERS];
	int autoReturns[NUM_CONTROLLERS];
	int autoDrive = AUTODRIVE;
	int grabTurnLeft = GRABTURNLEFT;
	int grabTurnRight = GRABTURNRIGHT;
	int commandNumber = 0;

	Command_Node* head;
	Command_Node* currentCommand;
public:

	void nothing() {
		addCommand(DELAY, WAIT);
	}
	void driveStraight() {
		addCommand(MOVE, AUTO_D);
	}
	void driveBack() {
		addCommand(MOVE, AUTO_BACK);
	}
	void getContainer() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(MOVE, AUTO_D);
		addCommand(DELAY, WAIT);
		addCommand(MOVE, 1);
	}
	void getContainerBack() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(MOVE, AUTO_GCB);
	}
	void getTote() {
		addCommand(GRABBER, G_CLOSE);
		addCommand(DELAY, WAIT);
		addCommand(LIFTER, 1000);
		addCommand(MOVE, RIGHT);
		addCommand(DELAY, WAIT);
		addCommand(MOVE, AUTO_GT);
	}
	void getToteContainer() {
		addCommand(GRABBER, G_CLOSE); //Grab container
		addCommand(LIFTER, 2000); //Lift container
		addCommand(MOVE, 500); //Drive over tote
		addCommand(LIFTER, -500); //Lower container on tote
		addCommand(GRABBER, G_OPEN); //Release container on tote
		addCommand(LIFTER, -1500); //Lower to tote level
		addCommand(GRABBER, G_CLOSE); //Close on tote
		addCommand(LIFTER, 1000); //lift tote
		addCommand(MOVE, RIGHT); //turn to face auto zone
		addCommand(MOVE, AUTO_GTC); //drive to auto zone
		addCommand(MOVE, LEFT);//turn while in the auto zone
	}
	void driveStraightTest() {
		addCommand(MOVE, 4);
	}

	Robot() :
			lw(NULL) {
		head = NULL;
		currentCommand = NULL;
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			controllers[i] = NULL;
		}

		controllers[0] = NewXboxController::getInstance(1);
		controllers[1] = new LifterControl();
		//controllers[1] = new LifterBrake();
		controllers[2] = new CompressorControl();
		//controllers[3] = new ArcadeDrive();
		controllers[3] = new MecanumDrive();
		//controllers[3] = new DriveTest();
		controllers[4] = new GrabberControl();
		controllers[5] = new CameraControl();
		controllers[6] = NewXboxController::getInstance(0);


		//lifter = new LifterControlTester();
		//controllers[5] = new MecanumDrive();
		controllers[6] = new AutoDelay();
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			autoReturns[i] = 0;
		}

		//NavX
		table = NetworkTable::GetTable("datatable");
		serial_port = new SerialPort(57600, SerialPort::kMXP);
		uint8_t update_rate_hz = 50;
#if defined(ENABLE_AHRS)
		imu = new AHRS(serial_port, update_rate_hz);
#elif defined(ENABLE_IMU_ADVANCED)
		imu = new IMUAdvanced(serial_port,update_rate_hz);
#else // ENABLE_IMU
		imu = new IMU(serial_port, update_rate_hz);
#endif
		if (imu) {
			LiveWindow::GetInstance()->AddSensor("IMU", "Gyro", imu);
		}
		first_iteration = true;
		//NavX
	}

private:
	LiveWindow *lw;
	//int automonusCommand;

	void addCommand(int index, int operation) {
		SmartDashboard::PutNumber("Auto Thingy", 1);
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
	}
	void Delete(Command_Node* node) {
		if (node != NULL) {
			Delete(node->nextCommand);
			delete node;
		}
		node = NULL;
	}
	void AutonomousInit() {
		commandNumber = 1;

		if (head != NULL) {
			Delete(head);
			head = NULL;
		}
		getContainer();
		/*int chooser = SmartDashboard::GetNumber("Auto Thingy");
		 switch (chooser) {
		 case 1:
		 SmartDashboard::PutString("ChooserValue", "You Failed At Life");
		 nothing();
		 break;
		 case 2:
		 SmartDashboard::PutString("ChooserValue",
		 "You can drive straight!");
		 driveStraight();
		 break;
		 case 3:
		 SmartDashboard::PutString("ChooserValue", "Driving back");
		 driveBack();
		 break;
		 case 4:
		 SmartDashboard::PutString("ChooserValue", "Get Container");
		 getContainer();
		 break;
		 case 5:
		 SmartDashboard::PutString("ChooserValue",
		 "Get Container Backwards");
		 getContainerBack();
		 break;
		 case 6:
		 SmartDashboard::PutString("ChooserValue", "Get the Tote");
		 getTote();
		 break;
		 case 7:
		 SmartDashboard::PutString("ChooserValue",
		 "You got everything!!!!!!!!!!");
		 getToteContainer();
		 break;
		 case 8:
		 SmartDashboard::PutString("ChooserValue",
		 "Drive forward test");
		 driveStraightTest();
		 break;
		 }*/
		currentCommand = head;
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			if (controllers[i] != NULL)
				controllers[i]->AutonomousInit();
		}

	}
	void AutonomousPeriodic() {

		SmartDashboard::PutBoolean("DoneAuto", currentCommand == NULL);
		if (currentCommand != NULL) {
			SmartDashboard::PutNumber("Command Number", commandNumber++);
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
		SmartDashboard::PutBoolean("IMU_Connected", imu->IsConnected());
		for (int i = 0; i < NUM_CONTROLLERS; i++) {
			SmartDashboard::PutNumber("State 2", i);
			if (controllers[i] != NULL)
				controllers[i]->TeleopInit();
		}

		//lifter->init();
		FrontRight->Reset();
		FrontLeft->Reset();
		RearRight->Reset();
		RearLeft->Reset();

	}

	void TeleopPeriodic() {
		
		//TODO Encoders
		SmartDashboard::PutNumber("FrontRight",FrontRight->GetRate());
		SmartDashboard::PutNumber("FrontLeft",FrontLeft->GetRate());
		SmartDashboard::PutNumber("RearRight",RearRight->GetRate());
		SmartDashboard::PutNumber("RearLeft",RearLeft->GetRate());

		
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
	void OperatorControl(void) {
		while (IsOperatorControl()) {
			if (first_iteration) {
				bool is_calibrating = imu->IsCalibrating();
				if (!is_calibrating) {
					Wait(0.3);
					imu->ZeroYaw();
					first_iteration = false;
				}
			}


#if defined (ENABLE_IMU_ADVANCED) || defined(ENABLE_AHRS)

#if defined (ENABLE_AHRS)

#endif
#endif
			Wait(0.2);				// wait for a while
		}
	}

}
;

START_ROBOT_CLASS(Robot);
