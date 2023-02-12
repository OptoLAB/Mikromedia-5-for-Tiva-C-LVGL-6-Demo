/*
 * Author: J. Bajic, 2022.
 */

#include "board_hw/mp3/MP3.h"
#include "board_hw/mp3/VS1053.h"
#include "board_hw/misc/rgb_led.h"
#include "board_hw/misc/buzzer.h"
#include "board_hw/tivaware_c_lib.h"
#include "fatfs/ff.h"
#include "gui/mp3_win.h"
#include <stdio.h>

lv_obj_t *bar_playing_time, *txt_playing_time;
lv_obj_t *slider_volume, *txt_volume;
lv_task_t *mp3_refresh_task;
lv_obj_t *txt_mp3;
lv_obj_t *selected_song;

FRESULT fr;
DIR dj;
FILINFO fno;
char song_path[300];
char song_folder[20]="music";

uint8_t play=0;
uint32_t playing_time=1;
int16_t mp3_vol=50;

extern uint8_t buzzer_audio_enable;
extern const lv_img_dsc_t music;


uint8_t num_of_songs=0;
uint8_t max_num_of_songs=255;
uint8_t max_song_name_len=255;
lv_obj_t * list_playlist;


static void find_next(void)
{
    lv_obj_t *next= lv_list_get_next_btn(list_playlist, selected_song);
    if(next)
    {
        sprintf(song_path,"%s/%s",song_folder,(char*)lv_list_get_btn_text(next));
        lv_label_set_text(txt_mp3, song_path);

        lv_obj_t *song_label=lv_list_get_btn_label(selected_song);
        lv_label_set_long_mode(song_label, LV_LABEL_LONG_CROP); //disable rolling
        song_label=lv_list_get_btn_label(next);
        lv_label_set_long_mode(song_label, LV_LABEL_LONG_SROLL_CIRC); //enable rolling

        selected_song=next;
        lv_list_set_btn_selected(list_playlist,selected_song);
    }
}
static void find_prev(void)
{
    lv_obj_t *prev= lv_list_get_prev_btn(list_playlist, selected_song);
    if(prev)
    {
        sprintf(song_path,"%s/%s",song_folder,(char*)lv_list_get_btn_text(prev));
        lv_label_set_text(txt_mp3, song_path);

        lv_obj_t *song_label=lv_list_get_btn_label(selected_song);
        lv_label_set_long_mode(song_label, LV_LABEL_LONG_CROP); //disable rolling
        song_label=lv_list_get_btn_label(prev);
        lv_label_set_long_mode(song_label, LV_LABEL_LONG_SROLL_CIRC); //enable rolling

        selected_song=prev;
        lv_list_set_btn_selected(list_playlist,selected_song);
    }
}

static void playlist_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_PRESSING)
    {
        lv_list_set_btn_selected(list_playlist, obj);
    }
    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();

        lv_list_set_btn_selected(list_playlist, obj);
        sprintf(song_path,"%s/%s",song_folder,(char*)lv_list_get_btn_text(obj));
        lv_label_set_text(txt_mp3, song_path);

        lv_obj_t *song_label=lv_list_get_btn_label(selected_song);
        lv_label_set_long_mode(song_label, LV_LABEL_LONG_CROP); //disable rolling
        song_label=lv_list_get_btn_label(obj);
        lv_label_set_long_mode(song_label, LV_LABEL_LONG_SROLL_CIRC); //enable rolling

        selected_song=obj;

    }
}
static void slider_volume_event_cb(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
       char data_string[20]={};
       sprintf(data_string,"Volume %d%%", lv_slider_get_value(slider));
       lv_label_set_text(txt_volume, data_string);
       mp3_vol=100-(uint8_t)lv_slider_get_value(slider);
       MP3_SetVolume(mp3_vol);
    }
    if(event == LV_EVENT_PRESSED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
    }
}

static void btn_play_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        play=1;
        MP3_Play(song_path);
        playing_time=MP3_GetPlayingTime();
    }
}

static void btn_next_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        find_next();
        MP3_Close();
        MP3_Play(song_path);

    }
}
static void btn_prev_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        find_prev();
        MP3_Close();
        MP3_Play(song_path);
        playing_time=MP3_GetPlayingTime();
    }
}
static void btn_pause_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        play=0;
    }
}
static void btn_stop_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        play=0;
        lv_bar_set_value(bar_playing_time, 0, LV_ANIM_OFF);
        lv_label_set_text(txt_playing_time, "0%");
        MP3_Close();

    }
}
static void close_win_mp3_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        MP3_Close();
        MP3_UnmountSD();
        lv_obj_t * win = lv_win_get_from_btn(btn);
        lv_task_del(mp3_refresh_task);
        lv_obj_del(win);
    }
}

