#include <lvgl/lv_port.h>
#include "board_hw/display/display.h"
#include "board_hw/touch/TSC2006.h"
#include "board_hw/misc/rgb_led.h"
#include "board_hw/misc/nec_decoder.h"
#include "board_hw/misc/ir_tsop.h"
#include "board_hw/misc/buzzer.h"
#if LV_USE_EPI
#include "driverlib/epi.h"
#endif

#define LV_CURSOR_INC  20
uint32_t sys_tick_cnt=0;
uint8_t gled_stat=0;

lv_indev_t * indev_touchpad;

lv_indev_t * indev_remote;
lv_obj_t *remote_cursor;
uint8_t cursor_is_visible=0;
lv_coord_t cursor_x_position=100;  //default x cursor position
lv_coord_t cursor_y_position=100;  //default y cursor position
extern uint8_t buzzer_audio_enable;
extern const lv_img_dsc_t cursor;
//lv_obj_t * mouse_cursor;
static void my_disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static bool my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool my_remote_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
#if LV_USE_GPU
static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width, const lv_area_t * fill_area, lv_color_t color);
#endif

void SysTickIntHandler(void)
{
    sys_tick_cnt++;
    lv_tick_inc(1);
    if(sys_tick_cnt>=1000)
    {
        //lv_tick_inc(5);
        sys_tick_cnt=0;
        gled_stat=gled_stat^0x01;
        //LEDs_Set(BLED,gled_stat);
    }
}

void lv_port_init(void)
{
    static lv_disp_buf_t  lv_disp_buf;      //Define buffer
    static lv_color_t lv_buffer_1[(LV_HOR_RES_MAX) * BUFFER_SIZE]; //First buffer
#if BUFFER_TYPE
    static lv_color_t lv_buffer_2[(LV_HOR_RES_MAX) * BUFFER_SIZE]; //Second buffer
    lv_disp_buf_init(&lv_disp_buf, lv_buffer_1, lv_buffer_2, (LV_HOR_RES_MAX) * BUFFER_SIZE); //Initialize the display buffer
#else
    lv_disp_buf_init(&lv_disp_buf, lv_buffer_1, NULL, (LV_HOR_RES_MAX) * BUFFER_SIZE); //Initialize the display buffer
#endif

    //Initialized in main                   //Initialize your display

    lv_disp_drv_t disp_drv;                 //Descriptor of a display driver
    lv_disp_drv_init(&disp_drv);            //Basic initialization
    disp_drv.hor_res = LV_HOR_RES_MAX;      //Set horizontal resolution of the display
    disp_drv.ver_res = LV_VER_RES_MAX;      //Set vertical resolution of the display
    disp_drv.flush_cb = my_disp_flush;      //Used to copy the buffer's content to the display
    disp_drv.buffer = &lv_disp_buf;         //Set a display buffer
#if LV_USE_GPU                              //Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)
    disp_drv.gpu_blend_cb = gpu_blend;      //Blend two color array using opacity
    disp_drv.gpu_fill_cb = gpu_fill;        //Fill a memory array with a color
#endif
    lv_disp_drv_register(&disp_drv);        //Finally register the driver

    //Initialized in main                   //Initialize your touch panel

    lv_indev_drv_t indev_drv;               //Descriptor of input device
    lv_indev_drv_init(&indev_drv);          //Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER; //Touchpad
    indev_drv.read_cb = my_touchpad_read;   //Callback fuction used for reading touchpad
    indev_touchpad=lv_indev_drv_register(&indev_drv);      //Finally register the driver

    //Initialized in main                   //Initialize your ir remote

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_remote_read;
    indev_remote = lv_indev_drv_register(&indev_drv);    //Register a ir remote input device

    remote_cursor = lv_img_create(lv_disp_get_scr_act(NULL), NULL);   //Set cursor. For simplicity set a + symbol now.
    static lv_style_t img_style;
    lv_img_set_src(remote_cursor, &cursor);
    lv_style_copy(&img_style, lv_img_get_style(remote_cursor, LV_IMG_STYLE_MAIN));
    img_style.image.color=LV_COLOR_RED;
    lv_img_set_style(remote_cursor, LV_IMG_STYLE_MAIN, &img_style);
    lv_indev_set_cursor(indev_remote, remote_cursor);
    lv_obj_set_hidden(remote_cursor, true);
    cursor_is_visible=0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void my_disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint16_t *sdram_addr=(uint16_t*)DISPLAY_getFrameBufferAddress();
    uint16_t *start=sdram_addr+((area->y1) * LV_HOR_RES_MAX);
    int32_t x, y;

    for(y = area->y1; y <= area->y2; y++)
    {
        for(x = area->x1; x <=area->x2; x++)
        {
#if LV_USE_EPI
            EPIWorkaroundHWordWrite(start+x, *(uint16_t*)&color_p->full);
#else
            start[x]=*(uint16_t*)&color_p->full;
#endif
            color_p++;
        }
        start+=LV_HOR_RES_MAX;
    }
    lv_disp_flush_ready(disp_drv);
}

