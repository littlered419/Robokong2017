#include <iostream>
#include <memory>
#include <string>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <encoder.h>
#include <DriverStation.h>
#include <CounterBase.h>
#include <PWMSpeedController.h>
#include <Spark.h>
#include <Servo.h>

/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will
 * automatically call your Autonomous and OperatorControl methods at the right
 * time as controlled by the switches on the driver station or the field
 * controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're
 * inexperienced, don't. Unless you know what you are doing, complex code will
 * be much more difficult under this system. Use IterativeRobot or Command-Based
 * instead if you're new.
 */

class Robot: public frc::SampleRobot {

	frc::RobotDrive myRobot { 0, 1 }; // robot drive system
	frc::Joystick stickL {1}; // only joystick
	frc::Joystick stickR {2};
	frc::Joystick gamePad {0};
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	frc::Encoder *E0;
	frc::Encoder *E1;
	const std::string autoNameCustom = "My Auto";

	//Define OP, IP, and L as Sparks and in their proper ports.
	frc::Spark *OP = new Spark(2);
	frc::Spark *IP = new Spark(3);
	frc::Spark *L = new Spark(4);

	//Define GearL, GearR, BlockL, and BlockR as servos and in their proper ports.
	Servo *GearL = new Servo(5);
	Servo *GearR = new Servo(6);
	Servo *BlockL = new Servo(7);
	Servo *BlockR = new Servo(8);

