#include "motorTemps.hpp"
#include "2616J.h"

pros::Mutex mutexControllerScreen;
constexpr char blankStr[] = "                 ";
constexpr int tempLimit = 55;
pros::Controller controller (pros::E_CONTROLLER_MASTER);

std::vector<motorCode> motorCodes =
        {
            {pros::Motor (MOTOR_DRIVE_BACK_LEFT), "BL"},      {pros::Motor (MOTOR_DRIVE_FRONT_LEFT), "FL"},
            {pros::Motor (MOTOR_DRIVE_BACK_RIGHT), "BR"},     {pros::Motor (MOTOR_DRIVE_FRONT_RIGHT), "FR"},
            {pros::Motor (MOTOR_LIFT), "LT"},        {pros::Motor (MOTOR_ANGLER), "AG"},
            {pros::Motor (MOTOR_INTAKE_LEFT), "LI"}, {pros::Motor (MOTOR_INTAKE_RIGHT), "RI"}
        };
//
// void overTempWarning()
// {
//     std::vector<motorCode> overTempMotors;
//     bool overTemp = false;
//     static bool overTempStatic;
//
//     for (auto& i: motorCodes) {
//         if (i.motor.get_temperature() >= tempLimit) {
//             if (!overTempStatic)
//                 controller.rumble("...");
//             overTempMotors.insert(overTempMotors.end(), i);
//             overTempStatic = true;
//             overTemp = true;
//         }
//     }
//
//     overTempStatic = overTemp;
//
//     mutexControllerScreen.take(TIMEOUT_MAX);
//     if (overTemp){
//         controller.set_text(0, 0, blankStr);
//         pros::delay(50);
//         controller.set_text(1, 0, blankStr);
//         pros::delay(50);
//         controller.set_text(2, 0, blankStr);
//         pros::delay(50);
//         std::vector<std::string> buffer;
//         for (auto& i : overTempMotors)
//             buffer.insert(buffer.end(), i.code + " - " + std::to_string(i.motor.get_temperature()));
//
//         if (buffer.size() % 2 == 0 && buffer.size() <= 6) {
//             for (int i = 0; i < buffer.size(); i += 2) {
//                 std::string str = buffer[i] + ' ' + buffer[i+1];
//                 controller.set_text(i / 2, 0, str.c_str());
//                 pros::delay(50);
//             }
//         } else if (buffer.size() <= 5) {
//             for (int i = 0; i < buffer.size() - 1; i += 2) {
//                 std::string str = buffer[i] + ' ' + buffer[i+1];
//                 controller.set_text(i / 2, 0, str.c_str());
//                 pros::delay(50);
//             }
//             controller.set_text((buffer.size()-1) / 2, 0, buffer.back().c_str());
//             pros::delay(50);
//         } else
//             controller.set_text(1, 0, "ALL MOTORS DEAD");
//             pros::delay(50);
//     }
//     mutexControllerScreen.give();
// }
//
// void OTWarning_task(void * a) {
//     while (true) {
//         overTempWarning();
//         pros::delay(500);
//     }
// }

void showTemps(void * a) {
    int count;
    bool released;
    while (true) {
        count = 0;
        released = false;
        mutexControllerScreen.take(TIMEOUT_MAX);
        while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            count++;
            if (count % 4 <= 1) {
              controller.clear();
              pros::delay(50);
                controller.print(0, 0, "BL ~%d  FL ~%d \xff",
                    (int)motorCodes[0].motor.get_temperature(), (int)motorCodes[1].motor.get_temperature());
                pros::delay(75);
            } else {
                controller.print(0, 0, "BR ~%d  FR ~%d\xff",
                    (int)motorCodes[2].motor.get_temperature(), (int)motorCodes[3].motor.get_temperature());
                pros::delay(75);
            }
            controller.print(1, 0, "TR ~%d  LT ~%d\xff",
                    (int)motorCodes[4].motor.get_temperature(), (int)motorCodes[5].motor.get_temperature());
                pros::delay(75);
            controller.print(2, 0, "LI ~%d  RT ~%d\xff",
                    (int)motorCodes[6].motor.get_temperature(), (int)motorCodes[7].motor.get_temperature());
                pros::delay(75);
            pros::delay(500);
            released = true;
        }
        if (released){
            controller.clear();
        }
        controller.set_text(0, 2, " >Jersey Devils<\xff");
        pros::delay(50);
        controller.print(1, 0, "Battery --> %d%%\xff", (int)pros::battery::get_capacity());
        pros::delay(50);
        controller.print(2, 0, "ContBat --> %d%%\xff", controller.get_battery_capacity());


        mutexControllerScreen.give();
        pros::delay(350);
    }
}
