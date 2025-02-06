[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL375

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl375/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL375是一款小而薄的3軸加速度計，可提供低功耗和高達±200g的高解析度量測。 數位輸出數據格式為16比特二進位補數數據，可通過SPI（3線或4線）或I2C數位介面訪問。 具有32級先進先出（FIFO）緩衝器的集成記憶體管理系統可以最小化主機處理器活動並降低整體系統功耗。 低功耗模式實現了基於運動的智慧電源管理，具有閾值感應和主動加速度量測功能，功耗極低。

LibDriver ADXL375是LibDriver推出的ADXL375的全功能驅動，該驅動提供加速度讀取、加速度FIFO模式採集、運動非運動狀態檢測、震動檢測等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver ADXL375的源文件。

/interface目錄包含了LibDriver ADXL375與平台無關的IIC、SPI總線模板。

/test目錄包含了LibDriver ADXL375驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver ADXL375編程範例。

/doc目錄包含了LibDriver ADXL375離線文檔。

/datasheet目錄包含了ADXL375數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、SPI總線模板，完成指定平台的IIC、SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/adxl375/index.html](https://www.libdriver.com/docs/adxl375/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。