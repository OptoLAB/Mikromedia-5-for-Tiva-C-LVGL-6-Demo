/*
 * Author: J. Bajic, 2022.
 */

#include "gui/debug_win.h"
#include "board_hw/board_hw.h"
#include "board_hw/misc/buzzer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

lv_task_t *debug_refresh_task;
lv_obj_t *txt_comm;
lv_obj_t *led_red, *led_green, *led_blue;
lv_obj_t *sw_rled, *sw_gled, *sw_bled;
lv_obj_t *img_debug;
lv_obj_t * cb_comm;

JSONobj_t ADC[3]={{"ADC0",0},{"ADC1",0},{"ADC2",0}};
JSONobj_t LED[3]={{"RLED",0},{"GLED",0},{"BLED",0}};
JSONobj_t ACCEL[3]={{"ax",0},{"ay",0},{"az",0}};
JSONobj_t menu={"tab",0};
uint8_t delay_count=0;
extern uint8_t buzzer_audio_enable;
extern const lv_img_dsc_t nRF_debug;
extern const lv_img_dsc_t usb_debug;

static void close_win_debug_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_obj_t * win = lv_win_get_from_btn(btn);
        lv_task_del(debug_refresh_task);
        lv_obj_del(win);
    }
}
static void cb_comm_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(lv_cb_is_checked(obj))
        {
            lv_cb_set_text(cb_comm, "nRF24");
            JSON_SetPort(RXTX_NRF);
            lv_img_set_src(img_debug, &nRF_debug);
        }
        else
        {
            lv_cb_set_text(cb_comm, "USB");
            JSON_SetPort(RXTX_USB);
            lv_img_set_src(img_debug, &usb_debug);
        }
    }
}

static void sw_rled_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(lv_sw_get_state(obj))LED[0].value=1;
        else LED[0].value=0;
    }
}
static void sw_gled_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(lv_sw_get_state(obj))LED[1].value=1;
        else LED[1].value=0;
    }
}
static void sw_bled_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(lv_sw_get_state(obj))LED[2].value=1;
        else LED[2].value=0;
    }
}
static void lv_create_switches(lv_obj_t * par)
{
    /*Create styles for the switch*/
    static lv_style_t bg_style;
    static lv_style_t indic_style;
    static lv_style_t knob_on_style;
    static lv_style_t knob_off_style;

    lv_style_copy(&bg_style, &lv_style_pretty);
    bg_style.body.radius = LV_RADIUS_CIRCLE;
    bg_style.body.padding.top = 6;
    bg_style.body.padding.bottom = 6;

    lv_style_copy(&indic_style, &lv_style_pretty_color);
    indic_style.body.radius = LV_RADIUS_CIRCLE;
    indic_style.body.main_color = lv_color_hex(0x9fc8ef);
    indic_style.body.grad_color = lv_color_hex(0x9fc8ef);
    indic_style.body.padding.left = 0;
    indic_style.body.padding.right = 0;
    indic_style.body.padding.top = 0;
    indic_style.body.padding.bottom = 0;

    lv_style_copy(&knob_off_style, &lv_style_pretty);
    knob_off_style.body.radius = LV_RADIUS_CIRCLE;
    knob_off_style.body.shadow.width = 4;
    knob_off_style.body.shadow.type = LV_SHADOW_BOTTOM;

    lv_style_copy(&knob_on_style, &lv_style_pretty_color);
    knob_on_style.body.radius = LV_RADIUS_CIRCLE;
    knob_on_style.body.shadow.width = 4;
    knob_on_style.body.shadow.type = LV_SHADOW_BOTTOM;

    /*Create a switch and apply the styles*/
    sw_rled = lv_sw_create(par, NULL);
    lv_sw_set_style(sw_rled, LV_SW_STYLE_BG, &bg_style);
    lv_sw_set_style(sw_rled, LV_SW_STYLE_INDIC, &indic_style);
    lv_sw_set_style(sw_rled, LV_SW_STYLE_KNOB_ON, &knob_on_style);
    lv_sw_set_style(sw_rled, LV_SW_STYLE_KNOB_OFF, &knob_off_style);
    lv_obj_align(sw_rled, NULL, LV_ALIGN_CENTER, -200, 70);
    lv_obj_set_event_cb(sw_rled, sw_rled_event_handler);

    sw_gled = lv_sw_create(par, sw_rled);
    lv_obj_align(sw_gled, NULL, LV_ALIGN_CENTER, 0, 70);
    lv_obj_set_event_cb(sw_gled, sw_gled_event_handler);

    sw_bled = lv_sw_create(par, sw_rled);
    lv_obj_align(sw_bled, NULL, LV_ALIGN_CENTER, 200, 70);
    lv_obj_set_event_cb(sw_bled, sw_bled_event_handler);
}

