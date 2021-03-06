#include "main.h"
#include "2616J.h"
#include "string.h"
#include "pros/apix.h"
#include "display/lvgl.h"
#include "display/lv_conf.h"
#include "motorTemps.hpp"

pros::Imu inertsensor(9);

extern char _PROS_COMPILE_DIRECTORY[30];
int LIFT_BOTTOM = 60; // TODO Make this the constant thing I forget how
pros::ADIDigitalIn AutonButton(1);
void auton_picker();
bool did_move_up = false; // Used to only call move up code when first pressed
bool doing_move_down = false; // Used to automatically move angler down
int kill_task = 0;

LV_IMG_DECLARE(red_flower);
LV_IMG_DECLARE(bluemask);
LV_IMG_DECLARE(blue_circle);
LV_IMG_DECLARE(push_cube);
LV_IMG_DECLARE(stack_cube);
LV_IMG_DECLARE(bigstack_mask);

int l;
int b;
int r;
AverageFilter<3> usfilt;

pros::ADIUltrasonic ultraLeft (US_LEFT_EMIT,US_LEFT_REC);
pros::ADIUltrasonic ultraRight (US_RIGHT_EMIT,US_RIGHT_REC);
pros::ADIUltrasonic ultraBack (US_BACK_EMIT,US_BACK_REC);


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
MotorGroup leftmg({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT});
MotorGroup rightmg({-MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
Motor rightBack(-MOTOR_DRIVE_BACK_RIGHT);
Motor leftBack(MOTOR_DRIVE_BACK_LEFT);

MotorGroup drive1({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
MotorGroup turnLeft({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});
MotorGroup turnRight({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});

Motor angler(-MOTOR_ANGLER);

MotorGroup intake({MOTOR_INTAKE_RIGHT, -MOTOR_INTAKE_LEFT});

MotorGroup lift({-MOTOR_LIFT});

int maxVelocity =200;

void driveVector(const double iforwardSpeed, const double iyaw) {
  // This code is taken from WPIlib. All credit goes to them. Link:
  // https://github.com/wpilibsuite/allwpilib/blob/master/wpilibc/src/main/native/cpp/Drive/DifferentialDrive.cpp#L73
  const double forwardSpeed = std::clamp(iforwardSpeed, -1.0, 1.0);
  const double yaw = std::clamp(iyaw, -1.0, 1.0);
	  double leftOutput = forwardSpeed + yaw;
  double rightOutput = forwardSpeed - yaw;
  if (const double maxInputMag = std::max<double>(std::abs(leftOutput), std::abs(rightOutput));
      maxInputMag > 1) {
    leftOutput /= maxInputMag;
    rightOutput /= maxInputMag;
  }

leftmg.moveVelocity(static_cast<int16_t>(leftOutput * maxVelocity ));
rightmg.moveVelocity(static_cast<int16_t>(rightOutput * maxVelocity));
}

// auto drive = ChassisControllerFactory::create(
// 	leftmg, rightmg,
// 	AbstractMotor::gearset::green,
// 	{4_in, 9.0_in}
// 	//distance between the center of the front drive wheels9
// );
int x;
okapi::Controller masterController;
//pros::Controller controller(pros::E_CONTROLLER_MASTER);
void my_task_fn(void *param)
{
	char s[30];
	while (1 == 1)
	{
		sprintf(s, "");
		//sprintf(s, "I%d (%.2f) A%d (%.2f)", intake.isOverTemp(), intake.getTemperature(), angler.isOverTemp(), angler.getTemperature());
		//sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
		// masterController.setText(2,1, "               ");
		masterController.setText(2, 1, s);
		pros::lcd::set_text(6, s);
		pros::delay(4000);
	}
	//Display position of (MotorGroup) on controller
}

bool taskwait;
void asynclift(void *param)
{
	taskwait = true;
  int movend = 2000;
	int switchPos =1400; // Encoder value where equation kicks in and lift stops going full speed. Lower if knocking over, raise if not getting high enough
	int minSpeed = 2500; // Minimum voltage to send to the lift. Maybe lower if knocking over stack or wobbly and changing a makes no change
	float a = 2000.0; // Higher value = higher speed for longer when going up
	float mult = .8;
	while(taskwait == true){pros::delay(5);}
	intake.moveRelative(-50, 80);
  while(angler.getPosition()< movend && kill_task==0){
//		intake.moveVoltage(((pros::millis()/500)%2)==1?12000:0);
	if (angler.getPosition() <= switchPos) { // Do full speed until switchPos
		angler.moveVoltage(12000);
	} else { // Do equation from switchPos until endif
		// Calculate speed based on angle
		int angleSpeed = int(
			(
				(1-pow((1/(a))*(angler.getPosition()),2))
			)*12000.0*mult
		);

		// Ensure speed is at least minSpeed
		angleSpeed = (angleSpeed > minSpeed) ? angleSpeed : minSpeed;
		// Set speed to the lift
		angler.moveVoltage(angleSpeed);
}
}
}

pros::Task angle_task (asynclift, (void*)"angle", "AngleTask");
static int autonid;

pros::Task showTemps_task (showTemps, (void *)0x0, "TempShower");

static lv_res_t null_handler(lv_obj_t *obj)
{

	pros::delay(250);
	return LV_RES_OK;
}

static lv_obj_t *win;
// LV_IMG_DECLARE(red_flower);
// LV_IMG_DECLARE(bluemask);
// LV_IMG_DECLARE(blue_circle);
// LV_IMG_DECLARE(push_cube);
// LV_IMG_DECLARE(stack_cube);
// LV_IMG_DECLARE(bigstack_mask);

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 *///---------------------------------------------------------------------------
void disabled() {
}

// LV_IMG_DECLARE(red_flower);
// LV_IMG_DECLARE(bluemask);
// LV_IMG_DECLARE(blue_circle);
// LV_IMG_DECLARE(push_cube);
// LV_IMG_DECLARE(stack_cube);
// LV_IMG_DECLARE(bigstack_mask);
lv_obj_t *scr = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t *img_var = lv_img_create(scr, NULL);

lv_obj_t *img_push = lv_img_create(scr, NULL);
lv_obj_t *img_stack = lv_img_create(scr, NULL);
lv_obj_t *img_blue = lv_img_create(scr, NULL);
lv_obj_t *img_bigstack = lv_img_create(scr, NULL);


MotorGroup bleftmg({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT});
MotorGroup brightmg({MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});

auto drive =	ChassisControllerBuilder()
			 .withMotors(
					bleftmg, brightmg )   // Right motors are 3 & 4
			 .withDimensions(AbstractMotor::gearset::green, {{4_in, 9_in}, imev5GreenTPR})
			 .build();
// auto drive = ChassisControllerFactory::create(
// bleftmg, brightmg,
// AbstractMotor::gearset::green,
// {4_in, 9.0_in});

// 	auto profileController = AsyncControllerFactory::motionProfile(
// 	0.90,  // Maximum linear velocity of the Chassis in m/s
// 	2.0,  // Maximum linear acceleration of the Chassis in m/s/s
// 	8.0, // Maximum linear jerk of the Chassis in m/s/s/s
// 	drive // Chassis Controller
// );
bool isValidEfficiency(MotorGroup x) {
  double eff = x.getEfficiency();
  return (eff > 0.1 );
}
// bool isEfficient(MotorGroup x, double y) {
//   if (y < 0.001) {
//     y = 0.001;
//   }
//   if (x.getEfficiency() < 0.001) {
//     if (x.getEfficiency() > y) {
//       return true;
//     }
//   }
//   return false;
// }

void wall_reset_back(){
drive->getModel()->driveVectorVoltage(-12000.0, 0.0);
pros::delay(100);
while(!isValidEfficiency(bleftmg)){}
 while(bleftmg.getEfficiency()  > 0.001 && brightmg.getEfficiency()  > 0.001 ){
//	 pros::lcd::print(2, "e  %lf",bleftmg.getEfficiency() );
//	printf("%f\n",bleftmg.getEfficiency() );
	pros::delay(100);
 }
 pros::delay(400);
drive->stop();
}


void wall_reset_front()  //Wall Reset front of the bot agains the wall
{

    while (abs(angler.getPosition()) <= 1200)
    { //bring angler and lift up
        angler.moveVoltage(12000);
        pros::delay(100);
        lift.moveVoltage(12000);
    }
    angler.moveVoltage(0);
    while (lift.getEfficiency() > 0.001)
    {
        pros::delay(20);
    }
    lift.moveVoltage(4000);

    drive->getModel()->driveVectorVoltage(12000.0, 0.0);
    pros::delay(300);
    while (bleftmg.getEfficiency() > 0.001 && brightmg.getEfficiency() > 0.001)
    {
        pros::lcd::print(2, "e  %lf", bleftmg.getEfficiency());
        //    printf("%f\n",bleftmg.getEfficiency() );
        pros::delay(100);
    }
    drive->stop();
}

void tray_down_efflimit() {
    angler.moveVoltage(-12000);
    pros::delay(100);
    while (angler.getEfficiency() > 50.0) {
        pros::lcd::print(4, "a  %lf", angler.getEfficiency());
        //    printf("%f\n",bleftmg.getEfficiency() );
        pros::delay(50);
    }
    angler.moveVoltage(0);
}


void initialize()
{
//	LV_IMG_DECLARE(red_flower);

inertsensor.reset();
   while(inertsensor.is_calibrating()){pros::delay(5);}

   pros::delay(200);
	  lv_obj_set_size(scr, 476, 272);
	lv_img_set_src(img_var, &red_flower);
	lv_obj_set_pos(img_var, 0, 0);
}
//------------------------------------------------------------------------------
static lv_res_t event_handler(lv_obj_t *obj)
{
	autonid++;
	if (autonid > 7)
	{
		autonid = 1;
	}
	printf("A=%d\n", autonid);
	if (autonid == 1)
	{
		lv_obj_set_hidden(img_stack, 1);
		lv_obj_set_hidden(img_push, 1);
		lv_obj_set_hidden(img_blue, 1);
		lv_obj_set_hidden(img_bigstack, 0);
	}
	if (autonid == 2)
	{
		lv_obj_set_hidden(img_stack, 1);
		lv_obj_set_hidden(img_push, 1);
		lv_obj_set_hidden(img_blue, 0);
		lv_obj_set_hidden(img_bigstack, 0);
	}
	if (autonid == 3)
	{
		lv_obj_set_hidden(img_stack, 0);
		lv_obj_set_hidden(img_push, 1);
		lv_obj_set_hidden(img_blue, 1);
		lv_obj_set_hidden(img_bigstack, 1);
	}
	if (autonid == 4)
	{
		lv_obj_set_hidden(img_stack, 0);
		lv_obj_set_hidden(img_push, 1);
		lv_obj_set_hidden(img_blue, 0);
		lv_obj_set_hidden(img_bigstack, 1);
	}
	if (autonid == 5)
	{
		lv_obj_set_hidden(img_stack, 0);
		lv_obj_set_hidden(img_push, 0);
		lv_obj_set_hidden(img_blue, 1);
		lv_obj_set_hidden(img_bigstack, 1);
	}
  if (autonid == 6)
  {
    lv_obj_set_hidden(img_stack, 0);
    lv_obj_set_hidden(img_push, 0);
    lv_obj_set_hidden(img_blue, 0);
    lv_obj_set_hidden(img_bigstack, 1);
  }
  if (autonid == 7)
  {
    lv_obj_set_hidden(img_stack, 1);
    lv_obj_set_hidden(img_push, 0);
    lv_obj_set_hidden(img_blue, 1);
    lv_obj_set_hidden(img_bigstack, 1);
  }
	pros::delay(50);
	return LV_RES_OK;
}
//------------------------------------------------------------------------------
void auton_picker()
{
	pros::delay(200);
	static lv_style_t bstyle;
	autonid = 1;
	lv_obj_set_size(scr, 476, 272);
	lv_img_set_src(img_var, &red_flower);
	lv_obj_set_pos(img_var, 0, 0);

	lv_obj_set_hidden(img_push, 1);
	static lv_style_t *pstyle = lv_img_get_style(img_push);
	pstyle->image.color = {0x0, 0x0, 0x0, 0xff};
	lv_img_set_style(img_push, pstyle);
	lv_img_set_style(img_stack, pstyle);
	lv_img_set_style(img_bigstack, pstyle);

	// lv_obj_set_hidden(img_stack, 1);
	// lv_obj_set_hidden(img_bigstack, 1);
	// lv_obj_set_hidden(img_blue, 1);
	lv_img_set_src(img_blue, &bluemask);
	lv_img_set_src(img_push, &push_cube);
	lv_img_set_src(img_stack, &stack_cube);
	lv_img_set_src(img_bigstack, &bigstack_mask);

	lv_obj_set_pos(img_blue, 0, 0);
	lv_obj_set_pos(img_push, 0, 0);
	lv_obj_set_pos(img_stack, 0, 0);
  lv_obj_set_pos(img_bigstack, 0, 0);

	lv_obj_set_hidden(img_stack, 1);
	lv_obj_set_hidden(img_push, 1);
	lv_obj_set_hidden(img_blue, 1);
	lv_obj_set_hidden(img_bigstack, 0);

	lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);

	lv_style_copy(&bstyle, &lv_style_plain);
	bstyle.body.border.color = {0xff, 0xff, 0xff, 0xff};
	bstyle.body.empty = true;
	lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, event_handler);
	lv_obj_set_pos(btn, 274, 83);
	lv_obj_set_height(btn, 130);
	lv_btn_set_style(btn, LV_BTN_STATE_REL, &bstyle);
	lv_btn_set_style(btn, LV_BTN_STATE_PR, &bstyle);
	while (true)
	{
		if (AutonButton.get_new_press()) {
			event_handler(NULL);
		pros::delay(50);
	  }
  }
}
/*

* Stacking Red Auton

*///----------------------------------------------------------------------------
void auton_stack_red()
{
	auto drive =	ChassisControllerBuilder()
				 .withMotors(
								leftmg, rightmg )   // Right motors are 3 & 4
				 .withDimensions(AbstractMotor::gearset::green, {{4_in, 9_in}, imev5GreenTPR})
				 .build();
	// auto drive = ChassisControllerFactory::create(
	// 	leftmg, rightmg,
	// 	AbstractMotor::gearset::green,
	// 	{4_in, 9.0_in}
	// 	//distance between the center of the front drive wheels9
	// );
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Stack Blue Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);

	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	drive->setMaxVelocity(100);
	drive->moveDistance(80_mm); //forward

	pros::delay(100);

	drive->setMaxVelocity(100);
	drive->getModel()->tank(-80, -100);

	intake.moveVoltage(-12000); //outake

	pros::delay(1000);

	drive->stop();

	intake.moveVoltage(12000); //intake
	pros::delay(150);

	drive->setMaxVelocity(100);
	drive->moveRaw(1200);//forward

	pros::delay(100);

	drive->setMaxVelocity(100);
	drive->moveRaw(-330);//backward

	intake.moveVoltage(0);//stop intake

	drive->setMaxVelocity(75);
	drive->turnRaw(355);//turn right

	drive->setMaxVelocity(100);
	drive->moveDistance(710_mm);//forward

	intake.moveVoltage(-4000);
	pros::delay(240);
	intake.moveVoltage(0);

	if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(12000);

	} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
	angler.moveVoltage(6000);

	} else if(abs(angler.getPosition())>1500) {
	angler.moveVoltage(3000);
	}
	pros::delay(2500);

	drive->setMaxVelocity(20);
	drive->moveDistance(25_mm);

	drive->moveDistance(-500_mm);//move backward

}

