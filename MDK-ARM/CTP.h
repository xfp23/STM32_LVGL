#ifndef CTP_H
#define CTP_H


/**
 * @file CTP.h
 * @author xfp23 (@github.com/xfp23)
 * @brief 此文件是电容触摸屏驱动芯片FT6336G的驱动，提供底层的触摸信号接口
 * @version 0.1
 * @date 2025-01-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */


/**
 * 
 * 监控模式
 *在此模式下，FT6336G 以较低的速度扫描面板。默认扫描速率为每秒 25 帧，主机处理器可以
 *增加或减少该比率。在此模式下，大多数算法都会停止。正在执行一个更简单的算法来确定是否
 *有触摸与否。当检测到触摸时，FT6336G 应立即进入 Active 模式以获取触摸信息
 *迅速地。在此模式下，串行端口关闭，并且不会与主机处理器传输数据
 *Monitor Mode
 *In this mode, FT6336G scans the panel at a reduced speed. The default scan rate is 25 frames per second and the host processor can
 *increase or decrease this rate. When in this mode, most algorithms are stopped. A simpler algorithm is being executed to determine if
 *there is a touch or not. When a touch is detected, FT6336G shall enter the Active mode immediately to acquire the touch information
 *quickly. During this mode, the serial port is closed and no data shall be transferred with the host processor
 * 
 * 休眠模式
 *在此模式下，芯片设置为掉电模式。它应响应来自主机处理器的“RESET”或“Wakeup”信号。这
 *因此芯片消耗的电流非常少，这有助于延长便携式设备的待机时间。
 * Hibernation Mode
 *In this mode, the chip is set in a power down mode. It shall respond to the “RESET” or “Wakeup” signal from the host processor. The
 *chip therefore consumes very little current, which help prolong the standby time for the portable devices.
 *
 *

 */
#include <stdint.h>
#include "i2c.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif





