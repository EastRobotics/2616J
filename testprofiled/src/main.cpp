#include "main.h"
#include "math.h"
#include "2616J.h"
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

 auto drive =	ChassisControllerBuilder()
 		    .withMotors(
 		        {-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT}, // Left motors are 1 & 2 (reversed)
 		        {MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT} )   // Right motors are 3 & 4
 				.withDimensions(AbstractMotor::gearset::green, {{4_in, 9_in}, imev5GreenTPR})
 				.build();

std::int16_t  basespeed = 9000;
int divisor = 5;
void opcontrol() {
char s[40];
pros::Imu inertsensor(9);
bool controlloop = true;
//while(controlloop){

inertsensor.reset();
   while(inertsensor.is_calibrating()){pros::delay(10);}
leftmg.moveVoltage(-basespeed);
// pros::delay(100);
rightmg.moveVoltage(-basespeed);
while(leftmg.getPosition() > -1200){
	rightmg.moveVoltage((rightmg.getPosition() > leftmg.getPosition()) ?
	 (-basespeed + (rightmg.getPosition() - leftmg.getPosition())):
	 (-basespeed)
 );


	pros::delay(10);
}
std::int16_t  basespeed = 11000;
pros::delay(250);
leftmg.tarePosition();
leftmg.moveVoltage(basespeed);
rightmg.moveVoltage(basespeed);

while(leftmg.getPosition() < 100){pros::delay(10);}
leftmg.moveVoltage(basespeed/divisor);
while(inertsensor.get_rotation() < 45){pros::delay(10);}
leftmg.moveVoltage(basespeed);
while(leftmg.getPosition()< 1000){pros::delay(10);}
rightmg.moveVoltage(basespeed/divisor);
while(inertsensor.get_rotation() > 0){pros::delay(10);}
rightmg.moveVoltage(basespeed);
pros::delay(1000);
rightmg.moveVoltage(0);
leftmg.moveVoltage(0);


// 		 drive->setMaxVelocity(80);
//
// 		 drive->moveDistanceAsync(2_ft);
// 		 while(rightmg.getPosition() < 1000){pros::delay(10);}
// 		 pros::lcd::print(1,"R%f\n",rightmg.getPosition());
// 		 drive->stop();
//
//      double cpos = rightmg.getPosition();
// 		 drive->turnAngleAsync(-50_deg);
// 		 while(inertsensor.get_rotation() < 40.0){pros::delay(10);}
// 		 pros::lcd::print(2,"1 R%f\n",inertsensor.get_rotation());
//      drive->stop();
//
//      cpos = rightmg.getPosition();
// 		 drive->moveDistanceAsync(2_ft);
// 		while(rightmg.getPosition()-cpos < 1000){pros::delay(10);}
// 		pros::lcd::print(3,"2 R%f\n",rightmg.getPosition());
//     drive->stop();
//
//     double crot = inertsensor.get_rotation();
// 		drive->turnAngleAsync(50_deg);
// 		while(inertsensor.get_rotation() -crot  > -40.0){pros::delay(10);}
// 		pros::lcd::print(4,"3 R%f\n",inertsensor.get_rotation());
//     drive->stop();
//
// //}


while(true){
  printf(s,"H: %f",inertsensor.get_rotation());
	pros::delay(100);
	 masterController.setText(1, 1, s);
	 pros::lcd::print(1,"R%f\n",inertsensor.get_rotation());

}


}
	// okapi::AverageFilter<10> avgFilter;
 // pros::ADIAnalogIn sensor (8);
 // while(true){
	//  pros::delay(10);
	//    std::cout << "Sensor Reading:" << avgFilter.filter(sensor.get_value())<< "Millis:" << pros::millis()<<"\n";
 // };

