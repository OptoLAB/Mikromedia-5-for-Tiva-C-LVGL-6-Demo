/*
 * Author: J. Bajic, 2022.
 */

#include "board_hw/misc/buzzer.h"
#include "board_hw/misc/sensors_adc.h"
#include "board_hw/accel/ADXL345.h"
#include "board_hw/touch/TSC2006.h"
#include "gui/sensors_win.h"
#include <stdio.h>

lv_obj_t *tabview_sensors;
lv_obj_t *txt_touch_data, *txt_bat_volts, *txt_pin;
lv_obj_t *txt_ext_temp, *txt_int_temp;
lv_obj_t *txt_ax, *txt_ay, *txt_az;
lv_obj_t * gauge_temp, *lmeter_pin;
lv_task_t *sensors_refresh_task;
lv_obj_t * chart_accel;
lv_obj_t * touch_arc;
lv_chart_series_t * ser_x, *ser_y, *ser_z;


extern uint8_t buzzer_audio_enable;
extern lv_font_t ni7seg_60;

static void tab_btns_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
    }
}
static void close_win_sensors_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_obj_t * win = lv_win_get_from_btn(btn);
        lv_task_del(sensors_refresh_task);
        lv_obj_del(win);
        lv_obj_del(touch_arc);
    }
}

static void lv_create_lmeter(lv_obj_t * par)
{
    /*Create a style for the line meter*/
    static lv_style_t style_lmeter;
    lv_style_copy(&style_lmeter, &lv_style_pretty_color);
    style_lmeter.line.width = 2;
    style_lmeter.line.color =  LV_COLOR_MAKE(0xE0, 0xE0, 0xE0);
    style_lmeter.body.main_color = LV_COLOR_GREEN;
    style_lmeter.body.grad_color = LV_COLOR_RED;
    style_lmeter.body.padding.left = 16;
    style_lmeter.line.width = 6;

    /*Create a line meter */
    lmeter_pin = lv_lmeter_create(par, NULL);
    lv_lmeter_set_range(lmeter_pin, 0, 99);
    lv_lmeter_set_value(lmeter_pin, 80);
    lv_lmeter_set_scale(lmeter_pin, 240, 31);
    lv_lmeter_set_style(lmeter_pin, LV_LMETER_STYLE_MAIN, &style_lmeter);
    lv_obj_set_size(lmeter_pin, 250, 250);
    lv_obj_set_pos(lmeter_pin,450,30);
}

