#include "main.h"
#include "2616j.h"
#define US_LEFT_EMIT 3
#define US_LEFT_REC 4
#define US_BACK_EMIT 5
#define US_BACK_REC 6
#define US_RIGHT_EMIT 7
#define US_RIGHT_REC 8

okapi::Controller masterController;
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {}

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

 MotorGroup leftmg({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT});
 MotorGroup rightmg({MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});

 Motor angler(-MOTOR_ANGLER);

 MotorGroup intake({MOTOR_INTAKE_RIGHT, -MOTOR_INTAKE_LEFT});

 MotorGroup lift({-MOTOR_LIFT});

void opcontrol() {

	// okapi::AverageFilter<10> avgFilter;
 // pros::ADIAnalogIn sensor (8);
 // while(true){
	//  pros::delay(10);
	//    std::cout << "Sensor Reading:" << avgFilter.filter(sensor.get_value())<< "Millis:" << pros::millis()<<"\n";
 // };

//}
int l;
int b;

pros::ADIUltrasonic ultraLeft (US_LEFT_EMIT,US_LEFT_REC);
pros::ADIUltrasonic ultraRight (US_RIGHT_EMIT,US_RIGHT_REC);
pros::ADIUltrasonic ultraBack (US_BACK_EMIT,US_BACK_REC);

while(true){
char st[30];
sprintf(st, "");
l = ultraLeft.get_value();
b = ultraBack.get_value();
 sprintf(st, "%d   %d",l,b);
 pros::lcd::print(1,"R%d\n",ultraRight.get_value());
 pros::lcd::print(2,"L%d\n",ultraLeft.get_value());
 pros::lcd::print(3,"B%d\n",ultraBack.get_value());

 //sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
 // masterController.setText(2,1, "               ");
 masterController.setText(2, 1, st);
 //pros::lcd::set_text(6, st);
 pros::delay(500);
 // drive.stop();
 // angler.moveVoltage(0);
}

	auto drive = ChassisControllerFactory::create(
		leftmg, rightmg,
		AbstractMotor::gearset::green,
		{4_in, 9.0_in});

auto profileController = AsyncControllerFactory::motionProfile(
  0.90,  // Maximum linear velocity of the Chassis in m/s
  2.0,  // Maximum linear acceleration of the Chassis in m/s/s
  8.0, // Maximum linear jerk of the Chassis in m/s/s/s
  drive // Chassis Controller
);
// profileController.generatePath({
//   Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
//   Point{0.5_ft, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
//   "A" // Profile name
// );
profileController.generatePath({
  Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
  Point{1_ft, 3.2_ft, 0_deg}}, // The next point in the profile, 3 feet forward
  "B" // Profile name
);
profileController.generatePath({
  Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
  Point{0.5_ft, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward

  "C" // Profile name
);


angler.moveVoltage(-1000);
lift.moveVoltage(-1200);

// drive.setMaxVelocity(100);
// drive.moveDistance(-80); //forward

pros::delay(100);

drive.setMaxVelocity(100);
drive.tank(80, 50);

intake.moveVoltage(-12000); //outake

pros::delay(300);

drive.stop();

intake.moveVoltage(12000); //intake
//pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(-1200);//forward
//pros::delay(100);

drive.setMaxVelocity(200);
// profileController.setTarget("A");
// profileController.waitUntilSettled();
profileController.setTarget("B");
profileController.waitUntilSettled();
//profileController.setTarget("C");
//profileController.waitUntilSettled();

drive.setMaxVelocity(100);
intake.moveVoltage(12000); //intake
pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(-1200);//forward
pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(330);//backward

intake.moveVoltage(0);//stop intake

drive.setMaxVelocity(75);
drive.turnAngle(355);//turn right

drive.setMaxVelocity(100);
angler.moveRelative(750, 10000);
drive.moveDistance(-680);//forward

// intake.moveVoltage(-4000);//outake slightly
// pros::delay(100);
intake.moveVoltage(0);

angler.moveAbsolute(1600, 3000);

// if (abs(angler.getPosition())<=1300) {//stack the cubes
//  angler.moveVoltage(12000);
//
// } else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
// angler.moveVoltage(3000);
//
// } else if(abs(angler.getPosition())>1500) {
// angler.moveVoltage(1000);
// }
pros::delay(500);

drive.setMaxVoltage(20);//slight nudge
drive.moveDistance(-25);

drive.moveDistance(500);//move backward

}
