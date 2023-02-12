/*
 * Author: J. Bajic, 2022.
 */

#include "board_hw/board_hw.h"

#include "gui/main_win.h"
#include "gui/sensors_win.h"
#include "gui/debug_win.h"
#include "gui/wifi_win.h"
#include "gui/mp3_win.h"
#include "gui/obj/clock_obj.h"
#include <stdio.h>

lv_obj_t *main_win;
lv_obj_t *txt_date, *txt_time;
lv_obj_t *pwr_stat, *bat_stat, *sd_stat, *no_bat, *wifi_stat;
lv_task_t *main_refresh_task;
lv_obj_t  *my_calendar, *my_clock;

uint8_t buzzer_audio_enable=1;
//5%  25%   50%    75%
uint16_t bat_level[4]={3600, 3750, 3840, 3980}; //for testing purpose

uint8_t calendar_visible=0;
uint8_t clock_visible=0;

extern lv_font_t ni7seg_36;
extern const lv_img_dsc_t lvgl_logo;
extern const lv_img_dsc_t ti_logo;
extern const lv_img_dsc_t mikroe_logo;
extern uint8_t wifi_state;

static void lv_create_calendar(lv_obj_t * par);
static void lv_create_clock(lv_obj_t * par);

void close_win_main(void)
{
    lv_task_del(main_refresh_task);
    lv_obj_del(main_win);
}

static void btn_sensors_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_create_win_sensors();
    }
}

static void btn_wifi_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_create_win_wifi();
    }
}
static void btn_mp3_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_create_win_mp3();
    }
}
static void btn_debug_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_create_win_debug();
    }
}
static void audio_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        buzzer_audio_enable^=0x01;
        if(buzzer_audio_enable)BUZZER_ClickSound();
    }
}

static void btn_txt_time_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(!clock_visible)
        {
            clock_visible=1;
            lv_create_clock(main_win);
        }
        else
        {
            struct tm rtc_time;
            SYSRTC_GetDateTime(&rtc_time);

            lv_clock_time_t clock_time=lv_clock_get_time(my_clock);

            rtc_time.tm_hour=(int)clock_time.hour;
            rtc_time.tm_min=(int)clock_time.min;
            rtc_time.tm_sec=(int)clock_time.sec;

            SYSRTC_SetDateTime(&rtc_time);

            lv_obj_del(my_clock);
            clock_visible=0;

        }
    }
}
static void btn_txt_date_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(!calendar_visible)
        {
            calendar_visible=1;
            lv_create_calendar(main_win);
        }
        else
        {
            struct tm rtc_time;
            SYSRTC_GetDateTime(&rtc_time);

            lv_calendar_date_t today=*lv_calendar_get_today_date(my_calendar);

            rtc_time.tm_year=today.year-2000+100;
            rtc_time.tm_mon=today.month;
            rtc_time.tm_mday=today.day;

            SYSRTC_SetDateTime(&rtc_time);

            lv_obj_del(my_calendar);
            calendar_visible=0;
        }
    }
}
static void calendar_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_calendar_date_t * date = lv_calendar_get_pressed_date(obj);
        if(date)
        {
            lv_calendar_set_today_date(obj, date);
        }
    }
}

static void lv_create_calendar(lv_obj_t * par)
{
    struct tm rtc;
    lv_calendar_date_t today;

    my_calendar = lv_calendar_create(par, NULL);
    lv_obj_set_size(my_calendar, 300, 230);
    lv_obj_set_pos(my_calendar, 495,5);
    lv_obj_set_event_cb(my_calendar, calendar_event_handler);

    /*Set the today*/
    SYSRTC_GetDateTime(&rtc);

    today.year = rtc.tm_year-100+2000;
    today.month = rtc.tm_mon;
    today.day = rtc.tm_mday;

    lv_calendar_set_today_date(my_calendar, &today);
    lv_calendar_set_showed_date(my_calendar, &today);

}

static void lv_create_clock(lv_obj_t * par)
{
    lv_clock_time_t clk_tm;
    struct tm rtc;

    my_clock =lv_clock_create(par);
    lv_obj_set_pos(my_clock, 565, 10);

    SYSRTC_GetDateTime(&rtc);

    clk_tm.hour=rtc.tm_hour;
    clk_tm.min=rtc.tm_min;
    clk_tm.sec=rtc.tm_sec;

    lv_clock_set_time(my_clock,&clk_tm);
}

