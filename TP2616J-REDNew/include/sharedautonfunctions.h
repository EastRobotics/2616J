#include <main.h>
#ifndef _TEAMFUNCTIONS
#define _TEAMFUNCTIONS

void chassis_move(int velo);
void chassis_move_absolute(double posit, int velo);
void wait_move(int ticks, float p);
extern int accelZ_init;
void wait_turn(int trn, int speed, int dir, int tout);
void setup_chassis(void);
void setup_ops(void);
void fire_ball(void);

void matchmanager(void);

#endif //_TEAMFUNCTIONS