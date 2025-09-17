#include <unistd.h>
#include <stdlib.h>
#include "git/lvgl/lvgl.h"
#include "git/lvgl/demos/lv_demos.h"

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

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_2(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());                  /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);             /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);                                /*Add a label to the button*/
    lv_label_set_text(label, "Button");                                     /*Set the labels text*/
    lv_obj_center(label);
}

int main(void)
{
    lv_init();

    /* DISPLAY: framebuffer */
    lv_display_t *disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, getenv("LV_LINUX_FBDEV_DEVICE") ?: "/dev/fb0");

    /* INPUT: evdev (dotyk/mysz) */
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
