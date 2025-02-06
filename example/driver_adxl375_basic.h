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
 * @file      driver_adxl375_basic.h
 * @brief     driver adxl375 basic header file
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

#ifndef DRIVER_ADXL375_BASIC_H
#define DRIVER_ADXL375_BASIC_H

#include "driver_adxl375_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup adxl375_example_driver adxl375 example driver function
 * @brief    adxl375 example driver modules
 * @ingroup  adxl375_driver
 * @{
 */

/**
 * @brief adxl375 basic example default definition
 */
#define ADXL375_BASIC_DEFAULT_RATE                          ADXL375_RATE_100                      /**< rate 100Hz */
#define ADXL375_BASIC_DEFAULT_SPI_WIRE                      ADXL375_SPI_WIRE_4                    /**< spi wire 4 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_ACTIVE_LEVEL        ADXL375_INTERRUPT_ACTIVE_LEVEL_LOW    /**< interrupt pin low */
#define ADXL375_BASIC_DEFAULT_AUTO_SLEEP                    ADXL375_BOOL_FALSE                    /**< disable auto sleep */
#define ADXL375_BASIC_DEFAULT_SLEEP                         ADXL375_BOOL_FALSE                    /**< disable sleep */
#define ADXL375_BASIC_DEFAULT_SLEEP_FREQUENCY               ADXL375_SLEEP_FREQUENCY_1HZ           /**< sleep frequency 1Hz */
#define ADXL375_BASIC_DEFAULT_JUSTIFY                       ADXL375_JUSTIFY_RIGHT                 /**< justify right */
#define ADXL375_BASIC_DEFAULT_MODE                          ADXL375_MODE_BYPASS                   /**< bypass mode */
#define ADXL375_BASIC_DEFAULT_TRIGGER_PIN                   ADXL375_INTERRUPT_PIN2                /**< trigger pin map interrupt pin 2 */
#define ADXL375_BASIC_DEFAULT_SHOCK_SUPPRESS                ADXL375_BOOL_FALSE                    /**< disable shock suppress */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_SINGLE_SHOCK_MAP    ADXL375_INTERRUPT_PIN1                /**< single shock map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_DOUBLE_SHOCK_MAP    ADXL375_INTERRUPT_PIN1                /**< double shock map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_ACTIVITY_MAP        ADXL375_INTERRUPT_PIN1                /**< activity map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_INACTIVITY_MAP      ADXL375_INTERRUPT_PIN1                /**< inactivity map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_DATA_READY_MAP      ADXL375_INTERRUPT_PIN1                /**< data ready map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_WATERMARK_MAP       ADXL375_INTERRUPT_PIN1                /**< watermark map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_OVERRUN_MAP         ADXL375_INTERRUPT_PIN1                /**< overrun map interrupt pin 1 */
#define ADXL375_BASIC_DEFAULT_LINK_ACTIVITY_INACTIVITY      ADXL375_BOOL_TRUE                     /**< enable activity inactivity */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_DATA_READY          ADXL375_BOOL_FALSE                    /**< disable data ready */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_WATERMARK           ADXL375_BOOL_FALSE                    /**< disable watermark */
#define ADXL375_BASIC_DEFAULT_INTERRUPT_OVERRUN             ADXL375_BOOL_FALSE                    /**< disable overrun */
#define ADXL375_BASIC_DEFAULT_ACTION_COUPLED                ADXL375_COUPLED_AC                    /**< action ac coupled */
#define ADXL375_BASIC_DEFAULT_INACTION_COUPLED              ADXL375_COUPLED_DC                    /**< inaction dc coupled */
#define ADXL375_BASIC_DEFAULT_WATERMARK                     16                                    /**< watermark 16 level */
#define ADXL375_BASIC_DEFAULT_OFFSET                        0.0f                                  /**< 0 offset */
#define ADXL375_BASIC_DEFAULT_SHOCK_THRESHOLD               3.0f                                  /**< shock threshold 3.0g */
#define ADXL375_BASIC_DEFAULT_DURATION                      10 * 1000                             /**< duration 10 ms */
#define ADXL375_BASIC_DEFAULT_LATENT                        20.0f                                 /**< latent 20 ms */
#define ADXL375_BASIC_DEFAULT_WINDOW                        80.0f                                 /**< window 80 ms */
#define ADXL375_BASIC_DEFAULT_ACTION_THRESHOLD              2.0f                                  /**< action threshold 2g */
#define ADXL375_BASIC_DEFAULT_INACTION_THRESHOLD            1.0f                                  /**< inaction threshold 1g */
#define ADXL375_BASIC_DEFAULT_INACTION_TIME                 3                                     /**< inaction 3s */

/**
 * @brief     basic example init
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t adxl375_basic_init(adxl375_interface_t interface, adxl375_address_t addr_pin);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adxl375_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *g pointer to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t adxl375_basic_read(float g[3]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
