/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_adxl375.h
 * @brief     driver adxl375 header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-02-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/02/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_ADXL375_H
#define DRIVER_ADXL375_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup adxl375_driver adxl375 driver function
 * @brief    adxl375 driver modules
 * @{
 */

/**
 * @addtogroup adxl375_basic_driver
 * @{
 */

/**
 * @brief adxl375 interface enumeration definition
 */
typedef enum
{
    ADXL375_INTERFACE_IIC = 0x00,        /**< iic interface function */
    ADXL375_INTERFACE_SPI = 0x01,        /**< spi interface function */
} adxl375_interface_t;

/**
 * @brief adxl375 address enumeration definition
 */
typedef enum
{
    ADXL375_ADDRESS_ALT_0 = 0xA6,        /**< addr pin connected to the GND */
    ADXL375_ADDRESS_ALT_1 = 0x3A,        /**< addr pin connected to the VCC */
} adxl375_address_t;

/**
 * @brief adxl375 bool enumeration definition
 */
typedef enum
{
    ADXL375_BOOL_FALSE = 0x00,        /**< false */
    ADXL375_BOOL_TRUE  = 0x01,        /**< true */
} adxl375_bool_t;

/**
 * @brief adxl375 coupled enumeration definition
 */
typedef enum
{
    ADXL375_COUPLED_DC = 0x00,        /**< DC coupled */
    ADXL375_COUPLED_AC = 0x01,        /**< AC coupled */
} adxl375_coupled_t;

/**
 * @brief adxl375 action inaction enumeration definition
 */
typedef enum
{
    ADXL375_ACTION_X   = 0x06,        /**< x axis action */
    ADXL375_ACTION_Y   = 0x05,        /**< y axis action */
    ADXL375_ACTION_Z   = 0x04,        /**< z axis action */
    ADXL375_INACTION_X = 0x02,        /**< x axis inaction */
    ADXL375_INACTION_Y = 0x01,        /**< y axis inaction */
    ADXL375_INACTION_Z = 0x00,        /**< z axis inaction */
} adxl375_action_inaction_t;

/**
 * @brief adxl375 shock axis enumeration definition
 */
typedef enum
{
    ADXL375_SHOCK_AXIS_X = 0x02,        /**< x axis shock */
    ADXL375_SHOCK_AXIS_Y = 0x01,        /**< y axis shock */
    ADXL375_SHOCK_AXIS_Z = 0x00,        /**< z axis shock */
} adxl375_shock_axis_t;

/**
 * @brief adxl375 action shock status enumeration definition
 */
typedef enum
{
    ADXL375_ACTION_SHOCK_STATUS_ACT_X   = (1 << 6),        /**< x axis action status */
    ADXL375_ACTION_SHOCK_STATUS_ACT_Y   = (1 << 5),        /**< y axis action status */
    ADXL375_ACTION_SHOCK_STATUS_ACT_Z   = (1 << 4),        /**< z axis action status */
    ADXL375_ACTION_SHOCK_STATUS_ASLEEP  = (1 << 3),        /**< asleep status */
    ADXL375_ACTION_SHOCK_STATUS_SHOCK_X = (1 << 2),        /**< x axis shock status */
    ADXL375_ACTION_SHOCK_STATUS_SHOCK_Y = (1 << 1),        /**< y axis shock status */
    ADXL375_ACTION_SHOCK_STATUS_SHOCK_Z = (1 << 0),        /**< z axis shock status */
} adxl375_action_shock_status_t;

/**
 * @brief adxl375 rate enumeration definition
 */
typedef enum
{
    ADXL375_RATE_0P1            = 0x00,        /**< 0.1Hz */
    ADXL375_RATE_0P2            = 0x01,        /**< 0.2Hz */
    ADXL375_RATE_0P39           = 0x02,        /**< 0.39Hz */
    ADXL375_RATE_0P78           = 0x03,        /**< 0.78Hz */
    ADXL375_RATE_1P56           = 0x04,        /**< 1.56Hz */
    ADXL375_RATE_3P13           = 0x05,        /**< 3.13Hz */
    ADXL375_RATE_6P25           = 0x06,        /**< 6.25Hz */
    ADXL375_RATE_12P5           = 0x07,        /**< 12.5Hz */
    ADXL375_RATE_25             = 0x08,        /**< 25Hz */
    ADXL375_RATE_50             = 0x09,        /**< 50Hz */
    ADXL375_RATE_100            = 0x0A,        /**< 100Hz */
    ADXL375_RATE_200            = 0x0B,        /**< 200Hz */
    ADXL375_RATE_400            = 0x0C,        /**< 400Hz */
    ADXL375_RATE_800            = 0x0D,        /**< 800Hz */
    ADXL375_RATE_1600           = 0x0E,        /**< 1600Hz */
    ADXL375_RATE_3200           = 0x0F,        /**< 3200Hz */
    ADXL375_LOW_POWER_RATE_12P5 = 0x17,        /**< low power 12.5Hz */
    ADXL375_LOW_POWER_RATE_25   = 0x18,        /**< low power 25Hz */
    ADXL375_LOW_POWER_RATE_50   = 0x19,        /**< low power 50Hz */
    ADXL375_LOW_POWER_RATE_100  = 0x1A,        /**< low power 100Hz */
    ADXL375_LOW_POWER_RATE_200  = 0x1B,        /**< low power 200Hz */
    ADXL375_LOW_POWER_RATE_400  = 0x1C,        /**< low power 400Hz */
} adxl375_rate_t;

/**
 * @brief adxl375 spi wire enumeration definition
 */
typedef enum
{
    ADXL375_SPI_WIRE_4 = 0x00,        /**< wire 4 */
    ADXL375_SPI_WIRE_3 = 0x01,        /**< wire 3 */
} adxl375_spi_wire_t;

/**
 * @brief adxl375 justify enumeration definition
 */
typedef enum
{
    ADXL375_JUSTIFY_RIGHT = 0x00,        /**< right justify */
    ADXL375_JUSTIFY_LEFT  = 0x01,        /**< left justify */
} adxl375_justify_t;

/**
 * @brief adxl375 sleep frequency enumeration definition
 */
typedef enum
{
    ADXL375_SLEEP_FREQUENCY_8HZ = 0x00,        /**< sleep 8Hz */
    ADXL375_SLEEP_FREQUENCY_4HZ = 0x01,        /**< sleep 4Hz */
    ADXL375_SLEEP_FREQUENCY_2HZ = 0x02,        /**< sleep 2Hz */
    ADXL375_SLEEP_FREQUENCY_1HZ = 0x03,        /**< sleep 1Hz*/
} adxl375_sleep_frequency_t;

/**
 * @brief adxl375 mode enumeration definition
 */
typedef enum
{
    ADXL375_MODE_BYPASS  = 0x00,        /**< bypass mode */
    ADXL375_MODE_FIFO    = 0x01,        /**< fifo mode */
    ADXL375_MODE_STREAM  = 0x02,        /**< stream mode */
    ADXL375_MODE_TRIGGER = 0x03,        /**< trigger mode */
} adxl375_mode_t;

/**
 * @brief adxl375 trigger status enumeration definition
 */
typedef enum
{
    ADXL375_TRIGGER_NOT_OCCURRED = 0x00,        /**< not occurred */
    ADXL375_TRIGGER_OCCURRED     = 0x01,        /**< occurred */
} adxl375_trigger_status_t;

/**
 * @}
 */

/**
 * @addtogroup adxl375_interrupt_driver
 * @{
 */

/**
 * @brief adxl375 interrupt enumeration definition
 */
typedef enum
{
    ADXL375_INTERRUPT_DATA_READY   = 0x07,        /**< data ready */
    ADXL375_INTERRUPT_SINGLE_SHOCK = 0x06,        /**< single shock */
    ADXL375_INTERRUPT_DOUBLE_SHOCK = 0x05,        /**< double shock */
    ADXL375_INTERRUPT_ACTIVITY     = 0x04,        /**< activity */
    ADXL375_INTERRUPT_INACTIVITY   = 0x03,        /**< inactivity */
    ADXL375_INTERRUPT_WATERMARK    = 0x01,        /**< watermark */
    ADXL375_INTERRUPT_OVERRUN      = 0x00,        /**< overrun */
} adxl375_interrupt_t;

/**
 * @brief adxl375 interrupt pin enumeration definition
 */
typedef enum
{
    ADXL375_INTERRUPT_PIN1 = 0x00,        /**< interrupt pin 1 */
    ADXL375_INTERRUPT_PIN2 = 0x01,        /**< interrupt pin 2 */
} adxl375_interrupt_pin_t;

/**
 * @brief adxl375 interrupt active_level enumeration definition
 */
typedef enum
{
    ADXL375_INTERRUPT_ACTIVE_LEVEL_HIGH = 0x00,        /**< interrupt active level high */
    ADXL375_INTERRUPT_ACTIVE_LEVEL_LOW  = 0x01,        /**< interrupt active level low */
} adxl375_interrupt_active_level_t;

/**
 * @}
 */

/**
 * @addtogroup adxl375_basic_driver
 * @{
 */

/**
 * @brief adxl375 handle structure definition
 */
typedef struct adxl375_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                       /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                      /**< point to a spi_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_spi;                                                                    /**< iic spi interface type */
} adxl375_handle_t;

