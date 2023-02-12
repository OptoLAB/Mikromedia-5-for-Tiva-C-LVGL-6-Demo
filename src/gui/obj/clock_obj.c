/*
 * Author: J. Bajic, 2022.
 */
#include "board_hw/misc/buzzer.h"
#include "gui/obj/clock_obj.h"
#include <stdio.h>

extern uint8_t buzzer_audio_enable;
extern lv_font_t ni7seg_36;


static void clock_sec_min_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {

        if(buzzer_audio_enable)BUZZER_ClickSound();

        if(lv_spinbox_get_value(obj)==59)
        {
            lv_spinbox_set_value(obj,0);
        }

        else
            lv_spinbox_increment(obj);
    }
}

static void clock_hour_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {

        if(buzzer_audio_enable)BUZZER_ClickSound();

        if(lv_spinbox_get_value(obj)==23)
        {
            lv_spinbox_set_value(obj,0);
        }
        else
            lv_spinbox_increment(obj);
    }
}

lv_obj_t *lv_clock_create(lv_obj_t *par)
{
    static lv_style_t style_sb;
    lv_style_copy(&style_sb, &lv_style_plain);
    style_sb.body.border.width = 0;
    style_sb.body.opa = 0;

    static lv_style_t clock_style;
    lv_style_copy(&clock_style, &lv_style_pretty);
    clock_style.text.font = &ni7seg_36;

    lv_obj_t * page =lv_page_create(par, NULL);
    lv_clock_ext_t * ext = lv_obj_allocate_ext_attr(page, sizeof(lv_clock_ext_t));

    lv_obj_set_pos(page,0,0);
    lv_obj_set_width(page, 230);
    lv_obj_set_height(page, 72);
    lv_page_set_style(page, LV_PAGE_STYLE_BG, &style_sb);

    ext->hour = lv_spinbox_create(page, NULL);
    lv_spinbox_set_range(ext->hour,0,23);
    lv_spinbox_set_digit_format(ext->hour, 2, 0);
    lv_obj_set_width(ext->hour, 63);
    lv_obj_set_pos(ext->hour,10,10);
    lv_obj_set_event_cb(ext->hour, clock_hour_event_handler);
    lv_spinbox_set_style(ext->hour, LV_SPINBOX_STYLE_BG, &clock_style);

    ext->min = lv_spinbox_create(page, NULL);
    lv_spinbox_set_range(ext->min,0,59);
    lv_spinbox_set_digit_format(ext->min, 2, 0);
    lv_obj_set_width(ext->min, 63);
    lv_obj_set_pos(ext->min,63+10+5,10);
    lv_obj_set_event_cb(ext->min, clock_sec_min_event_handler);
    lv_spinbox_set_style(ext->min, LV_SPINBOX_STYLE_BG, &clock_style);

    ext->sec = lv_spinbox_create(page, NULL);
    lv_spinbox_set_range(ext->sec,0,59);
    lv_spinbox_set_digit_format(ext->sec, 2, 0);
    lv_obj_set_width(ext->sec, 63);
    lv_obj_set_pos(ext->sec,126+10+10,10);
    lv_obj_set_event_cb(ext->sec, clock_sec_min_event_handler);
    lv_spinbox_set_style(ext->sec, LV_SPINBOX_STYLE_BG, &clock_style);

    lv_obj_t * sep1 = lv_label_create(page, NULL);
    lv_label_set_text(sep1, ":");
    lv_obj_align(sep1, ext->min, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_t * sep2 = lv_label_create(page, NULL);
    lv_label_set_text(sep2, ":");
    lv_obj_align(sep2, ext->min, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_ta_set_cursor_type(ext->hour, LV_CURSOR_NONE);
    lv_ta_set_cursor_type(ext->min, LV_CURSOR_NONE);
    lv_ta_set_cursor_type(ext->sec, LV_CURSOR_NONE);

    lv_spinbox_set_value(ext->hour,0);
    lv_spinbox_set_value(ext->min,0);
    lv_spinbox_set_value(ext->sec,0);

    return page;
}
void lv_clock_set_time(lv_obj_t * clock, lv_clock_time_t *time)
{
    lv_clock_ext_t * ext = (lv_clock_ext_t *)lv_obj_get_ext_attr(clock);

    lv_spinbox_set_value(ext->hour,time->hour);
    lv_spinbox_set_value(ext->min,time->min);
    lv_spinbox_set_value(ext->sec,time->sec);
}

lv_clock_time_t lv_clock_get_time(lv_obj_t * clock)
{
    lv_clock_time_t time;

    lv_clock_ext_t * ext = (lv_clock_ext_t *)lv_obj_get_ext_attr(clock);

    time.hour=lv_spinbox_get_value(ext->hour);
    time.min=lv_spinbox_get_value(ext->min);
    time.sec=lv_spinbox_get_value(ext->sec);

    return time;
}




