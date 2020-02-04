#include "main.h"
LV_IMG_DECLARE(red_flower);
LV_IMG_DECLARE(bluemask);
LV_IMG_DECLARE(blue_circle);
LV_IMG_DECLARE(push_cube);
LV_IMG_DECLARE(stack_cube);
LV_IMG_DECLARE(bigstack_mask);
int autonid;
static lv_obj_t * win;
lv_obj_t* scr = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t* img_var = lv_img_create(scr, NULL);

lv_obj_t* img_push = lv_img_create(scr, NULL);
lv_obj_t* img_stack = lv_img_create(scr, NULL);
lv_obj_t* img_blue = lv_img_create(scr, NULL);
lv_obj_t* img_bigstack = lv_img_create(scr, NULL);

#define BSTACK_CB 0x1
#define BLUE_MASK 0x2
#define PUSH_CB 0x4
#define STACK_CB 0x8

int states[] = {
  PUSH_CB,
  BSTACK_CB,
  BSTACK_CB | BLUE_MASK,
  STACK_CB,
  STACK_CB  | BLUE_MASK,
  PUSH_CB
};

void set_state(int mask) {
  lv_obj_set_hidden(img_stack,
      mask & STACK_CB
  );
  lv_obj_set_hidden(img_push,
      mask & PUSH_CB
  );
  lv_obj_set_hidden(img_blue, 
      mask & BLUE_MASK
  );
  lv_obj_set_hidden(img_bigstack,
      mask & BSTACK_CB
  );
}

static lv_res_t event_handler(lv_obj_t* obj) {
    autonid++;
    if (autonid > 5) {
        autonid = 1;
    }
    #ifdef DEBUG
    printf("A=%d\n", autonid);
    #endif
    set_state(states[autonid]);
    pros::delay(50);
    return LV_RES_OK;
}

void auton_picker() {
    pros::delay(200);
    static lv_style_t bstyle;
    autonid = 1;
    lv_obj_set_size(scr, 476, 272);
    lv_img_set_src(img_var, &red_flower);
    lv_obj_set_pos(img_var, 0, 0);

    lv_obj_set_hidden(img_push, 1);
    static lv_style_t * pstyle = lv_img_get_style(img_push);
    pstyle->image.color = {
        0x0,
        0x0,
        0x0,
        0xff
    };
    lv_img_set_style(img_push, pstyle);
    lv_img_set_style(img_stack, pstyle);
    lv_img_set_style(img_bigstack, pstyle);

    // lv_obj_set_hidden(img_stack, 1);
    // lv_obj_set_hidden(img_bigstack, 1);
    // lv_obj_set_hidden(img_blue, 1);
    lv_img_set_src(img_blue, &bluemask);
    lv_img_set_src(img_push, &push_cube);
    lv_img_set_src(img_stack, &stack_cube);
    lv_img_set_src(img_bigstack, &bigstack_mask);

    lv_obj_set_pos(img_blue, 0, 0);
    lv_obj_set_pos(img_push, 0, 0);
    lv_obj_set_pos(img_stack, 0, 0);
    lv_obj_set_pos(img_bigstack, 0, 0);

    set_state(states[1]);

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);

    lv_style_copy( &bstyle, &lv_style_plain);
    bstyle.body.border.color = {
        0xff,
        0xff,
        0xff,
        0xff
    };
    bstyle.body.empty = true;
    lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, event_handler);
    lv_obj_set_pos(btn, 274, 83);
    lv_obj_set_height(btn, 130);
    lv_btn_set_style(btn, LV_BTN_STATE_REL, &bstyle);
    lv_btn_set_style(btn, LV_BTN_STATE_PR, &bstyle);
    while (true) {
        if (AutonButton.get_new_press()) {
            event_handler(NULL);
            pros::delay(50);
        }
    }
}
