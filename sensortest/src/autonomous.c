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
//define motors here (#define (name) (port))
#define MOTOR_DRIVE_FRONT_RIGHT 10
#define MOTOR_DRIVE_FRONT_LEFT 2
#define MOTOR_DRIVE_BACK_RIGHT 8
#define MOTOR_DRIVE_BACK_LEFT 4
#define MOTOR_INTAKE 20
#define MOTOR_INDEXER 6
#define MOTOR_FLYWHEEL 9
#define MOTOR_DESCORER 17
#define ACCELEROMETER_X 'D'
#define ACCELEROMETER_Y 'D' //calibrates the y axis input
#define ACCELEROMETER_Z 'D'

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

#define turn 615
#define forward1 1250
#define backward1 1220
#define forward2 335
#define forward3 350
#define turnf 780
#define forward4 300
#define STOPTIP 100
#define turnb 100
#define backcap  -240
#define fwdcap2  450
#define cflagturn 380
#define bflag2 300
#define WALLDISTANCE 3




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

void wait_motor_move2(int motor, int ticks, float p, int actime){
  int st = millis();
  int  ft = st + actime;
   motor_tare_position(motor);
  int sp = (int)motor_get_position(motor);
  while ((abs(ticks)-abs(((int)motor_get_position(motor)))) > 10) { // 10 = threshold, change to change where stop
    motor_move_p(motor, ticks, p );
    printf("looping\n\r");
    delay(10);
  }
  set_motors(0);
}
void autonomous() {
   motor_set_reversed(8,1);
  motor_set_reversed(10,1);
  wait_motor_move2(10, 360, 80,10);
}
