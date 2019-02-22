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

  #include "main.h"

#define timeout(start, tout) ((tout + start) < millis())
#define between(value, min, max) ((value < max) && (value > min))
#define BALLFIREA 2000

int left_mg[] = {MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT};
int right_mg[] = {MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT};

adi_gyro_t gyro;
bool motion_limited(float limitvalue);
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

void chassis_tare(void)
{
  motor_tare_position(left_mg[0]);
  motor_tare_position(left_mg[1]);
  motor_tare_position(right_mg[0]);
  motor_tare_position(right_mg[1]);
}

void chassis_adjust_speed(int speed)
{
  motor_modify_profiled_velocity(right_mg[0], (speed));
  motor_modify_profiled_velocity(left_mg[0], (speed));
  motor_modify_profiled_velocity(right_mg[1], (speed));
  motor_modify_profiled_velocity(left_mg[1], (speed));
}

void wait_move(int ticks, float p, float limitvalue)
{
  int starttime = millis();
  chassis_tare();
  // int sp = (int)motor_get_position(left_mg[0]);
  chassis_move_absolute(ticks, p);
  while (fabs(motor_get_position(left_mg[0])) < fabs((float)(ticks)) && !timeout(starttime, 3000) && !motion_limited(limitvalue))
  {
    delay(20);
    if (fabs((float)(ticks)) - fabs(motor_get_position(left_mg[0])) < (0.2 * fabs((float)(ticks))))
    {
      chassis_adjust_speed(p / 2);
    }
  }
  chassis_move(0);
}

extern int accelZ_init;

void wait_turn(int trn, int speed, int dir, int tout)
{
  int starttime = millis();
  //motor_tare_position(left_mg[0]);
  chassis_tare();
  motor_move_absolute(right_mg[0], dir * trn, dir * speed);
  motor_move_absolute(left_mg[0], dir * -trn, dir * -speed);
  motor_move_absolute(right_mg[1], dir * trn, dir * speed);
  motor_move_absolute(left_mg[1], dir * -trn, dir * -speed);

  while (fabs(motor_get_position(left_mg[0])) < fabs((float)(trn)) && !timeout(starttime, 1000))
  {
    delay(20);
    if (fabs(trn * 0.2) > (abs(trn) - fabs(motor_get_position(left_mg[0]))))
    {
      motor_modify_profiled_velocity(right_mg[0], dir * (speed / 2));
      motor_modify_profiled_velocity(left_mg[0], dir * (-speed / 2));
      motor_modify_profiled_velocity(right_mg[1], dir * (speed / 2));
      motor_modify_profiled_velocity(left_mg[1], dir * (-speed / 2));
    }
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

  motor_move_relative(MOTOR_INDEXER, 1500, 127);
}

void stop_intake(void)
{
  motor_move(MOTOR_INTAKE, 127);
}

void index_until_shota()
{
  int starttime = millis();
  motor_move(MOTOR_INDEXER, 127);
  motor_move(MOTOR_INTAKE, 127);
  while (adi_analog_read('F') > BALLFIREA && !timeout(starttime, 1500))
  {
    //  printf("%d %f\r\n",millis() - intakerun,motor_get_power(MOTOR_INTAKE) );
    delay(5);
  }
  motor_move(MOTOR_INDEXER, 0);
  motor_move(MOTOR_INTAKE, 0);
}

void tune_turn(double degrees, int speed, int dir)
{
  while (!between(adi_gyro_get(gyro), (degrees * 10) - 30, (degrees * 10) + 30))
  {
    printf(" gyro %4.2f   %4.2f  %s\n", adi_gyro_get(gyro), degrees, (!between(adi_gyro_get('B'), (degrees * 10.0) + 30.0, (degrees * 10.0) - 30.0)) ? "stop" : " ");
    motor_move(right_mg[0], dir * -speed);
    motor_move(left_mg[0], dir * speed);
    motor_move(right_mg[1], dir * -speed);
    motor_move(left_mg[1], dir * speed);
    delay(5);
  }
  chassis_move(0);
}

lv_obj_t *pscreen, *leftsensortitle;
static lv_style_t style;
void warning_screen(char *message)
{

  lv_style_copy(&style, &lv_style_plain);
  style.text.color = LV_COLOR_BLACK;
  style.body.main_color = LV_COLOR_ORANGE;

  pscreen = lv_cont_create(lv_scr_act(), NULL);
  leftsensortitle = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_size(pscreen, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_style(pscreen, &style);

  lv_label_set_text(leftsensortitle, message);
  lv_obj_align(pscreen, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
  lv_obj_align(leftsensortitle, pscreen, LV_ALIGN_CENTER, 0, 0);
}

void setup_monitor(void)
{
  lv_style_copy(&style, &lv_style_plain);
  style.text.color = LV_COLOR_RED;
  style.body.main_color = LV_COLOR_BLACK;

  pscreen = lv_cont_create(lv_scr_act(), NULL);
  leftsensortitle = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_size(pscreen, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_style(pscreen, &style);

  //lv_label_set_text(leftsensortitle, message);
  lv_obj_align(pscreen, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
  lv_obj_align(leftsensortitle, pscreen, LV_ALIGN_CENTER, 0, 0);
}

void output_monitor(char *message)
{
  lv_label_set_text(leftsensortitle, message);
}
void sharedfunct_library_init()
{

  warning_screen("Gyro Initializing! Do not touch Robot!");

  gyro = adi_gyro_init('B', 1.17795); //needs calibration value for gyro
  delay(2000);
}

bool motion_limited(float limitvalue)
{

  int draw = (motor_get_current_draw(right_mg[0]) + motor_get_current_draw(right_mg[1]) + motor_get_current_draw(left_mg[0]) + motor_get_current_draw(left_mg[1])) / 4;
  int velocity = (motor_get_actual_velocity(right_mg[0]) + motor_get_actual_velocity(right_mg[1]) + motor_get_actual_velocity(left_mg[0]) + motor_get_actual_velocity(left_mg[1])) / 4;
  printf("d-%d v-%d\n",draw,velocity );

  return (limitvalue==0?false:(abs(draw / (int)velocity) < limitvalue) ? false : true);
}
void autonomous() {
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);
wait_move(-1000, -100, 40);
wait_move(1000, 100, 40);  
}
