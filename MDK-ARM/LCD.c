#include "LCD.h"
#include "font.h"
#include <stdarg.h>
#include <math.h>
void LCD_SetWindows(LCD_Class_t * lcd,uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	ILI93_WR_REG(&lcd->display,SET_X_ADDRESS);	
	ILI93_WR_DATA(&lcd->display,xStar>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&xStar);		
	ILI93_WR_DATA(&lcd->display,xEnd>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&xEnd);

	ILI93_WR_REG(&lcd->display,SET_Y_ADDRESS);	
	ILI93_WR_DATA(&lcd->display,yStar>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&yStar);		
	ILI93_WR_DATA(&lcd->display,yEnd>>8);
	ILI93_WR_DATA(&lcd->display,0x00FF&yEnd);
    ILI93_WriteRAM_Prepare(&lcd->display);
		
}   
	
void LCD_SetCursor(LCD_Class_t * lcd,uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_SetWindows(lcd,Xpos,Ypos,Xpos,Ypos);	
} 

/**
 * @brief 清除整个屏幕
 * 
 * @param lcd 对象指针
 * @param color 颜色
 */
void LCD_Fillclear(LCD_Class_t *lcd, LCD_Color_t color)
{
    // 提取 RGB565 格式中的颜色值
    // 将 RGB565 转换为 3 字节（扩展为 G、B、R）
    uint8_t data[3] = {
        (uint8_t)(color >> 8),  // 提取红色高 5 位并移位，填充为 8 位
        (uint8_t)color,  // 提取绿色高 6 位并移位，填充为 8 位
        0x00  // 提取蓝色高 5 位并移位，填充为 8 位
    };
	lcd->display.spi.size = 2;
    // 设置绘图窗口为整个屏幕
    LCD_SetWindows(lcd, 0, 0, lcd->width - 1, lcd->height - 1);

    // 拉低 CS 以开始传输
    HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port, lcd->display.Hardware.CS.Pin, GPIO_PIN_RESET);
    // 设置 RS 引脚以指定写入数据
    HAL_GPIO_WritePin(lcd->display.Hardware.RS.Port, lcd->display.Hardware.RS.Pin, GPIO_PIN_SET);

    // 遍历整个屏幕并填充颜色
    for (int i = 0; i < lcd->height; i++)
    {
        for (int j = 0; j < lcd->width; j++)
        {
            // 发送数据，RGB565 格式只需 2 字节
            if (lcd->display.spi.DMA == true)
            {
                HAL_SPI_Transmit_DMA(lcd->display.spi.handle, data, lcd->display.spi.size);
            }
            else
            {
				lcd->display.spi.size = 3;
                HAL_SPI_Transmit(lcd->display.spi.handle, data, lcd->display.spi.size, lcd->display.spi.Timeout);
            }
        }
    }

    // 拉高 CS 结束传输
    HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port, lcd->display.Hardware.CS.Pin, GPIO_PIN_SET);
}


/**
*@brief 设置屏幕显示方向
*/
void LCD_SetOrientation(LCD_Class_t *lcd,LCD_Dir_t direction)
{
   static uint16_t height_buf = 0;	//lcd->height;
   static uint16_t width_buf = 0; 	//lcd->width;
   static bool init = false ;
	if(!init)
	{
		init = true;
		height_buf = lcd->height;
		width_buf = lcd->width;
	}
    switch(direction)
    {
        case LCD_DIR_0 :
        lcd->height = height_buf;
        lcd->width = width_buf;
        break;
        case LCD_DIR_90 :
        lcd->height = width_buf;
        lcd->width = height_buf;
        break;
        case LCD_DIR_180 :
        lcd->height = height_buf;
        lcd->width = width_buf;
        break;
        case LCD_DIR_270 :
        lcd->height = width_buf;
        lcd->width = height_buf;
        break;
    }
	ILI93_WR_REG(&lcd->display,SET_DIRECTION);
	ILI93_WR_DATA(&lcd->display,(ILI93_Command_t)direction);
}

