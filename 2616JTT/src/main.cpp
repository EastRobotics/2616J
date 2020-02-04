#include "main.h"
#include "string.h"
#include "pros/apix.h"
#include "display/lvgl.h"
#include "display/lv_conf.h"

LV_IMG_DECLARE(red_flower);
LV_IMG_DECLARE(bluemask);
LV_IMG_DECLARE(blue_circle);
LV_IMG_DECLARE(push_cube);
LV_IMG_DECLARE(stack_cube);
LV_IMG_DECLARE(bigstack_mask);

int maxVelocity = 200;
bool did_move_up, doing_move_down;
int kill_task = 0;

extern char _PROS_COMPILE_DIRECTORY[30];
#define LIFT_BOTTOM 60

void auton_picker();
// bool did_move_up = false; // Used to only call move up code when first pressed
// bool doing_move_down = false; // Used to automatically move angler down

int x;

okapi::Controller masterController;
void my_task_fn(void * param) {
    char s[30];
    while (1 == 1) {
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




static lv_res_t null_handler(lv_obj_t * obj) {
    pros::delay(250);
    return LV_RES_OK;
}
static lv_obj_t * win;
void disabled() {}

void initialize() {
    inertsensor.reset();

    while (inertsensor.is_calibrating()) {
        pros::delay(5);
    }
    pros::delay(200);
    lv_obj_set_size(scr, 476, 272);
    lv_img_set_src(img_var, &red_flower);
    lv_obj_set_pos(img_var, 0, 0);
}


void competition_initialize() {
    auton_picker();
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

// Chassis Controller - lets us drive the robot around with open- or closed-loop control

void opcontrol() {

    kill_task = 1;

    bleftmg.setReversed(0);
    brightmg.setReversed(1);

    char s[30];

    okapi::AverageFilter<5> avgFilter;
    pros::ADIAnalogIn sensor(2);
    // while(true){
    //  std::cout << "Sensor Reading:" << avgFilter.filter(sensor.get_value())<< "Millis:" << pros::millis()<<"\n";
    // }
    lv_obj_t * img_var = lv_img_create(scr, NULL);

    pros::delay(100);

    LV_IMG_DECLARE(red_flower);
    lv_obj_set_size(scr, 476, 272);
    lv_img_set_src(img_var, &red_flower);
    lv_obj_set_pos(img_var, 0, 0);

    pros::delay(100);

    drive->setMaxVelocity(12000);
    //------------------------------------------------------------------------------
    intake.setBrakeMode(AbstractMotor::brakeMode::brake);
    angler.setBrakeMode(AbstractMotor::brakeMode::brake);
    lift.setBrakeMode(AbstractMotor::brakeMode::brake);

    int count = 0;
    //------------------------------------------------------------------------------
    while (true) {
        drive->getModel()->tank(masterController.getAnalog(ControllerAnalog::leftY),
            masterController.getAnalog(ControllerAnalog::rightY));

        if (IntakeOutButton.changedToPressed()) {
            count = pros::millis();
        }
        if (IntakeInButton.isPressed()) {
            intake.moveVoltage(12000);
            lift.moveVoltage(-1000);

            //resets counter to 0

        } else if (IntakeOutButton.isPressed()) {

            if (pros::millis() - count < 2000) {
                intake.moveVoltage(-6000);

            } else if (IntakeOutButton.isPressed()) {
                intake.moveVoltage(-12000);
            }
            // }else if(IntakeInButton.isPressed()){
            //  count=0;
            // }

        } else {
            intake.moveVoltage(0);
        }
        //------------------------------------------------------------------------------
        if (LiftUpButton.isPressed() && abs(angler.getPosition()) < 700) {
            angler.moveVoltage(12000);
            pros::delay(100);
            lift.moveVoltage(12000);
            //cout  << "button;"

        } else if (LiftUpButton.isPressed()) {
            lift.moveVoltage(12000);

        } else if (LiftDownButton.isPressed()) {
            // lift.moveVoltage(-12000);
            // pros::delay(400);
            // doing_move_down = true;

        } else {
            lift.moveVoltage(-800); //runs down lift constantly
        }
        //------------------------------------------------------------------------------

        if (AnglerUpButton.isPressed()) {
            intake.setBrakeMode(AbstractMotor::brakeMode::coast);
            // if (!did_move_up) {
            //  did_move_up = true;
            //  angler.moveAbsolute(2121, 100);
            // }
            int switchPos = 2500; // Encoder value where equation kicks in and lift stops going full speed. Lower if knocking over, raise if not getting high enough
            int minSpeed = 5000; // Minimum voltage to send to the lift. Maybe lower if knocking over stack or wobbly and changing a makes no change
            float a = 2300.0; // Higher value = higher speed for longer when going up
            float mult = 0.7; // Once shape is achieved with a, allows you to slow down the overall curve

            if (angler.getPosition() <= switchPos) { // Do full speed until switchPos
                angler.moveVoltage(12000);
            } else { // Do equation from switchPos until endif
                // Calculate speed based on angle
                int angleSpeed = int(
                    (
                        (1 - pow((1 / (a)) * (angler.getPosition()), 2))
                    ) * 12000.0 * mult
                );
                // Ensure speed is at least minSpeed
                angleSpeed = (angleSpeed > minSpeed) ? angleSpeed : minSpeed;
                // Set speed to the lift
                angler.moveVoltage(angleSpeed);
            }

        } else {
            intake.setBrakeMode(AbstractMotor::brakeMode::brake);
            did_move_up = false;
            if (AnglerDownButton.isPressed() && !(doing_move_down)) {
                doing_move_down = true;
                //  if (angler.getPosition() > LIFT_BOTTOM) {
                //   angler.moveVoltage(-12000);
                // } else {
                // 	angler.moveVoltage(0);
                // }

            } else if (LiftDownButton.isPressed() && !(doing_move_down)) {
                lift.moveVoltage(-12000);
                pros::delay(400);
                doing_move_down = true;
                //  if (angler.getPosition() > LIFT_BOTTOM) {
                //   angler.moveVoltage(-12000);
                // } else {
                // 	angler.moveVoltage(0);
                // }

            }

            if (doing_move_down) {
                if (angler.getPosition() > LIFT_BOTTOM) {
                    angler.moveVoltage(-11000);
                } else {
                    angler.moveVoltage(0);
                    doing_move_down = false;
                }
            } else {
                angler.moveVoltage(0);
            }
        }
        //------------------------------------------------------------------------------
        if (DoubleTower.isPressed()) {
            int startmillis = pros::millis();
            while (avgFilter.filter(sensor.get_value()) < 1500 && (pros::millis() - startmillis) < 800) {
                pros::delay(10); // Warning: may cause locking
                intake.moveVoltage(-6000);
            }

            pros::delay(80);
            intake.moveVoltage(0);
            //------------------------------------------------------------------------------
            pros::delay(20);
        }
    }
}