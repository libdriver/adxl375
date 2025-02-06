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
 * @file      driver_adxl375_register_test.c
 * @brief     driver adxl375 register test source file
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

#include "driver_adxl375_register_test.h"
#include <stdlib.h>

static adxl375_handle_t gs_handle;        /**< adxl375 handle */

/**
 * @brief     register test
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t adxl375_register_test(adxl375_interface_t interface, adxl375_address_t addr_pin)
{
    uint8_t res, reg, check;
    int8_t x, y, z;
    int8_t x_check, y_check, z_check;
    float f, f_check;
    int8_t i_reg;
    uint32_t us, us_check;
    adxl375_info_t info;
    adxl375_interface_t interface_test;
    adxl375_address_t addr;
    adxl375_bool_t bool_test;
    adxl375_coupled_t coupled;
    adxl375_rate_t rate;
    adxl375_interrupt_active_level_t pin_type;
    adxl375_interrupt_pin_t pin;
    adxl375_justify_t justify;
    adxl375_mode_t mode;
    adxl375_sleep_frequency_t sleep_frequency;
    adxl375_trigger_status_t trigger;

    /* link interface function */
    DRIVER_ADXL375_LINK_INIT(&gs_handle, adxl375_handle_t);
    DRIVER_ADXL375_LINK_IIC_INIT(&gs_handle, adxl375_interface_iic_init);
    DRIVER_ADXL375_LINK_IIC_DEINIT(&gs_handle, adxl375_interface_iic_deinit);
    DRIVER_ADXL375_LINK_IIC_READ(&gs_handle, adxl375_interface_iic_read);
    DRIVER_ADXL375_LINK_IIC_WRITE(&gs_handle, adxl375_interface_iic_write);
    DRIVER_ADXL375_LINK_SPI_INIT(&gs_handle, adxl375_interface_spi_init);
    DRIVER_ADXL375_LINK_SPI_DEINIT(&gs_handle, adxl375_interface_spi_deinit);
    DRIVER_ADXL375_LINK_SPI_READ(&gs_handle, adxl375_interface_spi_read);
    DRIVER_ADXL375_LINK_SPI_WRITE(&gs_handle, adxl375_interface_spi_write);
    DRIVER_ADXL375_LINK_DELAY_MS(&gs_handle, adxl375_interface_delay_ms);
    DRIVER_ADXL375_LINK_DEBUG_PRINT(&gs_handle, adxl375_interface_debug_print);
    DRIVER_ADXL375_LINK_RECEIVE_CALLBACK(&gs_handle, adxl375_interface_receive_callback);

    /* get information */
    res = adxl375_info(&info);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get info failed.\n");

        return 1;
    }
    else
    {
        /* print chip info */
        adxl375_interface_debug_print("adxl375: chip is %s.\n", info.chip_name);
        adxl375_interface_debug_print("adxl375: manufacturer is %s.\n", info.manufacturer_name);
        adxl375_interface_debug_print("adxl375: interface is %s.\n", info.interface);
        adxl375_interface_debug_print("adxl375: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        adxl375_interface_debug_print("adxl375: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        adxl375_interface_debug_print("adxl375: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        adxl375_interface_debug_print("adxl375: max current is %0.2fmA.\n", info.max_current_ma);
        adxl375_interface_debug_print("adxl375: max temperature is %0.1fC.\n", info.temperature_max);
        adxl375_interface_debug_print("adxl375: min temperature is %0.1fC.\n", info.temperature_min);
    }

    /* start register test */
    adxl375_interface_debug_print("adxl375: start register test.\n");

    /* adxl375_set_interface/adxl375_get_interface test */
    adxl375_interface_debug_print("adxl375: adxl375_set_interface/adxl375_get_interface test.\n");

    /* set IIC interface */
    res = adxl375_set_interface(&gs_handle, ADXL375_INTERFACE_IIC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interface failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interface iic.\n");
    res = adxl375_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interface failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interface %s.\n", interface_test==ADXL375_INTERFACE_IIC?"ok":"error");

    /* set SPI interface */
    res = adxl375_set_interface(&gs_handle, ADXL375_INTERFACE_SPI);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interface failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interface spi.\n");
    res = adxl375_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interface failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interface %s.\n", interface_test==ADXL375_INTERFACE_SPI?"ok":"error");

    /* adxl375_set_addr_pin/adxl375_get_addr_pin test */
    adxl375_interface_debug_print("adxl375: adxl375_set_addr_pin/adxl375_get_addr_pin test.\n");

    /* ALT0 */
    res = adxl375_set_addr_pin(&gs_handle, ADXL375_ADDRESS_ALT_0);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set addr pin failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set addr pin alt0.\n");
    res = adxl375_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get addr pin failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check addr pin %s.\n", addr==ADXL375_ADDRESS_ALT_0?"ok":"error");

    /* ALT1 */
    res = adxl375_set_addr_pin(&gs_handle, ADXL375_ADDRESS_ALT_1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set addr pin failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set addr pin alt1.\n");
    res = adxl375_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get addr pin failed.\n");

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check addr pin %s.\n", addr==ADXL375_ADDRESS_ALT_1?"ok":"error");

    /* set the interface */
    res = adxl375_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interface failed.\n");

        return 1;
    }

    /* set the address pin */
    res = adxl375_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set addr pin failed.\n");

        return 1;
    }

    /* init the adxl375 */
    res = adxl375_init(&gs_handle);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: init failed.\n");

        return 1;
    }

    /* adxl375_set_shock_threshold/adxl375_get_shock_threshold test */
    adxl375_interface_debug_print("adxl375: adxl375_set_shock_threshold/adxl375_get_shock_threshold test.\n");
    reg = rand()%256;
    res = adxl375_set_shock_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock threshold 0x%02X.\n", reg);
    res = adxl375_get_shock_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock threshold %s.\n", check==reg?"ok":"error");

    /* adxl375_set_offset/adxl375_get_offset test */
    adxl375_interface_debug_print("adxl375: adxl375_set_offset/adxl375_get_offset test.\n");
    x = rand()%128;
    y = rand()%128;
    y = -y;
    z = rand()%128;
    res = adxl375_set_offset(&gs_handle, x, y, z);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set offset failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set x offset %d.\n", x);
    adxl375_interface_debug_print("adxl375: set y offset %d.\n", y);
    adxl375_interface_debug_print("adxl375: set z offset %d.\n", z);
    res = adxl375_get_offset(&gs_handle, (int8_t *)&x_check, (int8_t *)&y_check, (int8_t *)&z_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get offset failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check x offset %s.\n", x==x_check?"ok":"error");
    adxl375_interface_debug_print("adxl375: check y offset %s.\n", y==y_check?"ok":"error");
    adxl375_interface_debug_print("adxl375: check z offset %s.\n", z==z_check?"ok":"error");

    /* adxl375_set_duration/adxl375_get_duration test */
    adxl375_interface_debug_print("adxl375: adxl375_set_duration/adxl375_get_duration test.\n");
    reg = rand()%256;
    res = adxl375_set_duration(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set duration failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set duration 0x%02X.\n", reg);
    res = adxl375_get_duration(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get duration failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check duration %s.\n", check==reg?"ok":"error");

    /* adxl375_set_latent/adxl375_get_latent test */
    adxl375_interface_debug_print("adxl375: adxl375_set_latent/adxl375_get_latent test.\n");
    reg = rand()%256;
    res = adxl375_set_latent(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set latent failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set latent 0x%02X.\n", reg);
    res = adxl375_get_latent(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get latent failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check latent %s.\n", check==reg?"ok":"error");

    /* adxl375_set_window/adxl375_get_window test */
    adxl375_interface_debug_print("adxl375: adxl375_set_window/adxl375_get_window test.\n");
    reg = rand()%256;
    res = adxl375_set_window(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set window failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set window 0x%02X.\n", reg);
    res = adxl375_get_window(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get window failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check window %s.\n", check==reg?"ok":"error");

    /* adxl375_set_action_threshold/adxl375_get_action_threshold test */
    adxl375_interface_debug_print("adxl375: adxl375_set_action_threshold/adxl375_get_action_threshold test.\n");
    reg = rand()%256;
    res = adxl375_set_action_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action threshold 0x%02X.\n", reg);
    res = adxl375_get_action_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action threshold %s.\n", check==reg?"ok":"error");

    /* adxl375_set_inaction_threshold/adxl375_get_inaction_threshold test */
    adxl375_interface_debug_print("adxl375: adxl375_set_inaction_threshold/adxl375_get_inaction_threshold test.\n");
    reg = rand()%256;
    res = adxl375_set_inaction_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction threshold 0x%02X.\n", reg);
    res = adxl375_get_inaction_threshold(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get inaction threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check inaction threshold %s.\n", check==reg?"ok":"error");

    /* adxl375_set_inaction_time/adxl375_get_inaction_time test */
    adxl375_interface_debug_print("adxl375: adxl375_set_inaction_time/adxl375_get_inaction_time test.\n");
    reg = rand()%256;
    res = adxl375_set_inaction_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction time failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction time 0x%02X.\n", reg);
    res = adxl375_get_inaction_time(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get inaction time failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check inaction time %s.\n", check==reg?"ok":"error");

    /* adxl375_set_action_inaction/adxl375_get_action_inaction test */
    adxl375_interface_debug_print("adxl375: adxl375_set_action_inaction/adxl375_get_action_inaction test.\n");

    /* action_x_enable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_X, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action x enable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_ACTION_X, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* action_x_disable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_X, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action x disable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_ACTION_X, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* action_y_enable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Y, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action y enable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_ACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* action_y_disable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Y, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action y disable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_ACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* action_z_enable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Z, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action z enable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_ACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* action_z_disable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Z, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action z disable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_ACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* inaction_x_enable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_X, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction x enable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_INACTION_X, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* inaction_x_disable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_X, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction x disable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_INACTION_X, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* inaction_y_enable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Y, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction y enable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_INACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* inaction_y_disable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Y, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction y disable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_INACTION_Y, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* inaction_z_enable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Z, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction z enable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_INACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* inaction_z_disable */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Z, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction z disable.\n");
    res = adxl375_get_action_inaction(&gs_handle, ADXL375_INACTION_Z, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action inaction %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_action_coupled/adxl375_get_action_coupled test */
    adxl375_interface_debug_print("adxl375: adxl375_set_action_coupled/adxl375_get_action_coupled test.\n");

    /* coupled dc */
    res = adxl375_set_action_coupled(&gs_handle, ADXL375_COUPLED_DC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action coupled dc.\n");
    res = adxl375_get_action_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action coupled %s.\n", coupled==ADXL375_COUPLED_DC?"ok":"error");

    /* coupled ac */
    res = adxl375_set_action_coupled(&gs_handle, ADXL375_COUPLED_AC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set action coupled ac.\n");
    res = adxl375_get_action_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get action coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check action coupled %s.\n", coupled==ADXL375_COUPLED_AC?"ok":"error");

    /* adxl375_set_inaction_coupled/adxl375_get_inaction_coupled test */
    adxl375_interface_debug_print("adxl375: adxl375_set_inaction_coupled/adxl375_get_inaction_coupled test.\n");

    /* coupled dc */
    res = adxl375_set_inaction_coupled(&gs_handle, ADXL375_COUPLED_DC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction coupled dc.\n");
    res = adxl375_get_inaction_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get inaction coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check inaction coupled %s.\n", coupled==ADXL375_COUPLED_DC?"ok":"error");

    /* coupled ac */
    res = adxl375_set_inaction_coupled(&gs_handle, ADXL375_COUPLED_AC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set inaction coupled ac.\n");
    res = adxl375_get_inaction_coupled(&gs_handle, &coupled);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get inaction coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check inaction coupled %s.\n", coupled==ADXL375_COUPLED_AC?"ok":"error");

    /* adxl375_set_shock_axis/adxl375_get_shock_axis test */
    adxl375_interface_debug_print("adxl375: adxl375_set_shock_axis/adxl375_get_shock_axis test.\n");

    /* shock_x_enable */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_X, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock axis x enable.\n");
    res = adxl375_get_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_X, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock axis %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* shock_x_disable */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_X, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock axis x disable.\n");
    res = adxl375_get_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_X, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock axis %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* shock_y_enable */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Y, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock axis y enable.\n");
    res = adxl375_get_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Y, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock axis %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* shock_y_disable */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Y, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock axis y disable.\n");
    res = adxl375_get_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Y, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock axis %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* shock_z_enable */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Z, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock axis z enable.\n");
    res = adxl375_get_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Z, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock axis %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* shock_z_disable */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Z, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock axis z disable.\n");
    res = adxl375_get_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Z, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock axis %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_shock_suppress/adxl375_get_shock_suppress test */
    adxl375_interface_debug_print("adxl375: adxl375_set_shock_suppress/adxl375_get_shock_suppress test.\n");

    /* enable */
    res = adxl375_set_shock_suppress(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock suppress failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock suppress enable.\n");
    res = adxl375_get_shock_suppress(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock suppress failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock suppress %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_shock_suppress(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock suppress failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set shock suppress disable.\n");
    res = adxl375_get_shock_suppress(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock suppress failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check shock suppress %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* get shock status test */
    adxl375_interface_debug_print("adxl375: get shock status test.\n");
    res = adxl375_get_shock_status(&gs_handle, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get shock status failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: shock status is 0x%02X.\n", reg);

    /* adxl375_set_rate/adxl375_get_rate test */
    adxl375_interface_debug_print("adxl375: adxl375_set_rate/adxl375_get_rate test.\n");

    /* 0.1Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_0P1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 0.1Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_0P1?"ok":"error");

    /* 0.2Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_0P2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 0.2Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_0P2?"ok":"error");

    /* 0.39Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_0P39);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 0.39Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_0P39?"ok":"error");

    /* 0.78Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_0P78);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 0.78Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_0P78?"ok":"error");

    /* 1.56Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_1P56);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 1.56Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_1P56?"ok":"error");

    /* 3.13Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_3P13);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 3.13Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_3P13?"ok":"error");

    /* 6.25Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_6P25);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 6.25Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_6P25?"ok":"error");

    /* 12.5Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_12P5);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 12.5Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_12P5?"ok":"error");

    /* 25Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_25);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 25Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_25?"ok":"error");

    /* 50Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_50);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 50Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_50?"ok":"error");

    /* 100Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_100);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 100Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_100?"ok":"error");

    /* 200Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_200);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 200Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_200?"ok":"error");

    /* 400Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_400);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 400Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_400?"ok":"error");

    /* 800Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_800);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 800Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_800?"ok":"error");

    /* 1600Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_1600);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 1600Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_1600?"ok":"error");

    /* 3200Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_3200);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate 3200Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_RATE_3200?"ok":"error");

    /* low 12.5Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_LOW_POWER_RATE_12P5);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate low 12.5Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_LOW_POWER_RATE_12P5?"ok":"error");

    /* low 25Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_LOW_POWER_RATE_25);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate low 25Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_LOW_POWER_RATE_25?"ok":"error");

    /* low 50Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_LOW_POWER_RATE_50);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate low 50Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_LOW_POWER_RATE_50?"ok":"error");

    /* low 100Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_LOW_POWER_RATE_100);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate low 100Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_LOW_POWER_RATE_100?"ok":"error");

    /* low 200Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_LOW_POWER_RATE_200);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate low 200Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_LOW_POWER_RATE_200?"ok":"error");

    /* low 400Hz */
    res = adxl375_set_rate(&gs_handle, ADXL375_LOW_POWER_RATE_400);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set rate low 400Hz.\n");
    res = adxl375_get_rate(&gs_handle, &rate);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get rate failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check rate %s.\n", rate==ADXL375_LOW_POWER_RATE_400?"ok":"error");

    /* adxl375_set_interrupt/adxl375_get_interrupt test */
    adxl375_interface_debug_print("adxl375: adxl375_set_interrupt/adxl375_get_interrupt test.\n");

    /* data enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt data ready enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_DATA_READY, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* data disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt data ready disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_DATA_READY, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* single enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt single enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* single disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt single disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* double enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt double enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* double disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt double disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* activity enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt activity enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* activity disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt activity disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* inactivity enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt inactivity enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* inactivity disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt inactivity disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* watermark enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt watermark enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_WATERMARK, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* watermark disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt watermark disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_WATERMARK, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* overrun enable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt overrun enable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_OVERRUN, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* overrun disable */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt overrun disable.\n");
    res = adxl375_get_interrupt(&gs_handle, ADXL375_INTERRUPT_OVERRUN, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_interrupt_map/adxl375_get_interrupt_map test */
    adxl375_interface_debug_print("adxl375: adxl375_set_interrupt_map/adxl375_get_interrupt_map test.\n");

    /* data pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map data ready pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DATA_READY, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* data pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map data ready pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DATA_READY, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* single shock pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map single shock pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* single pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map single pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* double shock pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map double shock pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* double pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map double pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* activity pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map activity pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* activity pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt activity pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* inactivity pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map inactivity pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* inactivity pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt inactivity pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* watermark pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map watermark pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_WATERMARK, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* watermark pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt watermark pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_WATERMARK, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* overrun pin1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt map overrun pin1.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_OVERRUN, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* overrun pin2 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt overrun pin2.\n");
    res = adxl375_get_interrupt_map(&gs_handle, ADXL375_INTERRUPT_OVERRUN, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt map %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* adxl375_get_interrupt_source test */
    adxl375_interface_debug_print("adxl375: adxl375_get_interrupt_source test.\n");
    res = adxl375_get_interrupt_source(&gs_handle, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt source failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt source 0x%02X.\n", res);

    /* adxl375_set_self_test/adxl375_get_self_test test */
    adxl375_interface_debug_print("adxl375: adxl375_set_self_test/adxl375_get_self_test test.\n");

    /* enable */
    res = adxl375_set_self_test(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set self test enable.\n");
    res = adxl375_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check self test %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_self_test(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set self test disable.\n");
    res = adxl375_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check self test %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_spi_wire/adxl375_get_spi_wire test */
    adxl375_interface_debug_print("adxl375: adxl375_set_spi_wire/adxl375_get_spi_wire test.\n");

    /* enable */
    res = adxl375_set_self_test(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set self test enable.\n");
    res = adxl375_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check self test %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_self_test(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set self test disable.\n");
    res = adxl375_get_self_test(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get self test failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check self test %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_interrupt_active_level/adxl375_get_interrupt_active_level test */
    adxl375_interface_debug_print("adxl375: adxl375_set_interrupt_active_level/adxl375_get_interrupt_active_level test.\n");

    /* high */
    res = adxl375_set_interrupt_active_level(&gs_handle, ADXL375_INTERRUPT_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt active level failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt active level high.\n");
    res = adxl375_get_interrupt_active_level(&gs_handle, &pin_type);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt active level failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt active level %s.\n", pin_type==ADXL375_INTERRUPT_ACTIVE_LEVEL_HIGH?"ok":"error");

    /* low */
    res = adxl375_set_interrupt_active_level(&gs_handle, ADXL375_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt active level failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set interrupt active level low.\n");
    res = adxl375_get_interrupt_active_level(&gs_handle, &pin_type);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt active level failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check interrupt active level %s.\n", pin_type==ADXL375_INTERRUPT_ACTIVE_LEVEL_LOW?"ok":"error");

    /* adxl375_set_justify/adxl375_get_justify test */
    adxl375_interface_debug_print("adxl375: adxl375_set_justify/adxl375_get_justify test.\n");

    /* right */
    res = adxl375_set_justify(&gs_handle, ADXL375_JUSTIFY_RIGHT);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set justify failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set justify right.\n");
    res = adxl375_get_justify(&gs_handle, &justify);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get justify failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check justify %s.\n", justify==ADXL375_JUSTIFY_RIGHT?"ok":"error");

    /* left */
    res = adxl375_set_justify(&gs_handle, ADXL375_JUSTIFY_LEFT);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set justify failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set justify left.\n");
    res = adxl375_get_justify(&gs_handle, &justify);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get justify failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check justify %s.\n", justify==ADXL375_JUSTIFY_LEFT?"ok":"error");

    /* adxl375_set_mode/adxl375_get_mode */
    adxl375_interface_debug_print("adxl375: adxl375_set_mode/adxl375_get_mode test.\n");

    /* bypass */
    res = adxl375_set_mode(&gs_handle, ADXL375_MODE_BYPASS);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set mode bypass.\n");
    res = adxl375_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check mode %s.\n", mode==ADXL375_MODE_BYPASS?"ok":"error");

    /* fifo */
    res = adxl375_set_mode(&gs_handle, ADXL375_MODE_FIFO);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set mode fifo.\n");
    res = adxl375_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check mode %s.\n", mode==ADXL375_MODE_FIFO?"ok":"error");

    /* stream */
    res = adxl375_set_mode(&gs_handle, ADXL375_MODE_STREAM);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set mode stream.\n");
    res = adxl375_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check mode %s.\n", mode==ADXL375_MODE_STREAM?"ok":"error");

    /* trigger */
    res = adxl375_set_mode(&gs_handle, ADXL375_MODE_TRIGGER);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set mode trigger.\n");
    res = adxl375_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get mode failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check mode %s.\n", mode==ADXL375_MODE_TRIGGER?"ok":"error");

    /* adxl375_set_trigger_pin/adxl375_get_trigger_pin test */
    adxl375_interface_debug_print("adxl375: adxl375_set_trigger_pin/adxl375_get_trigger_pin test.\n");

    /* pin1 */
    res = adxl375_set_trigger_pin(&gs_handle, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set trigger pin failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set trigger pin 1.\n");
    res = adxl375_get_trigger_pin(&gs_handle, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get trigger pin failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check trigger pin %s.\n", pin==ADXL375_INTERRUPT_PIN1?"ok":"error");

    /* pin2 */
    res = adxl375_set_trigger_pin(&gs_handle, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set trigger pin failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set trigger pin 2.\n");
    res = adxl375_get_trigger_pin(&gs_handle, &pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get trigger pin failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check trigger pin %s.\n", pin==ADXL375_INTERRUPT_PIN2?"ok":"error");

    /* adxl375_set_watermark/adxl375_get_watermark test */
    adxl375_interface_debug_print("adxl375: adxl375_set_watermark/adxl375_get_watermark test.\n");
    reg = rand()%32;
    res = adxl375_set_watermark(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set watermark failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set watermark 0x%02X.\n", reg);
    res = adxl375_get_watermark(&gs_handle, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get watermark failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check watermark %s.\n", check==reg?"ok":"error");

    /* adxl375_get_watermark_level test */
    adxl375_interface_debug_print("adxl375: adxl375_get_watermark_level test.\n");
    res = adxl375_get_watermark_level(&gs_handle, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get watermark level failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: watermark level is 0x%02X.\n", reg);

    /* adxl375_get_trigger_status test */
    adxl375_interface_debug_print("adxl375: adxl375_get_trigger_status test.\n");
    res = adxl375_get_trigger_status(&gs_handle, (adxl375_trigger_status_t *)&trigger);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get trigger status failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: trigger status is 0x%02X.\n", trigger);

    /* adxl375_set_link_activity_inactivity/adxl375_get_link_activity_inactivity test */
    adxl375_interface_debug_print("adxl375: adxl375_set_link_activity_inactivity/adxl375_get_link_activity_inactivity test.\n");

    /* enable */
    res = adxl375_set_link_activity_inactivity(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set link activity inactivity failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: enable link activity inactivity.\n");
    res = adxl375_get_link_activity_inactivity(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get link activity inactivity failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check link activity inactivity %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_link_activity_inactivity(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set link activity inactivity failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: disable link activity inactivity.\n");
    res = adxl375_get_link_activity_inactivity(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get link activity inactivity    failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check link activity inactivity %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_auto_sleep/adxl375_get_auto_sleep test */
    adxl375_interface_debug_print("adxl375: adxl375_set_auto_sleep/adxl375_get_auto_sleep test.\n");

    /* enable */
    res = adxl375_set_auto_sleep(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set auto sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: enable auto sleep.\n");
    res = adxl375_get_auto_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get auto sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check auto sleep %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_auto_sleep(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set auto sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: disable auto sleep.\n");
    res = adxl375_get_auto_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get auto sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check auto sleep %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_measure/adxl375_get_measure test */
    adxl375_interface_debug_print("adxl375: adxl375_set_measure/adxl375_get_measure test.\n");

    /* enable */
    res = adxl375_set_measure(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set measure failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: enable measure.\n");
    res = adxl375_get_measure(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get measure failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check measure %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_measure(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set measure failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: disable measure.\n");
    res = adxl375_get_measure(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get measure failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check measure %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_sleep/adxl375_get_sleep test */
    adxl375_interface_debug_print("adxl375: adxl375_set_sleep/adxl375_get_sleep test.\n");

    /* enable */
    res = adxl375_set_sleep(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: enable sleep.\n");
    res = adxl375_get_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check sleep %s.\n", bool_test==ADXL375_BOOL_TRUE?"ok":"error");

    /* disable */
    res = adxl375_set_sleep(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: disable sleep.\n");
    res = adxl375_get_sleep(&gs_handle, &bool_test);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check sleep %s.\n", bool_test==ADXL375_BOOL_FALSE?"ok":"error");

    /* adxl375_set_sleep_frequency/adxl375_get_sleep_frequency test */
    adxl375_interface_debug_print("adxl375: adxl375_set_sleep_frequency/adxl375_get_sleep_frequency test.\n");

    /* 8Hz */
    res = adxl375_set_sleep_frequency(&gs_handle, ADXL375_SLEEP_FREQUENCY_8HZ);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set sleep frequency 8 Hz.\n");
    res = adxl375_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check sleep frequency %s.\n", sleep_frequency==ADXL375_SLEEP_FREQUENCY_8HZ?"ok":"error");

    /* 4Hz */
    res = adxl375_set_sleep_frequency(&gs_handle, ADXL375_SLEEP_FREQUENCY_4HZ);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set sleep frequency 4 Hz.\n");
    res = adxl375_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check sleep frequency %s.\n", sleep_frequency==ADXL375_SLEEP_FREQUENCY_4HZ?"ok":"error");

    /* 2Hz */
    res = adxl375_set_sleep_frequency(&gs_handle, ADXL375_SLEEP_FREQUENCY_2HZ);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set sleep frequency 2 Hz.\n");
    res = adxl375_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check sleep frequency %s.\n", sleep_frequency==ADXL375_SLEEP_FREQUENCY_2HZ?"ok":"error");

    /* 1Hz */
    res = adxl375_set_sleep_frequency(&gs_handle, ADXL375_SLEEP_FREQUENCY_1HZ);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: set sleep frequency 1 Hz.\n");
    res = adxl375_get_sleep_frequency(&gs_handle, &sleep_frequency);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: check sleep frequency %s.\n", sleep_frequency==ADXL375_SLEEP_FREQUENCY_1HZ?"ok":"error");

    /* adxl375_shock_threshold_convert_to_register/adxl375_shock_threshold_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_shock_threshold_convert_to_register/adxl375_shock_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl375_shock_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: shock threshold convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_shock_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: shock threshold convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: shock threshold before is %.4f.\n", f);
    adxl375_interface_debug_print("adxl375: shock threshold register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: shock threshold convert is %.4f.\n", f_check);

    /* adxl375_offset_convert_to_register/adxl375_offset_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_offset_convert_to_register/adxl375_offset_convert_to_data test.\n");
    f = (rand()%100)/100.0f;
    res = adxl375_offset_convert_to_register(&gs_handle, f, (int8_t *)&i_reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: offset convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_offset_convert_to_data(&gs_handle, i_reg, (float *)&f_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: offset convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: offset before is %.4f.\n", f);
    adxl375_interface_debug_print("adxl375: offset register is 0x%02X.\n", i_reg);
    adxl375_interface_debug_print("adxl375: offset convert is %.4f.\n", f_check);

    /* adxl375_duration_convert_to_register/adxl375_duration_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_duration_convert_to_register/adxl375_duration_convert_to_data test.\n");
    us = (rand()%10)*625;
    res = adxl375_duration_convert_to_register(&gs_handle, us, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: duration convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_duration_convert_to_data(&gs_handle, reg, (uint32_t *)&us_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: duration convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: duration before is %d.\n", us);
    adxl375_interface_debug_print("adxl375: duration register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: duration convert is %d.\n", us_check);

    /* adxl375_latent_convert_to_register/adxl375_latent_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_latent_convert_to_register/adxl375_latent_convert_to_data test.\n");
    f = (rand()%800)/10.0f;
    res = adxl375_latent_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: latent convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_latent_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: latent convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: latent before is %.4f.\n", f);
    adxl375_interface_debug_print("adxl375: latent register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: latent convert is %.4f.\n", f_check);

    /* adxl375_window_convert_to_register/adxl375_window_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_window_convert_to_register/adxl375_window_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl375_window_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: window convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_window_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: window convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: window before is %.4f.\n", f);
    adxl375_interface_debug_print("adxl375: window register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: window convert is %.4f.\n", f_check);

    /* adxl375_action_threshold_convert_to_register/adxl375_action_threshold_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_action_threshold_convert_to_register/adxl375_action_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl375_action_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: action threshold convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_action_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: action threshold convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: action threshold before is %.4f.\n", f);
    adxl375_interface_debug_print("adxl375: action threshold register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: action threshold convert is %.4f.\n", f_check);

    /* adxl375_inaction_threshold_convert_to_register/adxl375_inaction_threshold_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_inaction_threshold_convert_to_register/adxl375_inaction_threshold_convert_to_data test.\n");
    f = (rand()%800)/100.0f;
    res = adxl375_inaction_threshold_convert_to_register(&gs_handle, f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: inaction threshold convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_inaction_threshold_convert_to_data(&gs_handle, reg, (float *)&f_check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: inaction threshold convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: inaction threshold before is %.4f.\n", f);
    adxl375_interface_debug_print("adxl375: inaction threshold register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: inaction threshold convert is %.4f.\n", f_check);

    /* adxl375_inaction_time_convert_to_register/adxl375_inaction_time_convert_to_data test */
    adxl375_interface_debug_print("adxl375: adxl375_inaction_time_convert_to_register/adxl375_inaction_time_convert_to_data test.\n");
    check = rand()%256;
    res = adxl375_inaction_time_convert_to_register(&gs_handle, check, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: inaction time convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    res = adxl375_inaction_time_convert_to_data(&gs_handle, reg, (uint8_t *)&check);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: inaction time convert to data failed.\n");
        (void)adxl375_deinit(&gs_handle);

        return 1;
    }
    adxl375_interface_debug_print("adxl375: inaction time before is 0x%02X.\n", check);
    adxl375_interface_debug_print("adxl375: inaction time register is 0x%02X.\n", reg);
    adxl375_interface_debug_print("adxl375: inaction time convert is 0x%02X.\n", check);

    /* finish register test */
    adxl375_interface_debug_print("adxl375: finish register test.\n");
    (void)adxl375_deinit(&gs_handle);

    return 0;
}
