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
  wait_move(2500, 150, 0);
  delay(250);

  wait_move(-800, 150, 9);
  delay(250);

  wait_turn(300, 127, mirror, 500);

  delay(4000);

  index_until_shota();
  delay(1000);
}
