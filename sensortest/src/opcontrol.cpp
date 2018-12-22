#include "main.h"



pros::ADIAnalogIn linesensor ('F');

void opcontrol() {
while(true){
printf("linesensor %d\r\n",linesensor.get_value());
pros::delay(10);

}


	pros::ADIAnalogIn sensore ('E');
pros::ADIAnalogIn sensorc ('C');
pros::ADIAnalogIn sensord ('D');
sensore.calibrate();
sensorc.calibrate();
sensord.calibrate();
	while(true){
		printf("X - %d Y- %d Z- %d\r\n",
		 sensore.get_value_calibrated(),
sensorc.get_value_calibrated(),
sensord.get_value_calibrated());
pros::delay(50);
	}
}