static void populate_playlist(void)
{
    // scan and search for MP3 files - copy files names to playlist items
    num_of_songs=0;
    lv_obj_t * new_song;
    fr = f_findfirst(&dj, &fno, "music", "*.mp3");
    while (fr == FR_OK && fno.fname[0] && num_of_songs<max_num_of_songs)
    {
        unsigned char song_len=strlen(fno.fname);
        if(song_len<max_song_name_len)
        {
            new_song = lv_list_add_btn(list_playlist, NULL, fno.fname);
            lv_obj_t *song_label=lv_list_get_btn_label(new_song);
            lv_label_set_long_mode(song_label, LV_LABEL_LONG_CROP); //disable rolling
            lv_obj_set_event_cb(new_song, playlist_event_handler);

            if(!num_of_songs)
            {
                sprintf(song_path,"%s/%s",song_folder,(char*)lv_list_get_btn_text(new_song));
                selected_song=new_song;
                lv_label_set_text(txt_mp3, song_path);
                lv_list_set_btn_selected(list_playlist,new_song);
            }
            num_of_songs++;

        }
        fr = f_findnext(&dj, &fno);
    }
    f_closedir(&dj);
}
static void lv_create_playlist(lv_obj_t * par)
{
    /*Create a list*/
    list_playlist = lv_list_create(par, NULL);
    lv_obj_set_size(list_playlist, 350, 320);
    lv_obj_set_pos(list_playlist, 420, 80);
    lv_list_set_single_mode(list_playlist,true);
    lv_list_set_scroll_propagation(list_playlist,false);
    lv_list_set_anim_time(list_playlist, 1);

    static lv_style_t style_sb_pr;
    lv_style_copy(&style_sb_pr, &lv_style_plain);
    style_sb_pr.body.main_color=LV_COLOR_MAKE(0x26, 0x80, 0xBA);
    style_sb_pr.body.grad_color=LV_COLOR_MAKE(0x26, 0x80, 0xBA);
    style_sb_pr.body.padding.top=0;
    style_sb_pr.body.padding.bottom=0;
    style_sb_pr.body.opa=100;
    style_sb_pr.text.font=&lv_font_roboto_16;

    static lv_style_t style_sb_rel;
    lv_style_copy(&style_sb_rel, &lv_style_plain);
    style_sb_rel.body.opa=0;
    style_sb_rel.body.padding.top=0;
    style_sb_rel.body.padding.bottom=0;
    style_sb_rel.text.font=&lv_font_roboto_16;

    static lv_style_t style_sb_srcl;
    lv_style_copy(&style_sb_srcl, &lv_style_plain);
    style_sb_srcl.body.main_color=LV_COLOR_MAKE(0xF0, 0xF0, 0xF0);
    style_sb_srcl.body.grad_color=LV_COLOR_MAKE(0xF0, 0xF0, 0xF0);
    style_sb_srcl.body.opa=80;

    lv_list_set_style(list_playlist, LV_LIST_STYLE_SCRL, &style_sb_srcl);
    lv_list_set_style(list_playlist, LV_LIST_STYLE_BTN_REL, &style_sb_rel);
    lv_list_set_style(list_playlist, LV_LIST_STYLE_BTN_PR, &style_sb_pr);
    lv_list_set_style(list_playlist, LV_LIST_STYLE_BTN_TGL_REL, &style_sb_rel);
    lv_list_set_style(list_playlist, LV_LIST_STYLE_BTN_TGL_PR, &style_sb_pr);

    lv_obj_t * txt_playlist = lv_label_create(par, NULL);
    lv_label_set_text(txt_playlist, "Playlist:");
    lv_obj_align(txt_playlist, list_playlist, LV_ALIGN_OUT_TOP_LEFT, 10, -5);
}

