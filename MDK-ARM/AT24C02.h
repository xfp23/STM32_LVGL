#ifndef AT24C02_H
#define AT24C02_H

/**
 * @file AT24C02.h
 * @author xfp23 (@github.com/xfp23)
 * @brief 此文件是AT24c02的驱动
 * @version 0.1
 * @date 2025-01-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/* AT24C01D 内部组织为 16 页，每页 8 字节。 AT24C02D内部是 组织为 32 页，每页 8 字节。*/

/* 4 位设备类型标识符后面是硬件从地址位 A2、A1 和 A0。这些位
可用于扩展地址空间，允许在同一块上最多允许八个串行 EEPROM 器件 硬连线设备地址输入引脚 A0、A1 和 A2。 A0、A1 和 A2 引脚使用内部专有的
如果允许引脚浮动，该电路会自动将引脚偏置为逻辑“0”状态。为了操作在各种应用环境中，下拉机制被有意强制下拉 */
#include "i2c.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define AT24C02_ADDR 0xA0

typedef union
{
    struct
    {
        struct
        {
            I2C_HandleTypeDef *handle; // 句柄
            uint8_t addr;              // 地址
        } i2c;
    };

} AT24C02_Class_t;

#ifdef __cplusplus
}
#endif
#endif