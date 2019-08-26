int slewRate;
unsigned char driveFL, driveBL, driveFR, driveBR;
char lastDriveFL, lastDriveBL, lastDriveFR, lastDriveBR;
_Bool driveFLReverse, driveBLReverse, driveFRReverse, driveBRReverse;
void driveInit(unsigned char, unsigned char, unsigned char, unsigned char);

void driveSetReverse(_Bool ,_Bool,_Bool,_Bool);

void driveRaw(int, int, int, int);
void enableSlew(int);
int slew(int, int);
void driveRawSlew(int, int, int, int);
void drive(int, int);
