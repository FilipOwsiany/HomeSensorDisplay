#include <unistd.h>
#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void lv_example_get_started_2(void)
{
    /* --- Button 1 --- */
    lv_obj_t * btn1 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn1, 10, 10);
    lv_obj_set_size(btn1, 120, 50);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Button 1");
    lv_obj_center(label1);

    /* --- Button 2 --- */
    lv_obj_t * btn2 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn2, 140, 10);
    lv_obj_set_size(btn2, 120, 50);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Button 2");
    lv_obj_center(label2);

    /* --- Button 3 --- */
    lv_obj_t * btn3 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn3, 270, 10);
    lv_obj_set_size(btn3, 120, 50);
    lv_obj_add_event_cb(btn3, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "Button 3");
    lv_obj_center(label3);
}

int main(void)
{
    lv_init();


    lv_display_t *disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, getenv("LV_LINUX_FBDEV_DEVICE") ?: "/dev/fb0");
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

    lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER,
                        getenv("EVDEV_NAME") ?: "/dev/input/event5");
    lv_indev_set_display(indev, disp);

    lv_example_get_started_2();

    while (1) {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
