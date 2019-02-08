#include "main.h"
#include "sharedautonfunctions.h"

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

// #define timeout(start, tout) ((tout + start) < millis())


#define mirror -1
adi_ultrasonic_t ult;

void flywheel_go(float speed);

void primary_autonomous()
{

  setup_chassis();
  setup_ops();

  // Forward to First Ball Under Cap and get ball
  //chassis_move_absolute(-2500, -127);

  wait_move(2500, 175);
  delay(500);

  // Return Back to Starting Position
  wait_move(-2580, -175);
  delay(250);

  // Turn to Face Flags
  wait_turn(625, 127, mirror, 1000);
  delay(150);

  // Move forward to first fire position
  wait_move(1200, 135);
  delay(500);

  // Fire the first ball at the top flag
  index_until_shota();
  delay(500);

  // Forward to second ball fire position
  wait_move(1200, 135);
  delay(200);

  // Fire the second ball at the middle flag
  index_until_shota(); //(MOTOR_INDEXER, 127);
  delay(200);

  // Turn Slightly to miss post
  wait_turn(100, 127, mirror, 1000);
  delay(100);

  // Forward to the bottom flag
  wait_move(1250, 90); // Need to check accellormeter
  delay(100);

  // turn slightly before backing out
  // wait_turn(200, 127, mirror, 800);
  // delay(100);

  // Back Out to forward cap position
  wait_move(-1850, -110);
  delay(130);

  // Turn to face cap
  wait_turn(-650, 127, mirror, 800);

  // Reverse the intake to flip cap
  motor_move(MOTOR_INTAKE, -127);

  // Forward to flip cap
  wait_move(1200, 110);

  // Turn Towards the center flags
  wait_turn(300, 150, mirror, 800);

  // Shoot ball if we have any
  //index_until_shota();
  delay(100);

  stop_intake();
  // move forward to hit the lower flag
  wait_move(2600, 110);

  // Turn into the lower flag

  wait_turn(270, 127, mirror, 800);

  delay(100);

  // move forward to push lower flag
  wait_move(200, 80);
  delay(2000);
}
