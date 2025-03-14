#include "flag.h"

UserTim_t UserTim = {0};
uint8_t lv_count = 0;

LCD_Class_t LCD = {
  .touch = {
  .i2c.addr = 0x70,
  .i2c.byte = NULL,
  .i2c.buf = NULL,
  .i2c.handle = &hi2c2,
  .i2c.Timeout = 100,
  .flag.DMA = 0,
  .Hardware.RST.Pin = CTP_RST_Pin,
  .Hardware.RST.Port = CTP_RST_GPIO_Port,
  },
  .display = {
  .spi.buf = NULL,
  .spi.bytes = NULL,
  .spi.DMA = 1,
  .spi.Timeout = 100,
  .spi.handle = &hspi1,
  .spi.size = 0,
  .Hardware.CS.Pin = LCD_CS_Pin,
  .Hardware.CS.Port = LCD_CS_GPIO_Port,
  .Hardware.LED.Pin = LCD_LED_Pin,
  .Hardware.LED.Port = LCD_LED_GPIO_Port,
  .Hardware.RS.Pin = LCD_RS_Pin,
  .Hardware.RS.Port = LCD_RS_GPIO_Port,
  .Hardware.RST.Pin = LCD_RST_Pin,
  .Hardware.RST.Port = LCD_RST_GPIO_Port,
  },
  .width = 240,
  .height = 320,
};
#include "lvgl.h"
#include <stdio.h>

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

#define BTN_SIZE  48   // 按钮大小
#define SPACING   5    // 按钮间距
#define OFFSET_Y  55   // **整体上移**
void lv_example_font_custom(void)
{
    /* 创建一个标签并设置文本 */
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "1");

    /* 创建一个新的字体对象，或使用现有的 48px 字体，调整字体大小 */
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);

    /* 设置标签的宽高，适应目标大小 */
    lv_obj_set_size(label, 60, 120);  // 设置宽60px，高120px

    /* 调整标签的位置 */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);  // 居中对齐
}


void lv_example_font_48(void)
{
    /* 创建一个标签并设置文本 */
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "1234");

    /* 设置字体为 48px */
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);  // 设置字体为 48px 字体

    /* 调整标签的位置 */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);  // 居中对齐
	
}


/* === 创建按钮 === */
static lv_obj_t *create_button(lv_obj_t *parent, const char *text, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_color_t color)
{
//    lv_obj_t *btn = lv_btn_create(parent);
//    lv_obj_set_size(btn, w, h);
//    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, x, y);
//    lv_obj_set_style_bg_color(btn, color, LV_PART_MAIN);
//    lv_obj_set_style_radius(btn, h / 2, LV_PART_MAIN);  // 圆角

//    lv_obj_t *label = lv_label_create(btn);
//    lv_label_set_text(label, text);
//    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
//    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, LV_PART_MAIN);  // **字体更清晰**
//    lv_obj_center(label);

//    return btn;
}

