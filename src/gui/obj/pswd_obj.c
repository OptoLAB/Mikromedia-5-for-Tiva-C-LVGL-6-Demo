/*
 * Author: J. Bajic, 2022.
 */
#include "board_hw/misc/buzzer.h"
#include "gui/obj/pswd_obj.h"
#include <string.h>

extern uint8_t buzzer_audio_enable;

static void kb_event_cb(lv_obj_t * event_kb, lv_event_t event)
{
    lv_kb_def_event_cb(event_kb, event);
    lv_obj_t *page=lv_obj_get_parent(lv_obj_get_parent(event_kb));
    lv_pswd_ext_t * ext = (lv_pswd_ext_t *)lv_obj_get_ext_attr(page);

    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
    }
    if(event == LV_EVENT_CANCEL)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        lv_event_send(page, LV_EVENT_CANCEL, NULL);
        lv_obj_del(page);
    }
    if(event == LV_EVENT_APPLY)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        strcpy(ext->name,(char*)lv_ta_get_text(ext->ta_name));
        strcpy(ext->pass,(char*)lv_ta_get_text(ext->ta_pass));

        lv_event_send(page, LV_EVENT_APPLY, NULL);
        lv_obj_del(page);
    }

}
static void ta_event_cb(lv_obj_t * ta, lv_event_t event)
{
    lv_obj_t *page=lv_obj_get_parent(lv_obj_get_parent(ta));
    lv_pswd_ext_t * ext = (lv_pswd_ext_t *)lv_obj_get_ext_attr(page);

    if(event == LV_EVENT_CLICKED)
    {
        if(buzzer_audio_enable)BUZZER_ClickSound();
        if(ext->kb != NULL)
            lv_kb_set_ta(ext->kb, ta);
    }
}

lv_obj_t * lv_pswd_create(lv_obj_t * par)
{
    static lv_style_t style_sb;
    lv_style_copy(&style_sb, &lv_style_plain);
    style_sb.body.border.width = 0;
    style_sb.body.opa = 0;

    lv_obj_t * page =lv_page_create(par, NULL);
    lv_pswd_ext_t * ext = lv_obj_allocate_ext_attr(page, sizeof(lv_pswd_ext_t));

    lv_obj_set_size(page,410+60,255);
    lv_page_set_style(page, LV_PAGE_STYLE_BG, &style_sb);

    ext->name[0]=0;
    ext->pass[0]=0;
    /* Create the ssid box */
    ext->ta_name= lv_ta_create(page, NULL);
    lv_ta_set_text(ext->ta_name, "");
    lv_ta_set_pwd_mode(ext->ta_name, false);
    lv_ta_set_one_line(ext->ta_name, true);
    lv_obj_set_width(ext->ta_name, 190);
    lv_obj_set_pos(ext->ta_name, 5+10, 32);
    lv_obj_set_event_cb(ext->ta_name, ta_event_cb);
    /* Create a label and position it above the text box */
    ext->label_name = lv_label_create(page, NULL);
    lv_label_set_text(ext->label_name, "Name:");
    lv_obj_align(ext->label_name, ext->ta_name, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

    /* Create the password box */
    ext->ta_pass = lv_ta_create(page, NULL);
    lv_ta_set_text(ext->ta_pass, "");
    lv_ta_set_pwd_mode(ext->ta_pass, true);
    lv_ta_set_one_line(ext->ta_pass, true);
    lv_obj_set_width(ext->ta_pass, 190);
    lv_obj_set_pos(ext->ta_pass, 205+40, 32);
    lv_ta_set_cursor_type(ext->ta_pass, LV_CURSOR_LINE | LV_CURSOR_HIDDEN);
    lv_obj_set_event_cb(ext->ta_pass, ta_event_cb);
    //lv_obj_align(ta_ap_pswd, NULL, LV_ALIGN_IN_TOP_RIGHT, -5, 20);

    /* Create a label and position it above the text box */
    ext->label_pass = lv_label_create(page, NULL);
    lv_label_set_text(ext->label_pass, "Password:");
    lv_obj_align(ext->label_pass, ext->ta_pass, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

    /* Create a keyboard and make it fill the width of the above text areas */
    ext->kb = lv_kb_create(page, NULL);
    lv_obj_set_pos(ext->kb, 5, 75);
    lv_obj_set_event_cb(ext->kb, kb_event_cb); /* Setting a custom event handler stops the keyboard from closing automatically */
    lv_obj_set_size(ext->kb,  390+60, 160);

    lv_kb_set_ta(ext->kb, ext->ta_name); /* Focus it on one of the text areas to start */
    lv_kb_set_cursor_manage(ext->kb, true); /* Automatically show/hide cursors on text areas */

    return page;
}

void lv_pswd_get_name(lv_obj_t * pswd, char *name)
{
    lv_pswd_ext_t * ext = (lv_pswd_ext_t *)lv_obj_get_ext_attr(pswd);

    if(ext->name[0]!=0)
    {
        strcpy(name,ext->name);
    }
}
void lv_pswd_get_passkey(lv_obj_t * pswd, char *passkey)
{
    lv_pswd_ext_t * ext = (lv_pswd_ext_t *)lv_obj_get_ext_attr(pswd);

    if(ext->pass[0]!=0)
    {
        strcpy(passkey,ext->pass);
    }
}
