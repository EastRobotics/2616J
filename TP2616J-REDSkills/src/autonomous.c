#include "main.h"

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

#define STOPTIP 100

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
  	motor_set_gearing(MOTOR_DESCORER, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_INTAKE, E_MOTOR_GEARSET_18);
    motor_set_gearing(MOTOR_INDEXER, E_MOTOR_GEARSET_18);
  	motor_set_gearing(MOTOR_FLYWHEEL, E_MOTOR_GEARSET_36);

    motor_set_reversed(10, 1);
    motor_set_reversed(8, 1);

// Flywheel and Intake Start
   // motor_move(MOTOR_FLYWHEEL, 127);
   motor_move(MOTOR_INTAKE, 127);

  posit = motor_get_position(10);
  dest = posit + 9000.0;
  printf("start motors\r\n");

//Forward to Get First Ball Under Cap, Flips the Cap
  drive_straight(10, 1300, 127, 200);
  set_motors(0);
  delay(1000);

//Backward after Getting Ball, Does a Wall Reset
  drive_straight(10, -3150, -85, 200);
  set_motors(0);
  delay(500);

//Forward after Wall Reset
  drive_straight(10, 123, 100, 200);
  set_motors(0);
  delay(500);

//Turn towards Left Flags
  turn_left(638, 120, 1000);
  motor_move(MOTOR_FLYWHEEL, 127);
//Left Top Flag
printf("turn\r\n");
motor_tare_position(10);
drive_straight(10, 2400, 127, 200);
  set_motors(0);
  delay(2000);
  printf("shoot\r\n");
delay(1000);
 motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);
 delay(1000);
 motor_tare_position(10);
 drive_straight(10, 350, 127, 200);
 set_motors(-30);

set_motors(0);

 delay(1000);

 motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);

 delay(1000);

//Left Bottom Flag
 bottomflag(112);
  // turn_left(75, 127, 200);
  // drive_straight(10, 1400, 90, 200);
  // set_motors(0);
  // delay(500);

//Backward to Front Red Tile
  drive_straight(10, -3100, -127, 200);
  set_motors(0);
  delay(1500);

//Turn in Front Red Tile
  turn_right(640, 127, 1000);
  set_motors(0);

//Wall Reset
  drive_straight(10, -375, -100, 200);
  set_motors(0);
  delay(500);

//Front Tilted Cap
  drive_straight(10, 3050, 95, 200);
  set_motors(0);
  delay(1000);

//Moving Caps Out of the Way
  drive_straight(10, 600, 115, 200);
  set_motors(0);
  delay(500);
  drive_straight(10, -240, -115, 200);
  set_motors(0);
  delay(500);
  motor_move(MOTOR_INTAKE, 127);

//Turn Towards Middle Flags
  turn_left(638, 127, 1000);

//Platform Reset
  drive_straight(10, -300, -115, 200);
  set_motors(0);
  delay(500);

//Center Top Flag
printf("turn\r\n");
motor_tare_position(10);
drive_straight(10, 1375, 127, 200);
  set_motors(0);
  printf("shoot\r\n");
delay(1000);
 motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);
 delay(1000);
 motor_tare_position(10);
 drive_straight(10, 800, 127, 200);
 set_motors(-30);

set_motors(0);

 delay(1000);

 motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);

 delay(1000);


//Center Bottom Flag
  turn_left(75, 127, 200);
  drive_straight(10, 1500, 90, 200);
  set_motors(0);
  delay(500);

//Backward After Middle Flags
  drive_straight(10, -1650, -115, 200);
  set_motors(0);
  delay(900);
  motor_move(MOTOR_INDEXER, -127);
  motor_move(MOTOR_INTAKE, -127);
  // motor_move(MOTOR_FLYWHEEL, 0);
  delay(100);

//Turn towards Right Flags
  turn_right(638, 127, 1000);
    motor_move(MOTOR_INDEXER, 0);

//Flip Cap
  drive_straight(10, 3500, 60, 200);
  set_motors(0);
  delay(1000);
  drive_straight(10, -150, -60, 200);
  set_motors(0);
  delay(1000);

//Bottom Blue Flag
  turn_left(632, 127, 1000);
  bottomflag(70);
  drive_straight(10, -1650, -115, 200);
  set_motors(0);
  delay(750);

//Turn towards Other Cap Flip
  turn_left(650, 127, 1000);

//Flip Other Cap
  drive_straight(10, 5350, 65, 200);
  set_motors(0);
  delay(600);
  motor_move(MOTOR_INTAKE, 0);
  drive_straight(10, 350, 65, 200);
  set_motors(0);
  delay(1000);


//Center Park
  turn_right(690, 127, 1000);
  drive_straight(10, -3100, -100, 200);
  set_motors(0);
  delay(1000);
  turn_left(675, 127, 1000);
  while ((adi_digital_read('A')) == 0) {set_motors(100);} set_motors(0); delay(1000);
  drive_straight(10, -1000, -210, 200);
  set_motors(0);
  delay(1000);
  drive_straight(10, -4540, -210, 200);
  set_motors(0);
  delay(1000);
  }

// //New Code
//   turn_left(632, 127, 1000);
//   bottomflag(70);
//   drive_straight(10, -3100, -115, 200);
//   set_motors(0);
//   delay(750);
//   motor_move(MOTOR_INTAKE, 127);
//   turn_left(650, 127, 1000);
//   drive_straight(10, 1550, 95, 200);
//   set_motors(0);
//   delay(1000);
//   drive_straight(10, -1550, -95, 200);
//   set_motors(0);
//   delay(1000);
//   turn_right(650, 127, 1000);
//     drive_straight(10, -300, -115, 200);
//     set_motors(0);
//     delay(500);
//   printf("turn\r\n");
//   motor_tare_position(10);
//   drive_straight(10, 1375, 127, 200);
//     set_motors(0);
//     printf("shoot\r\n");
//   delay(1000);
//    motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);
//    delay(1000);
//    motor_tare_position(10);
//    turn_right(660, 127, 1000);
//    drive_straight(10, 800, 100, 200);
//    set_motors(0);
//    delay(1000);
//    drive_straight(10, -3100, -100, 200);
//    set_motors(0);
//    delay(1000);
//    turn_right(675, 127, 1000);
//    while ((adi_digital_read('A')) == 0) {set_motors(100);} set_motors(0); delay(200);
//    drive_straight(10, -6325, -200, 200);
//    set_motors(0);
//    delay(1000);
