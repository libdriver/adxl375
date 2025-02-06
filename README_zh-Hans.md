[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL375

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl375/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL375是一款小而薄的3轴加速度计，可提供低功耗和高达±200g的高分辨率测量。数字输出数据格式为16位二进制补码数据，可通过SPI（3线或4线）或I2C数字接口访问。具有32级先进先出（FIFO）缓冲器的集成存储器管理系统可以最小化主机处理器活动并降低整体系统功耗。低功耗模式实现了基于运动的智能电源管理，具有阈值感应和主动加速度测量功能，功耗极低。

LibDriver ADXL375是LibDriver推出的ADXL375的全功能驱动，该驱动提供加速度读取、加速度FIFO模式采集、运动非运动状态检测、震动检测等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver ADXL375的源文件。

/interface目录包含了LibDriver ADXL375与平台无关的IIC、SPI总线模板。

/test目录包含了LibDriver ADXL375驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver ADXL375编程范例。

/doc目录包含了LibDriver ADXL375离线文档。

/datasheet目录包含了ADXL375数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC、SPI总线模板，完成指定平台的IIC、SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_adxl375_basic.h"

uint8_t res;
uint8_t i;
float g[3];

res = adxl375_basic_init(ADXL375_INTERFACE_IIC, ADXL375_ADDRESS_ALT_0);                
if (res != 0)
{
    return 1;
}

...
    
for (i = 0; i < 3; i++)
{
    res = adxl375_basic_read((float *)g);
    if (res != 0)
    {
        (void)adxl375_basic_deinit();
        
        return 1;
    }
    adxl375_interface_debug_print("adxl375: x is %0.3f.\n", g[0]);
    adxl375_interface_debug_print("adxl375: y is %0.3f.\n", g[1]);
    adxl375_interface_debug_print("adxl375: z is %0.3f.\n", g[2]);
    adxl375_interface_delay_ms(1000);
    
    ...
        
}

...
    
(void)adxl375_basic_deinit();

return 0;
```

#### example fifo

```C
#include "driver_adxl375_fifo.h"

uint8_t res;
float g[3];

void fifo_callback(float (*g)[3], uint16_t len)
{
    ...
    
    return 0;
}

res = gpio_interrupt_init(adxl375_fifo_irq_handler);
if (res != 0)
{
    return 1;
}
res = adxl375_fifo_init(ADXL375_INTERFACE_IIC, ADXL375_ADDRESS_ALT_0, fifo_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

while (1)
{
    adxl375_interface_delay_ms(10);
    
    ...    
}
...
    
(void)gpio_interrupt_deinit();
(void)adxl375_fifo_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_adxl375_interrupt.h"

uint8_t res;

void interrupt_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL375_INTERRUPT_DATA_READY :
        {
            ...
                
            break;
        }
        case ADXL375_INTERRUPT_SINGLE_SHOCK :
        {
            adxl375_interface_debug_print("adxl375: irq single shock.\n");
         
            ...
                
            break;
        }
        case ADXL375_INTERRUPT_DOUBLE_SHOCK :
        {
            adxl375_interface_debug_print("adxl375: irq double shock.\n");
            
            ...
                
            break;
        }
        case ADXL375_INTERRUPT_ACTIVITY :
        {
            adxl375_interface_debug_print("adxl375: irq activity.\n");
            
            ...
                
            break;
        }
        case ADXL375_INTERRUPT_INACTIVITY :
        {
            adxl375_interface_debug_print("adxl375: irq inactivity.\n");
            
            ...
                
            break;
        }
        case ADXL375_INTERRUPT_OVERRUN :
        {
            ...
                
            break;
        }
        default :
        {
            ...
                
            break;
        }
    }
}

res = gpio_interrupt_init(adxl375_interrupt_irq_handler);
if (res != 0)
{
    return 1;
}
res = adxl375_interrupt_init(ADXL375_INTERFACE_SPI, ADXL375_ADDRESS_ALT_0,
                             interrupt_callback,
                             ADXL375_BOOL_TRUE 
                             ADXL375_BOOL_TRUE,
                             ADXL375_BOOL_TRUE);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

...
    
while (1)
{
    if (adxl375_interrupt_server() != 0)
    {
        return 1;
    }
    adxl375_interface_delay_ms(10);
    
    ...
}

...
    
(void)gpio_interrupt_deinit();
(void)adxl375_interrupt_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/adxl375/index.html](https://www.libdriver.com/docs/adxl375/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。