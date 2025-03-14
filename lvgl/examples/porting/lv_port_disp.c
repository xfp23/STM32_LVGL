/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "lvgl.h"
#include "LCD.h"
#include "flag.h"

/*********************
 *      DEFINES
 *********************/
 
//#ifndef MY_DISP_HOR_RES
//    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
//    #define MY_DISP_HOR_RES    320
//#endif

//#ifndef MY_DISP_VER_RES
//    #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
//    #define MY_DISP_VER_RES    240
//#endif
#define MY_DISP_HOR_RES    240
#define MY_DISP_VER_RES    320
#define BYTE_PER_PIXEL 2 /*will be 2 for RGB565 */

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t * disp, const lv_area_t * area, lv_color16_t * px_map);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(disp,(lv_display_flush_cb_t) disp_flush);

    /* Example 1
     * One buffer for partial rendering*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_1_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];            /*A buffer for 10 rows*/
    lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_2_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];

//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
//    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];

//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];
//    lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
	LCD_Init(&LCD);
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/**
* 交换字节顺序
*/
uint16_t reverse_and_swap_rb(uint16_t color)
{
    // **按位反转**
    color = ((color & 0xAAAA) >> 1) | ((color & 0x5555) << 1);
    color = ((color & 0xCCCC) >> 2) | ((color & 0x3333) << 2);
    color = ((color & 0xF0F0) >> 4) | ((color & 0x0F0F) << 4);
    color = (color >> 8) | (color << 8);

    // **拆分 R、G、B 分量**
    uint16_t r = (color & 0xF800);  // 5-bit 红色 (bit 11-15)
    uint16_t g = (color & 0x07E0);  // 6-bit 绿色 (bit 5-10)
    uint16_t b = (color & 0x001F);  // 5-bit 蓝色 (bit 0-4)

    // **交换 R 和 B 的位置**
    return ((b << 11) | g | (r >> 11));
}



/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush(lv_display_t *disp_drv, const lv_area_t *area, lv_color16_t *px_map)
{
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++) {
        for (x = area->x1; x <= area->x2; x++) {
		uint16_t color565 = (px_map->red << 11) | (px_map->green << 5) | px_map->blue;
		LCD_Drawpoint(&LCD, x, y, color565);

            px_map++;  // 逐像素填充
        }
    }
    lv_display_flush_ready(disp_drv);  // LVGL 刷新完毕
}





#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
