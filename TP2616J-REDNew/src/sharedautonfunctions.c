#include "main.h"

#define timeout(start, tout) ((tout + start) < millis())

int left_mg[] = {MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT};
int right_mg[] = {MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT};



void chassis_move(int velo)
{
  motor_move(left_mg[0], velo);
  motor_move(right_mg[0], velo);
  motor_move(left_mg[1], velo);
  motor_move(right_mg[1], velo);
}

void chassis_move_absolute(double posit, int velo)
{
  motor_move_absolute(left_mg[0], posit, velo);
  motor_move_absolute(right_mg[0], posit, velo);
  motor_move_absolute(left_mg[1], posit, velo);
  motor_move_absolute(right_mg[1], posit, velo);
}

void wait_move(int ticks, float p)
{
  int starttime = millis();
  motor_tare_position(left_mg[0]);
  int sp = (int)motor_get_position(left_mg[0]);
  chassis_move_absolute(ticks, p);
  while (!motor_is_stopped(left_mg[0]) && !timeout(starttime, 1000))
  {
    delay(20);
  }
  chassis_move(0);
}

extern int accelZ_init;

void wait_turn(int trn, int speed, int dir, int tout)
{
  int starttime = millis();
  motor_tare_position(left_mg[0]);
  motor_move_absolute(right_mg[0], dir * trn, dir * speed);
  motor_move_absolute(left_mg[0], dir * -trn, dir * -speed);
  motor_move_absolute(right_mg[1], dir * trn, dir * speed);
  motor_move_absolute(left_mg[1], dir * -trn, dir * -speed);

  while (!motor_is_stopped(left_mg[0]) && !timeout(starttime, 1000))
  {
    delay(20);
  }
}
void setup_chassis(void)
{
  motor_set_gearing(MOTOR_DRIVE_FRONT_LEFT, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_DRIVE_BACK_LEFT, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_DRIVE_FRONT_RIGHT, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_DRIVE_BACK_RIGHT, E_MOTOR_GEARSET_18);

  motor_set_reversed(10, 1);
  motor_set_reversed(8, 1);
}

void setup_ops(void)
{
  motor_set_gearing(MOTOR_DESCORER, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_INTAKE, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_INDEXER, E_MOTOR_GEARSET_18);
  motor_set_gearing(MOTOR_FLYWHEEL, E_MOTOR_GEARSET_36);

  motor_move(MOTOR_FLYWHEEL, 127);
  motor_move(MOTOR_INTAKE, 127);
  int starttime = millis();
  while (!motor_is_stopped(MOTOR_FLYWHEEL) && !timeout(starttime, 1000))
  {
    // Repeat starting flywheel until flywheel is running
    motor_move(MOTOR_FLYWHEEL, 127);
    delay(100);
  }
  motor_move(MOTOR_INTAKE, 127); //intake starts
}

void fire_ball(void)
{
  motor_move_relative(MOTOR_INDEXER, 1000, 127);
}