void big_ol_deploy(void){
	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	// drive->setMaxVelocity(100);
	// drive->moveDistance(-80); //forward

	pros::delay(100);

	drive->setMaxVelocity(120);
	//drive->tank(80, 80);
	drive->moveDistanceAsync(-200_mm);

	intake.moveVoltage(-12000); //outake

	pros::delay(500);

	drive->stop();

	intake.moveVoltage(12000); //intake
}


//------------------------------------------------------------------------------
void auton_stack_blue(int direct){
  // auto drive = ChassisControllerFactory::create(
	// 	leftmg, rightmg,
	// 	AbstractMotor::gearset::green,
	// 	{4_in, 9.0_in}
	// 	//distance between the center of the front drive wheels9
	// );
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Stack Blue Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);
	bleftmg.setReversed(0);
	brightmg.setReversed(1);
  big_ol_deploy();//deploy
	pros::delay(100);
	// angler.moveVoltage(-1000);
	// lift.moveVoltage(-900);
	//
	// drive->setMaxVelocity(100);
	// drive->moveDistance(80_mm); //forward
	//
	// pros::delay(100);
	//
	// drive->setMaxVelocity(100);
	// drive->getModel()->tank(-80, -100);
	//
	// intake.moveVoltage(-12000); //outake
	//
	// pros::delay(1000);
	//
	// drive->stop();
	//
	// intake.moveVoltage(12000); //intake
	// pros::delay(150);

  lift.moveVoltage(-2000);

	drive->setMaxVelocity(80);
	drive->moveDistance(900_mm);//forward

	pros::delay(100);

  intake.moveVoltage(0);//stop intake

	driveVector(.25, 1*direct);
	if(direct==1){
	while(inertsensor.get_rotation()< 7){pros::delay(10);}
}else{
	while(inertsensor.get_rotation()> -7){pros::delay(10);}
}