static void lv_create_leds(lv_obj_t * par)
{
    //Red LED
    static lv_style_t style_led_red;
    lv_style_copy(&style_led_red, &lv_style_pretty_color);
    style_led_red.body.radius = LV_RADIUS_CIRCLE;
    style_led_red.body.main_color = LV_COLOR_MAKE(0xb5, 0x0f, 0x04);
    style_led_red.body.grad_color = LV_COLOR_MAKE(0x50, 0x07, 0x02);
    style_led_red.body.border.color = LV_COLOR_MAKE(0xfa, 0x0f, 0x00);
    style_led_red.body.border.width = 3;
    style_led_red.body.border.opa = LV_OPA_30;
    style_led_red.body.shadow.color = LV_COLOR_MAKE(0xb5, 0x0f, 0x04);
    style_led_red.body.shadow.width = 5;

    led_red  = lv_led_create(par, NULL);
    lv_led_set_style(led_red, LV_LED_STYLE_MAIN, &style_led_red);
    lv_obj_align(led_red, NULL, LV_ALIGN_CENTER, -200, 0);
    lv_led_off(led_red);

    //Green LED
    static lv_style_t style_led_green;
    lv_style_copy(&style_led_green, &style_led_red);
    style_led_green.body.main_color = LV_COLOR_MAKE(0x04, 0xb5, 0x0f);
    style_led_green.body.grad_color = LV_COLOR_MAKE(0x02, 0x50, 0x07);
    style_led_green.body.border.color = LV_COLOR_MAKE(00, 0xfa, 0x0f);
    style_led_green.body.shadow.color = LV_COLOR_MAKE(0x04, 0xb5, 0x0f);

    led_green = lv_led_create(par, NULL);
    lv_led_set_style(led_green, LV_LED_STYLE_MAIN, &style_led_green);
    lv_obj_align(led_green, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_led_off(led_green);

    //Blue LED
    static lv_style_t style_led_blue;
    lv_style_copy(&style_led_blue, &style_led_red);
    style_led_blue.body.main_color = LV_COLOR_MAKE(0x04, 0x0f, 0xb5);
    style_led_blue.body.grad_color = LV_COLOR_MAKE(0x02, 0x07, 0x50);
    style_led_blue.body.border.color = LV_COLOR_MAKE(00, 0x0f, 0xfa);
    style_led_blue.body.shadow.color = LV_COLOR_MAKE(0x04, 0x0f, 0xb5);

    led_blue  = lv_led_create(par, NULL);
    lv_led_set_style(led_blue, LV_LED_STYLE_MAIN, &style_led_blue);
    lv_obj_align(led_blue, NULL, LV_ALIGN_CENTER, 200, 0);
    lv_led_on(led_blue);
}

void lv_create_win_debug(void)
{
    /*Create a window*/
    lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "Debug (Serial or nRF24)");

    /*Add control button to the header*/
    lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, close_win_debug_cb);


    /*Add some dummy content*/
    lv_obj_t * txt = lv_label_create(win, NULL);
    lv_label_set_text(txt, "Connect to PC via USB connector marked ICDI of nRF24 wireless module\n"
            "and run TI GUI Composer app\n\n"
            "Select communication port:");

    cb_comm = lv_cb_create(win, NULL);
    lv_cb_set_text(cb_comm, "USB");
    lv_obj_align(cb_comm, NULL, LV_ALIGN_CENTER, -80, -108);
    lv_obj_set_event_cb(cb_comm, cb_comm_event_handler);

    lv_create_leds(win);
    lv_create_switches(win);

    static lv_style_t debug_img_style;
    lv_style_copy(&debug_img_style, &lv_style_plain);
    debug_img_style.image.color = LV_COLOR_ORANGE;//LV_COLOR_MAKE(0x26, 0x80, 0xBA);
    img_debug = lv_img_create(win, NULL);
    lv_obj_align(img_debug, NULL, LV_ALIGN_CENTER, 150, -125);
    lv_img_set_src(img_debug, &usb_debug);
    lv_img_set_style(img_debug, LV_IMG_STYLE_MAIN, &debug_img_style);

    debug_refresh_task=lv_task_create((lv_task_cb_t) lv_update_win_debug, 20, LV_TASK_PRIO_LOW, NULL);
}

void lv_update_win_debug(void)
{
    JSONobj_t temp={"none",0};

    ADCSensors_Read();
    ADC[0].value=ADCSensors_getPINDiodeV()*100;
    ADC[1].value=ADCSensors_getTempExternalC()*100;
    ADC[2].value=ADCSensors_getTempInternalC()*100;
    LEDs_Set(RLED,LED[0].value);
    LEDs_Set(GLED,LED[1].value);
    LEDs_Set(BLED,LED[2].value);
    if(LED[0].value)
    {
        lv_led_on(led_red);
        lv_sw_on(sw_rled,LV_ANIM_OFF);
    }
    else
    {
        lv_led_off(led_red);
        lv_sw_off(sw_rled,LV_ANIM_OFF);
    }
    if(LED[1].value)
    {
        lv_led_on(led_green);
        lv_sw_on(sw_gled,LV_ANIM_OFF);
    }
    else
    {
        lv_led_off(led_green);
        lv_sw_off(sw_gled,LV_ANIM_OFF);
    }
    if(LED[2].value)
    {
        lv_led_on(led_blue);
        lv_sw_on(sw_bled,LV_ANIM_OFF);
    }
    else
    {
        lv_led_off(led_blue);
        lv_sw_off(sw_bled,LV_ANIM_OFF);
    }

    ACCEL[0].value=((int16_t)ADXL345_ReadX());
    ACCEL[1].value=((int16_t)ADXL345_ReadY());
    ACCEL[2].value=((int16_t)ADXL345_ReadZ());

    if(JSON_Read(&temp))
    {
       JSON_Write(&temp,1);
       if(!strcmp(temp.name,"RLED"))LED[0]=temp;
       if(!strcmp(temp.name,"GLED"))LED[1]=temp;
       if(!strcmp(temp.name,"BLED"))LED[2]=temp;
       if(!strcmp(temp.name,"tab"))
       {
           menu=temp;
           JSON_Write(&menu,1);
       }
    }

    switch(menu.value)
    {
        case 0:
            JSON_Write(ADC,3);
            JSON_Write(LED,3);
            break;
        case 1:
            JSON_Write(ACCEL,3);
            break;
        case 2:
            delay_count++;
            if(delay_count>=10) //slow down
            {
                JSON_Write(&temp,1);
                JSON_Write(ADC,3);
                JSON_Write(LED,3);
                JSON_Write(ACCEL,3);
                delay_count=0;
            }
            break;
    }

}
