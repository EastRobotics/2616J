#include "main.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

#define turn 620
#define forward1 1250
#define backward1 1200
#define forward2 425
#define forward3 80
#define turnf 100
#define forward4 -1200

void set_motors(int speed) {
  motor_move(MOTOR_DRIVE_FRONT_LEFT,speed);
  motor_move(MOTOR_DRIVE_FRONT_RIGHT,speed);
  motor_move(MOTOR_DRIVE_BACK_RIGHT,speed);
  motor_move(MOTOR_DRIVE_BACK_LEFT,speed);
}

// Set motor to speed based on distance from `ticks`
void motor_move_p(int motor, int ticks, float p) {
    set_motors(p);
}

// Move to position `ticks` using p for speed
void wait_motor_move_p(int motor, int ticks, float p) {
  while ((ticks-motor_get_position(motor)) > 10) { // 10 = threshold, change to change where stop
    motor_move_p(motor, ticks, p);
    delay(25);
  }
}

void wait_motor_move_p_ac(int motor, int ticks, float p, int actime) {
  int st = millis();
  int  ft = st + actime;
  while ((ticks-motor_get_position(motor)) > 10) { // 10 = threshold, change to change where stop
    motor_move_p(motor, ticks, (millis() > ft )? p : (p<0)?-50:50 );

    delay(20);
  }
}

void wait_motor_move_ac(int motor, int ticks, float p, int actime) {
  int st = millis();
  int  ft = st + actime;
  int sp = (int)motor_get_position(motor);
      printf("ticks - %d pos - %d sp - %d\n\r",ticks,(int)motor_get_position(motor),sp);
  while ((abs(ticks)-abs(((int)motor_get_position(motor))-sp)) > 10) { // 10 = threshold, change to change where stop
    motor_move_p(motor, ticks, (millis() > ft )? p :  (p<0)?-50:50  );
      printf("ticks - %d pos - %d sp - %d\n\r",ticks,(int)motor_get_position(motor),sp);
    delay(20);
  }
}


// Move `ticks` forward from the current position using p for speed
void wait_motor_move_p_rel(int motor, int ticks, float p) {

  wait_motor_move_p(motor, motor_get_position(motor) + ticks, p);
}

void wait_motor_move_rel_ac(int motor, int ticks, float p, int actime) {

  wait_motor_move_p_ac(motor, ticks, p, actime);
}

// Move `ticks` forward from the current position using p for speed
void flywheel_go(float speed);


void autonomous() {
  double posit, dest;
  motor_set_gearing(MOTOR_DRIVE_FRONT_LEFT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DRIVE_BACK_LEFT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DRIVE_FRONT_RIGHT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DRIVE_BACK_RIGHT, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_DESCORER, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_INTAKE, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_INDEXER, E_MOTOR_GEARSET_18);
	motor_set_gearing(MOTOR_FLYWHEEL, E_MOTOR_GEARSET_36);
  //motor_set_brake_mode(10, E_MOTOR_BRAKE_COAST);
  //motor_set_brake_mode(4, E_MOTOR_BRAKE_COAST);
  //motor_set_brake_mode(8, E_MOTOR_BRAKE_COAST);
  //motor_set_brake_mode(2, E_MOTOR_BRAKE_COAST);
  motor_set_reversed(10, 1);
  motor_set_reversed(8, 1);

   motor_move(MOTOR_FLYWHEEL, 0); // flywheel starts
   motor_move(MOTOR_INTAKE, 127);//intake starts

  posit = motor_get_position(10);
  dest = posit + 9000.0;
  printf("start motors\r\n");

  wait_motor_move_ac(10, forward1, 127, 200);

  set_motors(0);
  delay(1000);

  wait_motor_move_ac(10, -backward1, -127, 200);
  set_motors(0);

  delay(1000);
///  motor_tare_position(10);
  motor_move_relative(10,-turn, -127);
  motor_move_relative(4, turn, 127);
  motor_move_relative(8, -turn, -127);
  motor_move_relative(2, turn, 127);
  while(motor_get_target_position(10) > motor_get_position(10))
{
  printf("turn - %f - %f\r\n",motor_get_target_position(10),motor_get_position(10));
  delay(10);
}
delay(1000);

// wait_motor_move_ac(10, 650, 127, 100);
// motor_move_relative(10, 600, 127);
// motor_move_relative(4, 600, 127);
// motor_move_relative(8, 600, 127);
// motor_move_relative(2, 600, 127);
// while(motor_get_target_position(10) > motor_get_position(10))
// {
//   delay(3000);
// }
//
printf("turn\r\n");
motor_tare_position(10);
wait_motor_move_ac(10, forward2, 127, 200);
  set_motors(0);
  printf("shoot\r\n");
delay(1000);
///  motor_tare_position(10);
motor_tare_position(10);
  motor_move_relative(10,-turn, -127);
  motor_move_relative(4, turn, 127);
  motor_move_relative(8, -turn, -127);
  motor_move_relative(2, turn, 127);
//   while(motor_get_target_position(10)-20 > motor_get_position(10))
// {
//   printf("turn - %f - %f\r\n",motor_get_target_position(10),motor_get_position(10));
//   delay(10);
// }
   motor_move(MOTOR_INTAKE, 0);
delay(1000);

motor_tare_position(10);
wait_motor_move_ac(10, forward3, 127, 200);
delay(1000);
motor_tare_position(10);
wait_motor_move_ac(10, forward4, -200, 200);
delay(1000);
set_motors(0);

// motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);
//  delay(1000);
//  motor_tare_position(10);
//  wait_motor_move_ac(10, forward3, 127, 200);
// set_motors(0);
//
//  delay(1000);
//
//  motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);
//
//  delay(1000);
//  motor_move_relative(10, turnf, 127);
//  motor_move_relative(4, -turnf, -127);
//  motor_move_relative(8, turnf, 127);
//  motor_move_relative(2, -turnf, -127);
//  delay(500);
//   motor_tare_position(10);
//   wait_motor_move_ac(10, forward4, 127, 200);
// //while(motor_get_target_position(10) > motor_get_position(10))
// {
//  printf("turn - %f - %f\r\n",motor_get_target_position(10),motor_get_position(10));
//  delay(2);
// }
//


// wait_motor_move_ac(10, 100, 127, 100);
// motor_move_relative(10, 600, 127);
// motor_move_relative(4, 600, 127);
// motor_move_relative(8, 600, 127);
// motor_move_relative(2, 600, 127);
// while(motor_get_target_position(10) > motor_get_position(10))
// {
//   delay(1000);
// }


//wait_motor_move_ac(10, forward4, 127, 200);

//  set_motors(100);
//
// delay(800);
//
// set_motors(0);
//
// motor_move(MOTOR_FLYWHEEL,0);
}