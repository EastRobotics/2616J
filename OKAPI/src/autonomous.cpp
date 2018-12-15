#include "main.h"

/**
 * 2616C Turning Point proposed autonomous run from the Red Front Position
 */

using namespace okapi;

Motor flywheelMotor = 1_mtr;
Motor intakeMotor = 13_mtr;
Motor indexerMotor = 15_mtr;
Motor descorerMotor = 14_mtr;

MotorGroup leftmotors({4});
MotorGroup rightmotors({10});

//rightmotors.okapi::AbstractMotor::setReversed(true);
auto drive = ChassisControllerFactory::create(
    4, 10,
    AbstractMotor::gearset::green,
    {4.125_in, 9.5_in});

void autonomous()
{
  //  flywheelMotor.move(127);
  //  intakeMotor.move(127);
  rightmotors.setReversed(true);

    printf("Left - %d     Right - %d \r\n",rightmotors.getCurrentLimit(),leftmotors.getCurrentLimit());
    drive.moveDistance(20_ft);
    pros::delay(100);
    drive.moveDistance(-20_ft);
    drive.turnAngle(-90_deg);
    drive.moveDistance(18_in);
    pros::delay(200);
    indexerMotor.moveRelative(360, 127);
    pros::delay(500);
    drive.moveDistance(18_in);
    pros::delay(200);
    indexerMotor.moveRelative(360, 127);
    pros::delay(500);
    drive.turnAngle(-30_deg);
    drive.moveDistance(18_in);
    pros::delay(250);
    drive.moveDistance(6_ft);
    drive.turnAngle(-90_deg);
    drive.moveDistance(12_in);
    drive.moveDistance(6_ft);
}
