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

#define turn 615
#define forward1 1250
#define backward1 1220
#define forward2 335
#define forward3 350
#define turnf 700
#define forward4 300
#define STOPTIP 100
#define turnb 100
#define backcap  -1950


#define turnf2 650
#define fwdcap2  1200
#define cflagturn 300
#define bflag2 2100
#define lfturn 300


#define WALLDISTANCE 3
  adi_ultrasonic_t ult;



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
    printf("ticks - %d pos - %d sp - %d\n\r",ticks,(int)motor_get_position(motor),sp);
   // printf("looping\n\r");
    delay(20);
  }
  set_motors(0);
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

int avgFilter_ult(){
  int ultout;
  for(int x=0; x<=5;x++){
     ultout+=adi_ultrasonic_get(ult);
     delay(25);
  }
return(ultout/5);


}

extern int accelZ_init;

void wait_motor_move_ac_ust(int motor, int ticks, float p, int actime) {

  int st = millis();
  int  ft = st + actime;
  adi_port_set_config ('A',E_ADI_DIGITAL_IN);

  int sp = (int)motor_get_position(motor);
      printf("ticks - %d pos - %d sp - %d\n\r",ticks,(int)motor_get_position(motor),sp);  //avgFilter_ult() > WALLDISTANCE  --&& adi_digital_read(1)
  while (((abs(ticks)-abs(((int)motor_get_position(motor))-sp)) > 10  && !adi_digital_read('A')  && (adi_analog_read_calibrated(ACCELEROMETER_Z) - accelZ_init) < STOPTIP) ){ // 10 =, change to change where stop
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
  char rpm[20];

  //ult = adi_ultrasonic_init(1, 2);
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
//while (true){
//  printf("-Ultra- %d\n\r",avgFilter_ult());
//}
   motor_move(MOTOR_FLYWHEEL, 127); // flywheel starts
   motor_move(MOTOR_INTAKE, 127);//intake starts

  posit = motor_get_position(10);
  dest = posit + 9000.0;
  printf("start motors\r\n");
 motor_move(MOTOR_FLYWHEEL, 127);
  wait_motor_move_ac(10, forward1, 127, 200);

  set_motors(0);
  delay(1000);

  wait_motor_move_ac(10, -backward1, -127, 200);
  set_motors(0);

  delay(1000);
 // motor_tare_position(10);
  motor_move_relative(10,turn, 127);
  motor_move_relative(4, -turn, -127);
  motor_move_relative(8, turn, 127);
  motor_move_relative(2, -turn, -127);
  while(motor_get_target_position(10) > motor_get_position(10))
{
//  avgFilter_ult();
  printf("turn - %f - %f\r\n",motor_get_target_position(10),motor_get_position(10));
  delay(20);
}
delay(1000);


printf("turn\r\n");
motor_tare_position(10);
wait_motor_move_ac(10, forward2, 127, 200);
  set_motors(0);
  printf("shoot\r\n");
delay(1000);
 motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);
 delay(1000);
 motor_tare_position(10);
 wait_motor_move_ac(10, forward3, 127, 200);
 set_motors(-30);

set_motors(0);

 delay(1000);

 motor_move_relative(MOTOR_INDEXER, 1000, 127);//(MOTOR_INDEXER, 127);

 delay(1000);
 motor_move_relative(10, turnf, 127);
 motor_move_relative(4, -turnf, -127);
 motor_move_relative(8, turnf, 127);
 motor_move_relative(2, -turnf, -127);
 delay(100);
  motor_tare_position(10);
  wait_motor_move_ac_ust(10, forward4, 90, 200);
  sprintf(rpm, "AccelZ: %d ",  (abs(adi_analog_read_calibrated(ACCELEROMETER_Z)) - accelZ_init));// ,avgFilter_ult()p
  printf("%s\n",rpm);
  controller_print(E_CONTROLLER_MASTER, 0, 0, rpm);
  while(adi_digital_read('A')==0 &&  (abs(adi_analog_read_calibrated(ACCELEROMETER_Z)) - accelZ_init) < STOPTIP)
  {
    sprintf(rpm, "AccelZ: %d ",  (abs(adi_analog_read_calibrated(ACCELEROMETER_Z)) - accelZ_init));// ,avgFilter_ult()p
    printf("%s\n",rpm);
    controller_print(E_CONTROLLER_MASTER, 0, 0, rpm);

  }
  sprintf(rpm, "AccelZ: %d ",  (abs(adi_analog_read_calibrated(ACCELEROMETER_Z)) - accelZ_init));// ,avgFilter_ult()p
  printf("%s\n",rpm);
  controller_print(E_CONTROLLER_MASTER, 0, 0, rpm);
  set_motors(0);
delay(200);
  motor_move_relative(10, turnb, 127);
  motor_move_relative(4, -turnb, -127);
  motor_move_relative(8, turnb, 127);
  motor_move_relative(2, -turnb, -127);
  delay(100);
printf(" before move\n");
motor_set_brake_mode(10, MOTOR_BRAKE_BRAKE);
motor_set_brake_mode(4, MOTOR_BRAKE_BRAKE);
motor_set_brake_mode(8, MOTOR_BRAKE_BRAKE);
motor_set_brake_mode(2, MOTOR_BRAKE_BRAKE);
wait_motor_move2(10, backcap, -127,10);
  delay(130);
    set_motors(0);
  delay(600);
  motor_tare_position(10);
  motor_move_relative(10, -turnf2, -127);
  motor_move_relative(4, turnf2, 127);
  motor_move_relative(8, -turnf2, -127);
  motor_move_relative(2, turnf2,127);
  while(turnf2>(abs(motor_get_position(10))+10))
   {  printf("turnval = %d motorpos = %f \n\r",turnf,motor_get_position(10));
  delay(10);}
// delay(500);10

  motor_move(MOTOR_INTAKE, -127);
  wait_motor_move2(10, fwdcap2, 110,10);
   
//delay(200);
motor_tare_position(10);
motor_move_relative(10, cflagturn, 127);
motor_move_relative(4, -cflagturn, -127);
motor_move_relative(8, cflagturn, 127);
motor_move_relative(2, -cflagturn, -127);
  while(cflagturn>(abs(motor_get_position(10))+10))
   {  printf("turnval = %d motorpos = %f \n\r",turnf,motor_get_position(10));
  delay(10);}
// delay(500);10

motor_move_relative(MOTOR_INDEXER, 1000, 127);
delay(100);
wait_motor_move2(10, 2600, 110,10);
motor_tare_position(10);
motor_move_relative(10, lfturn, 127);
motor_move_relative(4, -lfturn, -127);
motor_move_relative(8, lfturn, 127);
motor_move_relative(2, -lfturn, -127);
  while(lfturn>(abs(motor_get_position(10))+10))
   {  printf("turnval = %d motorpos = %f \n\r",turnf,motor_get_position(10));
  delay(10);}
  delay(100);
wait_motor_move2(10, 200, 80,10);
  delay(2000);
  //   set_motors(0);
  // while(true){
  //   printf("Pushbutton - %d\r\n",adi_digital_read('A'));
  // }
//while(motor_get_target_position(10) > motor_get_position(10))
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