//}
// int l;
// int b;
//
// pros::ADIUltrasonic ultraLeft (US_LEFT_EMIT,US_LEFT_REC);
// pros::ADIUltrasonic ultraRight (US_RIGHT_EMIT,US_RIGHT_REC);
// pros::ADIUltrasonic ultraBack (US_BACK_EMIT,US_BACK_REC);
// pros::delay(300);
// //while(true){
// char st[30];
// sprintf(st, "");
// l = ultraLeft.get_value();
// b = ultraBack.get_value();
//  sprintf(st, "%d   %d",l,b);
//  pros::lcd::print(1,"R%d\n",ultraRight.get_value());
//  pros::lcd::print(2,"L%d\n",ultraLeft.get_value());
//  pros::lcd::print(3,"B%d\n",ultraBack.get_value());
//
//  //sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
//  // masterController.setText(2,1, "               ");
//  masterController.setText(2, 1, st);
//  //pros::lcd::set_text(6, st);
//  pros::delay(50);
//  // drive.stop();
//  // angler.moveVoltage(0);
//
	//
	// auto drive = ChassisControllerFactory::create(
	// 	leftmg, rightmg,
	// 	AbstractMotor::gearset::green,
	// 	{4_in, 9.0_in});


//
// // auto profileController = AsyncControllerFactory::motionProfile(
// //   0.90,  // Maximum linear velocity of the Chassis in m/s
// //   2.0,  // Maximum linear acceleration of the Chassis in m/s/s
// //   8.0, // Maximum linear jerk of the Chassis in m/s/s/s
// //   drive // Chassis Controller
// // // );
// // auto profileController =
// // AsyncMotionProfileControllerBuilder()
// //
// // 	 .withLimits({4, 4, 20.0})
// // 	 .withOutput(drive)
// // 	 .buildMotionProfileController();
//
// // profileController->generatePath({
// //   {0_in, 0_in, 0.0_deg},  // Profile starting position, this will normally be (0, 0, 0)
// // 	  {3_in, 19_in, 0.0_deg},
// //   {12_in, 38_in, 0_deg}}, // The next point in the profile, 3 feet forward
// //   "A" // Profile name
// // );
// //
// #define max 12000
// #define T1   1000
// #define T2   2000
// #define DV    6
// char st[40];
// while(leftmg.getPosition() < T1 ){
// 	leftmg.moveVoltage(leftmg.getPosition() < T1?max:max/DV);
// 	rightmg.moveVoltage(rightmg.getPosition() < T1?max:max/DV);
// 	sprintf(st,"L: %4.1f  r: %4.1f",leftmg.getPosition(),rightmg.getPosition());
// 	masterController.setText(2, 1, st);
// }
// while(leftmg.getPosition() > T1 &&  leftmg.getPosition() <T2 ){
// 	leftmg.moveVoltage(max);
// 	rightmg.moveVoltage(max);
// 	sprintf(st,"L: %4.1f  r: %4.1f",leftmg.getPosition(),rightmg.getPosition());
// 	masterController.setText(2, 1, st);
// }
// while(rightmg.getPosition()>T2 ){
// 	leftmg.moveVoltage(leftmg.getPosition() > T2?max:max/DV);
// 	rightmg.moveVoltage(rightmg.getPosition() <T2?max:max/DV);
// 	sprintf(st,"L: %4.2f  r: %4.2f",leftmg.getPosition(),rightmg.getPosition());
// 	masterController.setText(2, 1, st);
// }
// leftmg.moveVoltage(0);
// rightmg.moveVoltage(0);
//
// // profileController.generatePath({
// //   Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
// //   Point{1_ft, 3.2_ft, 0_deg}}, // The next point in the profile, 3 feet forward
// //   "B" // Profile name
// // );
// // profileController.generatePath({
// //   Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
// //   Point{0.5_ft, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
// //
// //   "C" // Profile name
// // );
//
//
// // angler.moveVoltage(-1000);
// // lift.moveVoltage(-1200);
// //
// // // drive.setMaxVelocity(100);
// // // drive.moveDistance(-80); //forward
// //
// // pros::delay(100);
// //
// // intake.setBrakeMode(AbstractMotor::brakeMode::brake);
// //
// // drive.setMaxVelocity(100);
// // drive.moveDistance((b-l));
// // pros::delay(100);
// // 	drive.turnAngle(345);
// //
// //
// // while(true){
// // pros::lcd::print(1,"R%d\n",ultraRight.get_value());
// // pros::lcd::print(2,"L%d\n",ultraLeft.get_value());
// // pros::lcd::print(3,"B%d\n",ultraBack.get_value());
// // pros::delay(200);
// // //sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
// // // masterController.setText(2,1, "               ");
// // masterController.setText(2, 1, st);
// // }
// // drive.setMaxVelocity(100);
// // drive.tank(80, 50);
// //
// // intake.moveVoltage(-12000); //outake
// //
// // pros::delay(300);
// //
// // drive.stop();
// //
// // intake.moveVoltage(12000); //intake
// // //pros::delay(100);
// //
// // drive.setMaxVelocity(100);
// // drive.moveDistance(200);
// // drive.moveDistance(-1200);//forward
// // //pros::delay(100);
// //
// // drive.setMaxVelocity(200);
// // profileController->setTarget("A");
// // profileController->waitUntilSettled();
// // profileController.setTarget("B");
// // profileController.waitUntilSettled();
// // //profileController.setTarget("C");
// // //profileController.waitUntilSettled();
// //
// // drive.setMaxVelocity(100);
// // intake.moveVoltage(12000); //intake
// // pros::delay(100);
// //
// // drive.setMaxVelocity(100);
// // drive.moveDistance(-1200);//forward
// // pros::delay(100);
// //
// // drive.setMaxVelocity(100);
// // drive.moveDistance(330);//backward
// //
// // intake.moveVoltage(0);//stop intake
// //
// // drive.setMaxVelocity(75);
// // drive.turnAngle(355);//turn right
// //
// // drive.setMaxVelocity(100);
// // angler.moveRelative(750, 10000);
// // drive.moveDistance(-680);//forward
// //
// // // intake.moveVoltage(-4000);//outake slightly
// // // pros::delay(100);
// // intake.moveVoltage(0);
// //
// // angler.moveAbsolute(1600, 3000);
// //
// // // if (abs(angler.getPosition())<=1300) {//stack the cubes
// // //  angler.moveVoltage(12000);
// // //
// // // } else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
// // // angler.moveVoltage(3000);
// // //
// // // } else if(abs(angler.getPosition())>1500) {
// // // angler.moveVoltage(1000);
// // // }
// // pros::delay(500);
// //
// // drive.setMaxVoltage(20);//slight nudge
// // drive.moveDistance(-25);
// //
// // drive.moveDistance(500);//move backward
//
// //programming skills
// angler.moveVoltage(-1000);
// lift.moveVoltage(-1200);
//
// drive.setMaxVelocity(100);
// drive.moveDistance(-2000);//move across the field to collect cubes
//
// intake.moveVoltage(12000); //intake
//
// drive.turnAngle(135);//turn to face the small goal
//
// drive.setMaxVelocity(100);
// drive.moveDistance(-300);//drive to the goal
//
// angler.moveAbsolute(1800, 2500);//angler moves up to stack
//
// drive.setMaxVoltage(12000);
// drive.moveDistance(400);//move backward
//
// angler.moveAbsolute(0, 0);//move the angler down *check the value
//
// drive.turnAngle(-235);// turn to face goal behind
//
// drive.setMaxVelocity(100);
// drive.moveDistance(400);//drive towards the goal
//
// intake.moveVoltage(10000);//intake the cube to score
// pros::delay(100);
//
// angler.moveAbsolute(0, 0);
//
// lift.moveVoltage(1000);//move lift up to the level of the tower
// pros::delay(100);
//
// intake.moveVoltage(-12000);//outake to score the cube
// pros::delay(300);
//
// drive.setMaxVelocity(100);
// drive.moveVoltageAsync(200);///move away form the first tower
//
// lift.moveAbsolute(0, 0);
// angler.moveAbsolute(0, 0);
//
// drive.turnAngle(-90);// faces the second tower
//
// drive.setMaxVelocity(100);
// drive.moveVoltage(400);//moves towards the second tower
//
// intake.moveVoltage(10000);//intake the cube to score
// pros::delay(100);
//
// angler.moveAbsolute(0, 0);
//
// lift.moveVoltage(1000);//move lift up to the level of the tower
// pros::delay(100);
//
// intake.moveVoltage(-12000);//outake to score the cube
// pros::delay(300);
//
// drive.setMaxVelocity(100);
// drive.moveVoltageAsync(200);///move away form the second tower