	frc::DriverStation *ds;

public:
	Robot() {

		//Note SmartDashboard is not initialized here,
		//wait until RobotInit to make SmartDashboard calls
		ds = &frc::DriverStation::GetInstance();
		myRobot.SetExpiration(0.1);
		E0 = new Encoder(0,1,true,Encoder::EncodingType::k4X);
		E0->SetMaxPeriod(.1);
		E0->SetMinRate(10);
		E0->SetDistancePerPulse(5);
		E0->SetReverseDirection(true);
		E0->SetSamplesToAverage(7);
		E0->Reset();

		E1 = new Encoder(2,3,true,Encoder::EncodingType::k4X);
		E1->SetMaxPeriod(.1);
		E1->SetMinRate(10);
		E1->SetDistancePerPulse(5);
		E1->SetReverseDirection(false);
		E1->SetSamplesToAverage(7);
		E1->Reset();

	}//End Public Robot

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		//Reset both encoders
		E0->Reset();
		E1->Reset();
	}//End robotInit

	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */

	void Autonomous() {
		auto autoSelected = chooser.GetSelected();
		// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(1.0, 1.0); // spin at half speed
			frc::Wait(2);                // for 2 seconds

			//if(E0->GetRate() == 365){
				//myRobot.Drive(0.0, 0.0);  // stop robot
			//}//End if
		}//End if

		else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);


			//frc::Wait(2.0);            // for 2 seconds
			//myRobot.Drive(0.0, 0.0);  // stop robot

			//if(E0->GetDistance() >= 365){
			//	myRobot.Drive(0.0, 0.0);  // stop robot
			//}//End if

			//myRobot.Drive(-0.25, -0.25);  // drive forwards half speed

			while(E1->GetDistance() < 360){
				//myRobot.Drive(-0.75, -0.75);
				myRobot.TankDrive(-0.75, -0.75);
			}

			myRobot.Drive(0.0, 0.0);

		}//End else

	}//End Autonomous

	 //Runs the motors with arcade steering.

	void OperatorControl() override {
		char str[1000];
		char gstr[1000];
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled()) {
			double d1;
			d1=E0->GetRate();
			SmartDashboard::PutNumber("Rate", d1);
			SmartDashboard::PutNumber("Distance", E0->GetDistance());
			SmartDashboard::PutBoolean("Alive", true);
			std::sprintf(str, "%d, \n", d1);
						DriverStation::ReportError(str);

			double d2;
			d2=E1->GetRate();
			SmartDashboard::PutNumber("Rate1", d2);
			SmartDashboard::PutNumber("Distance1", E1->GetDistance());
			SmartDashboard::PutBoolean("Alive1", true);
			std::sprintf(gstr, "%d, \n", d2);
						DriverStation::ReportError(gstr);


		//Define the variable LB as one.  LB controls the direction of the robot
		int LB;
		LB = 1;

		//Define the variable A as one.  A controls the input motor.
		int A;
		A = 1;

		//Define the variable RB as one.  RB controls the output motor.
		int RB;
		RB = 1;

		//Define the variable Trigger as one.  Trigger controls the lift
		int Trigger;
		Trigger = 1;

		//Define the variable B as one.  B controls the gear gates.
		int B;
		B = 1;

		//Define the variable X as one.  X controls the fuel block.
		int X;
		X = 1;

			//Do anything within these brakets whenever the robot is enabled
			while(IsEnabled()){

			//Drive the robot in tank drive with the two joysticks, but if button 5 (LB) is
			//pressed, then change the front to the back and back to front.

			switch(LB){
			//If LB == 1 then drive the robot in the hopper orientation.
			case 1:
				//Drive the robot in tank drive in the hopper orientation.
				myRobot.TankDrive(gamePad.GetRawAxis(5), gamePad.GetRawAxis(1));

				//If the button (LB) is pressed, then make LB 2 and wait 0.25 seconds.
				if(gamePad.GetRawButton(5)){
					LB = 2;
					frc::Wait(0.25);
				}//End if(Direction == 1))
				break;//End case 1.

			//If LB == 2 then drive the robot in the gear orientation.
			case 2:
				//Drive the robot in tank drive in the gear orientation.
				myRobot.TankDrive(gamePad.GetRawAxis(1) / -1, gamePad.GetRawAxis(5) / -1);

				//If the button (LB) is pressed, then make LB 1 and wait 0.25 seconds
				if(gamePad.GetRawButton(5)){
					LB = 1;
					frc::Wait(0.25);
				}//End if(Direction == 1)
			break;//End case 2
			}//End switch(LB)


			//Make the motor to the fuel intake rotate on a toggle for button A.
			switch(A){
			//If A == 1 then tell the input motor to not move.
			case 1:
				//Set the input motors to 0.0 power (no power)
				IP->Set(0.0);

				//If the button (A) is pressed then change A to 2 and wait 0.25 seconds
				if(gamePad.GetRawButton(1)){
					A = 2;
					frc::Wait(0.25);
				}//End if(Input == 1)
			break;//End case 1

			//If A == 2 then drive the input motors at 75% power.
			case 2:
				//Set the input motors to 0.75 power (75% power)
				IP->Set(0.75);

				//If the button (A) is pressed then change A to be 1 and wait 0.25 seconds.
				if(gamePad.GetRawButton(1)){
					A = 1;
					frc::Wait(0.25);
				}//End if(Input == 1)
			break;//End case 2
			}//End switch(A)


			//Make the output motors move on a toggle when RB is pressed
			switch(RB){
			//When RB == 1 then tell the output motors to stop.
			case 1:
				//Set the output motors to 0.0 power (no power)
				OP->Set(0.0);

				//If the button (RB) is pressed then make RB 2 and wait 0.25 seconds.
				if(gamePad.GetRawButton(6)){
					RB = 2;
					frc::Wait(0.25);
				}//End if(Output == 1)
			break;//End case 1

			//If RB == 2 then drive the output motors at 100% power
			case 2:
				//Set the output motors 1.0 power (100%)
				OP->Set(-1.0);
				//If the button (RB) is pressed then change RB to be 1
				if(gamePad.GetRawButton(6)){
					RB = 1;
					frc::Wait(0.25);
				}//End if(Output == 1)
			break;//End case 2
			}//End switch(RB)


			//Control the climbing roller with both of the trigger axis
			switch(Trigger){
			//If Trigger == 1 then tell the lift motor to stop.
			case 1:
				//Set the lift motor to 0.0 power (no power).
				L->Set(0.0);

				//If both axises are being pushed then set Trigger to 1
				if(gamePad.GetRawAxis(3) && gamePad.GetRawAxis(2) > 0){
					Trigger = 1;
				}//End if(LiftUp && LiftDown > 0)

				//If LT is greater then 0 then set Trigger to be 2
				if(gamePad.GetRawAxis(2) > 0){
					Trigger = 2;
				}//End if(LiftDown > 0)

				//If RT is greater then 0 then set Trigger to be 3
				if(gamePad.GetRawAxis(3) > 0){
					Trigger = 3;
				}//End if(LiftUp > 0)
			break;//End case 1

			//If Trigger == 2 then drive the lift motor down.
			case 2:
				//Set the motor to rotate to the pressure put on the trigger divided by -1
				L->Set(gamePad.GetRawAxis(2) / -1);

				//If both axises are pressed then set Trigger to be 1
				if(gamePad.GetRawAxis(3) && gamePad.GetRawAxis(2) > 0){
					Trigger = 1;
				}//End if(LiftUp && LiftDown > 0)

				//If RT is greater then 0 then set Trigger to be 3
				if(gamePad.GetRawAxis(3) > 0){
					Trigger = 3;
				}//End if(LiftUp > 0)

				//If RT is is not being pressed then set Trigger to be 1
				if(gamePad.GetRawAxis(2) == 0){
					Trigger = 1;
				}//End if(LiftDown == 0)
			break;//End case 2

			//If Trigger == 3 then drive the lift motor up.
			case 3:
				//Set the lift motor to rotate with the same pressure applied to RT
				L->Set(gamePad.GetRawAxis(3));

				//If both triggers are pressed then set trigger to 1
				if(gamePad.GetRawAxis(3) && gamePad.GetRawAxis(2) > 0){
					Trigger = 1;
				}//End if(LiftUp && LiftDown > 0)

				//If LT is pressed then set Trigger to be 2
				if(gamePad.GetRawAxis(2) > 0){
					Trigger = 2;
				}//End if(LiftDown > 0)

				//If RT is not being pressed then set Trigger to be 1
				if(gamePad.GetRawAxis(3) == 0){
					Trigger = 1;
				}//End if(LiftUp == 0)
			break;//End case 3
			}//End switch(Trigger)

			//Gears start in the closed position at start, when button B is pressed close the
			//flood gates, when pressed again open them.  Repeat until disabled.

			switch(B){
			//When B == 1 then set the gear gates to the 'closed' position
			case 1:
				//Set GearL to be at starting angle 0.5 and GearR to be and angle 0.5.
				//5
				GearL->Set(0.5);
				//6
				GearR->Set(0.5);

				//If button B is pressed then define B as 2 and wait 0.25 seconds.
				if(gamePad.GetRawButton(2)){
					B = 2;
					frc::Wait(0.25);
				}//End if(Gear == 1)
			break;//End case 1

			//If B == 2 then set the gear gates to the 'open' position
			case 2:
				//Set GearL to 0.0 to open, and GearR to 0.0 for open.
				GearL->Set(1.0);
				GearR->Set(0.0);

				//If button B is pressed then define B to be 1 and wait 0.25 seconds
				if(gamePad.GetRawButton(2)){
					B = 1;
					frc::Wait(0.25);
				}//End if(Gear == 1)
			break;//End case 2
			}//End switch(B)

			//Set the Fuel block to the 'closed' position until the button is pressed then
			//change it to be 'open', repeat.
			switch(X){
			//If X == 1 then set the fuel block to the 'closed' position.
			case 1:
				//Set BlockL to 1.0, and BlockR to 0.0.
				BlockL->Set(1.0);
				BlockR->Set(0.0);

				//If the button has been pressed then change X to be 2 then wait 0.25 seconds.
				if(gamePad.GetRawButton(3)){
					X = 2;
					frc::Wait(0.25);
				}
			break;//End case 1.

			//If X == 2 then set the fuel block to the 'open' position.
			case 2:
				//Set BlockL to 0.6 and BlockR to 0.6
				BlockL->Set(0.4);
				BlockR->Set(0.6);

				//If the button has been pressed then change X to be 1 and wait 0.25 seconds.
				if(gamePad.GetRawButton(3)){
					X = 1;
					frc::Wait(0.25);
				}//End if(FuelBlock == 1)
			break;//End case 2
			}//End switch(X)

			// wait for a motor update time
			frc::Wait(0.005);
		}//End while(IsEnabled())
		}//End OperatorControl while
	}//End OperatorControl

	 //Runs during test mode

	void Test() override {

	}//End Test
};//End class robot

START_ROBOT_CLASS(Robot)
