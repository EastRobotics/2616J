#include "main.h"
#include "pros/vision.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

using namespace pros;
void opcontrol()
{


  pros::ADIGyro gyro('A');
    pros::Vision vis (4);
  
 pros::vision_color_code_t redflag = vis.create_color_code(1, 3);
 // pros::vision_signature_s_t bfg = vis.create_sig(2, 3);
  while (true) {
   
    vision_object_s_t rtn = vis.get_by_code(0, redflag);
    vision_object_s_t rtn2 = vis.get_by_sig(0, 3);
    vis.clear_led();
    // Get the gyro heading
   std::cout << "Red X-" << rtn.x_middle_coord << "Red Y-" << rtn.y_middle_coord << "RedS X-" << rtn2.x_middle_coord << "Reds Y-" << rtn2.y_middle_coord <<"\n";

   // std::cout << "Distance: " << gyro.get_value() << "\n";
    pros::delay(1000);
  }

  pros::Motor motor1(10);
  pros::Motor motor2(2);
  pros::Motor motor3(8);
  pros::Motor motor4(4);

  motor1.set_reversed(true);
  motor3.set_reversed(true);

  //   motor1.move_velocity(100);
  //   motor2.move_velocity(100);
  //   motor3.move_velocity(100);
  //   motor4.move_velocity(100);
  pros::delay(1000); // Move at 100 RPM for 1 second
  int direction = 1;
  int draw = 1;
  double velocity;
  while (1 == 1)
  {

    motor1.move_velocity(direction * 100);
    motor2.move_velocity(direction * 100);
    motor3.move_velocity(direction * 100);
    motor4.move_velocity(direction * 100);
    delay(100);
    draw = (motor1.get_current_draw() + motor2.get_current_draw() + motor3.get_current_draw() + motor4.get_current_draw()) / 4;
    velocity = (motor1.get_actual_velocity() + motor2.get_actual_velocity() + motor3.get_actual_velocity() + motor4.get_actual_velocity()) / 4;
    while (abs(draw / (int)velocity) < 20)
    {
      velocity = (motor1.get_actual_velocity() + motor2.get_actual_velocity() + motor3.get_actual_velocity() + motor4.get_actual_velocity()) / 4;
      draw = (motor1.get_current_draw() + motor2.get_current_draw() + motor3.get_current_draw() + motor4.get_current_draw()) / 4;
      std::cout << "Motor Current Draw: " << draw << "  Direction: " << direction << "  Ratio:" << abs(draw / (int)velocity) << "\n";

      delay(10);
    }
    motor1.move_velocity(0);
    motor2.move_velocity(0);
    motor3.move_velocity(0);
    motor4.move_velocity(0);
    delay(1000);
    velocity = (motor1.get_actual_velocity() + motor2.get_actual_velocity() + motor3.get_actual_velocity() + motor4.get_actual_velocity()) / 4;
    draw = (motor1.get_current_draw() + motor2.get_current_draw() + motor3.get_current_draw() + motor4.get_current_draw()) / 4;

    direction = direction * -1;
    std::cout << "Motor Current Draw: " << draw << "  Direction: " << direction << "\n";
  }
  motor1.move_velocity(0);
  motor2.move_velocity(0);
  motor3.move_velocity(0);
  motor4.move_velocity(0);
}
