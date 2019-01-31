#include "main.h"
#include "sharedautonfunctions.h"


int accelX_init;
int accelY_init;
int accelZ_init;
int line_init;
void on_center_button() {

}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
//adi_analog_calibrate('F');
  adi_analog_calibrate(ACCELEROMETER_X); //calibrates the x axis input
  adi_analog_calibrate(ACCELEROMETER_Y); //calibrates the y axis input
  adi_analog_calibrate(ACCELEROMETER_Z); //calibrates the z axis input

  accelX_init = adi_analog_read_calibrated(ACCELEROMETER_X);
  accelY_init = adi_analog_read_calibrated(ACCELEROMETER_Y);
  accelZ_init = adi_analog_read_calibrated(ACCELEROMETER_Z);
  line_init = adi_analog_read('F');
  printf("Init %d", accelZ_init);
  adi_port_set_config('F', E_ADI_ANALOG_IN);
  	// pros::ADIAnalogIn sensore ('E');
    // pros::ADIAnalogIn sensorc ('C');
    // pros::ADIAnalogIn sensord ('D');
    // sensore.calibrate();
    // sensorc.calibrate();
    // sensord.calibrate();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  matchmanager();
}