static void lv_create_gauge(lv_obj_t * par)
{
    /*Create a style*/
    static lv_style_t style_gauge;
    lv_style_copy(&style_gauge, &lv_style_pretty_color);
    style_gauge.body.main_color = lv_color_hex3(0x666);     /*Line color at the beginning*/
    style_gauge.body.grad_color =  lv_color_hex3(0x666);    /*Line color at the end*/
    style_gauge.body.padding.left = 10;                      /*Scale line length*/
    style_gauge.body.padding.inner = 8 ;                    /*Scale label padding*/
    style_gauge.body.border.color = lv_color_hex3(0x333);   /*Needle middle circle color*/
    style_gauge.line.width = 4;
    style_gauge.text.color = lv_color_hex3(0x333);
    style_gauge.line.color = LV_COLOR_RED;                  /*Line color after the critical value*/

    /*Describe the color for the needles*/
    static lv_color_t needle_colors[2];
    needle_colors[0] = LV_COLOR_BLUE;
    needle_colors[1] = LV_COLOR_ORANGE;

    /*Create a gauge*/
    gauge_temp = lv_gauge_create(par, NULL);
    lv_gauge_set_style(gauge_temp, LV_GAUGE_STYLE_MAIN, &style_gauge);
    lv_gauge_set_range(gauge_temp,20,55);
    lv_gauge_set_critical_value(gauge_temp,45);
    lv_gauge_set_needle_count(gauge_temp, 2, needle_colors);
    lv_gauge_set_scale(gauge_temp, 240, 36, 8);
    lv_obj_set_pos(gauge_temp,100,30);
    lv_obj_set_size(gauge_temp, 250, 250);
    /*Set the values*/
    lv_gauge_set_value(gauge_temp, 0, 10);
    lv_gauge_set_value(gauge_temp, 1, 20);
}
static void lv_create_chart(lv_obj_t * par)
{
    static lv_style_t accel_style;
    lv_style_copy(&accel_style, &lv_style_plain);
    accel_style.body.main_color= LV_COLOR_MAKE(0xF0, 0xF0, 0xF0);
    accel_style.body.grad_color= LV_COLOR_MAKE(0xF0, 0xF0, 0xF0);
    accel_style.line.color=LV_COLOR_MAKE(0xB0, 0xB0, 0xB0);
    accel_style.body.border.color=LV_COLOR_GRAY;
    accel_style.body.border.width=4;

    chart_accel = lv_chart_create(par, NULL);
    lv_obj_set_pos(chart_accel, 20, 20);
    lv_obj_set_size(chart_accel, 600, 300);
    lv_chart_set_type(chart_accel, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart_accel, -50, 50);
    lv_chart_set_div_line_count(chart_accel, 5, 10);
    lv_chart_set_point_count(chart_accel,50);
    lv_chart_set_update_mode(chart_accel, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_series_opa(chart_accel, LV_OPA_70);
    lv_chart_set_series_width(chart_accel, 4);
    lv_label_set_style(chart_accel, LV_LABEL_STYLE_MAIN, &accel_style);

    ser_x = lv_chart_add_series(chart_accel, LV_COLOR_RED);
    ser_y = lv_chart_add_series(chart_accel, LV_COLOR_GREEN);
    ser_z = lv_chart_add_series(chart_accel, LV_COLOR_BLUE);
}

void lv_create_win_sensors(void)
{
    /*Create a window*/
    lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "Sensors");

    /*Add control button to the header*/
    lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, close_win_sensors_cb);

    tabview_sensors = lv_tabview_create(win, NULL);
    lv_tabview_set_btns_pos(tabview_sensors, LV_TABVIEW_BTNS_POS_BOTTOM);
    lv_obj_t *tab_adc = lv_tabview_add_tab(tabview_sensors, "ADC");
    lv_obj_t *tab_accel = lv_tabview_add_tab(tabview_sensors, "Accel");
    lv_obj_t *tab_touch = lv_tabview_add_tab(tabview_sensors, "Touch");

    lv_obj_set_event_cb(tabview_sensors, tab_btns_event_cb);

    static lv_style_t style_txt_ext;
    lv_style_copy(&style_txt_ext, &lv_style_pretty_color);
    style_txt_ext.text.color=LV_COLOR_BLUE;
    txt_ext_temp = lv_label_create(tab_adc, NULL);
    lv_obj_set_pos(txt_ext_temp, 100, 250);
    lv_label_set_text(txt_ext_temp, "");
    lv_label_set_style(txt_ext_temp, LV_LABEL_STYLE_MAIN, &style_txt_ext);

    static lv_style_t style_txt_int;
    lv_style_copy(&style_txt_int, &lv_style_pretty_color);
    style_txt_int.text.color=LV_COLOR_ORANGE;
    txt_int_temp = lv_label_create(tab_adc, NULL);
    lv_obj_set_pos(txt_int_temp, 100, 280);
    lv_label_set_text(txt_int_temp, "");
    lv_label_set_style(txt_int_temp, LV_LABEL_STYLE_MAIN, &style_txt_int);

    txt_bat_volts = lv_label_create(tab_adc, NULL);
    lv_obj_set_pos(txt_bat_volts, 460, 280);
    lv_label_set_text(txt_bat_volts, "");

    static lv_style_t ni7seg_style;
    lv_style_copy(&ni7seg_style, &lv_style_plain);
    ni7seg_style.text.color = LV_COLOR_RED;
    ni7seg_style.text.font=&ni7seg_60;
    txt_pin = lv_label_create(tab_adc, NULL);
    lv_obj_set_pos(txt_pin,530,120);
    lv_label_set_text(txt_pin, "");
    lv_label_set_style(txt_pin, LV_LABEL_STYLE_MAIN, &ni7seg_style);

    lv_create_gauge(tab_adc);
    lv_create_lmeter(tab_adc);

    lv_create_chart(tab_accel);

    static lv_style_t style_txt_ax;
    lv_style_copy(&style_txt_ax, &lv_style_pretty_color);
    style_txt_ax.text.color=LV_COLOR_RED;
    txt_ax = lv_label_create(tab_accel, NULL);
    lv_obj_set_pos(txt_ax, 640, 40);
    lv_label_set_text(txt_ax, "");
    lv_label_set_style(txt_ax, LV_LABEL_STYLE_MAIN, &style_txt_ax);

    static lv_style_t style_txt_ay;
    lv_style_copy(&style_txt_ay, &lv_style_pretty_color);
    style_txt_ay.text.color=LV_COLOR_GREEN;
    txt_ay = lv_label_create(tab_accel, NULL);
    lv_obj_set_pos(txt_ay, 640, 70);
    lv_label_set_text(txt_ay, "");
    lv_label_set_style(txt_ay, LV_LABEL_STYLE_MAIN, &style_txt_ay);

    static lv_style_t style_txt_az;
    lv_style_copy(&style_txt_az, &lv_style_pretty_color);
    style_txt_az.text.color=LV_COLOR_BLUE;
    txt_az = lv_label_create(tab_accel, NULL);
    lv_obj_set_pos(txt_az, 640, 100);
    lv_label_set_text(txt_az, "");
    lv_label_set_style(txt_az, LV_LABEL_STYLE_MAIN, &style_txt_az);

    txt_touch_data = lv_label_create(tab_touch, NULL);
    lv_obj_set_pos(txt_touch_data, 10, 10);
    lv_label_set_text(txt_touch_data, "");


    static lv_style_t style_arc;
    lv_style_copy(&style_arc, &lv_style_plain);
    style_arc.line.color = LV_COLOR_RED;           /*Arc color*/
    style_arc.line.width = 5;                       /*Arc width*/
    touch_arc = lv_arc_create(lv_scr_act(), NULL);
    lv_arc_set_style(touch_arc, LV_ARC_STYLE_MAIN, &style_arc);          /*Use the new style*/
    lv_arc_set_angles(touch_arc, 90, 89);
    lv_obj_set_size(touch_arc, 20, 20);
    lv_obj_set_pos(touch_arc, 400, 240);
    lv_obj_set_hidden(touch_arc, true);

    sensors_refresh_task=lv_task_create((lv_task_cb_t) lv_update_win_sensors, 20, LV_TASK_PRIO_LOW, NULL);

}

