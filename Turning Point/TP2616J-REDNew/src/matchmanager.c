#include "main.h"


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

//using namespace okapi;

lv_obj_t *bluebtn;
lv_obj_t *redbtn;
lv_obj_t *btnm_red;
lv_obj_t *btnm_blue;
static lv_style_t style_bg_red, style_bg_blue, stylesensortitle;
//pros::ADIUltrasonic ultrasonic(ULTRASONICYELLOW, ULTRASONICORANGE);
//AverageFilter<5> USonicFltr;
static lv_res_t btn_click_action(lv_obj_t *btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    printf("Button %d is released\n", id);
    if (id == 1)
    {
        lv_btn_set_state(bluebtn, LV_BTN_STATE_TGL_REL);
    }
    else if (id == 2)
    {
        lv_btn_set_state(redbtn, LV_BTN_STATE_TGL_REL);
    }
    lv_btn_set_state(btn, LV_BTN_STATE_TGL_PR);
    /* The button is released.
     * Make something here */

    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

static lv_res_t btnm_action(lv_obj_t *btnm, const char *txt)
{
    printf("Button: %s released\n", txt);
    if (strcmp(txt, "Red Auton") == 0)
    {
        lv_obj_set_hidden(btnm_blue, true);
        lv_obj_set_hidden(btnm_red, false);
        lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_REL, &style_bg_red);
    }
    if (strcmp(txt, "Blue Auton") == 0)
    {
        lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_REL, &style_bg_blue);
        lv_obj_set_hidden(btnm_blue, false);
        lv_obj_set_hidden(btnm_red, true);
    }
    if (strcmp(txt, "Robot Skills") == 0)
    {
        lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_REL, &style_bg_red);
        lv_obj_set_hidden(btnm_blue, true);
        lv_obj_set_hidden(btnm_red, true);
    }
    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

