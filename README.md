[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL375

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl375/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The ADXL375 is a small, thin, 3-axis accelerometer that provides low power consumption and high resolution measurement up to ±200 g. The digital output data is formatted as 16-bit, twos complement data and is accessible through a SPI (3- or 4-wire) or I2C digital interface.An integrated memory management system with a 32-level first in, first out (FIFO) buffer can be used to store data to minimize host processor activity and lower overall system power consumption.Low power modes enable intelligent motion-based power management with threshold sensing and active acceleration measurement at extremely low power dissipation.

LibDriver ADXL375 is a full-featured driver of ADXL375 launched by LibDriver. It provides acceleration reading, acceleration FIFO mode acquisition,  activity /inactivity state detection, shock detection and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver ADXL375 source files.

/interface includes LibDriver ADXL375 IIC and SPI platform independent template.

/test includes LibDriver ADXL375 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver ADXL375 sample code.

/doc includes LibDriver ADXL375 offline document.

/datasheet includes ADXL375 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC and SPI platform independent template and finish your platform IIC and SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

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

### Document

Online documents: [https://www.libdriver.com/docs/adxl375/index.html](https://www.libdriver.com/docs/adxl375/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.