typedef enum 
{
Mode_Switch = 0x00, // 模式切换 写0x40到寄存器0x00切换到工厂模式读出值为0 (RW)
TD_STATUS = 0x02, // 报点个数 范围: 0x00~0x02 (RO)
P1_XH = 0x03, // 第一个点的x坐标高四位 bit7-bit6为第1点触摸事件标志，bit3-bit0为第1点的X坐标高4位。(RO)
P1_XL = 0x04,// 第一个点的x坐标低八位(RO)
P1_YH = 0x05, // 第一个点的y坐标高四位 bit7-bit4为第1点的ID，bit3-bit0为第1点的Y坐标高4位。(RO)
P1_YL = 0x06,// 第一个点的y坐标低八位(RO)
P1_WEIGHT = 0x07, // 第一个点的权重(RO)
P1_MISC = 0x08, // 第一个点的其他信息(RO)

P2_XH = 0x09, // 第二个点的x坐标高四位 bit7-bit6为第1点触摸事件标志，bit3-bit0为第1点的X坐标高4位。(RO)
P2_XL = 0x0A,// 第二个点的x坐标低八位(RO)
P2_YH = 0x0B, // 第二个点的y坐标高四位 bit7-bit4为第1点的ID，bit3-bit0为第1点的Y坐标高4位。(RO)
P2_YL = 0x0C,// 第二个点的y坐标低八位(RO)
P2_WEIGHT = 0x0D, // 第二个点的权重(RO)
P2_MISC = 0x0E, // 第二个点的其他信息(RO)

ID_G_THGROUP = 0X80, // 触摸阈值 触摸阈值 / 16 (RW)
ID_G_PEAKTH = 0x81, // 峰值阈值 ID_G_UCPEAKTH * TH_GROUP_DIVISOR = RV_G_PEAK_TH (RW)
ID_G_THDIFF = 0x85,// 点滤波范围阈值 default : 0xA0, 范围: 0x00~0xFF 。 约等于RV_G_THDIFF/16 (RW)
ID_G_CTRL = 0x86, // monitor模式开关 default : 0x00。 0x00 : 禁止  | 0x01 : 允许 (RW)
ID_G_TIMEENTERMONITOR = 0x87, // 没触摸进入monitor模式的延时 default : 0x1E 范围: 0x00~0x64 (RW)
                              //在指定时间内没有触摸则进入MONITOR状态，需要与参数“monitor模式开关”配合使用。单位为秒。 (RW)
ID_G_PERIODACTIVE = 0x88, // 工作模式扫描周期 default : 0x08 范围: 0x40~0x41。 用于控制报点率 (RW)
ID_G_PERIODMONITOR = 0x89, // monitor模式扫描周期  default : 0x08 范围: 0x40~0x41。 用于控制报点率 (RW)
ID_G_FRQHOPFLG = 0x8A, // 跳频标志位 default : 0x00 范围: 0x00~0x01 (RW)
ID_G_FREQ_HOPPING_EN = 0x8B, // 充电器标识 default : 0x00 范围: 0x00~0x01 (RW)
ID_G_CURFREQIDX = 0x8C, // 当前工作频点 (RW)

ID_G_TEST_MODE_FILTER = 0x96, // 量产测试模式下Alpha滤波使能
ID_G_CIPHER_MID = 0x9F, // 芯片代号（中字节） default : 0x26 范围: 0x00~0xff
ID_G_CIPHER_LOW = 0xA0, // 芯片代号（低字节） default: : 0x00 范围: 0x00~0x03
ID_G_LIB_VERSION_H = 0xA1,//APP 库文件版本号高字节
ID_G_LIB_VERSION_L = 0xA2, // APP 库文件版本号低字节
ID_G_CIPHER_HIGH = 0xA3,// 芯片代号
ID_G_MODE = 0xA4, // 报点INT模式 "用于控制报点INT模式
                  //0x00 : 不拉长报点时INT低电平时间
                  //0x01 : 拉长报点时INT低电平时间"
ID_G_PMODE = 0xA5, // 芯片的工作功耗模式
                  // 功耗模式：
                  // 0x00 : P_ACTIVE      // 活跃模式，芯片处于正常工作状态，功耗最大
                  // 0x01 : P_MONITOR     // 监控模式，芯片处于部分功能开启状态，功耗适中
                  // 0x02 : P_STANDBY     // 待机模式，芯片仅保持最基本的功能，功耗较低
                  // 0x03 : P_HIBERNATE   // 休眠模式，芯片几乎完全关闭，仅保留唤醒功能，功耗最低
ID_G_FIRMID = 0xA6, // 芯片的固件版本号


ID_G_FACTORY_MODE = 0xAE, // 工厂模式种类，默认值：0x00
                          // 工厂模式：
                          // 0x00 : F_NORMAL       // 正常模式，非测试状态
                          // 0x01 : F_TESTMODE_1   // 测试模式1，用于特定测试流程
                          // 0x02 : F_TESTMODE_2   // 测试模式2，用于其他测试场景

ID_G_FACE_DEC_MODE = 0xB0, // 近距离感应使能 default : 0x00 disable : 0x00 enable : 0x01
ID_G_STATE = 0xB1, // 工作模式，默认值：0x01
                   // "写0xAA, 0x55到这个寄存器可触发升级流程
                   // 0x00 : INFO模式            // 信息模式，用于查询或调试
                   // 0x01 : 正常工作模式         // 设备处于正常运行状态
                   // 0x03 : 工厂模式            // 工厂模式，用于生产或测试
                   // 0x04 : 自动校正模式         // 用于自动校正配置的模式


ID_G_SPEC_GESTURE_ENABLE = 0xD0, // 特殊手势模式使能寄存器
                                 // "A. 进入手势状态：在正常工作状态下向 0xD0 寄存器写 1；
                                 //    退出手势状态：向 0xD0 寄存器写 0。
                                 //  B. 使能手势功能：向 0xD1, 0xD2, 0xD5—0xD8 寄存器对应的位写 1，即可使能相应手势。"

// 手势相关寄存器说明
// 寄存器  |  Bit7      Bit6         Bit5               Bit4       Bit3           Bit2          Bit1        Bit0
//  0xD1   |  reserve   reserve      数字字符识别总开关    双击使能    下滑直线       上滑直线       右滑直线      左滑直线
//  0xD2   |     d          a           g                   c           e              m              w           o
//  0xD3   |                                          手势ID号（由手势检测模块生成）
//  0xD4   |                                          reserve（保留位）
//  0xD5   |     u          s           P                   L           q              b           保留           n
//  0xD6   |   保留        保留        △                   v           ^              >           保留           @
//  0xD7   |   保留        保留        2                   8           7              9           6              3
//  0xD8   |   保留        保留        保留                保留         r              y           k              h
//  0xD9   |                                           保留（保留位）
//  0xDA   |                                           DEBUG_USE（调试用途）

// 示例说明：
// 1. 若要使能“左滑直线”手势，需向 0xD1 寄存器的 Bit0 写 1。
// 2. 若要使能“数字字符识别”功能总开关，需向 0xD1 寄存器的 Bit5 写 1。
// 3. 手势 ID 信息存储在 0xD3 中，用于标识当前检测到的手势。

}CTP_Register_t; // 触摸信号寄存器