void lv_create_win_main(void)
{
    /*Create a window*/
    main_win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(main_win, "mikromedia 5 for Tiva C");
    lv_win_ext_t * ext = (lv_win_ext_t *)lv_obj_get_ext_attr(main_win);

    static lv_style_t generic_header_style;
    lv_style_copy(&generic_header_style, &lv_style_plain);
    generic_header_style.image.color = LV_COLOR_WHITE;
    generic_header_style.text.color = LV_COLOR_WHITE;
    generic_header_style.body.main_color=LV_COLOR_MAKE(0x26, 0x80, 0xBA);
    generic_header_style.body.grad_color=LV_COLOR_MAKE(0x26, 0x80, 0xBA);

    lv_obj_t *header=ext->header;
    lv_cont_set_style(header, LV_CONT_STYLE_MAIN, &generic_header_style);

    lv_obj_t * title=ext->title;
    static lv_style_t title_style;
    lv_style_copy(&title_style, &generic_header_style);
    title_style.text.color = LV_COLOR_MAKE(0xFF, 0xEE, 0xA0);
    title_style.text.font=&lv_font_roboto_28;
    lv_obj_set_pos(title, 240, 10);
    lv_label_set_style(title, LV_LABEL_STYLE_MAIN, &title_style);

    pwr_stat = lv_img_create(header, NULL);
    lv_obj_set_pos(pwr_stat, 20, 15);
    lv_img_set_src(pwr_stat, LV_SYMBOL_CHARGE);

    bat_stat = lv_img_create(header, NULL);
    lv_obj_set_pos(bat_stat, 55, 15);
    lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_EMPTY);

    sd_stat = lv_img_create(header, NULL);
    lv_obj_set_pos(sd_stat, 93, 15);
    lv_img_set_src(sd_stat, LV_SYMBOL_SD_CARD);

    static lv_style_t no_bat_style;
    lv_style_copy(&no_bat_style, &lv_style_plain);
    no_bat_style.image.color = LV_COLOR_RED;
    no_bat = lv_img_create(header, NULL);
    lv_obj_set_pos(no_bat, 60, 15);
    lv_img_set_src(no_bat, LV_SYMBOL_CLOSE);
    lv_img_set_style(no_bat, LV_IMG_STYLE_MAIN, &no_bat_style);

    lv_obj_t * audio_stat = lv_imgbtn_create(header, NULL);
    lv_imgbtn_set_src(audio_stat, LV_BTN_STATE_REL, LV_SYMBOL_VOLUME_MAX);
    lv_imgbtn_set_src(audio_stat, LV_BTN_STATE_PR, LV_SYMBOL_VOLUME_MAX);
    lv_imgbtn_set_src(audio_stat, LV_BTN_STATE_TGL_REL, LV_SYMBOL_MUTE);
    lv_imgbtn_set_src(audio_stat, LV_BTN_STATE_TGL_PR, LV_SYMBOL_MUTE);
    lv_imgbtn_set_style(audio_stat, LV_BTN_STATE_REL, &generic_header_style);        /*Use the darker style in the pressed state*/
    lv_imgbtn_set_style(audio_stat, LV_BTN_STATE_TGL_REL, &generic_header_style);
    lv_obj_set_pos(audio_stat, 120, 15);
    lv_imgbtn_set_toggle(audio_stat, true);
    lv_obj_set_event_cb(audio_stat, audio_event_cb);

    wifi_stat = lv_img_create(header, NULL);
    lv_obj_set_pos(wifi_stat, 150, 15);
    lv_img_set_src(wifi_stat, LV_SYMBOL_WIFI);

    static lv_style_t click_txt_style_pr;
    lv_style_copy(&click_txt_style_pr, &lv_style_plain);
    click_txt_style_pr.body.main_color = LV_COLOR_WHITE;
    click_txt_style_pr.body.grad_color = LV_COLOR_WHITE;
    click_txt_style_pr.body.opa = 100;
    click_txt_style_pr.text.color=LV_COLOR_WHITE;
    static lv_style_t click_txt_style_rel;
    lv_style_copy(&click_txt_style_rel, &click_txt_style_pr);
    click_txt_style_rel.body.opa = 0;

    lv_obj_t * btn_txt_time= lv_btn_create(header, NULL);
    lv_obj_set_pos(btn_txt_time, 595, 15);
    lv_obj_set_height(btn_txt_time, 30);
    lv_btn_set_style(btn_txt_time, LV_BTN_STYLE_PR, &click_txt_style_pr);
    lv_btn_set_style(btn_txt_time, LV_BTN_STYLE_REL, &click_txt_style_rel);
    lv_btn_set_style(btn_txt_time, LV_BTN_STATE_TGL_REL, &click_txt_style_pr);        /*Use the darker style in the pressed state*/
    lv_btn_set_style(btn_txt_time, LV_BTN_STATE_TGL_PR, &click_txt_style_rel);
    lv_btn_set_toggle(btn_txt_time, true);
    lv_obj_set_event_cb(btn_txt_time, btn_txt_time_event_cb);

    txt_time = lv_label_create(btn_txt_time, NULL);
    lv_obj_align(txt_time, btn_txt_time, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(txt_time, "00:00:00");

    lv_obj_t * btn_txt_date= lv_btn_create(header, NULL);
    lv_obj_set_pos(btn_txt_date, 697, 15);
    lv_obj_set_height(btn_txt_date, 30);
    lv_btn_set_style(btn_txt_date, LV_BTN_STYLE_PR, &click_txt_style_pr);
    lv_btn_set_style(btn_txt_date, LV_BTN_STYLE_REL, &click_txt_style_rel);
    lv_btn_set_style(btn_txt_date, LV_BTN_STATE_TGL_REL, &click_txt_style_pr);        /*Use the darker style in the pressed state*/
    lv_btn_set_style(btn_txt_date, LV_BTN_STATE_TGL_PR, &click_txt_style_rel);
    lv_btn_set_toggle(btn_txt_date, true);
    lv_obj_set_event_cb(btn_txt_date, btn_txt_date_event_cb);

    txt_date = lv_label_create(btn_txt_date, NULL);
    lv_obj_align(txt_date, btn_txt_date, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(txt_date, "00/00/00");


    //sensors btn
    lv_obj_t * btn_sensors = lv_btn_create(main_win, NULL);
    lv_obj_align(btn_sensors, NULL, LV_ALIGN_CENTER, -304, -120);
    lv_obj_set_size(btn_sensors, 140, 80);
    lv_obj_set_event_cb(btn_sensors, btn_sensors_event_cb);

    lv_obj_t *img_btn_sensors = lv_img_create(btn_sensors, NULL);
    lv_img_set_src(img_btn_sensors, LV_SYMBOL_EYE_OPEN);

    lv_obj_t * label_btn_sensors = lv_label_create(btn_sensors, NULL);
    lv_label_set_text(label_btn_sensors, "Sensors");

    //wifi btn
    lv_obj_t * btn_wifi = lv_btn_create(main_win, NULL);
    lv_obj_align(btn_wifi, NULL, LV_ALIGN_CENTER, -106, -120);
    lv_obj_set_size(btn_wifi, 140, 80);
    lv_obj_set_event_cb(btn_wifi, btn_wifi_event_cb);

    lv_obj_t *img_btn_wifi = lv_img_create(btn_wifi, NULL);
    lv_img_set_src(img_btn_wifi, LV_SYMBOL_WIFI);

    lv_obj_t * label_btn_wifi = lv_label_create(btn_wifi, NULL);
    lv_label_set_text(label_btn_wifi, "WiFi");

    //mp3 btn
    lv_obj_t * btn_mp3 = lv_btn_create(main_win, NULL);
    lv_obj_align(btn_mp3, NULL, LV_ALIGN_CENTER, 82, -120);
    lv_obj_set_size(btn_mp3, 140, 80);
    lv_obj_set_event_cb(btn_mp3, btn_mp3_event_cb);

    lv_obj_t *img_btn_mp3 = lv_img_create(btn_mp3, NULL);
    lv_img_set_src(img_btn_mp3, LV_SYMBOL_AUDIO);

    lv_obj_t * label_btn_mp3 = lv_label_create(btn_mp3, NULL);
    lv_label_set_text(label_btn_mp3, "MP3");

    //debug btn
    lv_obj_t * btn_debug = lv_btn_create(main_win, NULL);
    lv_obj_align(btn_debug, NULL, LV_ALIGN_CENTER, 270, -120);
    lv_obj_set_size(btn_debug, 140, 80);
    lv_obj_set_event_cb(btn_debug, btn_debug_event_cb);

    lv_obj_t *img_btn_debug = lv_img_create(btn_debug, NULL);
    lv_img_set_src(img_btn_debug, LV_SYMBOL_USB);

    lv_obj_t * label_btn_debug = lv_label_create(btn_debug, NULL);
    lv_label_set_text(label_btn_debug, "Debug");

    main_refresh_task=lv_task_create((lv_task_cb_t) lv_update_win_main, 50, LV_TASK_PRIO_LOW, NULL);

    lv_obj_t *img_mikroe = lv_img_create(main_win, NULL);
    lv_obj_align(img_mikroe, NULL, LV_ALIGN_CENTER, -300, 150);
    lv_img_set_src(img_mikroe, &mikroe_logo);

    lv_obj_t *img_lvgl = lv_img_create(main_win, NULL);
    lv_obj_align(img_lvgl, NULL, LV_ALIGN_CENTER, -60, 150);
    lv_img_set_src(img_lvgl, &lvgl_logo);

    lv_obj_t *img_ti = lv_img_create(main_win, NULL);
    lv_obj_align(img_ti, NULL, LV_ALIGN_CENTER, 185, 150);
    lv_img_set_src(img_ti, &ti_logo);

}

void lv_update_win_main(void)
{
    char data_string[30];

    uint16_t bat_volts=0;

    switch(PWR_GetStatus(&bat_volts))
    {
        case PWR_DC:
            lv_obj_set_hidden(pwr_stat, false);
            lv_img_set_src(pwr_stat, LV_SYMBOL_POWER);
            lv_obj_set_hidden(no_bat, false);
            lv_obj_set_hidden(bat_stat, false);
            lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_EMPTY);
            break;
        case PWR_USB:
            lv_obj_set_hidden(pwr_stat, false);
            lv_img_set_src(pwr_stat, LV_SYMBOL_USB);
            lv_obj_set_hidden(no_bat, false);
            lv_obj_set_hidden(bat_stat, false);
            lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_EMPTY);
            break;
        case PWR_BAT:
            lv_obj_set_hidden(no_bat, true);
            lv_obj_set_hidden(pwr_stat, true);
            lv_obj_set_hidden(bat_stat, false);
            if(bat_volts<bat_level[0])lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_EMPTY);
            else if(bat_volts<bat_level[1])lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_1);
            else if(bat_volts<bat_level[2])lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_2);
            else if(bat_volts<bat_level[3])lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_3);
            else lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_FULL);
            break;
        case PWR_BAT_CHRG:
            lv_obj_set_hidden(no_bat, true);
            lv_obj_set_hidden(pwr_stat, false);
            lv_img_set_src(pwr_stat, LV_SYMBOL_CHARGE);
            lv_obj_set_hidden(bat_stat, false);
            if(bat_volts<(bat_level[0]+250))lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_EMPTY);
            else if(bat_volts<(bat_level[1]+250))lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_1);
            else if(bat_volts<(bat_level[2]+250))lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_2);
            else if(bat_volts<(bat_level[3]+250))lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_3);
            else lv_img_set_src(bat_stat, LV_SYMBOL_BATTERY_FULL);
            break;
        case PWR_BAT_FLT:
            lv_obj_set_hidden(no_bat, false);
            lv_obj_set_hidden(pwr_stat, false);
            lv_img_set_src(pwr_stat, LV_SYMBOL_WARNING);
            lv_obj_set_hidden(bat_stat, true);
            break;
    }
    if(SD_isPresent())lv_obj_set_hidden(sd_stat, false);
    else lv_obj_set_hidden(sd_stat, true);

    if(!wifi_state)lv_obj_set_hidden(wifi_stat, true);
    else lv_obj_set_hidden(wifi_stat, false);


     if(!clock_visible&&!calendar_visible)
     {

        struct tm rtc_time;
        if(!SYSRTC_GetDateTime(&rtc_time))
        {
            sprintf(data_string,"%02d:%02d:%02d",rtc_time.tm_hour,rtc_time.tm_min,rtc_time.tm_sec);
            lv_label_set_text(txt_time, data_string);
            sprintf(data_string,"%02d/%02d/%02d",rtc_time.tm_mday,rtc_time.tm_mon,rtc_time.tm_year%100);
            lv_label_set_text(txt_date, data_string);
        }

    }
}
