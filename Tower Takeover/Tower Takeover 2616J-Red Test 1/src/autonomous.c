#include "main.h"

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f
#define beginning 4000


unsigned int intakerun;
int taskindexeron;

  void set_motors(int speed) {
    motor_move(MOTOR_DRIVE_FRONT_LEFT,speed);
    motor_move(MOTOR_DRIVE_FRONT_RIGHT,speed);
    motor_move(MOTOR_DRIVE_BACK_RIGHT,speed);
    motor_move(MOTOR_DRIVE_BACK_LEFT,speed);
  }

  void turn_right(int ticks, int speed, int delay_option) {
    motor_move_relative(MOTOR_DRIVE_FRONT_LEFT,ticks,speed);
    motor_move_relative(MOTOR_DRIVE_FRONT_RIGHT,-ticks,-speed);
    motor_move_relative(MOTOR_DRIVE_BACK_RIGHT,-ticks,-speed);
    motor_move_relative(MOTOR_DRIVE_BACK_LEFT,ticks,speed);
    motor_tare_position(10);
    delay(delay_option);
  }

  void turn_left(int ticks, int speed, int delay_option) {
    motor_move_relative(MOTOR_DRIVE_FRONT_LEFT,-ticks,-speed);
    motor_move_relative(MOTOR_DRIVE_FRONT_RIGHT,ticks,speed);
    motor_move_relative(MOTOR_DRIVE_BACK_RIGHT,ticks,speed);
    motor_move_relative(MOTOR_DRIVE_BACK_LEFT,-ticks,-speed);
    motor_tare_position(10);
    delay(delay_option);
    }

  // Set motor to speed based on distance from `ticks`
  void motor_move_p(int motor, int ticks, float p) {
      set_motors(p);
  }

  void drive_straight(int motor, int ticks, float p, int actime) {
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

  void autonomous() {
    double posit, dest;
    char rpm[20];

    motor_set_gearing(MOTOR_DRIVE_FRONT_LEFT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DRIVE_BACK_LEFT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DRIVE_FRONT_RIGHT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DRIVE_BACK_RIGHT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_LIFT, E_MOTOR_GEARSET_36);
  	motor_set_gearing(MOTOR_INTAKE_LEFT, E_MOTOR_GEARSET_18);
    motor_set_gearing(MOTOR_INTAKE_RIGHT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_ANGLER, E_MOTOR_GEARSET_36);

    motor_set_reversed(10, 1);
    motor_set_reversed(8, 1);

// Flywheel and Intake Start
   // motor_move(MOTOR_FLYWHEEL, 127);
   motor_move(MOTOR_INTAKE_LEFT, 127);
   motor_move(MOTOR_INTAKE_RIGHT, -127);

  posit = motor_get_position(10);
  dest = posit + 9000.0;
  printf("start motors\r\n");

  drive_straight(10, 1450, 127, 200);
  set_motors(0);
  delay(750);

  drive_straight(10, -600, -127, 200);
  set_motors(0);
  delay(750);

  turn_left(650, 90, 750);

  printf("turn\r\n");
motor_tare_position(10);
drive_straight(10, 1400, 127, 200);
set_motors(0);
delay(750);

 motor_move(MOTOR_INTAKE_LEFT, 0);
 motor_move(MOTOR_INTAKE_RIGHT, 0);
 drive_straight(10, -500, -100, 200);
 set_motors(0);
 delay(750);


 turn_right(650, 127, 100);

 drive_straight(10, 600, 127, 200);
 set_motors(0);
 delay(750);

 drive_straight(10, -1500, -127, 200);
 set_motors(0);
 delay(750);

 drive_straight(10, 50, 127, 200);
 set_motors(0);
 delay(750);

 turn_right(634, 100, 500);

 drive_straight(10, 2000, 127, 200);
 set_motors(0);
 delay(500);

 motor_move(MOTOR_ANGLER, 500);
 delay(1000);
 motor_move(MOTOR_INTAKE_LEFT, -127);
 motor_move(MOTOR_INTAKE_RIGHT, 127);
 delay(50);

 drive_straight(10, -1000, 127, 200);
 set_motors(0);
 delay(500);
}
