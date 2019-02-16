#include "main.h"
#include "sharedautonfunctions.h"

#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

// #define timeout(start, tout) ((tout + start) < millis())

#define mirror 1
adi_ultrasonic_t ult;

void flywheel_go(float speed);

void red_back_auton()
{
  // char display[20];
  lv_obj_t *pscreen, *leftsensortitle, *leftsensor;
  leftsensortitle = lv_label_create(pscreen, NULL);
  // adi_gyro_t gyro = adi_gyro_init('B', 1.068);
  // delay(2000);

  setup_chassis();



  setup_ops();

  // Forward to First Ball Under Cap and get ball
  //chassis_move_absolute(-2500, -127);

  pscreen = lv_cont_create(lv_scr_act(), NULL);
  leftsensortitle = lv_label_create(pscreen, NULL);
  // while(true){

  //   sprintf(display," gyro %4.2f\n", adi_gyro_get(gyro));
  //   lv_label_set_text(leftsensortitle, display);
  //   lv_obj_align(leftsensortitle, leftsensor, LV_ALIGN_IN_TOP_MID, 0, 0);
  //   delay(100);
  // }
      motor_move(MOTOR_FLYWHEEL, 120);
  wait_move(2450, 150, 0);
  delay(250);

  wait_move(-283, -80, 0);
  delay(250);

  wait_turn(375, 80, mirror, 500);
  delay(100);
  wait_move(-290, -400, 0);
  delay(250);

  delay(3000);

  index_until_shota();
  delay(1500);
    motor_move(MOTOR_FLYWHEEL, 114);
    delay(3000);
    index_until_shota();

    motor_move(MOTOR_FLYWHEEL,0);
    delay(250);

      wait_move(300, 150, 0);
      delay(250);
      wait_turn(-1110, -80, mirror, 500);
      delay(500);
     wait_move(-700, -100, 40);
     delay(100);
     wait_move(100, 100, 0);
     delay(50);
     wait_move(-950, -200, 0);
  while(true){{}}
}
