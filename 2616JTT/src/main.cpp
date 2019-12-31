#include "main.h"
#include "2616J.h"
#include "string.h"
#include "pros/apix.h"

#include "display/lvgl.h"
#include "display/lv_conf.h"

extern char _PROS_COMPILE_DIRECTORY[30];
int LIFT_BOTTOM = 50; // TODO Make this the constant thing I forget how
pros::ADIDigitalIn AutonButton(1);
void auton_picker();
bool did_move_up = false; // Used to only call move up code when first pressed
bool doing_move_down = false; // Used to automatically move angler down

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
MotorGroup leftmg({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT});
MotorGroup rightmg({-MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});

MotorGroup drive1({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
MotorGroup turnLeft({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});
MotorGroup turnRight({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});

Motor angler(-MOTOR_ANGLER);

MotorGroup intake({MOTOR_INTAKE_RIGHT, -MOTOR_INTAKE_LEFT});

MotorGroup lift({-MOTOR_LIFT});

auto drive = ChassisControllerFactory::create(
	leftmg, rightmg,
	AbstractMotor::gearset::green,
	{4_in, 9.0_in}
	//distance between the center of the front drive wheels9
);
int x;
okapi::Controller masterController;
void my_task_fn(void *param)
{
	char s[30];
	while (1 == 1)
	{

		sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
		// masterController.setText(2,1, "               ");
		masterController.setText(2, 1, s);
		pros::lcd::set_text(6, s);
		pros::delay(100);
	}
	//Display position of (MotorGroup) on controller
}
static int autonid;

static lv_res_t null_handler(lv_obj_t *obj)
{

	pros::delay(250);
	return LV_RES_OK;
}

static lv_obj_t *win;


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 *///---------------------------------------------------------------------------
void disabled() {
}

LV_IMG_DECLARE(red_flower);
LV_IMG_DECLARE(bluemask);
LV_IMG_DECLARE(blue_circle);
LV_IMG_DECLARE(push_cube);
LV_IMG_DECLARE(stack_cube);
LV_IMG_DECLARE(bigstack_mask);
lv_obj_t *scr = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t *img_var = lv_img_create(scr, NULL);

lv_obj_t *img_push = lv_img_create(scr, NULL);
lv_obj_t *img_stack = lv_img_create(scr, NULL);
lv_obj_t *img_blue = lv_img_create(scr, NULL);
lv_obj_t *img_bigstack = lv_img_create(scr, NULL);

void initialize()
{
	LV_IMG_DECLARE(red_flower);
	lv_obj_set_size(scr, 476, 272);
	lv_img_set_src(img_var, &red_flower);
	lv_obj_set_pos(img_var, 0, 0);
}
//------------------------------------------------------------------------------
static lv_res_t event_handler(lv_obj_t *obj)
{
	autonid++;
	if (autonid > 5)
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
	pros::delay(100);
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

	lv_obj_set_hidden(img_stack, 1);
	lv_obj_set_hidden(img_bigstack, 1);
	lv_obj_set_hidden(img_blue, 1);
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
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Stack Blue Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);

	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	drive.setMaxVelocity(100);
	drive.moveDistance(80); //forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.tank(-80, -100);

	intake.moveVoltage(-12000); //outake

	pros::delay(1000);

	drive.stop();

	intake.moveVoltage(12000); //intake
	pros::delay(150);

	drive.setMaxVelocity(100);
	drive.moveDistance(1200);//forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.moveDistance(-330);//backward

	intake.moveVoltage(0);//stop intake

	drive.setMaxVelocity(75);
	drive.turnAngle(355);//turn right

	drive.setMaxVelocity(100);
	drive.moveDistance(710);//forward

	intake.moveVoltage(-4000);
	pros::delay(240);
	intake.moveVoltage(0);

	if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(12000);

	} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
	angler.moveVoltage(3000);

	} else if(abs(angler.getPosition())>1500) {
	angler.moveVoltage(1000);
	}
	pros::delay(2500);

	drive.setMaxVoltage(20);
	drive.moveDistance(25);

	drive.moveDistance(-500);//move backward

}
//------------------------------------------------------------------------------
void auton_big_stack()
{
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Stack Blue Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);

	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	drive.setMaxVelocity(100);
	drive.moveDistance(80); //forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.tank(-80, -100);

	intake.moveVoltage(-12000); //outake

	pros::delay(1000);

	drive.stop();

	intake.moveVoltage(12000); //intake
	pros::delay(150);

	drive.setMaxVelocity(100);
	drive.moveDistance(1200);//forward
	pros::delay(100);
	//above stays the same



  //new stuff of backing up added above in space
	//code below is correct

  intake.moveVoltage(12000); //intake
  pros::delay(150);

  drive.setMaxVelocity(100);
  drive.moveDistance(1200);//forward
  pros::delay(100);

  drive.setMaxVelocity(100);
  drive.moveDistance(-330);//backward

  intake.moveVoltage(0);//stop intake

	drive.setMaxVelocity(75);
	drive.turnAngle(355);//turn right

	drive.setMaxVelocity(100);
	drive.moveDistance(710);//forward

	intake.moveVoltage(-4000);//outake slightly
	pros::delay(240);
	intake.moveVoltage(0);

	if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(12000);

	} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
	angler.moveVoltage(3000);

	} else if(abs(angler.getPosition())>1500) {
	angler.moveVoltage(1000);
	}
	pros::delay(2500);

	drive.setMaxVoltage(20);//slight nudge
	drive.moveDistance(25);

	drive.moveDistance(-500);//move backward

}
//------------------------------------------------------------------------------
void auton_stack_blue(){
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Stack Blue Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);

	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	drive.setMaxVelocity(100);
	drive.moveDistance(80); //forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.tank(-80, -100);

	intake.moveVoltage(-12000); //outake

	pros::delay(1000);

	drive.stop();

	intake.moveVoltage(12000); //intake
	pros::delay(150);

	drive.setMaxVelocity(100);
	drive.moveDistance(1200);//forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.moveDistance(-330);//backward

	intake.moveVoltage(0);//stop intake

	drive.setMaxVelocity(75);
	drive.turnAngle(-355);//turn left

	drive.setMaxVelocity(100);
	drive.moveDistance(710);//forward

	intake.moveVoltage(-4000);
	pros::delay(240);
	intake.moveVoltage(0);

	if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(12000);

	} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
	angler.moveVoltage(3000);

	} else if(abs(angler.getPosition())>1500) {
	angler.moveVoltage(1000);
	}
	pros::delay(2500);

	drive.setMaxVoltage(20);
	drive.moveDistance(25);

	drive.moveDistance(-500);//move backward

}
//------------------------------------------------------------------------------
void auton_big_stack_blue()
{
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Stack Blue Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);

	angler.moveVoltage(-1000);
	lift.moveVoltage(-1200);

	drive.setMaxVelocity(100);
	drive.moveDistance(80); //forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.tank(-80, -100);

	intake.moveVoltage(-12000); //outake

	pros::delay(1000);

	drive.stop();

	intake.moveVoltage(12000); //intake
	pros::delay(150);

	drive.setMaxVelocity(100);
	drive.moveDistance(1200);//forward

	pros::delay(100);

	drive.setMaxVelocity(100);
	drive.moveDistance(-330);//backward

	intake.moveVoltage(0);//stop intake

	drive.setMaxVelocity(75);
	drive.turnAngle(-355);//turn left

	drive.setMaxVelocity(100);
	drive.moveDistance(710);//forward

	intake.moveVoltage(-4000);
	pros::delay(240);
	intake.moveVoltage(0);

	if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(12000);

	} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
	angler.moveVoltage(3000);

	} else if(abs(angler.getPosition())>1500) {
	angler.moveVoltage(1000);
	}
	pros::delay(2500);

	drive.setMaxVoltage(20);
	drive.moveDistance(25);

	drive.moveDistance(-500);//move backward

}
//------------------------------------------------------------------------------

