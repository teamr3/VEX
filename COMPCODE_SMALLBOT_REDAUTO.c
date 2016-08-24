#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in3,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl2,  lift,           sensorDigitalOut)
#pragma config(Sensor, I2C_1,  shooterIEM,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           feed,          tmotorVex393, openLoop)
#pragma config(Motor,  port2,           shooter0,      tmotorVex393, openLoop)
#pragma config(Motor,  port3,           shooter1,      tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port4,           shooter2,      tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port5,           shooter3,      tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port6,           leftDrive1,    tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port7,           leftDrive2,    tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port8,           rightDrive1,   tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_3, 1000)
#pragma config(Motor,  port9,           rightDrive2,   tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port10,          intake1,       tmotorVex393, openLoop)
#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "navx.c"
#include "shooterpidapril15_small.c"
#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


void forward(float ticks,float power);
void backward(float ticks);
void teleop();
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
  motor[intake1] = -127;
  	motor[rightDrive1] = -127;
		motor[rightDrive2] = -127;
		motor[leftDrive1] = +127;
		motor[leftDrive2] = +127;
  wait1Msec (900);
   	motor[rightDrive1] = +127;
		motor[rightDrive2] = +127;
		motor[leftDrive1] = -127;
		motor[leftDrive2] = -127;
		wait1Msec (30);
   	motor[rightDrive1] = 0;
		motor[rightDrive2] = 0;
		motor[leftDrive1] = 0;
		motor[leftDrive2] = 0;
		wait1Msec (1231232132);

// navxTurn (0, 1500);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
task shooter();
task usercontrol()
{
	// User control code here, inside the loop
StartTask(shooter);
	while (true)
	{
	teleop();
	}
}
int forward_output = 0;
int turn_output = 0;
float shot = 3;

void teleop()
{
	forward_output = -vexRT[Ch3];
	turn_output = vexRT[Ch1];
	int left_output = turn_output - forward_output;
	int right_output = turn_output + forward_output;

	if( abs(left_output) > 127 )
		left_output = 127 * left_output/abs(left_output);
	if( abs(right_output) > 127 )
		right_output = 127 * right_output/abs(right_output);

	if (vexRT[Btn5U] == 1)
	{
	motor[leftDrive1] = left_output*.5;
	motor[leftDrive2] = left_output*.5;
	motor[rightDrive1] = right_output*.5;
	motor[rightDrive2] = right_output*.5;
}else
{
	motor[leftDrive1] = left_output;
	motor[leftDrive2] = left_output;
	motor[rightDrive1] = right_output;
	motor[rightDrive2] = right_output;
}
	if( vexRT[Btn8Uxmtr2] == 1 )
	shot = 1;

	if( vexRT[Btn8Dxmtr2] == 1 )
	shot =2;

	if( vexRT[Btn7Dxmtr2] == 1 )
	shot=3;

	if( vexRT[Btn6U] == 1 )
	{
		motor[intake1] = -127;
  }
  else if( vexRT[Btn6D] == 1 )
  {
		motor[intake1] = 127;
  }
  else if (vexRT[Btn5DXmtr2] == 1  )
  {
  	motor[intake1] = -127;
  }
  else
  {
		motor[intake1] = 0;
  }

	if( vexRT[Btn6UXmtr2] == 1 )
	{
		motor[feed] = 127;
	}
  else if( vexRT[Btn6DXmtr2] == 1 )
  {
		motor[feed] = -127;
  }
  else if (vexRT[Btn5DXmtr2] == 1  && shooter_velocity >3310)
  {
  	motor [feed] = 127;
  }
	else
		motor[feed] = 0;
}

task shooter()
{
	initialize_shooter();
	while(true)
	{
		if (shot == 1)
		{
			setpoint_rpm = 2100;
		}
		if (shot == 2)
		{
			setpoint_rpm = 3500;
		}
		if (shot ==3)
		{
			setpoint_rpm = 0;
		}
		iterative(setpoint_rpm);
	}
}



void forward(float ticks, float power){
	float prevEnc = nMotorEncoder[leftDrive1];
	while(true){
		motor[rightDrive1] = +power;
		motor[rightDrive2] = +power;
		motor[leftDrive1] = -power;
		motor[leftDrive2] = -power;
		if((prevEnc - nMotorEncoder[leftDrive1]) < ticks){
			break;
		}
	}
	motor[rightDrive1] = 0;
	motor[rightDrive2] = 0;
	motor[leftDrive1] = 0;
	motor[leftDrive2] = 0;

}
 void backward(float ticks){
	float prevEnc = nMotorEncoder[rightDrive1];
	while(true){
		motor[rightDrive1] = -127;
		motor[rightDrive2] = -127;
		motor[leftDrive1] = 127;
		motor[leftDrive2] = 127;
		if((nMotorEncoder[rightDrive1])-prevEnc  > ticks){
			break;
		}
	}
	motor[rightDrive1] = 0;
	motor[rightDrive2] = 0;
	motor[leftDrive1] = 0;
	motor[leftDrive2] = 0;

}
