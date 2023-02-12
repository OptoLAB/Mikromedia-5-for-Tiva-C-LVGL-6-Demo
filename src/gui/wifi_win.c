/*
 * Author: J. Bajic, 2022.
 */

#include "board_hw/wifi/wifi_weather_demo.h"
#include "board_hw/misc/buzzer.h"
#include "board_hw/misc/rgb_led.h"
#include "gui/wifi_win.h"
#include "gui/obj/pswd_obj.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lv_obj_t * win;
lv_task_t * wifi_refresh_task;
lv_obj_t * txt_status, *txt_weather;
lv_obj_t * weather_img;
lv_obj_t * mbox_connect;

char ap_ssid[20]="MyAP";
char ap_pswd[20]="12345678";

char city[20]="N/A";
char temperature[6]="0";
char weather[20]="N/A";
char icon[5]="01d";
char weather_text[55]={};

uint8_t wifi_state=WIFI_DISCONNECTED;

extern const lv_img_dsc_t d01;
extern const lv_img_dsc_t d02;
extern const lv_img_dsc_t d03;
extern const lv_img_dsc_t d04;
extern const lv_img_dsc_t d09;
extern const lv_img_dsc_t d10;
extern const lv_img_dsc_t d11;
extern const lv_img_dsc_t d13;
extern const lv_img_dsc_t d50;
extern const lv_img_dsc_t n01;
extern const lv_img_dsc_t n02;
extern const lv_img_dsc_t n10;

extern uint8_t buzzer_audio_enable;

static void lv_create_pswd(lv_obj_t * par);
static void lv_create_connect_mbox(lv_obj_t * par);

static void btn_debug_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(wifi_state==WIFI_DISCONNECTED)
        {
            WIFI_Exec((_i8*)ap_ssid, (_i8*)ap_pswd);
        }
        else
        {
            lv_label_set_text(txt_status, "Status: Disconnect from AP first");
        }

    }
}
static void wifi_pswd_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CANCEL)
    {
        //nothing to do
    }
    if(event == LV_EVENT_APPLY)
    {
        lv_pswd_get_name(obj, ap_ssid);
        lv_pswd_get_passkey(obj, ap_pswd);
    }
}
static void connect_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        char *txt=(char *)lv_mbox_get_active_btn_text(obj);
        if(!strcmp(txt, "Connect"))
        {
            if(wifi_state==WIFI_DISCONNECTED)
            {
                wifi_state=WIFI_CONFIG;
                lv_mbox_set_text(mbox_connect, "Configuring...");
            }
            else
            {
                lv_mbox_set_text(mbox_connect, "Already connected");
            }
        }
        if(!strcmp(txt, "Cancel"))
        {
            lv_mbox_start_auto_close(mbox_connect, 0);
        }
    }
}

static void  btn_connect_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_create_connect_mbox(win);
    }
}
static void  btn_disconnect_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(wifi_state==WIFI_CONNECTED)
        {
            wifi_state=WIFI_DISCONNECT;
        }
    }
}
static void btn_ap_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_create_pswd(win);
    }
}

static void btn_refresh_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(wifi_state==WIFI_CONNECTED)
        {
            lv_label_set_text(txt_status, "Status: Acquiring data...");
            wifi_state=WIFI_GET_DATA;
        }
        else
        {
            lv_label_set_text(txt_status, "Status: Connect to AP first");
        }
    }
}

static void close_win_wifi_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_obj_t * win = lv_win_get_from_btn(btn);
        lv_task_del(wifi_refresh_task);
        lv_obj_del(win);
    }
}

static void updateWeatherIcon(char *icon)
{
    if(strcmp(icon,"01d")==0)lv_img_set_src(weather_img, &d01);
    else if(strcmp(icon,"01n")==0)lv_img_set_src(weather_img, &n01);
    else if(strcmp(icon,"02d")==0)lv_img_set_src(weather_img, &d02);
    else if(strcmp(icon,"02n")==0)lv_img_set_src(weather_img, &n02);
    else if(strcmp(icon,"10d")==0)lv_img_set_src(weather_img, &d10);
    else if(strcmp(icon,"10n")==0)lv_img_set_src(weather_img, &n10);
    else if((strcmp(icon,"03d")==0) || (strcmp(icon,"03n")==0))lv_img_set_src(weather_img, &d03);
    else if((strcmp(icon,"04d")==0) || (strcmp(icon,"04n")==0))lv_img_set_src(weather_img, &d04);
    else if((strcmp(icon,"09d")==0) || (strcmp(icon,"09n")==0))lv_img_set_src(weather_img, &d09);
    else if((strcmp(icon,"11d")==0) || (strcmp(icon,"11n")==0))lv_img_set_src(weather_img, &d11);
    else if((strcmp(icon,"13d")==0) || (strcmp(icon,"13n")==0))lv_img_set_src(weather_img, &d13);
    else if((strcmp(icon,"50d")==0) || (strcmp(icon,"50n")==0))lv_img_set_src(weather_img, &d50);
    else lv_img_set_src(weather_img, &d01);
    lv_obj_set_hidden(weather_img, false);
}