void LCD_Init(LCD_Class_t * lcd)
{ 
	CTP_Init(&lcd->touch);
    ILI93_Init(&lcd->display);
	LCD_SetOrientation(lcd,LCD_DIR_0);
    LCD_Fillclear(lcd,WHITE); // 白色
}

/**
 * @brief LCD复位重启
 * 
 * @param lcd 
 */
void LCD_Reset(LCD_Class_t * lcd)
{
    HAL_GPIO_WritePin(lcd->display.Hardware.RST.Port,lcd->display.Hardware.RST.Pin,GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(lcd->display.Hardware.RST.Port,lcd->display.Hardware.RST.Pin,GPIO_PIN_RESET);
	HAL_Delay(100);	
	HAL_GPIO_WritePin(lcd->display.Hardware.RST.Port,lcd->display.Hardware.RST.Pin,GPIO_PIN_SET);
	HAL_Delay(50);
}

/**
 * @brief 向屏幕写入16位的数据
 * 
 * @param lcd lcd对象
 * @param Data 要写入的数据
 */
void Lcd_WriteData_16Bit(LCD_Class_t * lcd,uint16_t Data)
{	
    uint8_t data[2] = { (uint8_t)(Data >> 8) ,(uint8_t)Data};
   HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port,lcd->display.Hardware.CS.Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(lcd->display.Hardware.RS.Port,lcd->display.Hardware.RS.Pin,GPIO_PIN_SET);
  HAL_SPI_Transmit_DMA(lcd->display.spi.handle,data,2);
  HAL_GPIO_WritePin(lcd->display.Hardware.CS.Port,lcd->display.Hardware.CS.Pin,GPIO_PIN_SET);
}


/**
*@brief 在屏幕上显示一个点
*@param LCD_Class_t 屏幕对象
*@param 点的横坐标
*@param 点的纵坐标
*@param 点的颜色
*/
void LCD_Drawpoint(LCD_Class_t *lcd, uint16_t x, uint16_t y,LCD_Color_t color)
{
	LCD_SetCursor(lcd, x, y);
	Lcd_WriteData_16Bit(lcd, (uint16_t)color);	
}

/**
* @brief lcd屏幕画直线
* @param LCD_Class_t 屏幕对象 
* @param 起始X坐标
* @param 起始Y坐标
* @param 终点x坐标
* @param 终点y坐标
* @param ... 可选参数，控制线条的粗细程度,默认为1
* @param LCD_Color_t 颜色
*/
void LCD_DrawLine(LCD_Class_t *lcd, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, LCD_Color_t color, ...)
{
    int line_kness = 1;
    va_list args;
    va_start(args, color);
    line_kness = va_arg(args, int);
    va_end(args);

    if (line_kness <= 0) line_kness = 1;

    int dx = end_x - start_x;
    int dy = end_y - start_y;
    int dx2 = 2 * abs(dx);
    int dy2 = 2 * abs(dy);
    int x = start_x;
    int y = start_y;
    int x_incr = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int y_incr = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

    if (abs(dx) > abs(dy)) {
        int error = dy2 - abs(dx);
        while (x != end_x) {
            for (int i = -line_kness / 2; i <= line_kness / 2; i++) {
                LCD_Drawpoint(lcd, x + i, y, color);
            }
            if (error > 0) {
                y += y_incr;
                error -= dx2;
            }
            x += x_incr;
            error += dy2;
        }
    } else {
        int error = dx2 - abs(dy);
        while (y != end_y) {
            for (int i = -line_kness / 2; i <= line_kness / 2; i++) {
                LCD_Drawpoint(lcd, x, y + i, color);
            }
            if (error > 0) {
                x += x_incr;
                error -= dy2;
            }
            y += y_incr;
            error += dx2;
        }
    }

    for (int i = -line_kness / 2; i <= line_kness / 2; i++) {
        LCD_Drawpoint(lcd, x + i, y, color);
    }
}

/**
* @brief LCD画空心圆
* @param LCD_Class_t lcd对象
* @param x   圆心的x坐标
* @param y   圆心的y坐标
* @param rad 圆的半径
* @param color 圆的颜色
* @param ...  可选参数,圆的线条粗细程度
*/
void LCD_DrawCircle(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t rad, LCD_Color_t color, ...)
{
    int line_kness = 1;
    va_list args;
    va_start(args, color);
    line_kness = va_arg(args, int);
    va_end(args);

    if (line_kness <= 0) line_kness = 1;

    int x_pos = rad, y_pos = 0;
    int decision = 1 - rad; // 初始决策参数

    while (x_pos >= y_pos)
    {
        // 如果线条粗细大于1，绘制圆边缘附近的点来加粗线条
        for (int i = -line_kness + 1; i <= line_kness - 1; i++) 
        {
            // 画8个对称点，并通过偏移让线条加粗
            LCD_Drawpoint(lcd, x + x_pos + i, y + y_pos, color);
            LCD_Drawpoint(lcd, x - x_pos + i, y + y_pos, color);
            LCD_Drawpoint(lcd, x + x_pos + i, y - y_pos, color);
            LCD_Drawpoint(lcd, x - x_pos + i, y - y_pos, color);
            LCD_Drawpoint(lcd, x + y_pos + i, y + x_pos, color);
            LCD_Drawpoint(lcd, x - y_pos + i, y + x_pos, color);
            LCD_Drawpoint(lcd, x + y_pos + i, y - x_pos, color);
            LCD_Drawpoint(lcd, x - y_pos + i, y - x_pos, color);
        }

        y_pos++;
        if (decision <= 0)
        {
            decision += 2 * y_pos + 1;
        }
        else
        {
            x_pos--;
            decision += 2 * (y_pos - x_pos) + 1;
        }
    }
}


/**
* @brief LCD画实心圆
* @param LCD_Class_t lcd对象
* @param x   圆心的x坐标
* @param y   圆心的y坐标
* @param rad 圆的半径
* @param color 圆的颜色
*/
void LCD_DrawFille_Circle(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t rad, LCD_Color_t color)
{
    int x_pos = rad, y_pos = 0;
    int decision = 1 - rad; // 初始决策参数

    while (x_pos >= y_pos)
    {
        // 绘制每一行的点
        for (int i = x - x_pos; i <= x + x_pos; i++)
        {
            LCD_Drawpoint(lcd, i, y + y_pos, color);
            LCD_Drawpoint(lcd, i, y - y_pos, color);
        }
        for (int i = x - y_pos; i <= x + y_pos; i++)
        {
            LCD_Drawpoint(lcd, i, y + x_pos, color);
            LCD_Drawpoint(lcd, i, y - x_pos, color);
        }

        y_pos++;
        if (decision <= 0)
        {
            decision += 2 * y_pos + 1;
        }
        else
        {
            x_pos--;
            decision += 2 * (y_pos - x_pos) + 1;
        }
    }
}

/**
 * @brief 屏幕画空心椭圆
 * 
 * @param lcd 屏幕对象
 * @param x 椭圆圆心x坐标
 * @param y 椭圆圆心y坐标
 * @param a 半长轴半径
 * @param b 半短轴半径
 * @param color 颜色
 * @param ... 可选参数，线条的粗细程度
 */
void LCD_DrawOval(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t a, uint16_t b, LCD_Color_t color, ...)
{
    va_list args;
    va_start(args, color);
    int line_kness = va_arg(args, int);
    if (line_kness <= 0) line_kness = 1;
    va_end(args);

    int x_pos = 0;
    int y_pos = b;
    int a2 = a * a;  // a^2
    int b2 = b * b;  // b^2
    int err = b2 - a2 * b + a2 / 4;  // 初始误差
    int e2;

    // 绘制第一个象限
    while (a2 * y_pos > b2 * x_pos)
    {
        // 绘制四个对称点，考虑线条粗细
        for (int i = -line_kness / 2; i <= line_kness / 2; i++)
        {
            for (int j = -line_kness / 2; j <= line_kness / 2; j++)
            {
                LCD_Drawpoint(lcd, x + x_pos + i, y + y_pos + j, color);  // 第一象限
                LCD_Drawpoint(lcd, x - x_pos + i, y + y_pos + j, color);  // 第二象限
                LCD_Drawpoint(lcd, x + x_pos + i, y - y_pos + j, color);  // 第三象限
                LCD_Drawpoint(lcd, x - x_pos + i, y - y_pos + j, color);  // 第四象限
            }
        }

        e2 = err;
        if (e2 <= 0)
        {
            x_pos++;
            err += 2 * b2 * x_pos + b2;  // 增加x
        }
        if (e2 > 0)
        {
            y_pos--;
            err -= 2 * a2 * y_pos + a2;  // 减少y
        }
    }

    // 绘制第二个象限
    x_pos = a;
    y_pos = 0;
    err = a2 - b2 * a + b2 / 4;

    while (b2 * x_pos > a2 * y_pos)
    {
        // 绘制四个对称点，考虑线条粗细
        for (int i = -line_kness / 2; i <= line_kness / 2; i++)
        {
            for (int j = -line_kness / 2; j <= line_kness / 2; j++)
            {
                LCD_Drawpoint(lcd, x + x_pos + i, y + y_pos + j, color);  // 第一象限
                LCD_Drawpoint(lcd, x - x_pos + i, y + y_pos + j, color);  // 第二象限
                LCD_Drawpoint(lcd, x + x_pos + i, y - y_pos + j, color);  // 第三象限
                LCD_Drawpoint(lcd, x - x_pos + i, y - y_pos + j, color);  // 第四象限
            }
        }

        e2 = err;
        if (e2 <= 0)
        {
            y_pos++;
            err += 2 * a2 * y_pos + a2;  // 增加y
        }
        if (e2 > 0)
        {
            x_pos--;
            err -= 2 * b2 * x_pos + b2;  // 减少x
        }
    }
}

/**
 * @brief 屏幕画实心椭圆
 * 
 * @param lcd 
 * @param x 
 * @param y 
 * @param a 
 * @param b 
 * @param color 
 */
void LCD_DrawFillOval(LCD_Class_t *lcd, uint16_t x, uint16_t y, uint16_t a, uint16_t b, LCD_Color_t color)
{
    int x_pos = 0;
    int y_pos = b;
    int a2 = a * a;  // a^2
    int b2 = b * b;  // b^2
    int err = b2 - a2 * b + a2 / 4;  // 初始误差
    int e2;

    // 填充椭圆的上半部分
    while (a2 * y_pos > b2 * x_pos)
    {
        // 绘制从左到右的线段，填充上半部分
        for (int i = x - x_pos; i <= x + x_pos; i++)
        {
            LCD_Drawpoint(lcd, i, y + y_pos, color);  // 第一象限的点
            LCD_Drawpoint(lcd, i, y - y_pos, color);  // 第四象限的点
        }

        e2 = err;
        if (e2 <= 0)
        {
            x_pos++;
            err += 2 * b2 * x_pos + b2;  // 增加x
        }
        if (e2 > 0)
        {
            y_pos--;
            err -= 2 * a2 * y_pos + a2;  // 减少y
        }
    }

    // 填充椭圆的下半部分
    x_pos = a;
    y_pos = 0;
    err = a2 - b2 * a + b2 / 4;

    while (b2 * x_pos > a2 * y_pos)
    {
        // 绘制从左到右的线段，填充下半部分
        for (int i = x - x_pos; i <= x + x_pos; i++)
        {
            LCD_Drawpoint(lcd, i, y + y_pos, color);  // 第一象限的点
            LCD_Drawpoint(lcd, i, y - y_pos, color);  // 第四象限的点
        }

        e2 = err;
        if (e2 <= 0)
        {
            y_pos++;
            err += 2 * a2 * y_pos + a2;  // 增加y
        }
        if (e2 > 0)
        {
            x_pos--;
            err -= 2 * b2 * x_pos + b2;  // 减少x
        }
    }
}

/**
 * @brief 屏幕画空心矩形
 * 
 * @param lcd 
 * @param x1 左上角点的横坐标
 * @param y1 左上角点的纵坐标
 * @param x2 右下角点的横坐标
 * @param y2 右下角点的纵坐标
 * @param color 矩形颜色
 * @param ... 可选参数，线条的粗细 默认为1
 */
void LCD_DrawRectangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, LCD_Color_t color, ...)
{
    int line_kness = 1;
    va_list args;
    va_start(args, color);
    line_kness = va_arg(args, int);
    va_end(args);

    // 确保粗细参数大于0
    if (line_kness <= 0) line_kness = 1;

    // 绘制矩形的上边和下边
    for (int i = -line_kness; i <= line_kness; i++) {
        for (int x = x1; x <= x2; x++) {
            LCD_Drawpoint(lcd, x, y1 + i, color);  // 上边
            LCD_Drawpoint(lcd, x, y2 + i, color);  // 下边
        }
    }

    // 绘制矩形的左边和右边
    for (int i = -line_kness; i <= line_kness; i++) {
        for (int y = y1; y <= y2; y++) {
            LCD_Drawpoint(lcd, x1 + i, y, color);  // 左边
            LCD_Drawpoint(lcd, x2 + i, y, color);  // 右边
        }
    }

    // 处理矩形四个角的对齐
    for (int i = -line_kness; i <= line_kness; i++) {
        LCD_Drawpoint(lcd, x1 + i, y1 + i, color);  // 左上角
        LCD_Drawpoint(lcd, x2 + i, y1 + i, color);  // 右上角
        LCD_Drawpoint(lcd, x1 + i, y2 + i, color);  // 左下角
        LCD_Drawpoint(lcd, x2 + i, y2 + i, color);  // 右下角
    }
}