intake.moveVoltage(12000);//start intake at max voltage
// pros::delay(50);
// intake.moveVoltage(0);

  drive->moveDistance(1.1_ft);
  pros::delay(200);

  intake.moveVoltage(1200);//run intake low speed

	drive->moveDistance(-1.1_ft);
	if(direct==1){
	drive->turnAngle(-27_deg);
}else{
	drive->turnAngle(27_deg);
}
//		driveVector(.1, -1);
//	while(ine,rtsensor.get_rotation()>5){pros::delay(8);}

	intake.moveVoltage(0);//intake stop
	drive->setMaxVelocity(100);
	//drive->moveDistance(-1.25_ft);//backward

  drive->getModel()->forward(-100);//backward
for(int x=0;x<3;x++){usfilt.filter(ultraBack.get_value());}
while(usfilt.filter(ultraBack.get_value())>850){pros::delay(10);}
//controller.print(2,0,"%d",usfilt.filter(ultraBack.get_value()));}
drive->stop();

	 pros::delay(200);
//	drive->setMaxVelocity();

	driveVector(.5, direct==1?-1:1);
//	while(inertsensor.get_rotation()> direct==1?-90:90){pros::delay(10);}

	if(direct==1){
	while(inertsensor.get_rotation()> -75){pros::delay(10);}
}else{
	while(inertsensor.get_rotation()< 90){pros::delay(10);}
}
drive->stop();
//	drive->turnRaw(direct * -355);//turn left
	drive->setMaxVelocity(120);
  drive->getModel()->forward(120);

  pros::delay(150);
  while (bleftmg.getEfficiency() > 0.1 && brightmg.getEfficiency() > 0.1)
  {
    pros::delay(10);
  }
  pros::delay(300);
  drive->stop();

