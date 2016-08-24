#pragma config(UART_Usage, UART1, uartUserControl, baudRate57600, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartUserControl, baudRate57600, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    light1,         sensorReflection)
#pragma config(Sensor, in2,    light2,         sensorReflection)
#pragma config(Sensor, dgtl1,  ,               sensorDigitalOut)
#pragma config(Sensor, dgtl2,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl3,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl4,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl5,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl6,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl7,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl8,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl9,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl10, ,               sensorDigitalIn)
#pragma config(Sensor, dgtl11, ,               sensorTouch)
#pragma config(Sensor, dgtl12, ,               sensorDigitalIn)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           intake1,       tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port2,           leftDrive1,    tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port3,           leftDrive2,    tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port4,           rightDrive1,   tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port5,           rightDrive2,   tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port6,           feed,          tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port7,           shooter1,      tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port8,           shooter2,      tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port9,           shooter3,      tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port10,          intake2,       tmotorVex393HighSpeed, openLoop)
#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#include "navx.c"
#include "shooterpidapril15.c"

void findBall();
void calculateBallX();
void turn();
void driveForward();
void teleop();
void TurnCutie (float ticks, float power);
void forward(float ticks,float power);
void backward(float ticks);
void runTilWall(float output);
void runTilWallBumper (float output);
void drive(float output, long duration);
void driveTilDetect(float output);

float pixy_output = 0;
task PIXYReceive();
task shooter();
task check4Balls();
task countBalls();
bool fked_up = false;
bool fetching = true;
float prev_time_kill;
bool has4Balls = false;
float trigger = 0;
float shot = 3;
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
		 SensorValue[dgtl1] = 0;
	   enc = nMotorEncoder[rightDrive1];
	   wait10Msec(1);

 		StartTask(PIXYReceive);
 		StartTask(NAVXReceive);
 		StartTask(countBalls);
 		StartTask(shooter);
 		StartTask(check4Balls);

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
 		  /////////////////////////////////////////////////////////////////////////
   		//HARDCODE AUTO CODE ////////////////////////////////////////////////////
		  /////////////////////////////////////////////////////////////////////////
 				motor[intake1] = -127;
				motor[intake2] = 127;

			StartTask (shooter);
			shot = 1;

		   navxTurn (61,1100);

      //navxForward(1400, 61);
    	navxForward(850, 61);
    	wait1Msec (100);
    	navxTurn (63,200);
    	navxTurn (61,200);
    	navXForward(300,61);
		  // navxForwardFast (1100,61);
      wait1Msec(350);
      navxTurn(15, 1200);
      navxForward(440, 15);

	    ok_shoot_buddy = true;
	    wait1Msec(3000);
	    ok_shoot_buddy = false;

	    navxBackward (300,0);
      navxTurn (90,1200);
	    forward (110,50);

	    wait1Msec (1000);
	    backward (150);
	    navxTurn (15,1200);
	    forward (200,70);
	    navxTurn (15,503);
	    ok_shoot_buddy = true;
	    wait1Msec (3000);
	    ok_shoot_buddy= false;


	    wait1Msec (12312312312);



	  /////////////////////////////////////////////////////////////////////////
    //BALL TRACKING /////////////////////////////////////////////////////////
	  /////////////////////////////////////////////////////////////////////////
		SensorValue[dgtl1] = 0;

 		fetching = true;
	  ballCount = 0;
	 TurnCutie (200,50);
	 while(!has4Balls)
   {
   prev_time_kill = time1[T1];
    turn();
    driveForward();
   }
   fetching = false;




	 ///////////////////////////////////////////////////////////////////////////
   //TRACK NET NOW ///////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////
   SensorValue[dgtl1] = 1;
   ClearTimer (T4);
	 SensorValue[dgtl1] = 1;
	 navxTurn (45,1500);

		//	  while (time1[T4] < 10);
		//   {
		//   turn();
		// 		}

	 runTilWall (40);
	 drive(-30, 200);
	 navxTurn (-45,1500);

	 	ClearTimer (T1);
		while (true)
	 	{
	  	motor[rightDrive1] = 60;
			motor[rightDrive2] = 60;
			motor[leftDrive1] = -60;
			motor[leftDrive2] = -60;

			lightsensor = SensorValue[light2];
	  	if (lightsensor < 2500)
	  	{
				trigger = 1;
				motor[rightDrive1] = 0;
				motor[rightDrive2] = 0;
				motor[leftDrive1] = 0;
				motor[leftDrive2] = 0;
				break;
			}


	  	if (SensorValue[dgtl11] == 1)
	  	{
	  		trigger = 2;
	  		navxTurn( -45,500);
	  		break;
	  	}
		}

	 if (trigger == 2)
	 {
	   lightsensor = SensorValue[light2];
	   while (lightsensor >2500)
	   {
	  		lightsensor = SensorValue[light2];
	  		motor[rightDrive1] = -60;
				motor[rightDrive2] = -60;
				motor[leftDrive1] = 60;
				motor[leftDrive2] = 60;
	 		}
		 		motor[rightDrive1] = 0;
				motor[rightDrive2] = 0;
				motor[leftDrive1] = 0;
				motor[leftDrive2] = 0;
				trigger = 1;
	 }

	 if (trigger == 1)
	 {
	   	  trigger = 0;
	   		navxTurn (45,1500);
	 }

	 ok_shoot_buddy = true;
	 wait1Msec (3000);
	 ok_shoot_buddy= false;

	 ///////////////////////////////////////////////////////////////////////////
   //TRACK MORE BALLS NOW ////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	// User control code here, inside the loop
  StartTask(shooter);
	while (true)
	{
	   		teleop();
	}
}


