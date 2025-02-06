### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(adxl375 REQUIRED)
```

### 3. ADXL375

#### 3.1 Command Instruction

1. Show adxl375 chip and driver information.

   ```shell
   adxl375 (-i | --information)
   ```

2. Show adxl375 help.

   ```shell
   adxl375 (-h | --help)
   ```

3. Show adxl375 pin connections of the current board.

   ```shell
   adxl375 (-p | --port)
   ```

4. Run adxl375 register test.

   ```shell
   adxl375 (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

5. Run adxl375 read test, num means the test times.

   ```shell
   adxl375 (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

6. Run adxl375 fifo test.

   ```shell
   adxl375 (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

7. Run adxl375 interrupt test.

   ```shell
   adxl375 (-t int | --test=int) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

8. Run adxl375 basic function, num is the read times.

   ```shell
   adxl375 (-e basic | --example=basic) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

9. Run adxl375 fifo function, num is the read times.

   ```shell
   adxl375 (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

10. Run adxl375 interrupt function, mask is the interrupt mask, bit 0 is the tap enable mask, bit 1 is the action enable mask, bit 2 is the inaction enable mask.

    ```shell
    adxl375 (-e int | --example=int) [--addr=<0 | 1>] [--interface=<iic | spi>] [--mask=<msk>]
    ```

#### 3.2 Command Example

```shell
./adxl375 -i

adxl375: chip is Analog Devices ADXL375.
adxl375: manufacturer is Analog Devices.
adxl375: interface is IIC SPI.
adxl375: driver version is 1.0.
adxl375: min supply voltage is 2.0V.
adxl375: max supply voltage is 3.6V.
adxl375: max current is 0.14mA.
adxl375: max temperature is 85.0C.
adxl375: min temperature is -40.0C.
```

```shell
./adxl375 -p

adxl375: SPI interface SCK connected to GPIO11(BCM).
adxl375: SPI interface MISO connected to GPIO9(BCM).
adxl375: SPI interface MOSI connected to GPIO10(BCM).
adxl375: SPI interface CS connected to GPIO8(BCM).
adxl375: IIC interface SCL connected to GPIO3(BCM).
adxl375: IIC interface SDA connected to GPIO2(BCM).
adxl375: INT connected to GPIO17(BCM).
```

```shell
./adxl375 -t reg --interface=spi

adxl375: chip is Analog Devices ADXL375.
adxl375: manufacturer is Analog Devices.
adxl375: interface is IIC SPI.
adxl375: driver version is 1.0.
adxl375: min supply voltage is 2.0V.
adxl375: max supply voltage is 3.6V.
adxl375: max current is 0.14mA.
adxl375: max temperature is 85.0C.
adxl375: min temperature is -40.0C.
adxl375: start register test.
adxl375: adxl375_set_interface/adxl375_get_interface test.
adxl375: set interface iic.
adxl375: check interface ok.
adxl375: set interface spi.
adxl375: check interface ok.
adxl375: adxl375_set_addr_pin/adxl375_get_addr_pin test.
adxl375: set addr pin alt0.
adxl375: check addr pin ok.
adxl375: set addr pin alt1.
adxl375: check addr pin ok.
adxl375: adxl375_set_shock_threshold/adxl375_get_shock_threshold test.
adxl375: set shock threshold 0x67.
adxl375: check shock threshold ok.
adxl375: adxl375_set_offset/adxl375_get_offset test.
adxl375: set x offset 70.
adxl375: set y offset -105.
adxl375: set z offset 115.
adxl375: check x offset ok.
adxl375: check y offset ok.
adxl375: check z offset ok.
adxl375: adxl375_set_duration/adxl375_get_duration test.
adxl375: set duration 0x51.
adxl375: check duration ok.
adxl375: adxl375_set_latent/adxl375_get_latent test.
adxl375: set latent 0xFF.
adxl375: check latent ok.
adxl375: adxl375_set_window/adxl375_get_window test.
adxl375: set window 0x4A.
adxl375: check window ok.
adxl375: adxl375_set_action_threshold/adxl375_get_action_threshold test.
adxl375: set action threshold 0xEC.
adxl375: check action threshold ok.
adxl375: adxl375_set_inaction_threshold/adxl375_get_inaction_threshold test.
adxl375: set inaction threshold 0x29.
adxl375: check inaction threshold ok.
adxl375: adxl375_set_inaction_time/adxl375_get_inaction_time test.
adxl375: set inaction time 0xCD.
adxl375: check inaction time ok.
adxl375: adxl375_set_action_inaction/adxl375_get_action_inaction test.
adxl375: set action x enable.
adxl375: check action inaction ok.
adxl375: set action x disable.
adxl375: check action inaction ok.
adxl375: set action y enable.
adxl375: check action inaction ok.
adxl375: set action y disable.
adxl375: check action inaction ok.
adxl375: set action z enable.
adxl375: check action inaction ok.
adxl375: set action z disable.
adxl375: check action inaction ok.
adxl375: set inaction x enable.
adxl375: check action inaction ok.
adxl375: set inaction x disable.
adxl375: check action inaction ok.
adxl375: set inaction y enable.
adxl375: check action inaction ok.
adxl375: set inaction y disable.
adxl375: check action inaction ok.
adxl375: set inaction z enable.
adxl375: check action inaction ok.
adxl375: set inaction z disable.
adxl375: check action inaction ok.
adxl375: adxl375_set_action_coupled/adxl375_get_action_coupled test.
adxl375: set action coupled dc.
adxl375: check action coupled ok.
adxl375: set action coupled ac.
adxl375: check action coupled ok.
adxl375: adxl375_set_inaction_coupled/adxl375_get_inaction_coupled test.
adxl375: set inaction coupled dc.
adxl375: check inaction coupled ok.
adxl375: set inaction coupled ac.
adxl375: check inaction coupled ok.
adxl375: adxl375_set_shock_axis/adxl375_get_shock_axis test.
adxl375: set shock axis x enable.
adxl375: check shock axis ok.
adxl375: set shock axis x disable.
adxl375: check shock axis ok.
adxl375: set shock axis y enable.
adxl375: check shock axis ok.
adxl375: set shock axis y disable.
adxl375: check shock axis ok.
adxl375: set shock axis z enable.
adxl375: check shock axis ok.
adxl375: set shock axis z disable.
adxl375: check shock axis ok.
adxl375: adxl375_set_shock_suppress/adxl375_get_shock_suppress test.
adxl375: set shock suppress enable.
adxl375: check shock suppress ok.
adxl375: set shock suppress disable.
adxl375: check shock suppress ok.
adxl375: get shock status test.
adxl375: shock status is 0x0A.
adxl375: adxl375_set_rate/adxl375_get_rate test.
adxl375: set rate 0.1Hz.
adxl375: check rate ok.
adxl375: set rate 0.2Hz.
adxl375: check rate ok.
adxl375: set rate 0.39Hz.
adxl375: check rate ok.
adxl375: set rate 0.78Hz.
adxl375: check rate ok.
adxl375: set rate 1.56Hz.
adxl375: check rate ok.
adxl375: set rate 3.13Hz.
adxl375: check rate ok.
adxl375: set rate 6.25Hz.
adxl375: check rate ok.
adxl375: set rate 12.5Hz.
adxl375: check rate ok.
adxl375: set rate 25Hz.
adxl375: check rate ok.
adxl375: set rate 50Hz.
adxl375: check rate ok.
adxl375: set rate 100Hz.
adxl375: check rate ok.
adxl375: set rate 200Hz.
adxl375: check rate ok.
adxl375: set rate 400Hz.
adxl375: check rate ok.
adxl375: set rate 800Hz.
adxl375: check rate ok.
adxl375: set rate 1600Hz.
adxl375: check rate ok.
adxl375: set rate 3200Hz.
adxl375: check rate ok.
adxl375: set rate low 12.5Hz.
adxl375: check rate ok.
adxl375: set rate low 25Hz.
adxl375: check rate ok.
adxl375: set rate low 50Hz.
adxl375: check rate ok.
adxl375: set rate low 100Hz.
adxl375: check rate ok.
adxl375: set rate low 200Hz.
adxl375: check rate ok.
adxl375: set rate low 400Hz.
adxl375: check rate ok.
adxl375: adxl375_set_interrupt/adxl375_get_interrupt test.
adxl375: set interrupt data ready enable.
adxl375: check interrupt ok.
adxl375: set interrupt data ready disable.
adxl375: check interrupt ok.
adxl375: set interrupt single enable.
adxl375: check interrupt ok.
adxl375: set interrupt single disable.
adxl375: check interrupt ok.
adxl375: set interrupt double enable.
adxl375: check interrupt ok.
adxl375: set interrupt double disable.
adxl375: check interrupt ok.
adxl375: set interrupt activity enable.
adxl375: check interrupt ok.
adxl375: set interrupt activity disable.
adxl375: check interrupt ok.
adxl375: set interrupt inactivity enable.
adxl375: check interrupt ok.
adxl375: set interrupt inactivity disable.
adxl375: check interrupt ok.
adxl375: set interrupt watermark enable.
adxl375: check interrupt ok.
adxl375: set interrupt watermark disable.
adxl375: check interrupt ok.
adxl375: set interrupt overrun enable.
adxl375: check interrupt ok.
adxl375: set interrupt overrun disable.
adxl375: check interrupt ok.
adxl375: adxl375_set_interrupt_map/adxl375_get_interrupt_map test.
adxl375: set interrupt map data ready pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt map data ready pin2.
adxl375: check interrupt map ok.
adxl375: set interrupt map single shock pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt map single pin2.
adxl375: check interrupt map ok.
adxl375: set interrupt map double shock pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt map double pin2.
adxl375: check interrupt map ok.
adxl375: set interrupt map activity pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt activity pin2.
adxl375: check interrupt map ok.
adxl375: set interrupt map inactivity pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt inactivity pin2.
adxl375: check interrupt map ok.
adxl375: set interrupt map watermark pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt watermark pin2.
adxl375: check interrupt map ok.
adxl375: set interrupt map overrun pin1.
adxl375: check interrupt map ok.
adxl375: set interrupt overrun pin2.
adxl375: check interrupt map ok.
adxl375: adxl375_get_interrupt_source test.
adxl375: check interrupt source 0x00.
adxl375: adxl375_set_self_test/adxl375_get_self_test test.
adxl375: set self test enable.
adxl375: check self test ok.
adxl375: set self test disable.
adxl375: check self test ok.
adxl375: adxl375_set_spi_wire/adxl375_get_spi_wire test.
adxl375: set self test enable.
adxl375: check self test ok.
adxl375: set self test disable.
adxl375: check self test ok.
adxl375: adxl375_set_interrupt_active_level/adxl375_get_interrupt_active_level test.
adxl375: set interrupt active level high.
adxl375: check interrupt active level ok.
adxl375: set interrupt active level low.
adxl375: check interrupt active level ok.
adxl375: adxl375_set_justify/adxl375_get_justify test.
adxl375: set justify right.
adxl375: check justify ok.
adxl375: set justify left.
adxl375: check justify ok.
adxl375: adxl375_set_mode/adxl375_get_mode test.
adxl375: set mode bypass.
adxl375: check mode ok.
adxl375: set mode fifo.
adxl375: check mode ok.
adxl375: set mode stream.
adxl375: check mode ok.
adxl375: set mode trigger.
adxl375: check mode ok.
adxl375: adxl375_set_trigger_pin/adxl375_get_trigger_pin test.
adxl375: set trigger pin 1.
adxl375: check trigger pin ok.
adxl375: set trigger pin 2.
adxl375: check trigger pin ok.
adxl375: adxl375_set_watermark/adxl375_get_watermark test.
adxl375: set watermark 0x1A.
adxl375: check watermark ok.
adxl375: adxl375_get_watermark_level test.
adxl375: watermark level is 0x00.
adxl375: adxl375_get_trigger_status test.
adxl375: trigger status is 0x00.
adxl375: adxl375_set_link_activity_inactivity/adxl375_get_link_activity_inactivity test.
adxl375: enable link activity inactivity.
adxl375: check link activity inactivity ok.
adxl375: disable link activity inactivity.
adxl375: check link activity inactivity ok.
adxl375: adxl375_set_auto_sleep/adxl375_get_auto_sleep test.
adxl375: enable auto sleep.
adxl375: check auto sleep ok.
adxl375: disable auto sleep.
adxl375: check auto sleep ok.
adxl375: adxl375_set_measure/adxl375_get_measure test.
adxl375: enable measure.
adxl375: check measure ok.
adxl375: disable measure.
adxl375: check measure ok.
adxl375: adxl375_set_sleep/adxl375_get_sleep test.
adxl375: enable sleep.
adxl375: check sleep ok.
adxl375: disable sleep.
adxl375: check sleep ok.
adxl375: adxl375_set_sleep_frequency/adxl375_get_sleep_frequency test.
adxl375: set sleep frequency 8 Hz.
adxl375: check sleep frequency ok.
adxl375: set sleep frequency 4 Hz.
adxl375: check sleep frequency ok.
adxl375: set sleep frequency 2 Hz.
adxl375: check sleep frequency ok.
adxl375: set sleep frequency 1 Hz.
adxl375: check sleep frequency ok.
adxl375: adxl375_shock_threshold_convert_to_register/adxl375_shock_threshold_convert_to_data test.
adxl375: shock threshold before is 4.2700.
adxl375: shock threshold register is 0x05.
adxl375: shock threshold convert is 3.9000.
adxl375: adxl375_offset_convert_to_register/adxl375_offset_convert_to_data test.
adxl375: offset before is 0.9000.
adxl375: offset register is 0x04.
adxl375: offset convert is 0.7840.
adxl375: adxl375_duration_convert_to_register/adxl375_duration_convert_to_data test.
adxl375: duration before is 5625.
adxl375: duration register is 0x09.
adxl375: duration convert is 5625.
adxl375: adxl375_latent_convert_to_register/adxl375_latent_convert_to_data test.
adxl375: latent before is 16.3000.
adxl375: latent register is 0x0D.
adxl375: latent convert is 16.2500.
adxl375: adxl375_window_convert_to_register/adxl375_window_convert_to_data test.
adxl375: window before is 3.2600.
adxl375: window register is 0x02.
adxl375: window convert is 2.5000.
adxl375: adxl375_action_threshold_convert_to_register/adxl375_action_threshold_convert_to_data test.
adxl375: action threshold before is 5.4000.
adxl375: action threshold register is 0x06.
adxl375: action threshold convert is 4.6800.
adxl375: adxl375_inaction_threshold_convert_to_register/adxl375_inaction_threshold_convert_to_data test.
adxl375: inaction threshold before is 2.2600.
adxl375: inaction threshold register is 0x02.
adxl375: inaction threshold convert is 1.5600.
adxl375: adxl375_inaction_time_convert_to_register/adxl375_inaction_time_convert_to_data test.
adxl375: inaction time before is 0x54.
adxl375: inaction time register is 0x54.
adxl375: inaction time convert is 0x54.
adxl375: finish register test.
```

```shell
./adxl375 -t read --interface=spi --times=3

adxl375: chip is Analog Devices ADXL375.
adxl375: manufacturer is Analog Devices.
adxl375: interface is IIC SPI.
adxl375: driver version is 1.0.
adxl375: min supply voltage is 2.0V.
adxl375: max supply voltage is 3.6V.
adxl375: max current is 0.14mA.
adxl375: max temperature is 85.0C.
adxl375: min temperature is -40.0C.
adxl375: start read test.
adxl375: set align right.
x is 0.15 g.
y is 0.20 g.
z is 0.59 g.
x is 0.78 g.
y is 1.23 g.
z is 0.24 g.
x is 0.78 g.
y is 1.13 g.
z is 0.29 g.
adxl375: set align left.
x is 0.59 g.
y is 0.73 g.
z is 1.03 g.
x is 0.20 g.
y is 0.39 g.
z is 0.88 g.
x is 0.20 g.
y is 0.59 g.
z is 0.93 g.
adxl375: set rate 0.1Hz.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
adxl375: set rate 0.2Hz.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
adxl375: set rate 0.39Hz.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
x is 0.34 g.
y is 0.39 g.
z is 0.88 g.
adxl375: set rate 0.78Hz.
x is 1.03 g.
y is 0.44 g.
z is -0.05 g.
x is 1.03 g.
y is 0.44 g.
z is -0.05 g.
x is 1.03 g.
y is 0.49 g.
z is -0.05 g.
adxl375: set rate 1.56Hz.
x is 1.08 g.
y is 0.49 g.
z is -0.05 g.
x is 0.93 g.
y is 0.44 g.
z is -0.05 g.
x is 1.08 g.
y is 0.49 g.
z is -0.10 g.
adxl375: set rate 3.13Hz.
x is 1.13 g.
y is 0.39 g.
z is -0.05 g.
x is 1.08 g.
y is 0.44 g.
z is -0.05 g.
x is 1.08 g.
y is 0.49 g.
z is -0.05 g.
adxl375: set rate 6.25Hz.
x is 1.03 g.
y is 0.39 g.
z is -0.05 g.
x is 1.08 g.
y is 0.44 g.
z is -0.10 g.
x is 1.08 g.
y is 0.44 g.
z is -0.10 g.
adxl375: set rate 12.5Hz.
x is 1.08 g.
y is 0.39 g.
z is -0.10 g.
x is 1.08 g.
y is 0.44 g.
z is -0.05 g.
x is 1.08 g.
y is 0.44 g.
z is -0.05 g.
adxl375: set rate 25Hz.
x is 1.08 g.
y is 0.44 g.
z is -0.15 g.
x is 1.13 g.
y is 0.39 g.
z is -0.05 g.
x is 1.08 g.
y is 0.44 g.
z is -0.10 g.
adxl375: set rate 50Hz.
x is 1.08 g.
y is 0.44 g.
z is -0.10 g.
x is 1.08 g.
y is 0.39 g.
z is -0.05 g.
x is 1.08 g.
y is 0.44 g.
z is -0.10 g.
adxl375: set rate 100Hz.
x is 1.08 g.
y is 0.44 g.
z is -0.05 g.
x is 1.13 g.
y is 0.88 g.
z is 0.15 g.
x is 1.08 g.
y is 0.88 g.
z is 0.15 g.
adxl375: set rate 200Hz.
x is 0.98 g.
y is 0.83 g.
z is 0.15 g.
x is 1.13 g.
y is 0.88 g.
z is 0.15 g.
x is 1.08 g.
y is 0.88 g.
z is 0.15 g.
adxl375: set rate 400Hz.
x is 1.13 g.
y is 0.83 g.
z is 0.20 g.
x is 1.13 g.
y is 0.83 g.
z is 0.20 g.
x is 1.08 g.
y is 0.88 g.
z is 0.10 g.
adxl375: set rate 800Hz.
x is 1.27 g.
y is 0.78 g.
z is 0.15 g.
x is 1.13 g.
y is 0.78 g.
z is 0.10 g.
x is 1.23 g.
y is 0.88 g.
z is 0.20 g.
adxl375: set rate 1600Hz.
x is 1.27 g.
y is 0.39 g.
z is -0.29 g.
x is 1.18 g.
y is 0.49 g.
z is 0.00 g.
x is 1.08 g.
y is 0.49 g.
z is 0.00 g.
adxl375: set rate 3200Hz.
x is 1.08 g.
y is 0.78 g.
z is 0.10 g.
x is 1.27 g.
y is 0.88 g.
z is 0.10 g.
x is 0.98 g.
y is 0.78 g.
z is 0.49 g.
adxl375: set rate low power 12.5Hz.
x is 0.88 g.
y is 1.18 g.
z is -1.37 g.
x is 1.08 g.
y is 0.44 g.
z is 0.00 g.
x is 1.13 g.
y is 0.39 g.
z is 0.00 g.
adxl375: set rate low power 25Hz.
x is 1.08 g.
y is 0.49 g.
z is -0.10 g.
x is 1.23 g.
y is 0.29 g.
z is 0.00 g.
x is 1.13 g.
y is 0.29 g.
z is 0.00 g.
adxl375: set rate low power 50Hz.
x is 1.08 g.
y is 0.49 g.
z is 0.00 g.
x is 0.83 g.
y is 0.64 g.
z is 0.00 g.
x is 1.13 g.
y is 0.49 g.
z is 0.00 g.
adxl375: set rate low power 100Hz.
x is 0.88 g.
y is 0.44 g.
z is -0.24 g.
x is 1.03 g.
y is 0.34 g.
z is 0.00 g.
x is 1.18 g.
y is 0.34 g.
z is 0.00 g.
adxl375: set rate low power 200Hz.
x is 1.13 g.
y is 0.49 g.
z is -0.05 g.
x is 0.93 g.
y is 0.49 g.
z is -0.15 g.
x is 0.88 g.
y is 0.54 g.
z is -0.10 g.
adxl375: set rate low power 400Hz.
x is 1.23 g.
y is 0.49 g.
z is 0.00 g.
x is 1.08 g.
y is 0.59 g.
z is -0.05 g.
x is 1.13 g.
y is 0.34 g.
z is -0.05 g.
adxl375: finish read test.
```

```shell
./adxl375 -t fifo --interface=spi

adxl375: chip is Analog Devices ADXL375.
adxl375: manufacturer is Analog Devices.
adxl375: interface is IIC SPI.
adxl375: driver version is 1.0.
adxl375: min supply voltage is 2.0V.
adxl375: max supply voltage is 3.6V.
adxl375: max current is 0.14mA.
adxl375: max temperature is 85.0C.
adxl375: min temperature is -40.0C.
adxl375: start fifo test.
adxl375: irq water mark with 16.
adxl375: irq water mark with 16.
adxl375: irq water mark with 16.
adxl375: finish fifo test.
```

```shell
./adxl375 -t int --interface=spi

adxl375: chip is Analog Devices ADXL375.
adxl375: manufacturer is Analog Devices.
adxl375: interface is IIC SPI.
adxl375: driver version is 1.0.
adxl375: min supply voltage is 2.0V.
adxl375: max supply voltage is 3.6V.
adxl375: max current is 0.14mA.
adxl375: max temperature is 85.0C.
adxl375: min temperature is -40.0C.
adxl375: start shock action test.
adxl375: please single shock.
adxl375: irq single shock.
adxl375: detect single shock.
adxl375: please double shock.
adxl375: detect double shock.
adxl375: please action.
adxl375: irq activity.
adxl375: detect action.
adxl375: please stop action.
adxl375: irq inactivity.
adxl375: detect stop.
adxl375: finish shock action test.
```

```shell
./adxl375 -e basic --interface=spi --times=3

adxl375: x is 0.147.
adxl375: y is 0.539.
adxl375: z is 0.343.
adxl375: x is 1.225.
adxl375: y is 0.490.
adxl375: z is 0.098.
adxl375: x is 1.225.
adxl375: y is 0.490.
adxl375: z is 0.098.
```

```shell
./adxl375 -e fifo --interface=spi --times=3

adxl375: fifo read 3.
adxl375: fifo read 2.
adxl375: fifo read 1.
```

```shell
./adxl375 -e int --interface=spi --mask=7

adxl375: irq inactivity.
adxl375: irq activity.
adxl375: irq single shock.
adxl375: irq activity.
adxl375: irq single shock.
adxl375: irq inactivity.
adxl375: irq activity.
adxl375: irq single shock.
adxl375: irq inactivity.
adxl375: irq activity.
adxl375: irq single shock.
adxl375: irq inactivity.
```

```shell
./adxl375 -h

Usage:
  adxl375 (-i | --information)
  adxl375 (-h | --help)
  adxl375 (-p | --port)
  adxl375 (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]
  adxl375 (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  adxl375 (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>]
  adxl375 (-t int | --test=int) [--addr=<0 | 1>] [--interface=<iic | spi>]
  adxl375 (-e basic | --example=basic) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  adxl375 (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  adxl375 (-e int | --example=int) [--addr=<0 | 1>] [--interface=<iic | spi>] [--mask=<msk>]

Options:
      --addr=<0 | 1>                 Set the chip address.([default: 0])
  -e <basic | fifo | int>, --example=<basic | fifo | int>
                                     Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
      --interface=<iic | spi>        Set the chip interface.([default: iic])
      --mask=<msk>                   Set the interrupt mask, bit 0 is the shock enable mask,
                                     bit 1 is the action enable mask, bit 2 is the inaction enable mask.([default: 15])
  -p, --port                         Display the pin connections of the current board.
  -t <reg | read | fifo | int>, --test=<reg | read | fifo | int>
                                     Run the driver test.
      --times=<num>                  Set the running times.([default: 3])
```