/**
 * @brief 在屏幕上显示实心圆
 * 
 * @param lcd 
 * @param x1 左上角点的横坐标
 * @param y1 左上角点的纵坐标
 * @param x2 右下角点的横坐标
 * @param y2 右下角点的纵坐标
 * @param color 矩形颜色
 */
void LCD_DrawFillRectangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, LCD_Color_t color)
{
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            LCD_Drawpoint(lcd, x, y, color);  // 填充矩形的每个点
        }
    }
}

/**
 * @brief 在屏幕画三角形
 * 
 * @param lcd 
 * @param x1 顶点坐x标
 * @param y1 顶点y坐标
 * @param x2 底边顶点1x坐标
 * @param y2 底边顶点1y坐标
 * @param x3 底边顶点2x坐标
 * @param y3 底边顶点2x坐标
 * @param color 三角形颜色
 * @param ... 可选参数，线条粗细，默认为1
 */
void LCD_DrawTriangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, LCD_Color_t color, ...)
{
    int line_kness = 1;
    va_list args;
    va_start(args, color);
    line_kness = va_arg(args, int);
    va_end(args);

    // 确保粗细参数大于0
    if (line_kness <= 0) line_kness = 1;

    // 绘制三角形的三条边
    LCD_DrawLine(lcd, x1, y1, x2, y2, color, line_kness);  // 边1
    LCD_DrawLine(lcd, x2, y2, x3, y3, color, line_kness);  // 边2
    LCD_DrawLine(lcd, x3, y3, x1, y1, color, line_kness);  // 边3
}