bool lastLightSensor = false;
float lastChange = 0;

task check4Balls(){
	while (true)
	{
	float	  lightsensor = SensorValue[light1];
	bool ballThere = lightsensor < 3000;

	if(ballThere != lastLightSensor){
		lastLightSensor = ballThere;
		lastChange = time1[T1];
	}
	has4Balls = ((time1[T1] - lastChange) > 1000 && lastLightSensor);
	}
}
task shooter()
{
	initialize_shooter();
	while(true)
	{
		if (shot == 1)
		{
			setpoint_rpm = 3000;
		}
		if (shot == 2)
		{
			setpoint_rpm = 2600;
		}
		if (shot == 3)
		{
			setpoint_rpm = 0;
		}
		iterative(setpoint_rpm);
	}
}

task PIXYReceive()
{
 while (true)
 {
   port[0] = SensorValue[dgtl2];
   port[1] = SensorValue[dgtl3];
   port[2] = SensorValue[dgtl4];
   port[3] = SensorValue[dgtl5];
   port[4] = SensorValue[dgtl6];
   port[5] = SensorValue[dgtl7];
   port[6] = SensorValue[dgtl8];
   port[7] = SensorValue[dgtl9];
   port[8] = SensorValue[dgtl10];
   port[9] = SensorValue[dgtl12];

   enc = nMotorEncoder[rightDrive1];

   if( SensorValue[dgtl12] == 1 )
   {
     calculateBallX();
   }
 }
}
void forward(float ticks, float power){
	float prevEnc = nMotorEncoder[rightDrive1];
	while(true){
		motor[rightDrive1] = power;
		motor[rightDrive2] = power;
		motor[leftDrive1] = -power;
		motor[leftDrive2] = -power;
		if((prevEnc - nMotorEncoder[rightDrive1]) > ticks){
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

bool incremented = false;

void TurnCutie(float ticks, float power){
	float prevEnc = nMotorEncoder[rightDrive1];
	while(true){
		motor[rightDrive1] =-power;
		motor[rightDrive2] =-power;
		motor[leftDrive1] = -power;
		motor[leftDrive2] = -power;
		if((prevEnc - nMotorEncoder[rightDrive1]) > ticks){
			break;
		}
	}
	motor[rightDrive1] = 0;
	motor[rightDrive2] = 0;
	motor[leftDrive1] = 0;
	motor[leftDrive2] = 0;
}


task countBalls()
{
	while(true)
	{
		  lightsensor = SensorValue[light1];
			if( hasBall && !incremented )
			{
				ballCount++;
				incremented = true;
			}
			else
			{
				hasBall = lightsensor < 3000;
				if( !hasBall )
					incremented = false;
			}
		 float prev_time = time1[T4];
		  if(fetching)
		  {
				if( ballCount == 3 && (time1[T4]-prev_time) <120 )
				{
					motor[feed] = -127;
				}
				else
				{
					motor[feed] = 0;
				}
				}
			wait1Msec(1);
	}
}


void calculateBallX()
{
	int num = 0;
	int i = 0;
	int mask = 0x01;
	for( i = 0; i < 9; i++ )
	{
		num += port[i]*(mask << i);
	}
	ballX = num;
}

void findBall()
{
	bool ballFound = false;
	while(!ballFound)
	{
		navxTurnForBall(angle+30);
		ballFound = ballX < 500;
		wait1Msec(200);
	}
}

float pixy_kp = 0.1;//0.05
float pixy_ki = 0;
float pixy_kd = 0.05;
float setpoint = 160;

float PIXY_TOLERANCE = 10;
float MIN_PIXY_LOOP_TIME = 10;

void turn()
{
	float delta_time = 0;
	float prev_time = 0;

	float pixy_prev_error = 0;
	float pixy_error = 0;
	float pixy_delta_error = 0;

	float pixy_error_sum = 0;
	float pixy_output = 0;


	ClearTimer(T2);
	prev_time = time1[T2];

	while( abs(setpoint - ballX) > PIXY_TOLERANCE && (time1[T1] - prev_time_kill) < 4000 )
	{
		if( (time1[T2] - prev_time) > MIN_PIXY_LOOP_TIME )
		{
			//Delta
			delta_time = time1[T2] - prev_time;
			pixy_delta_error = pixy_error - pixy_prev_error;

			//Set Previous
			prev_time = time1[T2];

			pixy_prev_error = pixy_error;
			pixy_error = setpoint - ballX;
			pixy_error_sum = pixy_error_sum + pixy_error*delta_time/1000.0;

			pixy_output  =  pixy_kp*pixy_error  +  pixy_ki*pixy_error_sum  -  pixy_kd*pixy_delta_error/delta_time;

			pixy_output = -pixy_output;
			float max_value = 127*0.7;
			if(abs(pixy_output) > max_value )
			{
				pixy_output = max_value *(pixy_output/abs(pixy_output));
			}
			if( ballX > 500 )
				pixy_output = 10;
			float def_value = 0;

			float range = 30;
			float max_def_value = 20; //20
			if( abs(pixy_error) > range )
			{
				float a = abs(pixy_error) * max_def_value/range;
				if( a > max_def_value )
					a = max_def_value;
				def_value = (pixy_output/abs(pixy_output))*a;
			}


			left_output  = (pixy_output+def_value);
			right_output = (pixy_output+def_value);
			motor[leftDrive1] = (int)left_output;
			motor[leftDrive2] = (int)left_output;
			motor[rightDrive1] = (int)right_output;
			motor[rightDrive2] = (int)right_output;
		}
		if (has4Balls){
    	break;
    }
	}
	left_output = 0;
	right_output = 0;
	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;
}
float drive_output = 50;
	float delta_time222 = 0;
void driveForward()
{
	motor[intake1] = -127;
	motor[intake2] = 127;



	float prev_time = 0;
	float prev_angle = 0;

	float pixy_prev_error = 0;
	float pixy_error = 0;
	float pixy_delta_error = 0;

	float pixy_error_sum = 0;
	float pixy_output = 0;


	ClearTimer(T3);
	prev_time = time1[T3];

	int current_ballcount = ballCount;

	while((time1[T1] - prev_time_kill) < 1000 )
	{
		if( (time1[T3] - prev_time) > MIN_PIXY_LOOP_TIME  )
		{
							if (SensorValue[dgtl11] == 1 )
							{
							navxBackward (500,angle);
							TurnCutie (200,50);
							break;
							}

			//Delta

			pixy_delta_error = pixy_error - pixy_prev_error;

			//Set Previous
			prev_time = time1[T3];
			prev_angle = ballX;

			pixy_prev_error = pixy_error;
			pixy_error = setpoint - ballX;
			pixy_error_sum = pixy_error_sum + pixy_error*delta_time/1000.0;



			pixy_output  =  pixy_kp*pixy_error  +  pixy_ki*pixy_error_sum;  //-  pixy_kd*pixy_delta_error/delta_time;

			pixy_output = -pixy_output;

			if(abs(pixy_output) > 127 )
				pixy_output = 127*pixy_output/abs(pixy_output);

			float def_value = 0;
			float range = 30;
			float max_def_value = 20;
			if( abs(pixy_error) > range )
			{
				float a = abs(pixy_error) * max_def_value/range;
				if( a > max_def_value )
					a = max_def_value;
				def_value = (pixy_output/abs(pixy_output))*a;
			}


			pixy_output = def_value*0.75;//(pixy_output * 0.75) * 0.4 + def_value;
			left_output = pixy_output - drive_output;
			right_output = pixy_output + drive_output;
			motor[leftDrive1] = (int)left_output;
			motor[leftDrive2] = (int)left_output;
			motor[rightDrive1] = (int)right_output;
			motor[rightDrive2] = (int)right_output;

//		prev_time = time1[T3];

	    if (has4Balls){
	    	break;
	    }


		}
	}

	left_output = 0;
	right_output = 0;
	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;
	motor[port1] = 0;
	motor[port10] = 0;
}

void runTilWall(float output)
{
	bool hitWall = false;
	left_output = -output;
	right_output = output;
	int count = 0;

	float deltaEnc = 0;
	float lastEnc = 0;
	lastEnc = nMotorEncoder[rightDrive1];

	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;
	wait1Msec(500);

	while(!hitWall)
	{
		wait1Msec(200);
		deltaEnc = nMotorEncoder[rightDrive1] - lastEnc;
		lastEnc = nMotorEncoder[rightDrive1];
		hitWall = abs(deltaEnc) < 5;
	}

	left_output = 0;
	right_output = 0;
	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;

}
void runTilWallBumper(float output)
{
	bool hitWall = false;
	left_output = -output;
	right_output = output;
	int count = 0;

	float deltaEnc = 0;
	float lastEnc = 0;
	lastEnc = nMotorEncoder[rightDrive1];

	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;
	wait1Msec(500);

	while(!hitWall)
	{
		hitWall = SensorValue [dgtl11];
		wait1Msec (20);
	}

	left_output = 0;
	right_output = 0;
	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;

}

	bool hitWall = false;
	float deltaEnc = 0;
	float lastEnc =  nMotorEncoder[rightDrive1];


void drive(float output,long duration)
{
	left_output = -output;
	right_output = output;

	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;

	wait1Msec(duration);

	left_output = 0;
	right_output = 0;
	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;

}
void driveTilDetect(float output)
{
	left_output = -output;
	right_output = output;

	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;
	while(ballX > 500)
	{
		wait1Msec(10);
	}

	left_output = 0;
	right_output = 0;
	motor[leftDrive1] = (int)left_output;
	motor[leftDrive2] = (int)left_output;
	motor[rightDrive1] = (int)right_output;
	motor[rightDrive2] = (int)right_output;

}

int forward_output = 0;
int turn_output = 0;

void teleop()
{
	forward_output = vexRT[Ch3];
	turn_output = vexRT[Ch1];
	int left_output = turn_output - forward_output;
	int right_output = turn_output + forward_output;

	if( abs(left_output) > 127 )
		left_output = 127 * left_output/abs(left_output);
	if( abs(right_output) > 127 )
		right_output = 127 * right_output/abs(right_output);

	motor[leftDrive1] = left_output;
	motor[leftDrive2] = left_output;
	motor[rightDrive1] = right_output;
	motor[rightDrive2] = right_output;


//  if( vexRT[Btn7U] == 1 )
//		shooter_pid_on = true;
//	if( vexRT[Btn7D] == 1 )
//		shooter_pid_on = false;
	if( vexRT[Btn5D] == 1 )
	shot = 1;

	if( vexRT[Btn7U] == 1 )
	shot = 2;

	if( vexRT[Btn7D] == 1 )
	shot = 3;


	if( vexRT[Btn6U] == 1 )
	{
		motor[intake1] = -127;
		motor[intake2] = 127;
  }
  else if( vexRT[Btn6D] == 1 )
  {
		motor[intake1] = 127;
		motor[intake2] = -127;
  }
  else
  {
		motor[intake1] = 0;
		motor[intake2] = 0;
  }
	if( vexRT[Btn5U] == 1 )
	{
		motor[feed] = -127;

	}//else	if( vexRT[Btn5D] == 1 )
//	motor[feed] = 127;
else
		motor[feed] = 0;
}