//	drive->moveDistance(345_mm);//forward

	// intake.moveVoltage(-4000);//outake a little bit for stacking purposes
	// pros::delay(240);
	// intake.moveVoltage(0);
  angler.tarePosition();
while(abs(angler.getPosition())<2100){
	// if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(10000);
  //
	// } else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
	// angler.moveVoltage(9000);
  //
	// } else if(abs(angler.getPosition())>1500) {
	// angler.moveVoltage(6000);
// }}
}

	pros::delay(1100);

	drive->setMaxVelocity(50);
	drive->moveDistance(25_mm);//the nudge

//------------------------------------------------------------------------------
  drive->setMaxVelocity(100);
  drive->moveDistance(-450_mm);//move backward
  pros::delay(200);

  //towers start from here

   drive->setMaxVelocity(75);
   drive->turnAngle(-144_deg);//turn to face tower
   pros::delay(200);

   drive->setMaxVelocity(100);
   //drive->moveRaw(-1100);//wall reset 1
   wall_reset_back();
   pros::delay(200);

   angler.moveVoltage(-12000);
   while(abs(angler.getPosition()) > 300){ pros::delay(10);}
   angler.moveVoltage(0);
   pros::delay(100);
   intake.moveVoltage(8000);//intake runs to collect cube to place in first tower

   drive->setMaxVelocity(100);//drive towards the tower
   drive->moveDistance(3.8_ft);
   pros::delay(100);

   intake.moveVoltage(0);

   drive->setMaxVelocity(75);
   drive->moveDistance(-300_mm);//back up slightly

   intake.moveVoltage(-6000);//outake the cube into stacking position
   pros::delay(400);
   intake.moveVoltage(0);

  while(abs(angler.getPosition()) <= 1200){
    angler.moveVoltage(12000);//raise lift and angler
    pros::delay(100);
    lift.moveVoltage(12000);
  }

    angler.moveVoltage(0);
    pros::delay(700);

    lift.moveVoltage(2000);
    intake.moveVoltage(-4500);//outake to place into tower

    drive->setMaxVelocity(100);
    drive->moveDistance(150_mm);//drive towards to tower slightly

    pros::delay(200);
    drive->moveDistance(-10_mm);//backup

    drive->turnAngle(-95_deg);//turn to face second stack
    pros::delay(100);

    angler.moveVoltage(10000);//move angler down
    pros::delay(300);
    angler.moveVoltage(0);

    lift.moveVoltage(-12000);//move lift down
    pros::delay(350);
    lift.moveVoltage(-1500);

    pros::delay(50);

    drive->setMaxVelocity(100);
    drive->getModel()->forward(-200);
  //  pros::delay(300);
    wall_reset_back();

    // drive->moveRaw(-1400);//wall reset 2
    // pros::delay(200);

    lift.moveVoltage(-2000);
    intake.moveVoltage(12000);
    drive->setMaxVelocity(100);
    drive->moveDistance(7.75_ft);//drive and collect second stack

    pros::delay(50);
    intake.moveVoltage(0);

    drive->setMaxVelocity(100);
    drive->moveDistance(-0.2_ft);//drive back a little

    pros::delay(50);

    drive->setMaxVelocity(100);
    drive->turnAngle(-49_deg);//turn to face second goal
    pros::delay(100);
    intake.moveVoltage(8000);

    pros::delay(50);

    drive->setMaxVelocity(100);
    drive->moveDistance(4.0_ft);//drive towards the goal
    intake.moveVoltage(0);
    pros::delay(50);

    while(abs(angler.getPosition())<2100){
    	if (abs(angler.getPosition())<=1300) {//stack the cubes
    	 angler.moveVoltage(12000);

    	} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
    	angler.moveVoltage(10000);

    	} else if(abs(angler.getPosition())>1500) {
    	angler.moveVoltage(7000);
    }}
    pros::delay(1200);

    drive->setMaxVelocity(100);
    drive->moveDistance(25_mm);//the nudge

    drive->setMaxVelocity(100);
    drive->moveDistance(-500_mm);//move backward
    pros::delay(50);

    angler.moveVoltage(-12000);
    pros::delay(400);
    angler.moveVoltage(0);

    drive->setMaxVelocity(30);
    drive->turnAngle(50_deg);//turn to face wall
    pros::delay(400);

    angler.tarePosition();//tare the position

    drive->getModel()->forward(200);
    pros::delay(400);

    wall_reset_front();
    // while(abs(angler.getPosition()) <= 1200){//bring angler and lift up
    //   angler.moveVoltage(12000);
    //   pros::delay(100);
    //   lift.moveVoltage(12000);
    // }
    // angler.moveVoltage(0);
    // pros::delay(50);
    //
    // drive->setMaxVelocity(100);
    // drive->moveRaw(1300);//wall reset 3
    // pros::delay(200);

    drive->setMaxVelocity(100);
    drive->moveDistance(-300_mm);//move backward from wall rest

    pros::delay(50);

    drive->setMaxVelocity(50);
    drive->turnAngle(98_deg);//turn to face second tower

    pros::delay(50);

    lift.moveVoltage(-12000);//move lift down
    pros::delay(500);
    lift.moveVoltage(-1500);

    pros::delay(50);
    tray_down_efflimit();
    // angler.moveVoltage(6000);//move angler down
    // pros::delay(100);
    // angler.moveVoltage(0);

    pros::delay(50);

    angler.moveVoltage(-12000);
    pros::delay(500);
    angler.moveVoltage(0);
    pros::delay(100);
    intake.moveVoltage(11000);//intake runs to collect cube to place in first tower

    drive->setMaxVelocity(100);//drive towards the tower
    drive->moveDistance(3.0_ft);
    pros::delay(10);

    intake.moveVoltage(0);

    drive->setMaxVelocity(50);
    drive->moveDistance(-300_mm);//back up slightly

    intake.moveVoltage(-6000);//outake the cube into stacking position
    pros::delay(400);
    intake.moveVoltage(0);

    pros::delay(50);

    angler.tarePosition();

    while(abs(angler.getPosition()) <= 1200){
      angler.moveVoltage(12000);//bring angler and lift up
      pros::delay(100);
      lift.moveVoltage(12000);
    }

    angler.moveVoltage(0);
    pros::delay(500);

    lift.moveVoltage(2000);
    drive->moveDistance(150_mm);//drive closer to goal slightly
    intake.moveVoltage(-5000);//outake to place into goal

    pros::delay(500);

    drive->setMaxVelocity(100);
    drive->moveDistance(-100_mm);

}