void LCD_DrawFilledTriangle(LCD_Class_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, LCD_Color_t color)
{
    // 确保三个点按 y 坐标排序
    if (y1 > y2) { SWAP(x1, x2); SWAP(y1, y2); }
    if (y1 > y3) { SWAP(x1, x3); SWAP(y1, y3); }
    if (y2 > y3) { SWAP(x2, x3); SWAP(y2, y3); }

    // 计算三个边的斜率
    int dx1 = x2 - x1, dy1 = y2 - y1;
    int dx2 = x3 - x1, dy2 = y3 - y1;
    int dx3 = x3 - x2, dy3 = y3 - y2;

    // 画三角形的实心区域
    for (int y = y1; y <= y3; y++) {
        int x_start, x_end;

        if (y < y2) {
            // 第一条边 (x1, y1) 到 (x2, y2)
            x_start = x1 + dx1 * (y - y1) / dy1;
            x_end = x1 + dx2 * (y - y1) / dy2;
        } else {
            // 第二条边 (x2, y2) 到 (x3, y3)
            x_start = x2 + dx3 * (y - y2) / dy3;
            x_end = x1 + dx2 * (y - y1) / dy2;
        }

        if (x_start > x_end) { SWAP(x_start, x_end); } // 保证从左到右绘制

        for (int x = x_start; x <= x_end; x++) {
            LCD_Drawpoint(lcd, x, y, color);
        }
    }
}