static void lv_create_pswd(lv_obj_t * par)
{
    lv_obj_t *wifi_pswd=lv_pswd_create(par);
    lv_obj_align(wifi_pswd, NULL, LV_ALIGN_CENTER, -140, -30);
    lv_obj_set_event_cb(wifi_pswd, wifi_pswd_event_cb);
}
static void lv_create_connect_mbox(lv_obj_t * par)
{
    static const char * btns[] ={"Connect", "Cancel", ""};

    mbox_connect = lv_mbox_create(par, NULL);
    lv_mbox_set_text(mbox_connect, "Connect to AP");
    lv_mbox_add_btns(mbox_connect, btns);
    lv_obj_set_width(mbox_connect, 350);
    lv_obj_set_event_cb(mbox_connect, connect_event_handler);
    lv_obj_align(mbox_connect, NULL, LV_ALIGN_CENTER, -140, 20); /*Align to the corner*/
}

void lv_create_win_wifi(void)
{
    /*Create a window*/
    win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "Wifi Weather Demo");

    /*Add control button to the header*/
    lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, close_win_wifi_cb);

    txt_weather = lv_label_create(win, NULL);
    lv_obj_align(txt_weather, NULL, LV_ALIGN_CENTER, -320, -110);
    lv_label_set_text(txt_weather, "none");

    txt_status = lv_label_create(win, NULL);
    lv_obj_align(txt_status, NULL, LV_ALIGN_CENTER, -320, -180);
    if(wifi_state==WIFI_DISCONNECTED)
        lv_label_set_text(txt_status, "Status: Disconnected");
    else
    {
        sprintf(weather_text,"Status: Connected to %s", ap_ssid);
        lv_label_set_text(txt_status, weather_text);
    }

    lv_obj_t * btn_debug = lv_btn_create(win, NULL);
    lv_obj_align(btn_debug, NULL, LV_ALIGN_CENTER, 280, 150);
    lv_obj_set_size(btn_debug, 140, 80);
    lv_obj_set_event_cb(btn_debug, btn_debug_event_cb);
    lv_obj_t * label_btn_debug = lv_label_create(btn_debug, NULL);
    lv_label_set_text(label_btn_debug, "Debug");

    weather_img = lv_img_create(win, NULL);
    lv_obj_align(weather_img, NULL, LV_ALIGN_CENTER, -140, -100);
    lv_img_set_src(weather_img, &d01);
    lv_obj_set_hidden(weather_img, true);
    updateWeatherIcon(icon);
    int temp=atoi(temperature);
    sprintf(weather_text,"%s\n%d 'C\n%s", city,temp,weather);
    lv_label_set_text(txt_weather, weather_text);

    lv_obj_t * btn_ap = lv_btn_create(win, NULL);
    lv_obj_align(btn_ap, NULL, LV_ALIGN_CENTER, -320, 150);
    lv_obj_set_size(btn_ap, 140, 80);
    lv_obj_set_event_cb(btn_ap, btn_ap_event_cb);
    lv_obj_t * label_btn_ap = lv_label_create(btn_ap, NULL);
    lv_label_set_text(label_btn_ap, "Setup AP");

    lv_obj_t * btn_connect = lv_btn_create(win, NULL);
    lv_obj_align(btn_connect, NULL, LV_ALIGN_CENTER, -160, 150);
    lv_obj_set_size(btn_connect, 140, 80);
    lv_obj_set_event_cb(btn_connect, btn_connect_event_cb);
    lv_obj_t * label_btn_connect = lv_label_create(btn_connect, NULL);
    lv_label_set_text(label_btn_connect, "Connect");

    lv_obj_t * btn_disconnect = lv_btn_create(win, NULL);
    lv_obj_align(btn_disconnect, NULL, LV_ALIGN_CENTER, 0, 150);
    lv_obj_set_size(btn_disconnect, 140, 80);
    lv_obj_set_event_cb(btn_disconnect, btn_disconnect_event_cb);
    lv_obj_t * label_btn_disconnect = lv_label_create(btn_disconnect, NULL);
    lv_label_set_text(label_btn_disconnect, "Disconnect");

    //debug btn
    lv_obj_t * btn_refresh = lv_btn_create(win, NULL);
    lv_obj_align(btn_refresh, NULL, LV_ALIGN_CENTER, 280, -150);
    lv_obj_set_size(btn_refresh, 140, 80);
    lv_obj_set_event_cb(btn_refresh, btn_refresh_event_cb);

    lv_obj_t *img_btn_refresh = lv_img_create(btn_refresh, NULL);
    lv_img_set_src(img_btn_refresh, LV_SYMBOL_REFRESH);

    lv_obj_t * label_btn_refresh = lv_label_create(btn_refresh, NULL);
    lv_label_set_text(label_btn_refresh, "Refresh");

    wifi_refresh_task=lv_task_create((lv_task_cb_t) lv_update_win_wifi, 100, LV_TASK_PRIO_LOW, NULL);

    WIFI_initializeAppVariables();
}