static bool my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint8_t touchpad_db_fl=0; //debouncing flag
    static uint16_t last_x = 0, last_y = 0;

    if(TSC2006_isPressed())
    {
        if(touchpad_db_fl)
        {
            TSC2006_readXYCalibrated(&last_x, &last_y);
            data->state = LV_INDEV_STATE_PR;
            if(cursor_is_visible)
            {
                cursor_is_visible=0;
                lv_obj_set_hidden(remote_cursor, true);
            }
        }
        else
        {
            touchpad_db_fl=1;
        }
    }
    else
    {
        touchpad_db_fl=0;
        data->state = LV_INDEV_STATE_REL;
    }

    //Set the last pressed coordinates
    data->point.x = (lv_coord_t)last_x;
    data->point.y = (lv_coord_t)last_y;

    //Return `false` because we are not buffering and no more data to read
    return false;
}

static bool my_remote_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /*Get the current x and y coordinates*/
    if(NEC_GetStatus())
    {
        if(!cursor_is_visible)
        {
            cursor_is_visible=1;
            lv_obj_set_hidden(remote_cursor, false);
        }
        switch(NEC_GetCommand())
        {
            case IR_BTN_UP:
                if(buzzer_audio_enable)BUZZER_ClickSound();
                if(cursor_y_position>0) cursor_y_position-=LV_CURSOR_INC;
                else cursor_y_position=LV_VER_RES_MAX-LV_CURSOR_INC;
                break;
            case IR_BTN_DOWN:
                if(buzzer_audio_enable)BUZZER_ClickSound();
                if(cursor_y_position<(LV_VER_RES_MAX-LV_CURSOR_INC)) cursor_y_position+=LV_CURSOR_INC;
                else cursor_y_position=0;
                break;
            case IR_BTN_LEFT:
                if(buzzer_audio_enable)BUZZER_ClickSound();
                if(cursor_x_position>0) cursor_x_position-=LV_CURSOR_INC;
                else cursor_x_position=LV_HOR_RES_MAX-LV_CURSOR_INC;
                break;
            case IR_BTN_RIGHT:
                if(buzzer_audio_enable)BUZZER_ClickSound();
                if(cursor_x_position<(LV_HOR_RES_MAX-LV_CURSOR_INC)) cursor_x_position+=LV_CURSOR_INC;
                else cursor_x_position=0;
                break;
            case IR_BTN_OK:
                data->state = LV_INDEV_STATE_PR;
                break;
            default:
                data->state = LV_INDEV_STATE_REL;
                break;
        }
        NEC_ClearStatus();
    }
    data->point.x=cursor_x_position;
    data->point.y=cursor_y_position;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}
/*OPTIONAL: GPU INTERFACE*/
#if LV_USE_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
    //It's an example code which should be done by your GPU
    uint32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
                    const lv_area_t * fill_area, lv_color_t color)
{
    //It's an example code which should be done by your GPU
    int32_t x, y;
    dest_buf += dest_width * fill_area->y1; //Go to the first line

    for(y = fill_area->y1; y <= fill_area->y2; y++) {
        for(x = fill_area->x1; x <= fill_area->x2; x++) {
            dest_buf[x] = color;
        }
        dest_buf+=dest_width;    //Go to the next line
    }
}

#endif  /*LV_USE_GPU*/
