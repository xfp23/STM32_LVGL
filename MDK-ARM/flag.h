#ifndef FLAG_H
#define FLAG_H

#include "LCD.h"
#include "main.h"
#include "spi.h"
#include "i2c.h"
//#include "lv_example_osal.h"
#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
#include "lv_port_indev.h"       // LVGL的触屏支持
#include "gui_guider.h"


#ifndef ON
#define ON (1)
#endif

#ifndef OFF
#define OFF (0)
#endif

typedef union
{
	struct 
	{
	uint8_t	systim_1ms :1;
	uint8_t	systim_10ms :1;
	uint8_t	systim_100ms :1;
	uint8_t systim_500ms :1;
	}bytes;
}UserTim_t;

extern UserTim_t UserTim;

extern LCD_Class_t LCD;

extern uint8_t lv_count;

extern void lv_port_disp_init(void);

extern void lv_test(void);
extern void lv_example_chart_1(void);
//extern void lv_example_osal(void);

#endif