void auton_push()
{
	// static lv_obj_t *auton_label = lv_label_create(lv_scr_act(), NULL);
	// lv_label_set_text(auton_label, "Push Auton Started");
	// lv_obj_set_pos(auton_label, 100, 100);

	angler.moveVoltage(-1000);
  lift.moveVoltage(-1000);

  drive.setMaxVelocity(100);
  drive.moveDistance(200); //backward

  drive.setMaxVelocity(100);
  drive.moveDistance(-500); //backward

  pros::delay(100);

  drive.setMaxVelocity(100);
  drive.moveDistance(300); //forward


  intake.moveVoltage(-12000); //outake
  pros::delay(800);

  intake.moveVoltage(0);


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
	switch (autonid)
	{
	case 1:
		auton_big_stack();
		break;
	case 2:
		auton_big_stack_blue();
		break;
	case 3:
		auton_stack_red();
		break;
	case 4:
		auton_stack_blue();
		break;
	case 5:
		auton_push();
		break;
	}
while(true){
	pros::delay(50);
}
}
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

//------------------------------------------------------------------------------
char s[30];

 pros::Task my_cpp_task (my_task_fn, (void*)"PROS", "My Task");
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

 ControllerButton LiftUpButton(ControllerDigital::L1);
 ControllerButton LiftDownButton(ControllerDigital::L2);

  ControllerButton DoubleTower(ControllerDigital::A);
	drive.setMaxVoltage(12000);
