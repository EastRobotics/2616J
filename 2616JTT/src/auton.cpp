#include "main.h"

int l, b, r;
bool taskwait;
void asynclift(void * param) {
    taskwait = true;
    int movend = 2000;
    int switchPos = 1400; // Encoder value where equation kicks in and lift stops going full speed. 
    // Lower if knocking over,
    // raise if not getting high enough
    int minSpeed = 2500; // Minimum voltage to send to the lift. Maybe lower if knocking over stack or wobbly and changing a makes no change
    float a = 2000.0; // Higher value = higher speed for longer when going up
    float mult = .8;
    while (taskwait == true) {
        pros::delay(5);
    }
    intake.moveRelative(-50, 80);
    while (angler.getPosition() < movend && kill_task == 0) {
        //		intake.moveVoltage(((pros::millis()/500)%2)==1?12000:0);
        if (angler.getPosition() <= switchPos) { // Do full speed until switchPos
            angler.moveVoltage(12000);
        } else { // Do equation from switchPos until endif
            // Calculate speed based on angle									// v = 1 - ((p/a)^2)
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
    }
}

void auton_stack_red() {
    auto drive = ChassisControllerBuilder()
        .withMotors(
            leftmg, rightmg) // Right motors are 3 & 4
        .withDimensions(AbstractMotor::gearset::green, {
            {
                4_in, 9_in
            },
            imev5GreenTPR
        })
        .build();
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
    drive->moveRaw(1200); //forward

    pros::delay(100);

    drive->setMaxVelocity(100);
    drive->moveRaw(-330); //backward

    intake.moveVoltage(0); //stop intake

    drive->setMaxVelocity(75);
    drive->turnRaw(355); //turn right

    drive->setMaxVelocity(100);
    drive->moveDistance(710_mm); //forward

    intake.moveVoltage(-4000);
    pros::delay(240);
    intake.moveVoltage(0);

    if (abs(angler.getPosition()) <= 1300) { //stack the cubes
        angler.moveVoltage(12000);

    } else if (abs(angler.getPosition()) > 1300 && abs(angler.getPosition()) <= 1500) {
        angler.moveVoltage(3000);

    } else if (abs(angler.getPosition()) > 1500) {
        angler.moveVoltage(1000);
    }
    pros::delay(2500);

    drive->setMaxVelocity(20);
    drive->moveDistance(25_mm);

    drive->moveDistance(-500_mm); //move backward

}

void big_ol_deploy(void) {
    angler.moveVoltage(-1000);
    lift.moveVoltage(-1200);

    // drive->setMaxVelocity(100);
    // drive->moveDistance(-80); //forward

    pros::delay(100);

    drive->setMaxVelocity(110);
    //drive->tank(80, 80);
    drive->moveDistanceAsync(-200_mm);

    intake.moveVoltage(-12000); //outake

    pros::delay(500);

    drive->stop();

    intake.moveVoltage(12000); //intake
}

//------------------------------------------------------------------------------
void auton_stack_blue(int direct) {
    bleftmg.setReversed(0);
    brightmg.setReversed(1);
    big_ol_deploy();
    pros::delay(500);
    drive->setMaxVelocity(80);
    drive->moveDistance(900_mm);

    pros::delay(100);

    driveVector(.3, 1 * direct);
    if (direct == 1) {
        while (inertsensor.get_rotation() < 10) {
            pros::delay(10);
        }
    } else {
        while (inertsensor.get_rotation() > -10) {
            pros::delay(10);
        }
    }
    drive->moveDistance(1.1_ft);
    pros::delay(200);
    drive->moveDistance(-1.1_ft);
    if (direct == 1) {
        drive->turnAngle(-20_deg);
    } else {
        drive->turnAngle(20_deg);
    }
    // driveVector(.1, -1);
    // while(inertsensor.get_rotation()>5){pros::delay(8);}

    intake.moveVoltage(0); //stop intake
    drive->setMaxVelocity(150);
    drive->moveDistance(-1.5_ft); //backward

    pros::delay(250);
    // drive->setMaxVelocity();
    driveVector(.5, direct == 1 ? -1 : 1);
    // while(inertsensor.get_rotation()> direct==1?-90:90){pros::delay(10);}
    if (direct == 1) {
        while (inertsensor.get_rotation() > -90) {
            pros::delay(10);
        }
    } else {
        while (inertsensor.get_rotation() < 90) {
            pros::delay(10);
        }
    }
    // drive->turnRaw(direct * -355);//turn left

    drive->setMaxVelocity(100);
    drive->moveDistance(400_mm); //forward

    intake.moveVoltage(-4000);
    pros::delay(240);
    intake.moveVoltage(0);

    if (abs(angler.getPosition()) <= 1300) { //stack the cubes
        angler.moveVoltage(12000);
    } else if (abs(angler.getPosition()) > 1300 && abs(angler.getPosition()) <= 1500) {
        angler.moveVoltage(3000);
    } else if (abs(angler.getPosition()) > 1500) {
        angler.moveVoltage(1000);
    }
    pros::delay(2500);

    drive->setMaxVelocity(100);
    drive->moveDistance(25_mm);

    drive->moveDistance(-500_mm); //move backward

}

//------------------------------------------------------------------------------
void auton_big_stack_blue(int direct) {

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
    drive->moveDistance(-775_mm); //forward_mm
    //pros::delay(100);

    drive->setMaxVelocity(200);
    intake.moveVoltage(1000);
    // profileController.setTarget("A");
    // profileController.waitUntilSettled();
    // profileController.setTarget(direct==-1?"B":"A",direct==-1?false:true);
    // profileController.setTarget(direct==1?"B":"A");
    // profileController.waitUntilSettled();
    // profileController.setTarget("C");
    // profileController.waitUntilSettled();

    driveVector(1, 0);
    while (leftmg.getPosition() < 65) {
        pros::delay(5);
    }
    driveVector(1, -0.6);
    while (inertsensor.get_rotation() < 40) {
        pros::delay(5);
    }
    driveVector(1, 0);
    while (leftmg.getPosition() < 50) {
        pros::delay(5);
    }

    driveVector(1, 0.6);
    while (inertsensor.get_rotation() > 0) {
        pros::delay(5);
    }
    driveVector(1, 0);
    while (leftmg.getPosition() < 200) {
        pros::delay(5);
    }

    intake.moveVoltage(12000); //intake
    drive->setMaxVelocity(100);
    intake.moveVoltage(12000); //intake
    pros::delay(50);

    drive->setMaxVelocity(100);
    drive->moveDistance(-1400_mm); //forward
    pros::delay(50);
    intake.moveVoltage(0);

    l = ultraLeft.get_value();
    b = ultraBack.get_value();
    r = ultraRight.get_value();

    intake.moveVoltage(700);
    intake.setBrakeMode(AbstractMotor::brakeMode::hold);
    drive->setMaxVelocity(115);
    drive->moveRaw((direct == 1 ? b - l : b - r)); //backward

    drive->setMaxVelocity(75);

    drive->turnRaw(direct * 335); //turn left
    intake.moveVoltage(0);
    drive->setMaxVelocity(100);
    taskwait = false;

    // angler.moveRelative(750, 10000);
    drive->moveRawAsync(-400); //forward

    //intake.moveRelative(600,-200);

    // intake.moveVoltage(-4000);//outake slightly
    // pros::delay(100);
    drive->waitUntilSettled();
    intake.moveVoltage(0);

    //angler.moveAbsolute(1800, 2500);

    pros::delay(1200);

    drive->setMaxVelocity(20); //slight nudge
    drive->moveRaw(-25);

    drive->setMaxVelocity(12000);
    drive->moveRaw(0); //move backward
    //drive->moveDistance(80);
    int atimerend = pros::millis();
    int atimetotal = atimerend - atimerstart;
    char st[30];
    sprintf(st, "");
    sprintf(st, "%d ", atimetotal);
    //sprintf(s, "AE:%05.1f IE:%05.1f CE%d ", angler.getEfficiency(), intake.getEfficiency(), x++);
    // masterController.setText(2,1, "               ");
    masterController.setText(2, 1, st);
    pros::lcd::set_text(6, st);
    // pros::delay(2000);
    // 	drive->stop();
    // 	angler.moveVoltage(0);
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

void auton_push() {
    bleftmg.setReversed(0);
    brightmg.setReversed(1);
    angler.moveVoltage(-1000);
    lift.moveVoltage(-1000);

    drive->setMaxVelocity(100);
    drive->moveRaw(200); //backward

    drive->setMaxVelocity(100);
    drive->moveRaw(500); //backward

    pros::delay(100);

    drive->setMaxVelocity(100);
    drive->moveRaw(300); //forward

    intake.moveVoltage(-12000); //outake
    pros::delay(800);

    intake.moveVoltage(0);
}
pros::Task angle_task(asynclift, (void * )
    "angle", "AngleTask");


void autonomous() {
    switch (autonid) {
    case 1:
        auton_big_stack_blue(-1);
        break;
    case 2:
        auton_big_stack_blue(1);
        break;
    case 3:
        auton_stack_blue(-1);
        break;
    case 4:
        auton_stack_blue(1);
        break;
    case 5:
        auton_push();
        break;
    }
    while (true) {
        pros::delay(50);
    }
}