void lv_create_win_mp3(void)
{
    /*Create a window*/
    lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "MP3 player");

    /*Add control button to the header*/
    lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, close_win_mp3_cb);

    txt_mp3 = lv_label_create(win, NULL);
    lv_obj_align(txt_mp3, NULL, LV_ALIGN_CENTER, -335, -185);
    lv_label_set_text(txt_mp3, "Press button to play");

    lv_obj_t * btn_prev = lv_btn_create(win, NULL);
    lv_obj_align(btn_prev, NULL, LV_ALIGN_CENTER, -310, -100);
    lv_obj_set_size(btn_prev, 60, 50);
    lv_obj_set_event_cb(btn_prev, btn_prev_event_cb);
    lv_obj_t *img = lv_img_create(btn_prev, NULL);
    lv_img_set_src(img, LV_SYMBOL_PREV);

    lv_obj_t * btn_play = lv_btn_create(win, NULL);
    lv_obj_align(btn_play, NULL, LV_ALIGN_CENTER, -310+72, -100);
    lv_obj_set_size(btn_play, 60, 50);
    lv_obj_set_event_cb(btn_play, btn_play_event_cb);
    img = lv_img_create(btn_play, NULL);

    lv_img_set_src(img, LV_SYMBOL_PLAY);

    lv_obj_t * btn_pause = lv_btn_create(win, NULL);
    lv_obj_align(btn_pause, NULL, LV_ALIGN_CENTER, -310+144, -100);
    lv_obj_set_size(btn_pause, 60, 50);
    lv_obj_set_event_cb(btn_pause, btn_pause_event_cb);
    img = lv_img_create(btn_pause, NULL);
    lv_img_set_src(img, LV_SYMBOL_PAUSE);

    lv_obj_t * btn_stop = lv_btn_create(win, NULL);
    lv_obj_align(btn_stop, NULL, LV_ALIGN_CENTER, -310+216, -100);
    lv_obj_set_size(btn_stop, 60, 50);
    lv_obj_set_event_cb(btn_stop, btn_stop_event_cb);
    img = lv_img_create(btn_stop, NULL);
    lv_img_set_src(img, LV_SYMBOL_STOP);

    lv_obj_t * btn_next = lv_btn_create(win, NULL);
    lv_obj_align(btn_next, NULL, LV_ALIGN_CENTER, -310+288, -100);
    lv_obj_set_size(btn_next, 60, 50);
    lv_obj_set_event_cb(btn_next, btn_next_event_cb);
    img = lv_img_create(btn_next, NULL);
    lv_img_set_src(img, LV_SYMBOL_NEXT);

    lv_obj_t * slider_volume = lv_slider_create(win, NULL);
    lv_obj_set_pos(slider_volume, 40, 195);
    lv_obj_set_width(slider_volume, 340);
    lv_slider_set_value(slider_volume,50,LV_ANIM_OFF);
    lv_obj_set_event_cb(slider_volume, slider_volume_event_cb);
    txt_volume = lv_label_create(win, NULL);
    lv_label_set_text(txt_volume, "Volume 50%");
    lv_obj_set_auto_realign(slider_volume, true);
    lv_obj_align(txt_volume, slider_volume, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    txt_playing_time = lv_label_create(win, NULL);
    lv_label_set_text(txt_playing_time, "0%");
    bar_playing_time = lv_bar_create(win, NULL);
    lv_obj_set_size(bar_playing_time, 340, 30);
    lv_obj_set_pos(bar_playing_time,40,305);
    lv_obj_set_auto_realign(bar_playing_time, true);
    lv_obj_align(txt_playing_time, bar_playing_time, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    static lv_style_t music_style;
    lv_style_copy(&music_style, &lv_style_plain);
    music_style.image.color = LV_COLOR_MAKE(0x26, 0x80, 0xBA);
    lv_obj_t *img_music = lv_img_create(win, NULL);
    lv_obj_set_pos(img_music, 495, 140);
    lv_img_set_src(img_music, &music);
    lv_img_set_style(img_music, LV_IMG_STYLE_MAIN, &music_style);

    lv_create_playlist(win);

    mp3_refresh_task=lv_task_create((lv_task_cb_t) lv_update_win_mp3, 10, LV_TASK_PRIO_MID, NULL);

    MP3_MountSD();
    populate_playlist();
    MP3_SetVolume(mp3_vol);
    play=0;
    MP3_Close();
}

void lv_update_win_mp3(void)
{
    char data_string[20]={};

    if(play)
    {
        MP3_Play(song_path);
        float mp3_prog=(((float)MP3_GetPlayingPosition()/playing_time)*100);
        sprintf(data_string,"%d%%",(uint16_t)mp3_prog);
        lv_bar_set_value(bar_playing_time, (uint16_t)mp3_prog, LV_ANIM_OFF);
        lv_label_set_text(txt_playing_time, data_string);
        if(MP3_GetState()==FINISHED)
        {
            find_next();
            //play=0;
        }
    }
}

