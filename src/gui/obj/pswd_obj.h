/*
 * Author: J. Bajic, 2022.
 */

#ifndef GUI_PSWD_OBJ_H_
#define GUI_PSWD_OBJ_H_


#include "lvgl/lvgl.h"

typedef struct
{
    lv_page_ext_t page;
    lv_obj_t *ta_name;
    lv_obj_t *label_pass;
    lv_obj_t *ta_pass;
    lv_obj_t *label_name;
    lv_obj_t *kb;
    char name[20];
    char pass[20];
} lv_pswd_ext_t;

lv_obj_t * lv_pswd_create(lv_obj_t * par);
void lv_pswd_get_name(lv_obj_t * pswd, char *name);
void lv_pswd_get_passkey(lv_obj_t * pswd, char *passkey);

#endif /* GUI_PSWD_OBJ_H_ */