void lv_update_win_wifi(void)
{
    _i32 retVal = -1;

    switch(wifi_state)
    {
        case WIFI_DISCONNECTED://Disconnected
            //standby
            break;
        case WIFI_CONFIG: //Initializing
            retVal = WIFI_configureToDefaultState();
            if(retVal < 0)
            {
                lv_mbox_set_text(mbox_connect, "Failed at Config");
                wifi_state=WIFI_DISCONNECTED;
            }
            else
            {
                wifi_state=WIFI_START;
                lv_mbox_set_text(mbox_connect, "Starting...");
            }
            break;
        case WIFI_START://Starting device
            retVal = sl_Start(0, 0, 0);
            if ((retVal < 0) || (ROLE_STA != retVal) )
            {
                lv_mbox_set_text(mbox_connect, "Failed at Start");
                wifi_state=WIFI_DISCONNECTED;
            }
            else
            {
                wifi_state=WIFI_CONNECT;
                lv_mbox_set_text(mbox_connect, "Connecting...");
            }
            break;
        case WIFI_CONNECT://Connecting
            retVal = WIFI_establishConnectionWithAP((_i8*)ap_ssid, (_i8*)ap_pswd);
            if (retVal < 0)
            {
                lv_mbox_set_text(mbox_connect, "Failed at Connect");
                wifi_state=WIFI_DISCONNECTED;
            }
            else
            {
                wifi_state=WIFI_CONNECTED;
                lv_mbox_start_auto_close(mbox_connect, 0);
                sprintf(weather_text,"Status: Connected to %s", ap_ssid);
                lv_label_set_text(txt_status, weather_text);
            }
            break;
        case WIFI_CONNECTED: //Connected
            //standby
            break;
        case WIFI_GET_DATA://Getting data
            retVal = WIFI_getWeather();
            if(retVal < 0)
            {
                wifi_state=WIFI_CONNECTED;
                lv_label_set_text(txt_status, "Status: Failed at Data Get");
            }
            else
            {
                WIFI_extractWeatherData(city,temperature,weather,icon);
                int temp=atoi(temperature);
                sprintf(weather_text,"%s\n%d 'C\n%s", city,temp,weather);
                lv_label_set_text(txt_weather, weather_text);
                updateWeatherIcon(icon);
                wifi_state=WIFI_CONNECTED;
                sprintf(weather_text,"Status: Connected to %s", ap_ssid);
                lv_label_set_text(txt_status, weather_text);
            }

            break;
        case WIFI_DISCONNECT://Disconnecting
            retVal = WIFI_disconnectFromAP();
            if (retVal < 0)
            {
                lv_label_set_text(txt_status, "Status: Failed at Disconnect");
                wifi_state=WIFI_DISCONNECTED;
            }
            else
            {
                wifi_state=WIFI_DISCONNECTED;
                sl_Stop(0xFF);
                lv_label_set_text(txt_status, "Status: Disconnected");
            }
            break;

    }
}