void matchmanager(void)
{

    static lv_style_t style_btn_prs;

    lv_style_copy(&style_btn_prs, &lv_style_pretty_color);
    style_btn_prs.body.main_color = LV_COLOR_RED;
    style_btn_prs.body.border.color = LV_COLOR_SILVER;
    style_btn_prs.body.border.width = 0;
    style_btn_prs.body.radius = 0;

    lv_style_copy(&stylesensortitle, &lv_style_plain);

    stylesensortitle.text.line_space = 1;
    stylesensortitle.text.color = LV_COLOR_RED;
    lv_style_copy(&style_bg_red, &lv_style_plain);
    style_bg_red.body.main_color = LV_COLOR_RED;
    style_bg_red.body.grad_color = LV_COLOR_SILVER;
    style_bg_red.body.padding.hor = 5;
    style_bg_red.body.padding.ver = 3;
    style_bg_red.body.padding.inner = 3;
    lv_style_copy(&style_bg_blue, &lv_style_plain);
    style_bg_blue.body.main_color = LV_COLOR_BLUE;
    style_bg_blue.body.grad_color = LV_COLOR_SILVER;
    style_bg_blue.body.padding.hor = 5;
    style_bg_blue.body.padding.ver = 3;
    style_bg_blue.body.padding.inner = 3;
    static const char *btnm_map[] = {"\201Red Auton", "Robot Skills", "Blue Auton", ""};

    static const char *btnm_map_red[] = {"\201Front", "\n", "Back", ""};
    static const char *btnm_map_blue[] = {"\201Front", "\n", "Back", ""};

    lv_obj_t *primaryscreen, *sensorbar, *leftsensor, *middlesensor,
        *rightsensor, *leftsensortext, *middlesensortext,
        *rightsensortext, *leftsensortitle, *middlesensortitle, *rightsensortitle;
    char leftsensorstr[15];

    primaryscreen = lv_cont_create(lv_scr_act(), NULL); //lv_scr_act()
    sensorbar = lv_cont_create(primaryscreen, NULL);
    leftsensor = lv_cont_create(sensorbar, NULL);
    middlesensor = lv_cont_create(sensorbar, NULL);
    rightsensor = lv_cont_create(sensorbar, NULL);

    leftsensortext = lv_label_create(leftsensor, NULL);
    middlesensortext = lv_label_create(middlesensor, NULL);
    rightsensortext = lv_label_create(rightsensor, NULL);

    leftsensortitle = lv_label_create(leftsensor, NULL);
    rightsensortitle = lv_label_create(rightsensor, NULL);
    middlesensortitle = lv_label_create(middlesensor, NULL);
    //sprintf(leftsensorstr,"%d",ultrasonic.get_value());

    lv_obj_set_size(primaryscreen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_size(sensorbar, LV_HOR_RES, LV_VER_RES / 5);
    lv_obj_set_size(leftsensor, LV_HOR_RES / 3, LV_VER_RES / 5);
    lv_obj_set_size(middlesensor, LV_HOR_RES / 3, LV_VER_RES / 5);
    lv_obj_set_size(rightsensor, LV_HOR_RES / 3, LV_VER_RES / 5);
    lv_obj_set_pos(primaryscreen, 0, 0);
    lv_obj_set_pos(sensorbar, 0, LV_VER_RES - LV_VER_RES / 5);
    //lv_obj_set_pos(leftsensor, 0, LV_VER_RES - LV_VER_RES/5);
    //lv_obj_set_pos(middlesensor, LV_HOR_RES/3, LV_VER_RES - LV_VER_RES/5);
    //lv_obj_set_pos(rightsensor, (LV_HOR_RES/3)*2, LV_VER_RES - LV_VER_RES/5);

    /*Create a default button matrix*/
    lv_obj_t *btnm1 = lv_btnm_create(primaryscreen, NULL);
    lv_btnm_set_map(btnm1, btnm_map);
    lv_btnm_set_action(btnm1, btnm_action);
    lv_obj_set_size(btnm1, LV_HOR_RES, LV_VER_RES / 3);
    lv_btnm_set_toggle(btnm1, true, 0);

    btnm_red = lv_btnm_create(primaryscreen, NULL);

    lv_btnm_set_map(btnm_red, btnm_map_red);
    lv_btnm_set_action(btnm_red, btnm_action);
    lv_obj_set_size(btnm_red, LV_HOR_RES / 2 / 2, LV_VER_RES / 3); //lv_obj_set_pos(btnm_red,(LV_HOR_RES/2)/4,(LV_VER_RES/2/4)+(LV_VER_RES/3)/2);
    lv_obj_set_pos(btnm_red, 60, 100);
    lv_btnm_set_style(btnm_red, LV_BTNM_STYLE_BTN_TGL_REL, &style_bg_red);
    lv_btnm_set_toggle(btnm_red, true, 0);

    btnm_blue = lv_btnm_create(primaryscreen, NULL);

    lv_btnm_set_map(btnm_blue, btnm_map_blue);
    lv_btnm_set_action(btnm_blue, btnm_action);
    lv_obj_set_size(btnm_blue, LV_HOR_RES / 2 / 2, LV_VER_RES / 3);
    lv_obj_set_pos(btnm_blue, (LV_HOR_RES / 2) + (LV_HOR_RES / 2) / 4, 100);
    lv_btnm_set_style(btnm_blue, LV_BTNM_STYLE_BTN_TGL_REL, &style_bg_blue);
    lv_btnm_set_toggle(btnm_blue, true, 0);
    lv_obj_set_hidden(btnm_blue, true);
    lv_obj_set_hidden(btnm_red, true);

    lv_obj_align(leftsensor, sensorbar, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_align(middlesensor, sensorbar, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(rightsensor, sensorbar, LV_ALIGN_IN_RIGHT_MID, 0, 0);

    lv_obj_set_style(leftsensortitle, &stylesensortitle);
    lv_label_set_text(leftsensortitle, "Ultrasonic");
    lv_obj_set_style(middlesensortitle, &stylesensortitle);
    lv_label_set_text(middlesensortitle, "Gyro");
    lv_obj_set_style(rightsensortitle, &stylesensortitle);
    lv_label_set_text(rightsensortitle, "Accel");
    lv_obj_align(leftsensortitle, leftsensor, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_align(middlesensortitle, middlesensor, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_align(rightsensortitle, rightsensor, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_align(leftsensortext, leftsensor, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_align(middlesensortext, middlesensor, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_align(rightsensortext, rightsensor, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_t *resetbutton = lv_btn_create(primaryscreen, NULL);
    lv_obj_t *resetbuttonlbl = lv_label_create(resetbutton, NULL);
    lv_btn_set_style(resetbutton, LV_BTN_STYLE_PR, &style_btn_prs);
    ;
    lv_btn_set_style(resetbutton, LV_BTN_STYLE_REL, &lv_style_pretty_color);
    lv_obj_set_size(resetbutton, 100, 50);
    lv_label_set_text(resetbuttonlbl, "Reset\nSensors");
    lv_obj_align(resetbutton, primaryscreen, LV_ALIGN_CENTER, 0, 45);

    while (true)
    {
       // sprintf(leftsensorstr, "%.0f", USonicFltr.filter(ultrasonic.get_value()));
        //leftsensorstr << int(USonicFltr.filter( ultrasonic.get_value()));const std::string tmp = stringstream.str();
        //const std::string tmp = leftsensorstr.str();
        lv_label_set_static_text(leftsensortext, leftsensorstr);
        //	std::cout << "Distance: " << USonicFltr.filter(ultrasonic.get_value()) << std::endl;
        //printf("%s\r\n", leftsensorstr);
        delay(250);
    }
}