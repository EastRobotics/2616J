#include "main.h"
#include "2616J.h"
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO *
 * If no competition control is connected, this function will run immediately
 * following initialize()
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.

 */
 int intakerun;
 void set_motorso(int speed) {
   motor_move(MOTOR_DRIVE_FRONT_LEFT,speed);
   motor_move(MOTOR_DRIVE_FRONT_RIGHT,-speed);
   motor_move(MOTOR_DRIVE_BACK_RIGHT,-speed); 
   motor_move(MOTOR_DRIVE_BACK_LEFT,speed);
 }

 // Set motor to speed based on distance from `ticks`
 void motor_move_po(int motor, int ticks, float p) {
     set_motorso(p);
 }
 void wait_motor_move_aco(int motor, int ticks, float p, int actime) {
   int st = millis();
   int  ft = st + actime;
   int sp = (int)motor_get_position(motor);
       printf("ticks - %d pos - %d sp - %d\n\r",ticks,(int)motor_get_position(motor),sp);
   while ((abs(ticks)-abs(((int)motor_get_position(motor))-sp)) > 10) { // 10 = threshold, change to change where stop
     motor_move_po(motor, ticks, (millis() > ft )? p :  (p<0)?-50:50  );
       printf("ticks - %d pos - %d sp - %d\n\r",ticks,(int)motor_get_position(motor),sp);
     delay(20);
   }
 }


void opcontrol() {

	int left;
	int right;
	double driveMult = 1;

	int anglerUp;
	int anglerDown;

	int intakeIn;
	int intakeOut;

	bool liftUp;
	bool liftDown;

  //bool atSpeed;

	int count = 0;
  int timeout = 0;


//  char rpm[10] = "";


	motor_set_gearing(MOTOR_DRIVE_FRONT_LEFT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DRIVE_BACK_LEFT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DRIVE_FRONT_RIGHT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DRIVE_BACK_RIGHT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_ANGLER, E_MOTOR_GEARSET_36);
	motor_set_gearing(MOTOR_INTAKE_LEFT, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_INTAKE_RIGHT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_LIFT, E_MOTOR_GEARSET_36);
  //adi_port_set_config ('F',E_ADI_DIGITAL_IN);
  motor_set_reversed(8,0);
  motor_set_reversed(10,0);

	while(true) {
//  printf("%d\r\n",adi_analog_read_calibrated('F'));
		left = controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_LEFT_Y);
		right = controller_get_analog(E_CONTROLLER_MASTER, E_CONTROLLER_ANALOG_RIGHT_Y);

		intakeIn = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_R1);
		intakeOut = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_R2);

		anglerUp = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_UP);
    anglerDown = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_DOWN);

		liftUp = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L1);
		liftDown = controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L2);

		if(controller_get_digital_new_press(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)) {
			driveMult = 1;
		} else if(controller_get_digital_new_press(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_Y)) {
			driveMult = 0.7;
			//change low drive speed by changing (0.5) to something different
		}
      // sets the speed of the motors and how they should function
		motor_move(MOTOR_DRIVE_FRONT_LEFT, left * driveMult);
		motor_move(MOTOR_DRIVE_FRONT_RIGHT, -1 * right * driveMult);
		motor_move(MOTOR_DRIVE_BACK_LEFT, left * driveMult);
		motor_move(MOTOR_DRIVE_BACK_RIGHT, -1 * right * driveMult);

		if (intakeIn == 1){
			motor_move(MOTOR_INTAKE_LEFT, intakeIn * -127);
      motor_move(MOTOR_INTAKE_RIGHT, intakeIn * 127);
      motor_move(MOTOR_LIFT, liftDown * 30);
      motor_set_brake_mode(MOTOR_ANGLER, MOTOR_BRAKE_BRAKE);
    }
		else if (intakeOut == 1){
      motor_move(MOTOR_INTAKE_LEFT, intakeOut * 80);
      motor_move(MOTOR_INTAKE_RIGHT, intakeOut * -80);
    }
		else {
			motor_move(MOTOR_INTAKE_LEFT, 0);
      motor_move(MOTOR_INTAKE_RIGHT, 0);
      motor_move(MOTOR_LIFT, 0);
      motor_move(MOTOR_ANGLER, 0);
    }


		if (anglerUp == 1){
			motor_move(MOTOR_ANGLER, anglerUp * 64);
}
    else if (anglerDown == 1){
			motor_move(MOTOR_ANGLER, anglerDown * -102);
    }
		else {
		 	motor_move(MOTOR_ANGLER, 0);
}


if (liftUp == 1 && adi_digital_read('B') ){
//  while (!adi_digital_read('B')) {
  //  motor_set_brake_mode(MOTOR_ANGLER, MOTOR_BRAKE_BRAKE);
//}
  motor_tare_position(MOTOR_ANGLER);
   motor_move(MOTOR_ANGLER, 127);

}else if(liftUp == 1 && motor_get_position(MOTOR_ANGLER) <300 ){
     motor_move(MOTOR_ANGLER,127);

}else if(liftUp == 1 && motor_get_position(MOTOR_ANGLER) >300)
{
   motor_set_brake_mode(MOTOR_ANGLER, MOTOR_BRAKE_BRAKE);
   motor_move(MOTOR_LIFT,-127);
}

else if (liftDown == 1){
  motor_move(MOTOR_LIFT, liftDown * 127);
}

else {
  motor_move(MOTOR_LIFT, 0);
}

   }

}