//------------------------------------------------------------------------------
void auton_big_stack_blue(int direct)
{

int atimerstart = pros::millis();

	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	// drive->setMaxVelocity(100);
	// drive->moveDistance(-80); //forward

	pros::delay(100);

	drive->setMaxVelocity(110);
	//drive->tank(80, 80);
	drive->moveDistanceAsync(200_mm);

	intake.moveVoltage(-12000); //outake

	pros::delay(500);

	drive->stop();

	intake.moveVoltage(12000); //intake
	//pros::delay(100);
   angler.moveVoltage(0);
	drive->setMaxVelocity(100);
	drive->moveDistance(-775_mm);//forward_mm
	//pros::delay(100);

	drive->setMaxVelocity(200);
	intake.moveVoltage(1000);
	// profileController.setTarget("A");
	// profileController.waitUntilSettled();
//	profileController.setTarget(direct==-1?"B":"A",direct==-1?false:true);
	// profileController.setTarget(direct==1?"B":"A");
	// profileController.waitUntilSettled();
	//profileController.setTarget("C");
	//profileController.waitUntilSettled();

	driveVector(1,0);
	while(leftmg.getPosition() < 65){pros::delay(5);}
	driveVector(1,-0.6);
	while(inertsensor.get_rotation() < 40){pros::delay(5);}
	driveVector(1,0);
	while(leftmg.getPosition() < 50){pros::delay(5);}

	driveVector(1,0.6);
	while(inertsensor.get_rotation()> 0){pros::delay(5);}
	driveVector(1,0);
	while(leftmg.getPosition() < 200){pros::delay(5);}


	intake.moveVoltage(12000); //intake
	drive->setMaxVelocity(100);
	intake.moveVoltage(12000); //intake
	pros::delay(50);

	drive->setMaxVelocity(100);
	drive->moveDistance(-1400_mm);//forward
	pros::delay(50);
  intake.moveVoltage(0);

	l = ultraLeft.get_value();
	b = ultraBack.get_value();
	r = ultraRight.get_value();

	intake.moveVoltage(700);
	intake.setBrakeMode(AbstractMotor::brakeMode::hold);
	drive->setMaxVelocity(115);
	drive->moveRaw((direct==1?b-l:b-r));//backward



	drive->setMaxVelocity(75);

	drive->turnRaw(direct * 335);//turn left
	intake.moveVoltage(0);
	drive->setMaxVelocity(100);
  taskwait = false;


//	angler.moveRelative(750, 10000);
	drive->moveRawAsync(-350);//forward

	//intake.moveRelative(600,-200);

	// intake.moveVoltage(-4000);//outake slightly
	// pros::delay(100);
	drive->waitUntilSettled();
	intake.moveVoltage(0);

	//angler.moveAbsolute(1800, 2500);


	pros::delay(1200);

	drive->setMaxVelocity(20);//slight nudge
	drive->moveRaw(-25);

	drive->setMaxVelocity(12000);
	drive->moveRaw(0);//move backward
  //drive->moveDistance(80);
	int atimerend = pros::millis();
 int atimetotal = atimerend - atimerstart;
 char st[30];
 sprintf(st, "");
	sprintf(st, "%d ",atimetotal);
	//sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
	// masterController.setText(2,1, "               ");
	masterController.setText(2, 1, st);
	pros::lcd::set_text(6, st);
  // pros::delay(2000);
	// drive->stop();
	// angler.moveVoltage(0);
  // intake.moveVoltage(0);
  // kill_task = 1;
  // angle_task.suspend();
	// angler.moveRelative(1500,-200);
	// pros::delay(2000);
	// 	angler.moveVoltage(0);
  // drive->turnAngle(direct * 300);
  // int totaldistance =1300;
	// b = ultraBack.get_value();
	// pros::delay(1000);
	// //intake.moveVoltage(12000);
	// drive->moveDistance(-1 *(totaldistance - 	b)) ;

}
//------------------------------------------------------------------------------