/**
 * @brief adxl375 information structure definition
 */
typedef struct adxl375_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} adxl375_info_t;

/**
 * @}
 */

/**
 * @defgroup adxl375_link_driver adxl375 link driver function
 * @brief    adxl375 link driver modules
 * @ingroup  adxl375_driver
 * @{
 */

/**
 * @brief     initialize adxl375_handle_t structure
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] STRUCTURE adxl375_handle_t
 * @note      none
 */
#define DRIVER_ADXL375_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a spi_init function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_SPI_INIT(HANDLE, FUC)          (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a spi_deinit function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_SPI_DEINIT(HANDLE, FUC)        (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a spi_read function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_SPI_READ(HANDLE, FUC)          (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a spi_write function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_SPI_WRITE(HANDLE, FUC)         (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to an adxl375 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_ADXL375_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup adxl375_basic_driver adxl375 basic driver function
 * @brief    adxl375 basic driver modules
 * @ingroup  adxl375_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an adxl375 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl375_info(adxl375_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] interface chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t adxl375_set_interface(adxl375_handle_t *handle, adxl375_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *interface pointer to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl375_get_interface(adxl375_handle_t *handle, adxl375_interface_t *interface);

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t adxl375_set_addr_pin(adxl375_handle_t *handle, adxl375_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl375_get_addr_pin(adxl375_handle_t *handle, adxl375_address_t *addr_pin);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to an adxl375 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_irq_handler(adxl375_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an adxl375 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 * @note      none
 */
uint8_t adxl375_init(adxl375_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an adxl375 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t adxl375_deinit(adxl375_handle_t *handle);

/**
 * @brief         read the data
 * @param[in]     *handle pointer to an adxl375 handle structure
 * @param[out]    **raw pointer to a raw data buffer
 * @param[out]    **g pointer to a converted data buffer
 * @param[in,out] *len pointer to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 * @note          none
 */
uint8_t adxl375_read(adxl375_handle_t *handle, int16_t (*raw)[3], float (*g)[3], uint16_t *len);

/**
 * @brief     set the shock threshold
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] threshold shock threshold
 * @return    status code
 *            - 0 success
 *            - 1 set shock threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_shock_threshold(adxl375_handle_t *handle, uint8_t threshold);

/**
 * @brief      get the shock threshold
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *threshold pointer to a shock threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shock threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_shock_threshold(adxl375_handle_t *handle, uint8_t *threshold);

/**
 * @brief      convert the shock threshold to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  g shock threshold
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_shock_threshold_convert_to_register(adxl375_handle_t *handle, float g, uint8_t *reg);

/**
 * @brief      convert the register raw data to the shock threshold
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *g pointer to a shock threshold buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_shock_threshold_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *g);

/**
 * @brief     set the axis offset
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] x x axis offset
 * @param[in] y y axis offset
 * @param[in] z z axis offset
 * @return    status code
 *            - 0 success
 *            - 1 set offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_offset(adxl375_handle_t *handle, int8_t x, int8_t y, int8_t z);

/**
 * @brief      get the axis offset
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *x pointer to an x axis offset
 * @param[out] *y pointer to a y axis offset
 * @param[out] *z pointer to a z axis offset
 * @return     status code
 *             - 0 success
 *             - 1 get offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_offset(adxl375_handle_t *handle, int8_t *x, int8_t *y, int8_t *z);

/**
 * @brief      convert the offset to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  g offset
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_offset_convert_to_register(adxl375_handle_t *handle, float g, int8_t *reg);

/**
 * @brief      convert the register raw data to the offset
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *g pointer to an offset buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_offset_convert_to_data(adxl375_handle_t *handle, int8_t reg, float *g);

/**
 * @brief     set the duration
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] t duration
 * @return    status code
 *            - 0 success
 *            - 1 set duration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_duration(adxl375_handle_t *handle, uint8_t t);

/**
 * @brief      get the duration
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *t pointer to a duration buffer
 * @return     status code
 *             - 0 success
 *             - 1 get duration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_duration(adxl375_handle_t *handle, uint8_t *t);

/**
 * @brief      convert the duration to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  us duration
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_duration_convert_to_register(adxl375_handle_t *handle, uint32_t us, uint8_t *reg);

/**
 * @brief      convert the register raw data to the duration
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *us pointer to a duration buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_duration_convert_to_data(adxl375_handle_t *handle, uint8_t reg, uint32_t *us);

/**
 * @brief     set the latent
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] t latent time
 * @return    status code
 *            - 0 success
 *            - 1 set latent failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_latent(adxl375_handle_t *handle, uint8_t t);

/**
 * @brief      get the latent
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *t pointer to a latent time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get latent failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_latent(adxl375_handle_t *handle, uint8_t *t);

/**
 * @brief      convert the latent to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  ms time
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_latent_convert_to_register(adxl375_handle_t *handle, float ms, uint8_t *reg);

/**
 * @brief      convert the register raw data to the latent
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *ms pointer to a latent buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_latent_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *ms);

/**
 * @brief     set the window
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] t window time
 * @return    status code
 *            - 0 success
 *            - 1 set window failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_window(adxl375_handle_t *handle, uint8_t t);

/**
 * @brief      get the window
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *t pointer to a window time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get window failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_window(adxl375_handle_t *handle, uint8_t *t);

/**
 * @brief      convert the window time to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  ms window time
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_window_convert_to_register(adxl375_handle_t *handle, float ms, uint8_t *reg);

/**
 * @brief      convert the register raw data to the window time
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *ms pointer to a window time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_window_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *ms);

/**
 * @brief     set the action threshold
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] threshold action threshold
 * @return    status code
 *            - 0 success
 *            - 1 set action threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_action_threshold(adxl375_handle_t *handle, uint8_t threshold);

/**
 * @brief      get the action threshold
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *threshold pointer to an action threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get action threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_action_threshold(adxl375_handle_t *handle, uint8_t *threshold);

/**
 * @brief      convert the action threshold to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  g action threshold
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_action_threshold_convert_to_register(adxl375_handle_t *handle, float g, uint8_t *reg);

/**
 * @brief      convert the register raw data to the action threshold
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *g pointer to an action threshold buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_action_threshold_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *g);

/**
 * @brief     set the inaction threshold
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] threshold inaction threshold
 * @return    status code
 *            - 0 success
 *            - 1 set inaction threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_inaction_threshold(adxl375_handle_t *handle, uint8_t threshold);

/**
 * @brief      get the inaction threshold
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *threshold pointer to an inaction threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inaction threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_inaction_threshold(adxl375_handle_t *handle, uint8_t *threshold);

/**
 * @brief      convert the inaction threshold to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  g inaction threshold
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_inaction_threshold_convert_to_register(adxl375_handle_t *handle, float g, uint8_t *reg);

/**
 * @brief      convert the register raw data to the inaction threshold
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *g pointer to an inaction threshold buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_inaction_threshold_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *g);

/**
 * @brief     set the inaction time
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] t inaction time
 * @return    status code
 *            - 0 success
 *            - 1 set inaction time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_inaction_time(adxl375_handle_t *handle, uint8_t t);

/**
 * @brief      get the inaction time
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *t pointer to an inaction time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inaction time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_inaction_time(adxl375_handle_t *handle, uint8_t *t);

/**
 * @brief      convert the inaction time to the register raw data
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  s inaction time
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_inaction_time_convert_to_register(adxl375_handle_t *handle, uint8_t s, uint8_t *reg);

/**
 * @brief      convert the register raw data to the inaction time
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register raw data
 * @param[out] *s pointer to an inaction time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_inaction_time_convert_to_data(adxl375_handle_t *handle, uint8_t reg, uint8_t *s);

/**
 * @brief     enable or disable the action or inaction
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] type action or inaction type
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set action inaction failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_action_inaction(adxl375_handle_t *handle, adxl375_action_inaction_t type, adxl375_bool_t enable);

/**
 * @brief      get the action or inaction status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  type action or inaction type
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get action inaction failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_action_inaction(adxl375_handle_t *handle, adxl375_action_inaction_t type, adxl375_bool_t *enable);

/**
 * @brief     set the action coupled
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] coupled action coupled
 * @return    status code
 *            - 0 success
 *            - 1 set action coupled failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_action_coupled(adxl375_handle_t *handle, adxl375_coupled_t coupled);

/**
 * @brief      get the action coupled
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *coupled pointer to an action coupled buffer
 * @return     status code
 *             - 0 success
 *             - 1 get action coupled failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_action_coupled(adxl375_handle_t *handle, adxl375_coupled_t *coupled);

/**
 * @brief     set the inaction coupled
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] coupled inaction coupled
 * @return    status code
 *            - 0 success
 *            - 1 set inaction coupled failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_inaction_coupled(adxl375_handle_t *handle, adxl375_coupled_t coupled);

/**
 * @brief      get the inaction coupled
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *coupled pointer to an inaction coupled buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inaction coupled failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_inaction_coupled(adxl375_handle_t *handle, adxl375_coupled_t *coupled);

/**
 * @brief     enable or disable the shock axis
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] axis shock axis
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set shock axis failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_shock_axis(adxl375_handle_t *handle, adxl375_shock_axis_t axis, adxl375_bool_t enable);

/**
 * @brief      get the shock axis status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  axis shock axis
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shock axis failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_get_shock_axis(adxl375_handle_t *handle, adxl375_shock_axis_t axis, adxl375_bool_t *enable);

/**
 * @brief     enable or disable the shock suppress
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set shock suppress failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_shock_suppress(adxl375_handle_t *handle, adxl375_bool_t enable);

/**
 * @brief      get the shock suppress status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shock suppress failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_shock_suppress(adxl375_handle_t *handle, adxl375_bool_t *enable);

/**
 * @brief      get the shock status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *status pointer to a shock status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shock status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_shock_status(adxl375_handle_t *handle, uint8_t *status);

/**
 * @brief     set the sampling rate
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] rate sampling rate
 * @return    status code
 *            - 0 success
 *            - 1 set rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_rate(adxl375_handle_t *handle, adxl375_rate_t rate);

/**
 * @brief      get the sampling rate
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *rate pointer to a sampling rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_rate(adxl375_handle_t *handle, adxl375_rate_t *rate);

/**
 * @brief     enable or disable the interrupt
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] type interrupt type
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_interrupt(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_bool_t enable);

/**
 * @brief      get the interrupt status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  type interrupt type
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_interrupt(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_bool_t *enable);

/**
 * @brief     set the interrupt map
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] type interrupt type
 * @param[in] pin interrupt pin
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt map failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_interrupt_map(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_interrupt_pin_t pin);

/**
 * @brief      get the interrupt map
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  type interrupt type
 * @param[out] *pin pointer to an interrupt pin buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt map failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_interrupt_map(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_interrupt_pin_t *pin);

/**
 * @brief      get the interrupt source
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *source pointer to an interrupt source buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt source failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_interrupt_source(adxl375_handle_t *handle, uint8_t *source);

/**
 * @brief     enable or disable the self test
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set self test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_self_test(adxl375_handle_t *handle, adxl375_bool_t enable);

/**
 * @brief      get the self test status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_self_test(adxl375_handle_t *handle, adxl375_bool_t *enable);

/**
 * @brief     set the chip spi wire
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] wire spi wire
 * @return    status code
 *            - 0 success
 *            - 1 set spi wire failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_spi_wire(adxl375_handle_t *handle, adxl375_spi_wire_t wire);

/**
 * @brief      get the chip spi wire
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *wire pointer to a spi wire buffer
 * @return     status code
 *             - 0 success
 *             - 1 get spi wire failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_spi_wire(adxl375_handle_t *handle, adxl375_spi_wire_t *wire);

/**
 * @brief     set the interrupt active level
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] active_level interrupt active level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_interrupt_active_level(adxl375_handle_t *handle, adxl375_interrupt_active_level_t active_level);

/**
 * @brief      get the interrupt active level
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *active_level pointer to an interrupt active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_interrupt_active_level(adxl375_handle_t *handle, adxl375_interrupt_active_level_t *active_level);

/**
 * @brief     enable or disable the justify
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set justify failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_justify(adxl375_handle_t *handle, adxl375_justify_t enable);

/**
 * @brief      get the justify status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get justify failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_justify(adxl375_handle_t *handle, adxl375_justify_t *enable);

/**
 * @brief     set the chip mode
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_mode(adxl375_handle_t *handle, adxl375_mode_t mode);

/**
 * @brief      get the chip mode
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *mode pointer to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_mode(adxl375_handle_t *handle, adxl375_mode_t *mode);

/**
 * @brief     set the trigger pin
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] pin trigger pin
 * @return    status code
 *            - 0 success
 *            - 1 set trigger pin failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_trigger_pin(adxl375_handle_t *handle, adxl375_interrupt_pin_t pin);

/**
 * @brief      get the trigger pin
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *pin pointer to a trigger pin buffer
 * @return     status code
 *             - 0 success
 *             - 1 get trigger pin failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_trigger_pin(adxl375_handle_t *handle, adxl375_interrupt_pin_t *pin);

/**
 * @brief     set the fifo watermark
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] level watermark level
 * @return    status code
 *            - 0 success
 *            - 1 set watermark failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      level <= 0x1F
 */
uint8_t adxl375_set_watermark(adxl375_handle_t *handle, uint8_t level);

/**
 * @brief      get the fifo watermark
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *level pointer to a watermark level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get watermark failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_watermark(adxl375_handle_t *handle, uint8_t *level);

/**
 * @brief      get the current fifo watermark level
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *level pointer to a current watermark level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get watermark level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_watermark_level(adxl375_handle_t *handle, uint8_t *level);

/**
 * @brief      get the trigger status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *status pointer to a trigger status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get trigger status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_trigger_status(adxl375_handle_t *handle, adxl375_trigger_status_t *status);

/**
 * @brief     enable or disable the activity and inactivity linking
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set link activity inactivity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_link_activity_inactivity(adxl375_handle_t *handle, adxl375_bool_t enable);

/**
 * @brief      get the activity and inactivity linking status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get link activity inactivity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_link_activity_inactivity(adxl375_handle_t *handle, adxl375_bool_t *enable);

/**
 * @brief     enable or disable the auto sleep
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_auto_sleep(adxl375_handle_t *handle, adxl375_bool_t enable);

/**
 * @brief      get the auto sleep status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto sleep failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_auto_sleep(adxl375_handle_t *handle, adxl375_bool_t *enable);

/**
 * @brief     enable or disable the measure
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set measure failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_measure(adxl375_handle_t *handle, adxl375_bool_t enable);

/**
 * @brief      get the measure status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get measure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_measure(adxl375_handle_t *handle, adxl375_bool_t *enable);

/**
 * @brief     enable or disable the sleep mode
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_sleep(adxl375_handle_t *handle, adxl375_bool_t enable);

/**
 * @brief      get the sleep mode status
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sleep failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_sleep(adxl375_handle_t *handle, adxl375_bool_t *enable);

/**
 * @brief     set the sleep frequency
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] sleep_frequency sleep frequency
 * @return    status code
 *            - 0 success
 *            - 1 set sleep frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_sleep_frequency(adxl375_handle_t *handle, adxl375_sleep_frequency_t sleep_frequency);

/**
 * @brief      get the sleep frequency
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *sleep_frequency pointer to a sleep frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sleep frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_sleep_frequency(adxl375_handle_t *handle, adxl375_sleep_frequency_t *sleep_frequency);

/**
 * @}
 */

/**
 * @defgroup adxl375_extern_driver adxl375 extern driver function
 * @brief    adxl375 extern driver modules
 * @ingroup  adxl375_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl375_set_reg(adxl375_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl375_get_reg(adxl375_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
