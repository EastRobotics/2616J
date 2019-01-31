#include "main.h"
#include "sharedautonfunctions.h"


#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

// #define timeout(start, tout) ((tout + start) < millis())

#define turn 520
#define forward1 2500  // 1250
#define backward1 2470 //1235
#define forward2 310
#define forward3 350
#define turnf 700
#define forward4 300
#define STOPTIP 100
#define turnb 100
#define backcap -1850
#define turnf2 650
#define fwdcap2 1200
#define cflagturn 300
#define bflag2 2480
#define lfturn 270

#define WALLDISTANCE 3
#define mirror 1
adi_ultrasonic_t ult;


void flywheel_go(float speed);

void primary_autonomous()
{

  setup_chassis();
  setup_ops();

  // Forward to First Ball Under Cap and get ball
  wait_move(forward1, 100);
  delay(500);

  // Return Back to Starting Position
  wait_move(-backward1, -127);
  delay(500);

  // Turn to Face Flags
  wait_turn(turn, 127, mirror, 1000);
  delay(200);

  // Move forward to first fire position
  wait_move(forward2, 127);
  delay(200);

  // Fire the first ball at the top flag
  fire_ball();
  delay(200);

  // Forward to second ball fire position
  wait_move(forward3, 127);

  // Fire the second ball at the middle flag
  fire_ball(); //(MOTOR_INDEXER, 127);
  delay(100);

  // Turn Slightly to miss post
  wait_turn(turnb, 127, mirror, 1000);
  delay(100);

  // Forward to turn the bottom flag
  wait_move(forward4, 90); // Need to check accellormeter
  delay(200);

  // turn slightly before backing out
  wait_turn(turnb, 127, mirror, 800);
  delay(100);

  // Back Out to forward cap position
  wait_move(backcap, -110);
  delay(130);

  // Turn to face cap
  wait_turn(turnf2, 127, mirror, 800);

  // Reverse the intake to flip cap
  motor_move(MOTOR_INTAKE, -127);

  // Forward to flip cap
  wait_move(fwdcap2, 110);

  // Turn Towards the center flags
  wait_turn(cflagturn, 127, mirror, 800);

  // Shoot ball if we have any
  fire_ball();
  delay(100);

  // move forward to hit the lower flag
  wait_move(2600, 110);

  // Turn into the lower flag
  wait_turn(lfturn, 127, mirror, 800);

  delay(100);

  // move forward to push lower flag
  wait_move(200, 80);
  delay(2000);
}