void auton_push()
{
	// auto drive = ChassisControllerFactory::create(
	// 	leftmg, rightmg,
	// 	AbstractMotor::gearset::green,
	// 	{4_in, 9.0_in}
	// 	//distance between the center of the front drive wheels9
	// );
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Push Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);
	bleftmg.setReversed(0);
	brightmg.setReversed(1);
	angler.moveVoltage(-1000);
  lift.moveVoltage(-1000);

  drive->setMaxVelocity(100);
  drive->moveRaw(-300); //backward

  drive->setMaxVelocity(100);
  drive->moveRaw(500); //backward

  pros::delay(100);

  drive->setMaxVelocity(100);
  drive->moveRaw(300); //forward


  intake.moveVoltage(-12000); //outake
  pros::delay(400);

  intake.moveVoltage(0);


}
//------------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------------
void auton_front(int direct)
{
  bleftmg.setReversed(0);
	brightmg.setReversed(1);
  big_ol_deploy();//deploy
  pros::delay(100);

  intake.moveVoltage(11000);
  drive->setMaxVelocity(100);
  drive->moveDistance(520_mm);
  pros::delay(100);

  driveVector(.25, 1*direct);
  if(direct==1){
  while(inertsensor.get_rotation()< 35){pros::delay(10);}
  }else{
  while(inertsensor.get_rotation()> -35){pros::delay(10);}
  }
  pros::delay(100);

  drive->setMaxVelocity(100);
  drive->moveDistance(2.6_ft);
  pros::delay(100);

  intake.moveVoltage(1200);

  drive->setMaxVelocity(100);
  drive->moveDistance(-2.4_ft);
  pros::delay(100);

  driveVector(.125, direct==1?-1:1);
//	while(inertsensor.get_rotation()> direct==1?-90:90){pros::delay(10);}
	 if(direct==1){
  	while(inertsensor.get_rotation()> -70){pros::delay(10);}
  }else{
  	while(inertsensor.get_rotation()< 70){pros::delay(10);}
  }

  intake.moveVoltage(0);
  drive->setMaxVelocity(100);
  drive->moveDistance(240_mm);//forward towards goal

  if (abs(angler.getPosition())<=1300) {//stack the cubes
   angler.moveVoltage(12000);

  } else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
  angler.moveVoltage(3000);

  } else if(abs(angler.getPosition())>1500) {
  angler.moveVoltage(1000);
  }
  pros::delay(2000);

  drive->setMaxVelocity(100);
  drive->moveDistance(25_mm);//the nudge

  drive->setMaxVelocity(100);
  drive->moveDistance(-500_mm);//move backward

}
//------------------------------------------------------------------------------
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */

