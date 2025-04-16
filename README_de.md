[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL375

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl375/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der ADXL375 ist ein kleiner, dünner 3-Achsen-Beschleunigungsmesser mit geringem Stromverbrauch und hoher Messauflösung bis zu ±200 g. Die digitalen Ausgangsdaten sind als 16-Bit-Zweierkomplementdaten formatiert und über eine SPI- (3- oder 4-adrig) oder I2C-Digitalschnittstelle zugänglich. Ein integriertes Speicherverwaltungssystem mit einem 32-stufigen FIFO-Puffer (First In, First Out) kann zum Speichern von Daten verwendet werden, um die Aktivität des Hostprozessors zu minimieren und den Gesamtstromverbrauch des Systems zu senken. Energiesparmodi ermöglichen eine intelligente bewegungsbasierte Energieverwaltung mit Schwellenwerterkennung und aktiver Beschleunigungsmessung bei extrem geringer Verlustleistung.

LibDriver ADXL375 ist der voll funktionsfähige Treiber von ADXL375, der von LibDriver eingeführt wurde. Er bietet Beschleunigungsmessung, Beschleunigungserfassung im FIFO-Modus, Aktivitäts-/Inaktivitätszustandserkennung, Stoßerkennung und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver ADXL375-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver ADXL375 IIC, SPI.

/test enthält den Testcode des LibDriver ADXL375-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver ADXL375-Beispielcode.

/doc enthält das LibDriver ADXL375-Offlinedokument.

/Datenblatt enthält ADXL375-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/adxl375/index.html](https://www.libdriver.com/docs/adxl375/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.