#include "main.h"
#include "2616J.hpp"

using namespace okapi;
class Shooter
{ 
okapi::Motor IntakeM;
okapi::Motor IndexerM;
okapi::Motor FlywheelM;
pros::ADIAnalogIn BallFireDetect;

public:
 Shooter(int intake, int index, int flywheel,int linesense):
   IntakeM(intake, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees),
   IndexerM(index, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees),
   FlywheelM(flywheel, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees),
   BallFireDetect(linesense){

    FlywheelM.moveVoltage(10800);
  }

    void runIntakeForward(){}
  void runIntakeBackward(){}
  void FlywheelStop(){
    FlywheelM.moveVoltage(0);
  };
  void fire(){
    IndexerM.moveRelative(1000, 200);
  //       intakerun = millis();
  // taskindexeron = 1;
  // motor_move(MOTOR_INDEXER,127);
  // motor_move(MOTOR_INTAKE,127);
  // while(adi_analog_read_calibrated('F')>BALLFIRE && ((millis() - intakerun) < 1500 ) ){
  // //  printf("%d\r\n",adi_analog_read_calibrated('F'));
  // printf("%d %f\r\n",millis() - intakerun,motor_get_power(MOTOR_INTAKE) );
  //  delay(5);
  // }
  // motor_move(MOTOR_INDEXER,0);
  // motor_move(MOTOR_INTAKE,0);
  // taskindexeron = 0;
  }

};