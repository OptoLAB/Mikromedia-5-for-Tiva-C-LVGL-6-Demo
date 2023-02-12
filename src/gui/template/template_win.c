/*
 * Author: J. Bajic, 2022.
 */
#include "board_hw/misc/buzzer.h"
#include "gui/template/template_win.h"
#include <stdio.h>

lv_task_t *_refresh_task;

extern uint8_t buzzer_audio_enable;

static void close_win__cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_obj_t * win = lv_win_get_from_btn(btn);
        lv_task_del(_refresh_task);
        lv_obj_del(win);

    }
}

void lv_create_win_(void)
{
    /*Create a window*/
    lv_obj_t *win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "Window name");

    /*Add control button to the header*/
    lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, close_win__cb);

    _refresh_task=lv_task_create((lv_task_cb_t) lv_update_win_, 100, LV_TASK_PRIO_LOW, NULL);
}

void lv_update_win_(void)
{

}
