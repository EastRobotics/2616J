#include "main.h"

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f
#define beginning 4000

#define STOPTIP 100
#define BALLFIRE 2000
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

  void shot() {
   delay(1000);
   motor_move_relative(MOTOR_INDEXER, 1000, 127);
   delay(1000);
   motor_tare_position(10);
}

  void bottomflag(int turn) {
   motor_move_relative(10, turn, 127);
   motor_move_relative(4, -turn, -127);
   motor_move_relative(8, turn, 127);
   motor_move_relative(2, -turn, -127);
   delay(100);
   while ((adi_digital_read('A')) == 0) {set_motors(90);}
   set_motors(0);
   delay(200);
   motor_set_brake_mode(10, MOTOR_BRAKE_BRAKE); motor_set_brake_mode(4, MOTOR_BRAKE_BRAKE); motor_set_brake_mode(8, MOTOR_BRAKE_BRAKE); motor_set_brake_mode(2, MOTOR_BRAKE_BRAKE);
   delay(500);
  }

  void holdball(int indexer) {
    motor_move_relative(6, indexer, 127);
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

void index_shot(){
  intakerun = millis();
  taskindexeron = 1;
  motor_move(MOTOR_INDEXER,127);
  motor_move(MOTOR_INTAKE,127);
  int timeout = 0; // Start a timeout
  while(adi_analog_read('F')>BALLFIRE && ((millis() - intakerun) < 1500 && (timeout++ < 400/5)) ){
   printf("%d\r\n",adi_analog_read('F'));
//  printf("%d %f\r\n",millis() - intakerun,motor_get_power(MOTOR_INTAKE) );
   delay(5);
  }
  motor_move(MOTOR_INDEXER,0);
  motor_move(MOTOR_INTAKE,0);
  taskindexeron = 0;

}

void doub_shot(){
  index_shot();
  drive_straight(10, 250, 127, 200);
  index_shot();
  set_motors(0);
  motor_move(MOTOR_INTAKE,0);
  motor_move(MOTOR_INDEXER,0);
}

  void autonomous() {
    double posit, dest;
    char rpm[20];

    motor_set_gearing(MOTOR_DRIVE_FRONT_LEFT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DRIVE_BACK_LEFT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DRIVE_FRONT_RIGHT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DRIVE_BACK_RIGHT, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_DESCORER, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_INTAKE, E_MOTOR_GEARSET_18);
    motor_set_gearing(MOTOR_INDEXER, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_FLYWHEEL, E_MOTOR_GEARSET_36);

    motor_set_reversed(10, 1);
    motor_set_reversed(8, 1);

// Flywheel and Intake Start
   // motor_move(MOTOR_FLYWHEEL, 127);
   motor_move(MOTOR_INTAKE, 127);
   motor_move(MOTOR_FLYWHEEL, 127);

  posit = motor_get_position(10);
  dest = posit + 9000.0;
  printf("start motors\r\n");

  drive_straight(10, 1450, 127, 200);
  set_motors(0);
  delay(750);

  drive_straight(10, -1600, -127, 200);
  set_motors(0);
  delay(750);

  turn_left(644, 90, 750);

  printf("turn\r\n");
motor_tare_position(10);
drive_straight(10, 315, 127, 200);
set_motors(0);
delay(1000);
doub_shot();
motor_move(MOTOR_INTAKE, 127);

 turn_left(100, 127, 100);
 drive_straight(10, 1300, 100, 200);
 set_motors(0);
 delay(1000);


 turn_left(50, 127, 100);

 holdball(250);

 drive_straight(10, -500, -127, 200);
 set_motors(0);
 delay(750);

 motor_move(MOTOR_INTAKE, -127);
 turn_right(634, 100, 500);

 drive_straight(10, -450, -90, 200);
 set_motors(0);
 delay(100);

 drive_straight(10, 1525, 60, 200);
 set_motors(0);
 delay(500);

 //Park Code

 motor_move(MOTOR_INTAKE, 0);

 drive_straight(10, 425, 60, 200);
 set_motors(0);
 delay(250);

 // drive_straight(10, -20, -60, 200);
 // set_motors(0);
 // delay(250);

 turn_left(628, 100, 500);

 drive_straight(10, -3950, -200, 200);
 set_motors(-5);
 delay(500);

 drive_straight(10, -200, -200, 200);
 set_motors(-5);
 delay(500);
}
