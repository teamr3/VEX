#include "main.h"

void arcadedrive() {
	int x, y, driveleft, driveright, maxvalue;

	if(abs(joystickGetAnalog(1,1))>15)
		x = joystickGetAnalog(1,1);
	else
		x = 0;
	if(abs(joystickGetAnalog(1,3))>15)
		y = -joystickGetAnalog(1,3);
	else
		y = 0;
	driveleft = y - x;
	driveright = y + x;

	maxvalue = abs(driveleft);

	if (abs(driveright) > maxvalue)
		maxvalue = abs(driveright);

	if (maxvalue > 127){
		driveleft = 127 * driveleft / maxvalue;
		driveright = 127 * driveright / maxvalue;}

	motorSet(9, driveright);
	motorSet(8, driveright);
	motorSet(7, -driveleft);
	motorSet(6, -driveleft);
}

void tankdrive() {
	int y = joystickGetAnalog(1,3);
	int x = joystickGetAnalog(1,2);
	//DRIVETRAIN -- 2&3 = RIGHT, 4&5 = LEFT

	if (abs(x)>15||abs(y)>15) {
		motorSet(9,-x);
		motorSet(8,-x);
		motorSet(7,y);
		motorSet(6,y);}
	else{
		motorStop(9);
		motorStop(8);
		motorStop(7);
		motorStop(6);}
}

void intake() {
	if (joystickGetDigital(1,6,JOY_UP)==true) //INTAKE
		motorSet(10,127);
	else if (joystickGetDigital(1,6,JOY_DOWN)==true) // REVERSE
		motorSet(10,-127);
	else
		motorStop(10);
}

void feed() {
	if (joystickGetDigital(1,7,JOY_UP)==true) //FEED
		motorSet(1,127);
	else if (joystickGetDigital(1,5,JOY_UP)==true) // REVERSE
		motorSet(1,-127);
	else
		motorStop(1);
}

void shooter() {
	if (joystickGetDigital(1,7,JOY_DOWN)==true){ //SHOOT
		motorSet(2,127);
		motorSet(3,127);
		motorSet(4,127);
		motorSet(5,127);}
	else if (joystickGetDigital(1,5,JOY_DOWN)==true){ // REVERSE
		motorSet(2,-127);
		motorSet(3,-127);
		motorSet(4,-127);
		motorSet(5,-127);}
	else {
		motorStop(2);
		motorStop(3);
		motorStop(4);
		motorStop(5);}
}

void operatorControl() {
	int x=2;
	while (1) {
		//****CONTROLLER 1****

		arcadedrive();

		intake(); //INTAKE (port 1)

		//****CONTROLLER 2****

		feed(); //FEED (port 10)

		shooter(); //SHOOTER (port 2 3 4 5)

		delay(25);
	}
}
