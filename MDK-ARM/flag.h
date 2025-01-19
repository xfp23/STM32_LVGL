#ifndef FLAG_H
#define FLAG_H

#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
#include "lv_port_indev.h"       // LVGL的触屏支持
#include "LCD.h"

#ifdef __cplusplus
extern "C"
{
#endif


extern LCD_Class_t LCD;

extern void lv_port_disp_init(void);
#ifdef __cplusplus
}
#endif


#endif
