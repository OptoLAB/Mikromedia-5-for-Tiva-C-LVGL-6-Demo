#ifndef LV_PORT_H
#define LV_PORT_H

#include "lvgl/lvgl.h"

#define LV_USE_EPI      1
#define BUFFER_TYPE     1   //0-single buffer, 1-dual buffer
#define BUFFER_SIZE     32  //number of lines

void lv_port_init(void);

#endif /*LV_PORT_DISP_H*/

