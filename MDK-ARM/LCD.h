#ifndef LCD_H
#define LCD_H

/**
 * @file LCD.h
 * @author xfp23 (@github.com/xfp23)
 * @brief 
 * @version 0.1
 * @date 2025-01-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "CTP.h" // 触摸驱动
#include "ILI9341.h" // 驱动芯片驱动


#ifdef __cplusplus
extern "C"
{
#endif
	
#define SWAP(a, b) { int temp = a; a = b; b = temp; }

// RGB(565) 
typedef enum 
{
    BLACK     = 0x0000,    // 黑色 RGB(0, 0, 0)  
    WHITE     = 0xFFFF,    // 白色 RGB(255, 255, 255)  
    RED       = 0xF800,    // 红色 RGB(255, 0, 0)  
    BLUE      = 0x001F,    // 蓝色 RGB(0, 0, 255)  
    GREEN     = 0x07E0,    // 绿色 RGB(0, 255, 0)  
    YELLOW    = 0xFFE0,    // 黄色 RGB(255, 255, 0)  
    CYAN      = 0x07FF,    // 青色 RGB(0, 255, 255)  
    MAGENTA   = 0xF81F,    // 品红 RGB(255, 0, 255)  
    GRAY      = 0x8410,    // 灰色 RGB(169, 169, 169)  
    ORANGE    = 0xFD20,    // 橙色 RGB(255, 165, 0)  
    PURPLE    = 0x8010,    // 紫色 RGB(128, 0, 128)  
    PINK      = 0xF81F,    // 粉红 RGB(255, 192, 203)  
    LIME      = 0x07E0,    // 青柠色 RGB(0, 255, 0)  
    BROWN     = 0xBC40,    // 棕色 RGB(139, 69, 19)  
    TEAL      = 0x0410,    // 水鸭色 RGB(0, 128, 128)  
    TURQUOISE = 0x40FF,    // 绿松石色 RGB(64, 224, 208)  
    VIOLET    = 0x915C,    // 紫罗兰色 RGB(238, 130, 238)  
    BEIGE     = 0xF5D7,    // 米色 RGB(245, 245, 220)  
    PEACH     = 0xFC98,    // 桃色 RGB(255, 218, 185)  
    IVORY     = 0xFFFF,    // 象牙色 RGB(255, 255, 240)  
    MINT      = 0x3FFE,    // 薄荷色 RGB(189, 252, 201)  
    AMBER     = 0xFF7E,    // 琥珀色 RGB(255, 191, 0)  
    INDIGO    = 0x4B30,    // 靛青 RGB(75, 0, 130)  
    GOLD      = 0xFFDF,    // 金色 RGB(255, 215, 0)  
    SALMON    = 0xFAE7,    // 鲑鱼色 RGB(250, 128, 114)  
    TAN       = 0xD5A0,    // 黄褐色 RGB(210, 180, 140)  
    OLIVE     = 0x7BE0,    // 橄榄色 RGB(128, 128, 0)  
    SLATE     = 0x6B5A,    // 石板色 RGB(112, 128, 144)  
    SEPIA     = 0xB45C,    // 褐色 RGB(112, 66, 20)  
} LCD_Color_t;


typedef enum 
{
    LCD_DIR_0   = ROTATE_0,   // 竖屏
    LCD_DIR_90  = ROTATE_90,  // 横屏
    LCD_DIR_180 = ROTATE_180, // 竖屏反向
    LCD_DIR_270 = ROTATE_270  // 横屏反向
} LCD_Dir_t;


typedef struct
{
    CTP_Class_t touch;       // 触摸信号对象
    ILI9341_Class_t display; // 显示对象
    uint16_t width;          // 屏幕宽度
    uint16_t height;         // 屏幕高度
} LCD_Class_t;               // 屏幕对象


/**
 * @brief LCD初始化
 * 
 * @param lcd 
 */
extern void LCD_Init(LCD_Class_t * lcd);

extern void LCD_Reset(LCD_Class_t * lcd);

extern void LCD_Fillclear(LCD_Class_t *lcd, LCD_Color_t color);

extern void LCD_Drawpoint(LCD_Class_t *lcd, uint16_t x, uint16_t y,LCD_Color_t color);

extern void LCD_DrawLine(LCD_Class_t *lcd, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, LCD_Color_t color,...);

extern void LCD_DrawFille_Circle(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t rad, LCD_Color_t color);

extern void LCD_DrawCircle(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t rad, LCD_Color_t color, ...);

extern void LCD_DrawOval(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t a, uint16_t b, LCD_Color_t color,...);

extern void LCD_DrawFillOval(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t a, uint16_t b, LCD_Color_t color);

extern void LCD_DrawRectangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, LCD_Color_t color, ...);

extern void LCD_DrawFillRectangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, LCD_Color_t color);

extern void LCD_DrawTriangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, LCD_Color_t color, ...);

extern void LCD_DrawFilledTriangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, LCD_Color_t color);

extern void LCD_ShowChar(LCD_Class_t *lcd, uint16_t x, uint16_t y, LCD_Color_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode);

extern void LCD_ShowString(LCD_Class_t *lcd,uint16_t x, uint16_t y, LCD_Color_t fc, LCD_Color_t bc, uint8_t *str,uint8_t size,uint8_t mode);
#ifdef __cplusplus
}
#endif
#endif
