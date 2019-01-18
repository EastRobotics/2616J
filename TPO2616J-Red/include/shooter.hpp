#include "main.h"
class Shooter
{
okapi::Motor IntakeM;
okapi::Motor IndexerM;
okapi::Motor FlywheelM;
pros::ADIAnalogIn BallFireDetect;

public:
 Shooter(int intake, int index, int flywheel,int linesense);

    void runIntakeForward(){}
  void runIntakeBackward(){}
  void FlywheelStop();
  void fire();

};