/**
 * @brief 
 * 
 * @param lcd 
 * @param x 
 * @param y 
 * @param fc 
 * @param bc 
 * @param str 
 * @param size 
 * @param mode 
 */
void LCD_ShowString(LCD_Class_t *lcd,uint16_t x, uint16_t y, LCD_Color_t fc, LCD_Color_t bc, uint8_t *str,uint8_t size,uint8_t mode)
{
	uint16_t x0=x;							  	  
  	uint8_t bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(x>(lcd->width-size/2)||y>(lcd->height-size)) 
			return; 
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {          
		        if(*str==0x0D)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16)//字库中没有集成12X24 16X32的英文字体,用8X16代替
					{  
					LCD_ShowChar(lcd,x,y,fc,bc,*str,16,mode);
					x+=8; //字符,为全字的一半 
					}
					else
					{
					LCD_ShowChar(lcd,x,y,fc,bc,*str,size,mode);
					x+=size/2; //字符,为全字的一半 
					}
				} 
				str++; 
		        
	        }
        }
        // }else//中文 
        // {   
		// 	if(x>(lcd->width-size)||y>(lcd->height-size)) 
		// 	return;  
        //     bHz=0;//有汉字库    
		// 	if(size==32)
		// 	GUI_DrawFont32(x,y,fc,bc,str,mode);	 	
		// 	else if(size==24)
		// 	GUI_DrawFont24(x,y,fc,bc,str,mode);	
		// 	else
		// 	GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	    //     str+=2; 
	    //     x+=size;//下一个汉字偏移	    
        // }						 
    } 
}


