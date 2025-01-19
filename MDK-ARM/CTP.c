#include "CTP.h"
#include "main.h"
#include <stdarg.h>
#include <string.h>

// static uint8_t CTP_ADDR = 0x70; // 触摸信号iic地址

#ifndef ON
#define ON (0x01)
#endif

#ifndef OFF
#define OFF (0x00)
#endif

// 写字节
static void WDATA(CTP_Class_t *ctp)
{
    static uint8_t Werror = 0;
    if (ctp->i2c.byte == NULL)
        return;
    if (!ctp->flag.DMA)
    {
        if (HAL_I2C_Master_Transmit(ctp->i2c.handle, ctp->i2c.addr, ctp->i2c.byte, ctp->i2c.size, ctp->i2c.Timeout) != HAL_OK)
        {
            Werror = 1;
        }
        return;
    }
    if (HAL_I2C_Master_Transmit_DMA(ctp->i2c.handle, ctp->i2c.addr, ctp->i2c.byte, ctp->i2c.size) != HAL_OK)
    {
        Werror = 1;
        return;
    }
    Werror = 0;
}

// 读字节
static void RDATA(CTP_Class_t *ctp)
{
    static uint8_t Rerror = 0;
    if (ctp->i2c.buf == NULL)
        return;
    if (!ctp->flag.DMA)
    {
        if (HAL_I2C_Master_Receive(ctp->i2c.handle, ctp->i2c.addr, ctp->i2c.buf, ctp->i2c.size, ctp->i2c.Timeout) != HAL_OK)
        {
            Rerror = 1;
        }
        return;
    }
    if (HAL_I2C_Master_Receive_DMA(ctp->i2c.handle, ctp->i2c.addr, ctp->i2c.buf, ctp->i2c.size) != HAL_OK)
    {
        Rerror = 1;
        return;
    }
    Rerror = 0;
}

/**
 * @brief Get the Active Touch Points object
 *         获取当前有几个触控点
 * @param ctp 触摸对象
 * @return uint8_t 触控点的数量
 */
uint8_t getActiveTouchPoints(CTP_Class_t *ctp)
{
    uint8_t data = 0x00;
    uint8_t byte = (uint8_t)TD_STATUS;
    ctp->i2c.buf = &data;
    ctp->i2c.byte = &byte;
    ctp->i2c.size = 1;
    WDATA(ctp);
    RDATA(ctp);
    return data;
}

/**
 * @brief Get the Touch X object
 *        获取触控点的x坐标
 *
 * @param ctp   触摸对象
 * @param point 触控点数量
 */
void getTouchX(CTP_Class_t *ctp, uint8_t point)
{
    uint8_t bytes[4] = {P1_XH, P1_XL, P2_XH, P2_XL};
    uint8_t data[4] = {0};
    ctp->i2c.byte = &bytes[0];
    ctp->i2c.buf = &data[0];
    ctp->i2c.size = 1; // 获取低字节
    WDATA(ctp);
    RDATA(ctp);
    ctp->i2c.byte = &bytes[1];
    ctp->i2c.buf = &data[1];
    WDATA(ctp);
    RDATA(ctp);
    data[0] &= 0x0f;
    ctp->signal.x1 = ((uint16_t)data[0] << 8) | data[1];
    if (point < 2) // 坐标数量
        return;
    ctp->i2c.byte = &bytes[2];
    ctp->i2c.buf = &data[2];
    WDATA(ctp);
    RDATA(ctp);
    ctp->i2c.byte = &bytes[3];
    ctp->i2c.buf = &data[3];
    WDATA(ctp);
    RDATA(ctp);
    data[2] &= 0x0f;
    ctp->signal.x2 = ((uint16_t)data[2] << 8) | data[3];
}

/**
 * @brief Get the Touch Y object
 *         获取当前触控点的x坐标
 *
 * @param ctp
 * @param point  触控点数量
 */
