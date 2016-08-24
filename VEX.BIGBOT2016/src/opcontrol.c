#include "main.h"

void arcadedrive() {
	int x, y, driveleft, driveright, maxvalue;

	if(abs(joystickGetAnalog(1,1))>25)
		x = joystickGetAnalog(1,1);
	else
		x = 0;
	if(abs(joystickGetAnalog(1,3))>25)
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

	motorSet(2, driveright);
	motorSet(3, driveright);
	motorSet(4, -driveleft);
	motorSet(5, -driveleft);
}

void tankdrive() {
	int y = joystickGetAnalog(1,3);
	int x = joystickGetAnalog(1,2);
	//DRIVETRAIN -- 2&3 = RIGHT, 4&5 = LEFT

	if (abs(x)>15||abs(y)>15) {
		motorSet(2,-x);
		motorSet(3,-x);
		motorSet(4,y);
		motorSet(5,y);}
	else{
		motorStop(2);
		motorStop(3);
		motorStop(4);
		motorStop(5);}
}

void intake() {
	if (joystickGetDigital(1,6,JOY_UP)==true){ //INTAKE
		motorSet(1,-127);
		motorSet(10,127);}
	else if (joystickGetDigital(1,8,JOY_LEFT)==true){ // REVERSE
		motorSet(1,127);
		motorSet(10,-127);}
	else {
		motorStop(10);
		motorStop(1);}
}

void feed() {
	if (joystickGetDigital(1,7,JOY_UP)==true){ //FEED
		motorSet(6,127);}
	else if (joystickGetDigital(1,5,JOY_UP)==true){ // REVERSE
		motorSet(6,-127);}
	else
		motorStop(6);
}

void shooter() {
	if (joystickGetDigital(1,6,JOY_DOWN)==true){ //SHOOT
		motorSet(9,-127*.9);
		motorSet(8,-127*.9);
		motorSet(7,-127*.9);}
	else if (joystickGetDigital(1,5,JOY_DOWN)==true){ // FENDER
		motorSet(9,-127);
		motorSet(8,-127);
		motorSet(7,-127);}
	else if (joystickGetDigital(1, 7, JOY_RIGHT) == true){//REVERSE
		motorSet(9,127);
		motorSet(8,127);
		motorSet(7,127);}
	else {
		motorStop(9);
		motorStop(8);
		motorStop(7);}
}

void operatorControl() {
	//int x=1;
	while (1) {
		//****CONTROLLER 1****

		//if (joystickGetDigital(1,7,JOY_LEFT) == true && joystickGetDigital (1,8,JOY_RIGHT) == true){
			//x++;}
		//if ((x/2)*2 == x )
			//tankdrive();
		//else

		arcadedrive();

		intake(); //INTAKE (port 1 10)

		//****CONTROLLER 2****

		feed(); //FEED (port 6)

		shooter(); //SHOOTER (port 7 8)

		delay(25);}
}