void competition_initialize()
{
	pros::Task angle_task (asynclift, (void*)"angle", "AngleTask");
// 	profileController.generatePath({
// 	Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
// 	Point{1.25_ft, 2.5_ft, 0_deg}}, // The next point in the profile, 3 feet forward
// 	"B" // Profile name
// );

// profileController->generatePath({
// 		Point{1_ft, 0_ft, 0_deg},
// 	Point{0_ft, 3.2_ft, 0_deg}},  // Profile starting position, this will normally be (0, 0, 0)
// // The next point in the profile, 3 feet forward
// "A" // Profile name
// );
// profileController.generatePath({
// 	Point{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
// 	Point{0.5_ft, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
//
// 	"C" // Profile name


	auton_picker();
}

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

//------------------------------------------------------------------------------

// auto drive = ChassisControllerFactory::create(
// 	leftmg, rightmg,
//   AbstractMotor::gearset::green,
//   {4_in, 9.0_in}
// //distance between the center of the front drive wheels
// );
//------------------------------------------------------------------------------
void autonomous()
{
  auton_stack_blue(1);
}
// 	switch (autonid)
// 	{
// 	case 1:
//     auton_big_stack_blue(-1);
// 		break;
// 	case 2:
// 		auton_big_stack_blue(1);
// 		break;
// 	case 3:
// 		auton_stack_blue(-1);
// 		break;
// 	case 4:
// 		auton_stack_blue(1);
// 		break;
// 	case 5:
// 		auton_front(1);
// 		break;
//   case 6:
//   		auton_front(-1);
//   		break;
//   case 7:
//     auton_push();
//     break;
// 	}
// while(true){
// 	pros::delay(50);
// }
// }
//------------------------------------------------------------------------------
/*
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operatorS
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

// Chassis Control9ler - lets us drive the robot around with open- or closed-loop control

void opcontrol() {

kill_task = 1;

bleftmg.setReversed(0);
brightmg.setReversed(1);
//------------------------------------------------------------------------------
char s[30];

okapi::AverageFilter<5> avgFilter;
pros::ADIAnalogIn sensor (2);
// while(true){
//  std::cout << "Sensor Reading:" << avgFilter.filter(sensor.get_value())<< "Millis:" << pros::millis()<<"\n";
// }
 lv_obj_t *img_var = lv_img_create(scr, NULL);

pros::delay(100);

LV_IMG_DECLARE(red_flower);
 lv_obj_set_size(scr, 476, 272);
 lv_img_set_src(img_var, &red_flower);
 lv_obj_set_pos(img_var, 0, 0);

 pros::delay(100);

 ControllerButton IntakeInButton(ControllerDigital::R1);
 ControllerButton IntakeOutButton(ControllerDigital::R2);

 ControllerButton AnglerUpButton(ControllerDigital::up);
 ControllerButton AnglerDownButton(ControllerDigital::down);
 ControllerButton Angler10UpButton(ControllerDigital::left);

 ControllerButton LiftUpButton(ControllerDigital::L1);
 ControllerButton LiftDownButton(ControllerDigital::L2);

  ControllerButton DoubleTower(ControllerDigital::A);
	drive->setMaxVelocity(12000);
//------------------------------------------------------------------------------
intake.setBrakeMode(AbstractMotor::brakeMode::brake);
angler.setBrakeMode(AbstractMotor::brakeMode::brake);
lift.setBrakeMode(AbstractMotor::brakeMode::brake);

int count =0;
//------------------------------------------------------------------------------
	while (true) {
		drive->getModel()->tank(masterController.getAnalog(ControllerAnalog::leftY),
						masterController.getAnalog(ControllerAnalog::rightY));
//------------------------------------------------------------------------------
if (IntakeOutButton.changedToPressed()){
  count =pros::millis();
}

if(IntakeInButton.isPressed() && IntakeOutButton.isPressed()){
		 intake.moveVoltage(-11999);//run intake out fast

	} else if(IntakeInButton.isPressed()){
		 intake.moveVoltage(11999);//run intake in
     lift.moveVoltage(-1000);//run lift down

	 }else if(IntakeOutButton.isPressed()){
		 intake.moveVoltage(-6000);
     drive->getModel()->forward(-300);//run intake out slow

  }else{
		 intake.moveVoltage(0);
	 }
//------------------------------------------------------------------------------
  if(LiftUpButton.isPressed() && abs(angler.getPosition())<1200){
      angler.moveVoltage(12000);
      lift.moveVoltage(8000);
	//		pros::delay(100);
		//	lift.moveVoltage(12000);
			//cout  << "button;"

	}else if(LiftUpButton.isPressed() && abs(angler.getPosition()) >= 1200){
    angler.moveVoltage(0);
		lift.moveVoltage(12000);

	// }else if(LiftDownButton.isPressed() && abs(angler.getPosition()) <= 1300){
  //  lift.moveVoltage(-12000);
  //  doing_move_down = true;

}//else {
//    lift.moveVoltage(-800);
  //}

  if(LiftDownButton.isPressed() && abs(lift.getPosition()) > 1200){
    lift.moveVoltage(-12000);
  //  doing_move_down = true;

}else if(LiftDownButton.isPressed() && abs(lift.getPosition()) <= 1200){
  if (angler.getPosition() > LIFT_BOTTOM) {
    angler.moveVoltage(-12000);
  }else if(AnglerDownButton.isPressed()){
    angler.moveVoltage(0);
  }
  lift.moveVoltage(-12000);
}else{

  if(!AnglerUpButton.isPressed() && !AnglerDownButton.isPressed() &&!LiftUpButton.isPressed() && !LiftDownButton.isPressed()){
angler.moveVoltage(0);
lift.moveVoltage(0);
}
// if(!LiftUpButton.isPressed() && !LiftDownButton.isPressed()) {
// //lift.moveVoltage(-1000);
//// }
}

//------------------------------------------------------------------------------

 if(AnglerUpButton.isPressed()){
		 intake.setBrakeMode(AbstractMotor::brakeMode::coast);
		 // if (!did_move_up) {
			//  did_move_up = true;
			//  angler.moveAbsolute(2121, 100);
		 // }
		int switchPos = 2600;// Ecoder value where equation kicks in and lift stops going full speed. Lower if knocking over, raise if not getting high enough
		int minSpeed = 4500; // Minimum voltage to send to the lift. May be lower if knocking over stack or wobbly and changing a makes no change
		float a = 2200.0; // Higher value = higher speed for longer when going up
		float mult = 2.0; // Once shape is achieved with a, allows you to slow down the overall curve

		if (angler.getPosition() <= switchPos) { // Do full speed until switchPos
			angler.moveVoltage(12000);
		} else { // Do equation from switchPos until endif
			// Calculate speed based on angle
			int angleSpeed = int(
				(
					(1-pow((1/(a))*(angler.getPosition()),2))
				)*12000.0*mult
			);
			// Ensure speed is at least minSpeed
			angleSpeed = (angleSpeed > minSpeed) ? angleSpeed : minSpeed;
			// Set speed to the lift
			angler.moveVoltage(angleSpeed);
		}
  } else if (Angler10UpButton.isPressed()) {
    intake.setBrakeMode(AbstractMotor::brakeMode::coast);
    // if (!did_move_up) {
     //  did_move_up = true;
     //  angler.moveAbsolute(2121, 100);
    // }
   int switchPos = 2500;// Ecoder value where equation kicks in and lift stops going full speed. Lower if knocking over, raise if not getting high enough
   int minSpeed = 5000; // Minimum voltage to send to the lift. May be lower if knocking over stack or wobbly and changing a makes no change
   float a = 2300.0; // Higher value = higher speed for longer when going up
   float mult = 0.7; // Once shape is achieved with a, allows you to slow down the overall curve

   if (angler.getPosition() <= switchPos) { // Do full speed until switchPos
     angler.moveVoltage(12000);
   } else { // Do equation from switchPos until endif
     // Calculate speed based on angle
     int angleSpeed = int(
       (
         (1-pow((1/(a))*(angler.getPosition()),2))
       )*12000.0*mult
     );
     // Ensure speed is at least minSpeed
     angleSpeed = (angleSpeed > minSpeed) ? angleSpeed : minSpeed;
     // Set speed to the lift
     angler.moveVoltage(angleSpeed);
   }
	} else {
		intake.setBrakeMode(AbstractMotor::brakeMode::brake);
		did_move_up = false;
		if(AnglerDownButton.isPressed() ){ //&& !(doing_move_down)
		//	doing_move_down = true;
		  if (angler.getPosition() > LIFT_BOTTOM) {
		  angler.moveVoltage(-12000);
		} else {
	 	angler.moveVoltage(0);
		}
	}

	 // if (doing_move_down){
   //   if (lift.getPosition() <300){
		//  if (angler.getPosition() > LIFT_BOTTOM) {
		// 	 angler.moveVoltage(-11000);
		//  } else {
		// 	 angler.moveVoltage(0);
		// 	 doing_move_down = false;
		//  }
	 // } else {
		//  angler.moveVoltage(0);
	 // }}
 }
//------------------------------------------------------------------------------
if(DoubleTower.isPressed()){
int startmillis = pros::millis();

while (avgFilter.filter(sensor.get_value())< 1900 && (pros::millis() - startmillis) < 1500){
	 pros::delay(10); // Warning: may cause locking
	 intake.moveVoltage(-6000);
}

pros::delay(80);
intake.moveVoltage(0);
//------------------------------------------------------------------------------
	pros::delay(20);
}}
}
