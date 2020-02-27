#pragma once

#include <vector>
#include <string>
#include "main.h"

typedef struct motorCode {
    pros::Motor motor;
    std::string code;
} motorCode;

void overTempWarning(void);
void OTWarning_task(void * a);
void showTemps(void * a);