void lv_update_win_sensors(void)
{
    char data_string[60];
    uint16_t ax,ay,az, pin;
    float bat, te, ti;
    switch(lv_tabview_get_tab_act(tabview_sensors))
    {
        case 0:
            ADCSensors_Read();
            pin=ADCSensors_getPINDiodeRaw()/41;
            bat=ADCSensors_getBatteryV();
            te=ADCSensors_getTempExternalC();
            ti=ADCSensors_getTempInternalC();

            lv_lmeter_set_value(lmeter_pin, pin);
            lv_gauge_set_value(gauge_temp, 0, (int16_t)te);
            lv_gauge_set_value(gauge_temp, 1, (int16_t)ti);

            sprintf(data_string,"Ext. temp: %d 'C",(int)te);
            lv_label_set_text(txt_ext_temp, data_string);
            sprintf(data_string,"CPU temp: %d 'C",(int)ti);
            lv_label_set_text(txt_int_temp, data_string);

            sprintf(data_string,"%d",(int)pin);
            lv_label_set_text(txt_pin, data_string);
            sprintf(data_string,"battery voltage: %d mV",(int)(bat*1000));
            lv_label_set_text(txt_bat_volts, data_string);
            lv_obj_set_hidden(touch_arc, true);
            break;
        case 1:
            ADXL345_ReadXYZ(&ax, &ay, &az);
            lv_chart_set_next(chart_accel, ser_x, (lv_coord_t)((int16_t)ax)/10);
            lv_chart_set_next(chart_accel, ser_y, (lv_coord_t)((int16_t)ay)/10);
            lv_chart_set_next(chart_accel, ser_z, (lv_coord_t)((int16_t)az)/10);
            sprintf(data_string,"ax: %d",(int16_t)ax);
            lv_label_set_text(txt_ax, data_string);
            sprintf(data_string,"ay: %d",(int16_t)ay);
            lv_label_set_text(txt_ay, data_string);
            sprintf(data_string,"az: %d",(int16_t)az);
            lv_label_set_text(txt_az, data_string);
            lv_obj_set_hidden(touch_arc, true);
            break;
        case 2:
            TSC2006_readXYCalibrated(&ax, &ay);
            sprintf(data_string,"x_raw: %d y_raw: %d   x_cal: %d y_cal: %d",TSC2006_readX(),TSC2006_readY(), ax, ay);
            lv_label_set_text(txt_touch_data, data_string);
            lv_obj_set_hidden(touch_arc, false);
            lv_obj_set_pos(touch_arc, ax-10, ay-10);
            break;
    }
}

