/*
 * Author: J. Bajic, 2022.
 */

#ifndef GUI_CLOCK_OBJ_H_
#define GUI_CLOCK_OBJ_H_


#include "lvgl/lvgl.h"

typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} lv_clock_time_t;

typedef struct
{
    lv_page_ext_t page;
    lv_obj_t *hour;
    lv_obj_t *min;
    lv_obj_t *sec;
} lv_clock_ext_t;

lv_obj_t *lv_clock_create(lv_obj_t *par);
void lv_clock_set_time(lv_obj_t *clock, lv_clock_time_t *time);
lv_clock_time_t lv_clock_get_time(lv_obj_t *clock);

#endif /* GUI_CLOCK_OBJ_H_ */