//------------------------------------------------------------------------------
intake.setBrakeMode(AbstractMotor::brakeMode::brake);
angler.setBrakeMode(AbstractMotor::brakeMode::brake);
lift.setBrakeMode(AbstractMotor::brakeMode::brake);
//angler.tarePosition();
int count =0;
//------------------------------------------------------------------------------
	while (true) {
		drive.tank(masterController.getAnalog(ControllerAnalog::leftY),
						masterController.getAnalog(ControllerAnalog::rightY));
//------------------------------------------------------------------------------
   if(IntakeInButton.isPressed())
	 {
		 intake.moveVoltage(12000);
		 lift.moveVoltage(-1000);
     count =0;
		 //resets counter to 0

	} else if(IntakeOutButton.isPressed()){
			count++;

	 if(count < 100){
		 intake.moveVoltage(-6000);

	 }else if(IntakeOutButton.isPressed()){
		 intake.moveVoltage(-12000);

	 }else if(IntakeInButton.isPressed()){
		 count=0;
	 }

	 }else{
		 intake.moveVoltage(0);
	 }
//------------------------------------------------------------------------------
  if(LiftUpButton.isPressed() && abs(angler.getPosition())<600){
      angler.moveVoltage(12000);
			pros::delay(100);
			lift.moveVoltage(12000);
			//cout  << "button;"

	}else if(LiftUpButton.isPressed()){
		lift.moveVoltage(12000);

	}else if(LiftDownButton.isPressed()){
    lift.moveVoltage(-12000);

	}else{
		lift.moveVoltage(-800);//runs down lift constantly
	}
//------------------------------------------------------------------------------

 if(AnglerUpButton.isPressed())
	 {
		 if (!did_move_up) {
			 did_move_up = true;
			 angler.moveAbsolute(2121, 127);
		 }
		// int switchPos = 1100; // Encoder value where equation kicks in and lift stops going full speed. Lower if knocking over, raise if not getting high enough
		// int minSpeed = 3000; // Minimum voltage to send to the lift. Maybe lower if knocking over stack or wobbly and changing a makes no change
		// float a = 1200.0; // Higher value = higher speed for longer when going up
		//
		// if (angler.getPosition() <= switchPos) { // Do full speed until switchPos
		// 	angler.moveVoltage(12000);
		// } else { // Do equation from switchPos until endif
		// 	// Calculate speed based on angle
		// 	int angleSpeed = int(
		// 		(
		// 			(1-pow((1/(a))*(angler.getPosition()-switchPos),2))
		// 		)*12000.0
		// 	);
		// 	// Ensure speed is at least minSpeed
		// 	angleSpeed = (angleSpeed > minSpeed) ? angleSpeed : minSpeed;
		// 	// Set speed to the lift
		// 	angler.moveVoltage(angleSpeed);
		// }

	} else {
		did_move_up = false;
		if(AnglerDownButton.isPressed() && !(doing_move_down)){
			doing_move_down = true;
		//  if (angler.getPosition() > LIFT_BOTTOM) {
		//   angler.moveVoltage(-12000);
		// } else {
		// 	angler.moveVoltage(0);
		// }


	} else if(LiftDownButton.isPressed() && !(doing_move_down)){
		 pros::delay(300);
		 doing_move_down = true;
		//  if (angler.getPosition() > LIFT_BOTTOM) {
		//   angler.moveVoltage(-12000);
		// } else {
		// 	angler.moveVoltage(0);
		// }

	 }

	 if (doing_move_down){
		 if (angler.getPosition() > LIFT_BOTTOM) {
			 angler.moveVoltage(-12000);
		 } else {
			 angler.moveVoltage(0);
			 doing_move_down = false;
		 }
	 } else {
		 angler.moveVoltage(0);
	 }
 }
//------------------------------------------------------------------------------
if(DoubleTower.isPressed()){
intake.tarePosition();
intake.moveRelative(-750, 100);
pros::delay(500);

}
//------------------------------------------------------------------------------
	pros::delay(20);
 }
}