void getTouchY(CTP_Class_t *ctp, uint8_t point)
{
    uint8_t bytes[4] = {P1_YH, P1_YL, P2_YH, P2_YL};
    uint8_t data[4] = {0};
    ctp->i2c.byte = &bytes[0];
    ctp->i2c.buf = &data[0];
    ctp->i2c.size = 1; // 获取低字节
    WDATA(ctp);
    RDATA(ctp);
    ctp->i2c.byte = &bytes[1];
    ctp->i2c.buf = &data[1];
    WDATA(ctp);
    RDATA(ctp);
    data[0] &= 0x0f;
    ctp->signal.y1 = ((uint16_t)data[0] << 8) | data[1];
    if (point < 2) // 坐标数量
        return;
    ctp->i2c.byte = &bytes[2];
    ctp->i2c.buf = &data[2];
    WDATA(ctp);
    RDATA(ctp);
    ctp->i2c.byte = &bytes[3];
    ctp->i2c.buf = &data[3];
    WDATA(ctp);
    RDATA(ctp);
    data[2] &= 0x0f;
    ctp->signal.y2 = ((uint16_t)data[2] << 8) | data[3];
}

/**
 * @brief 中断回调
 * @note 请将这个函数放入INT引脚的中断回调里，否则无法正常工作
 *
 * @param ctp
 */
void CTPCallback(CTP_Class_t *ctp)
{
    ctp->flag.CTP_INT = ON;
}

/**
 * @brief Set the Touch Monitor Mode object
 *        设置屏幕是否进入低功耗模式
 *
 * @param ctp   触摸对象
 * @param mode  ON or OFF (0 or 1)
 * @param ...   如果是ON，请传入触摸进入monitor模式的延时（单位：秒），如果没传入，则默认3秒
 *
 */
void setTouchMonitorMode(CTP_Class_t *ctp, uint8_t mode, ...)
{
    va_list args;
    uint16_t Touchtime = 3; // 默认延时3秒
    uint8_t bytes[2] = {ID_G_CTRL, mode};

    ctp->i2c.size = 2;
    ctp->i2c.byte = bytes;
    WDATA(ctp);

    if (mode == OFF)
    {
        return;
    }

    va_start(args, mode);
    Touchtime = va_arg(args, int); // 获取延时参数
    if (Touchtime <= 0)
        Touchtime = 3; // 还原回默认值
    va_end(args);

    bytes[0] = ID_G_TIMEENTERMONITOR;
    bytes[1] = Touchtime;
    WDATA(ctp);
}

/**
 * @brief Set the Touch Power Mode object
 *          设置功耗模式
 *
 * @param ctp
 * @param CTP_Power_t mode
 */
void setTouchPowerMode(CTP_Class_t *ctp, CTP_Power_t mode)
{
    uint8_t bytes[2] = {ID_G_PMODE, mode};
    ctp->i2c.size = 2;
    ctp->i2c.byte = bytes;
    WDATA(ctp);
}

/**
 * @brief 初始化
 *
 * @param ctp
 */
void CTP_Init(CTP_Class_t *ctp)
{
    resetTouchController(ctp);

    uint8_t dma_buff = ctp->flag.DMA;
    memset((void *)&ctp->flag, 0, sizeof(CTP_Flag_t));
    memset((void *)&ctp->signal, 0, sizeof(ctp->signal));
    setTouchPowerMode(ctp, P_ACTIVE);
    setTouchMonitorMode(ctp, 0);
    ctp->flag.DMA = dma_buff;
}

/**
 * @brief 触控信号主循环
 *
 * @param ctp
 */
void CTP_mainLoop(CTP_Class_t *ctp)
{

    if (ctp->flag.CTP_INT == OFF)
        return;

    uint8_t pointNum = getActiveTouchPoints(ctp);
    getTouchX(ctp, pointNum);
    getTouchY(ctp, pointNum);

    ctp->flag.CTP_INT = OFF;
}

/**
 * @brief 复位屏幕的触摸信号
 *
 * @param ctp
 */
void resetTouchController(CTP_Class_t *ctp)
{
    HAL_GPIO_WritePin(ctp->Hardware.RST.Port, ctp->Hardware.RST.Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(ctp->Hardware.RST.Port, ctp->Hardware.RST.Pin, GPIO_PIN_SET);
    memset((void*)&ctp->signal,0,sizeof(ctp->signal));
}