typedef struct 
{
GPIO_TypeDef *Port;
uint16_t Pin;
}CTP_gpio_t;

typedef enum
{
    P_ACTIVE = 0x00,    // 活跃模式，芯片处于正常工作状态，功耗最大
    P_MONITOR = 0x01,   // 监控模式，芯片处于部分功能开启状态，功耗适中
    P_STANDBY = 0x02,   // 待机模式，芯片仅保持最基本的功能，功耗较低
    P_HIBERNATE = 0x03, // 休眠模式，芯片几乎完全关闭，仅保留唤醒功能，功耗最低
} CTP_Power_t;          // 功耗

typedef struct 
{
    uint8_t CTP_INT : 1; // 中断标志
    uint8_t DMA :1; // DMA开关
    uint8_t Reserve_bit :6; // 保留位
}CTP_Flag_t; // 触摸信号标志

typedef struct
{
    struct
    {
        uint16_t x1; // 第一个触控点的x坐标
        uint16_t x2; // 第二个触控点的x坐标
        uint16_t y1; // 第一个触控点的y坐标
        uint16_t y2; // 第二个触控点的y坐标
        // CTP_Register_t reg; // 当前要操作的寄存器
    } signal;

    struct
    {
        I2C_HandleTypeDef *handle; // 句柄
        const uint8_t addr;        // 地址
        uint8_t *byte;             // 要发送的字节指针
        volatile size_t size;      // 要发送的字节大小
        uint8_t *buf;              // 接收缓冲区
        uint16_t Timeout;          // 超时时间
    } i2c;

    volatile CTP_Flag_t flag; // 标志
    struct
    {
        CTP_gpio_t RST; // 复位引脚
    } Hardware;
} CTP_Class_t; // 触摸信号对象

 /**
 * @brief Get the Active Touch Points object
 *         获取当前有几个触控点
 * @param ctp 触摸对象
 * @return uint8_t 触控点的数量
 */
extern uint8_t getActiveTouchPoints(CTP_Class_t *ctp);

/**
 * @brief Get the Touch X object
 *        获取触控点的x坐标
 *
 * @param ctp   触摸对象
 * @param point 触控点数量
 */
extern void getTouchX(CTP_Class_t *ctp, uint8_t point);

/**
 * @brief Get the Touch Y object
 *         获取当前触控点的x坐标
 * 
 * @param ctp  
 * @param point  触控点数量
 */
extern void getTouchY(CTP_Class_t *ctp, uint8_t point);


/**
 * @brief 中断回调
 * @note 请将这个函数放入INT引脚的中断回调里，否则无法正常工作
 * 
 * @param ctp 
 */
extern void CTPCallback(CTP_Class_t *ctp);

/**
 * @brief Set the Touch Monitor Mode object
 *        设置屏幕是否进入低功耗模式
 *
 * @param ctp   触摸对象
 * @param mode  ON or OFF (0 or 1)
 * @param ...   如果是ON，请传入触摸进入monitor模式的延时（单位：秒），如果没传入，则默认3秒
 *
 */
extern void setTouchMonitorMode(CTP_Class_t *ctp, uint8_t mode, ...);

/**
 * @brief Set the Touch Power Mode object
 *          设置功耗模式
 * 
 * @param ctp 
 * @param CTP_Power_t mode 
 */
extern void setTouchPowerMode(CTP_Class_t *ctp,CTP_Power_t mode);

/**
 * @brief 初始化
 *
 * @param ctp
 */
extern void CTP_Init(CTP_Class_t *ctp);

/**
 * @brief 触控信号主循环 请将此函数放入主循环中，否则无法正常工作
 * 
 * @param ctp 
 */
extern void CTP_mainLoop(CTP_Class_t *ctp);

/**
 * @brief 复位触控功能
 * 
 * @param ctp 
 */
extern void resetTouchController(CTP_Class_t *ctp);

#ifdef __cplusplus
}
#endif

#endif


