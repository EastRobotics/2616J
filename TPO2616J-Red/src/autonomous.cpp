#include "main.h"
#include "2616J.hpp"
#include "shooter.hpp"

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
using namespace okapi;

void autonomous() {

     Motor m1(MOTOR_DRIVE_FRONT_RIGHT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
     Motor m2(MOTOR_DRIVE_BACK_RIGHT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
     Motor m3(MOTOR_DRIVE_FRONT_LEFT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
     Motor m4(MOTOR_DRIVE_BACK_LEFT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

    Motor IntakeM(MOTOR_INTAKE, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
    Motor IndexerM(MOTOR_INDEXER, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
    Motor FlywheelM(MOTOR_FLYWHEEL, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
    Motor DescorerM(MOTOR_DESCORER, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

    Shooter shooter(MOTOR_INTAKE,MOTOR_INDEXER,MOTOR_FLYWHEEL,6);

    MotorGroup mgleft = MotorGroup({m1, m2});
    MotorGroup mgrght = MotorGroup({m3, m4});

        auto drive = ChassisControllerFactory::create(
        mgleft, mgrght,
        AbstractMotor::gearset::green,
        {4.25_in, 9.5_in});

    pros::ADIAnalogIn BallFireDetect (6);

    drive.setTurnsMirrored(false);

    FlywheelM.set_brake_mode(MOTOR_BRAKE_COAST);
    FlywheelM.moveVelocity(10800);

    IntakeM.moveVelocity(12000);

    drive.setMaxVelocity(160);  // Set chassis speed to 80%
    
    drive.moveDistance(46.3_in);  // Move forard and retreive ball from under cap
    drive.moveDistance(-45.25_in);  // Move back to starting position
    drive.turnAngle(-90_deg);   //  Turn to face flag
    drive.moveDistance(12.5_in);
    //fire ball

    drive.moveDistance(13.0_in);
    //fire ball

    drive.turnAngle(-5_deg);
    drive.moveDistance(12_in);

    drive.moveDistance(6_ft);



}
