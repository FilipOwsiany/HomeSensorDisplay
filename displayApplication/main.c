#include <unistd.h>
#include <stdio.h>
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

    printf("Starting\n");

    #if BUILDING_FOR_RPI5 == 1
        printf("\n\nBuilding for RPI5\n\n");
        lv_display_t *disp = lv_linux_fbdev_create();
        lv_linux_fbdev_set_file(disp, getenv("LV_LINUX_FBDEV_DEVICE") ?: "/dev/fb0");
        lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

        lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER,
                            getenv("EVDEV_NAME") ?: "/dev/input/event5");
        lv_indev_set_display(indev, disp);
    #endif

    #if BUILDING_FOR_PC == 1
        printf("\n\nBuilding for Host\n\n");
        const char *w_env = getenv("LV_SDL_WIDTH");
        const char *h_env = getenv("LV_SDL_HEIGHT");
        int32_t w = w_env ? atoi(w_env) : 800;
        int32_t h = h_env ? atoi(h_env) : 480;

        extern lv_display_t * lv_sdl_window_create(int32_t width, int32_t height);
        lv_display_t *disp = lv_sdl_window_create(w, h);

        extern lv_indev_t * lv_sdl_mouse_create(void);
        lv_indev_t *indev = lv_sdl_mouse_create();
        lv_indev_set_display(indev, disp);
    #endif

    lv_example_get_started_2();

    while (1) {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
