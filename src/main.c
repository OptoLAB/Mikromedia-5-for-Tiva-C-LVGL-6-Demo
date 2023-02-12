#include "board_hw/board_hw.h"
#include "lvgl/lvgl.h"
#include "lvgl/lv_port.h"
#include "gui/main_win.h"


int main(void)
{
    BOARD_HW_Init();
    lv_init();
    lv_port_init();

    lv_create_win_main();
    while(1)
    {
        lv_task_handler();
        delayMS(5);
    }

}
