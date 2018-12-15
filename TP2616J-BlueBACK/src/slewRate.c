
/*
int slewRate = 0;
unsigned int driveFL, driveBL, driveFR, driveBR;
int lastDriveFL, lastDriveBL, lastDriveFR, lastDriveBR;
_Bool driveFLReverse, driveBLReverse, driveFRReverse, driveBRReverse;
// //
// void driveInit(unsigned char _driveFL, unsigned char _driveBL,
//                unsigned char _driveFR, unsigned char _driveBR) {
//   printf("[ELib] Initialized drive motors\n");
//   driveFL = _driveFL; // Set our front left motor
//   driveBL = _driveBL; // Set our back  left motor
//   driveFR = _driveFR; // Set our front right motor
//   driveBR = _driveBR; // Set our back  right motor
// }
//
// This is one of the dumbest things
// I was wrong

void driveSetReverse(_Bool _driveFLReverse, _Bool _driveBLReverse,
                     _Bool _driveFRReverse, _Bool _driveBRReverse) {
  driveFLReverse = _driveFLReverse;
  driveBLReverse = _driveBLReverse;
  driveFRReverse = _driveFRReverse;
  driveBRReverse = _driveBRReverse;
}

void driveRaw(int speedFL, int speedBL, int speedFR, int speedBR) {
  // Set drive front left
  lastDriveFL = speedFL; // Save speed for slew code
  driveIfValid(driveFL, speedFL * (driveFLReverse ? -1 : 1), "driveFL");
  // Set drive back  left
  lastDriveBL = speedBL; // Save speed for slew code
  driveIfValid(driveBL, speedBL * (driveBLReverse ? -1 : 1), "driveBL");
  // Set drive front right
  lastDriveFR = speedFR; // Save speed for slew code
  driveIfValid(driveFR, speedFR * (driveFRReverse ? -1 : 1), "driveFR");
  // Set drive back  right
  lastDriveBR = speedBR; // Save speed for slew code
  driveIfValid(driveBR, speedBR * (driveBRReverse ? -1 : 1), "driveBR");
}

void enableSlew(int _slewRate) { slewRate = _slewRate; }

int slew(int currentSpeed, int targetSpeed) {
  // If slew rate is off or we are closer to the target than slewRate
  if (slewRate == 0 || abs(currentSpeed - targetSpeed) < slewRate)
    return targetSpeed; // Return the target
  else {                // We need to slew
    // Add positive or negative slewRate to the given speed and return it
    currentSpeed += (currentSpeed > targetSpeed) ? -1 * slewRate : slewRate;
    return currentSpeed;
  }
}

void driveRawSlew(int speedFL, int speedBL, int speedFR, int speedBR) {
  driveRaw(slew(lastDriveFL, speedFL), slew(lastDriveBL, speedBL),
           slew(lastDriveFR, speedFR), slew(lastDriveBR, speedBR));
}

void drive(int speedForward, int speedTurn) {
  int speedFL = speedForward - speedTurn;
  int speedBL = speedForward - speedTurn;
  int speedFR = speedForward + speedTurn;
  int speedBR = speedForward + speedTurn;
  driveRawSlew(speedFL, speedBL, speedFR, speedBR);
}
*/