/* === iOS 计算器 UI === */
void lv_test()
{
//    /* 背景颜色 */
//    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

//    /* === 显示区域（数字结果框）=== */
//    lv_obj_t *display = lv_obj_create(lv_scr_act());
//    lv_obj_set_size(display, SCREEN_WIDTH - 30, 40);  // **更窄，避免被 AC 挡住**
//    lv_obj_align(display, LV_ALIGN_TOP_MID, 0, 10);  // **向上调整**
//    lv_obj_set_style_bg_color(display, lv_color_hex(0x333333), LV_PART_MAIN);
//    lv_obj_set_style_radius(display, 10, LV_PART_MAIN);
//    lv_obj_set_style_border_width(display, 0, LV_PART_MAIN);  // **去掉多余边框**
//    
//    lv_obj_t *display_label = lv_label_create(display);
//    lv_label_set_text(display_label, "0");
//    lv_obj_align(display_label, LV_ALIGN_RIGHT_MID, -10, 0);
//    lv_obj_set_style_text_color(display_label, lv_color_white(), LV_PART_MAIN);
//    lv_obj_set_style_text_font(display_label, &lv_font_montserrat_28, LV_PART_MAIN);  // **更清晰的字体**

//    /* === 按钮颜色定义 === */
//lv_color_t black = lv_color_hex(0x808080);     // 纯黑色（数字按钮）
//lv_color_t orange = lv_color_hex(0xFF9500);    // 运算符按钮（保持不变）
//lv_color_t light_gray = lv_color_hex(0xA5A5A5); // 功能按钮（保持不变）
//lv_color_t dark_gray = lv_color_hex(0x505050); // 默认按钮（保持不变）
////lv_color_t black = lv_color_hex(0x000000);  // 定义纯黑色


//    /* === 按键布局 === */
//    int row1 = OFFSET_Y;
//    int row2 = row1 + BTN_SIZE + SPACING;
//    int row3 = row2 + BTN_SIZE + SPACING;
//    int row4 = row3 + BTN_SIZE + SPACING;
//    int row5 = row4 + BTN_SIZE + SPACING;

//    create_button(lv_scr_act(), "AC", 10, row1, BTN_SIZE, BTN_SIZE, light_gray);
//    create_button(lv_scr_act(), "+/-", 65, row1, BTN_SIZE, BTN_SIZE, light_gray);
//    create_button(lv_scr_act(), "%", 120, row1, BTN_SIZE, BTN_SIZE, light_gray);
//    create_button(lv_scr_act(), "/", 175, row1, BTN_SIZE, BTN_SIZE, orange);

//    create_button(lv_scr_act(), "7", 10, row2, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "8", 65, row2, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "9", 120, row2, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "x", 175, row2, BTN_SIZE, BTN_SIZE, orange);

//    create_button(lv_scr_act(), "4", 10, row3, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "5", 65, row3, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "6", 120, row3, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "-", 175, row3, BTN_SIZE, BTN_SIZE, orange);

//    create_button(lv_scr_act(), "1", 10, row4, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "2", 65, row4, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "3", 120, row4, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "+", 175, row4, BTN_SIZE, BTN_SIZE, orange);

///* "0" 按钮（加宽） */
//lv_obj_t *zero_btn = lv_btn_create(lv_scr_act());
//lv_obj_set_size(zero_btn, 95, BTN_SIZE);  // **宽度减少**
//lv_obj_align(zero_btn, LV_ALIGN_TOP_LEFT, 10, row5);
//lv_obj_set_style_bg_color(zero_btn, dark_gray, LV_PART_MAIN);
//lv_obj_set_style_radius(zero_btn, 25, LV_PART_MAIN);

//lv_obj_t *zero_label = lv_label_create(zero_btn);
//lv_label_set_text(zero_label, "0");
//lv_obj_set_style_text_color(zero_label, lv_color_white(), LV_PART_MAIN);

///* **增加字体大小** */
//lv_obj_set_style_text_font(zero_label, &lv_font_montserrat_32, LV_PART_MAIN);  // **使用更大的字体**

//lv_obj_center(zero_label);


//    create_button(lv_scr_act(), ".", 120, row5, BTN_SIZE, BTN_SIZE, dark_gray);
//    create_button(lv_scr_act(), "=", 175, row5, BTN_SIZE, BTN_SIZE, orange);
}
void lv_example_chart_1(void)
{
    /*Create a chart*/
    lv_obj_t * chart;
    chart = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart, 200, 150);
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_SECONDARY_Y);

    uint32_t i;
	for(int i = 0; i < 10; i++) {
    lv_chart_set_next_value(chart, ser1, lv_rand(10, 50));
    lv_chart_set_next_value(chart, ser2, lv_rand(50, 90));
	}


    lv_chart_refresh(chart); /*Required after direct set*/
	//lv_example_font_48();
//	lv_example_font_custom();
}