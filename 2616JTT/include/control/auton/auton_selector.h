#ifndef _AUTON_H_
#define _AUTON_H_
extern int autonid;

extern lv_obj_t* scr;
extern lv_obj_t* img_var;

extern lv_obj_t* img_push;
extern lv_obj_t* img_stack;
extern lv_obj_t* img_blue;
extern lv_obj_t* img_bigstack;
static lv_res_t event_handler(lv_obj_t* );
void auton_picker(void);
#endif