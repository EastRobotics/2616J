#include "main.h"
MotorGroup leftmg({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT});
MotorGroup rightmg({-MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
MotorGroup drive1({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
MotorGroup turnLeft({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});
MotorGroup turnRight({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
Motor angler(-MOTOR_ANGLER);
MotorGroup intake({MOTOR_INTAKE_RIGHT, -MOTOR_INTAKE_LEFT});
MotorGroup lift({-MOTOR_LIFT});
MotorGroup bleftmg({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT});
MotorGroup brightmg({MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});

std::shared_ptr<ChassisController> drive = ChassisControllerBuilder()
    .withMotors(
        bleftmg, brightmg) // Right motors are 3 & 4
    .withDimensions(AbstractMotor::gearset::green, {
        {
            4_in, 9_in
        },
        imev5GreenTPR
    })
    .build();