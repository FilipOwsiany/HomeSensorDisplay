#include <unistd.h>
#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

/* --- UI --- */

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

static void ui_create(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
}

/* --- MAIN --- */

int main(void)
{
    lv_init();

    /* DISPLAY: SDL window */
    /* Możesz nadpisać rozmiar przez zmienne środowiskowe:
       LV_SDL_WIDTH / LV_SDL_HEIGHT (np. 800x480) */
    const char *w_env = getenv("LV_SDL_WIDTH");
    const char *h_env = getenv("LV_SDL_HEIGHT");
    int32_t w = w_env ? atoi(w_env) : 800;
    int32_t h = h_env ? atoi(h_env) : 480;

    /* Tworzy okno SDL i renderer dla LVGL */
    extern lv_display_t * lv_sdl_window_create(int32_t width, int32_t height);
    lv_display_t *disp = lv_sdl_window_create(w, h);

    /* INPUT: SDL mouse (opcjonalnie klawiatura/ts) */
    extern lv_indev_t * lv_sdl_mouse_create(void);
    lv_indev_t *indev = lv_sdl_mouse_create();
    lv_indev_set_display(indev, disp);

    /* (opcjonalnie) klawiatura:
       extern lv_indev_t * lv_sdl_keyboard_create(void);
       lv_sdl_keyboard_create(); */

    ui_create();

    while (1) {
        lv_timer_handler();
        usleep(5000); /* ~5 ms */
    }
    return 0;
}
