#include "main.h"
pros::Imu inertsensor(INERTIA_MODULE_PORT);
pros::ADIDigitalIn AutonButton(AUTON_SELECT);
pros::ADIUltrasonic ultraLeft(US_LEFT_EMIT, US_LEFT_REC);
pros::ADIUltrasonic ultraRight(US_RIGHT_EMIT, US_RIGHT_REC);
pros::ADIUltrasonic ultraBack(US_BACK_EMIT, US_BACK_REC);