/**
 * @brief 显示单个字符
 * 
 * @param lcd 屏幕对象
 * @param x x坐标
 * @param y y坐标
 * @param fc 显示的字体颜色
 * @param bc 显示的背景颜色
 * @param num 要显示的内容
 * @param size 要显示的字体大小
 * @param mode 模式
 */
void LCD_ShowChar(LCD_Class_t *lcd, uint16_t x, uint16_t y, LCD_Color_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode)
{
    uint8_t temp;
    uint8_t pos, t;

    num = num - ' ';                                      // 得到偏移后的值
    LCD_SetWindows(lcd,x, y, x + size / 2 - 1, y + size - 1); // 设置单个文字显示窗口
    if (!mode)                                            // 非叠加方式
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
                temp = ASCII_1206[num][pos]; // 调用1206字体
            else
                temp = ASCII_1608[num][pos]; // 调用1608字体
            for (t = 0; t < size / 2; t++)
            {
                if (temp & 0x01)
                    Lcd_WriteData_16Bit(lcd,fc);
                else
                    Lcd_WriteData_16Bit(lcd,bc);
                temp >>= 1;
            }
        }
    }
    else // 叠加方式
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
                temp = ASCII_1206[num][pos]; // 调用1206字体
            else
                temp = ASCII_1608[num][pos]; // 调用1608字体
            for (t = 0; t < size / 2; t++)
            {
                if (temp & 0x01)
                    LCD_Drawpoint(lcd,x + t, y + pos,fc); // 画一个点
                temp >>= 1;
            }
        }
    }
    LCD_SetWindows(lcd, 0, 0, lcd->width - 1, lcd->height - 1); // 恢复窗口为全屏
}


