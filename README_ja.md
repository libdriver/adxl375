[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL375

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl375/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL375 は、低消費電力で最大 ±200 g の高解像度測定を実現する小型薄型 3 軸加速度計です。デジタル出力データは 16 ビットの 2 の補数データとしてフォーマットされ、SPI (3 線または 4 線) または I2C デジタル インターフェイスを介してアクセスできます。32 レベルの先入れ先出し (FIFO) バッファを備えた統合メモリ管理システムを使用してデータを保存し、ホスト プロセッサのアクティビティを最小限に抑えてシステム全体の消費電力を削減できます。低消費電力モードでは、しきい値検知とアクティブ加速度測定によるインテリジェントなモーションベースの電力管理が、極めて低い消費電力で実現します。

LibDriver ADXL375 は、LibDriver によってリリースされた ADXL375のフル機能ドライバーです。加速読み取り、加速 FIFO モード取得、アクティビティ/非アクティビティ状態の検出、衝撃検出などの機能を提供します。LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver ADXL375のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver ADXL375用のプラットフォームに依存しないIIC, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver ADXL375ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver ADXL375プログラミング例が含まれています。

/ docディレクトリには、LibDriver ADXL375オフラインドキュメントが含まれています。

/ datasheetディレクトリには、ADXL375データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC, SPIバステンプレートを参照して、指定したプラットフォームのIIC, SPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/adxl375/index.html](https://www.libdriver.com/docs/adxl375/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。