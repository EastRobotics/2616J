#include "main.h"

void driveVector(const double iforwardSpeed,
    const double iyaw) {
    // This code is taken from WPIlib. All credit goes to them. Link:
    // https://github.com/wpilibsuite/allwpilib/blob/master/wpilibc/src/main/native/cpp/Drive/DifferentialDrive.cpp#L73
    const double forwardSpeed = std::clamp(iforwardSpeed, -1.0, 1.0);
    const double yaw = std::clamp(iyaw, -1.0, 1.0);
    double leftOutput = forwardSpeed + yaw;
    double rightOutput = forwardSpeed - yaw;
    if (const double maxInputMag = std::max < double > (std::abs(leftOutput), std::abs(rightOutput)); maxInputMag > 1) {
        leftOutput /= maxInputMag;
        rightOutput /= maxInputMag;
    }
    leftmg.moveVelocity(static_cast < int16_t > (leftOutput * maxVelocity));
    rightmg.moveVelocity(static_cast < int16_t > (rightOutput * maxVelocity));
}