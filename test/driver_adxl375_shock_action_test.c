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
 * @file      driver_adxl375_shock_action_test.c
 * @brief     driver adxl375 shock action test source file
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

#include "driver_adxl375_shock_action_test.h"

static adxl375_handle_t gs_handle;          /**< adxl375 handle */
static uint8_t gs_shock_action_flag;        /**< shock action flag */

/**
 * @brief  action test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl375_action_test_irq_handler(void)
{
    if (adxl375_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     interface test receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_adxl375_interface_test_receive_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL375_INTERRUPT_DATA_READY :
        {
            break;
        }
        case ADXL375_INTERRUPT_SINGLE_SHOCK :
        {
            gs_shock_action_flag |= 1 << 0;
            adxl375_interface_debug_print("adxl375: irq single shock.\n");
            
            break;
        }
        case ADXL375_INTERRUPT_DOUBLE_SHOCK :
        {
            gs_shock_action_flag |= 1 << 1;
            adxl375_interface_debug_print("adxl375: irq double shock.\n");
            
            break;
        }
        case ADXL375_INTERRUPT_ACTIVITY :
        {
            gs_shock_action_flag |= 1 << 2;
            adxl375_interface_debug_print("adxl375: irq activity.\n");
            
            break;
        }
        case ADXL375_INTERRUPT_INACTIVITY :
        {
            gs_shock_action_flag |= 1 << 3;
            adxl375_interface_debug_print("adxl375: irq inactivity.\n");
            
            break;
        }
        case ADXL375_INTERRUPT_OVERRUN :
        {
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief  test server
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
static uint8_t a_adxl375_test_server(void)
{
    int16_t raw[3];
    float g_out[3];
    uint16_t len;
    
    len = 1;
    
    /* read data */
    if (adxl375_read(&gs_handle, (int16_t (*)[3])&raw, (float (*)[3])&g_out, (uint16_t *)&len) != 0)
    {
        adxl375_interface_debug_print("adxl375: read failed.\n");
       
        return 1;
    }

    return 0;
}

/**
 * @brief     shock action test
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t adxl375_shock_action_test(adxl375_interface_t interface, adxl375_address_t addr_pin)
{
    uint8_t res;
    uint8_t source;
    int8_t reg;
    adxl375_info_t info;
    
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
    DRIVER_ADXL375_LINK_RECEIVE_CALLBACK(&gs_handle, a_adxl375_interface_test_receive_callback);
    
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
    
    /* set interface */
    res = adxl375_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interface failed.\n");
       
        return 1;
    }
    
    /* set address pin */
    res = adxl375_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set addr pin failed.\n");
       
        return 1;
    }
    
    /* adxl375 initialization */
    res = adxl375_init(&gs_handle);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: init failed.\n");
       
        return 1;
    }
    
    /* set 12.5 rate */
    res = adxl375_set_rate(&gs_handle, ADXL375_RATE_12P5);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 4 wire */
    res = adxl375_set_spi_wire(&gs_handle, ADXL375_SPI_WIRE_4);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set spi wire failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt low */
    res = adxl375_set_interrupt_active_level(&gs_handle, ADXL375_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt active level failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable auto sleep */
    res = adxl375_set_auto_sleep(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set auto sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable sleep */
    res = adxl375_set_sleep(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep 1Hz */
    res = adxl375_set_sleep_frequency(&gs_handle, ADXL375_SLEEP_FREQUENCY_1HZ);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set justify right */
    res = adxl375_set_justify(&gs_handle, ADXL375_JUSTIFY_RIGHT);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set justify failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set bypass mode */
    res = adxl375_set_mode(&gs_handle, ADXL375_MODE_BYPASS);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set mode failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set trigger pin int 2 */
    res = adxl375_set_trigger_pin(&gs_handle, ADXL375_INTERRUPT_PIN2);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set trigger pin failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    } 
    
    /* set watermark 16 level */
    res = adxl375_set_watermark(&gs_handle, 16);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set watermark failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set offset */
    res = adxl375_offset_convert_to_register(&gs_handle, 0.0f, (int8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: offset convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    } 
    res = adxl375_set_offset(&gs_handle, reg, reg, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set offset failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set shock */
    res = adxl375_shock_threshold_convert_to_register(&gs_handle, 3.0f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: shock threshold convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_shock_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set duration time */
    res = adxl375_duration_convert_to_register(&gs_handle, 10*1000, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: duration convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_duration(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set duration failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set latent time */
    res = adxl375_latent_convert_to_register(&gs_handle, 20.0f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: latent convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_latent(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set latent failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set window */
    res = adxl375_window_convert_to_register(&gs_handle, 80.0f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: window convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_window(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set window failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set shock x */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_X, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Y, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Z, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable suppress */
    res = adxl375_set_shock_suppress(&gs_handle, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock suppress failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set single shock */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set double shock */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* link activity and inactivity */
    res = adxl375_set_link_activity_inactivity(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set link activity inactivity failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action threshold */
    res = adxl375_action_threshold_convert_to_register(&gs_handle, 2.0f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: action threshold convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_action_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction threshold */
    res = adxl375_inaction_threshold_convert_to_register(&gs_handle, 1.0f, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: inaction threshold convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_inaction_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction threshold failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction time */
    res = adxl375_inaction_time_convert_to_register(&gs_handle, 3, (uint8_t *)&reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: inaction time convert to register failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_inaction_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction time failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action x */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_X, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action y */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Y, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action z */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Z, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction x */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_X, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction y */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Y, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction z */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Z, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set action ac coupled */
    res = adxl375_set_action_coupled(&gs_handle, ADXL375_COUPLED_AC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction dc coupled */
    res = adxl375_set_inaction_coupled(&gs_handle, ADXL375_COUPLED_DC);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set activity */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* map int 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inactivity */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1 */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_INTERRUPT_PIN1);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set overrun */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_BOOL_FALSE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear interrupt */
    res = adxl375_get_interrupt_source(&gs_handle, &source);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: get interrupt source failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measure */
    res = adxl375_set_measure(&gs_handle, ADXL375_BOOL_TRUE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set measure failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    adxl375_interface_delay_ms(10);
    
    /* start shock action test */
    adxl375_interface_debug_print("adxl375: start shock action test.\n"); 
    
    /* single shock test */
    adxl375_interface_debug_print("adxl375: please single shock.\n");
    gs_shock_action_flag = 0;
    while (1)
    {
        if ((gs_shock_action_flag & (1 << 0)) == (1 << 0))
        {
            adxl375_interface_debug_print("adxl375: detect single shock.\n");
               
            break;
        }
        if (a_adxl375_test_server() != 0)
        {
            return 1;
        }
        adxl375_interface_delay_ms(500);
    }
    
    /* double shock test */
    adxl375_interface_debug_print("adxl375: please double shock.\n");
    gs_shock_action_flag = 0;
    while (1)
    {
        if ((gs_shock_action_flag & (1 << 1)) == (1 << 1))
        {
            adxl375_interface_debug_print("adxl375: detect double shock.\n");
           
            break;
        }
        if (a_adxl375_test_server() != 0)
        {
            return 1;
        }
        adxl375_interface_delay_ms(500);
    }
    
    /* action */
    adxl375_interface_debug_print("adxl375: please action.\n");
    gs_shock_action_flag = 0;
    while (1)
    {
        if ((gs_shock_action_flag & (1 << 2)) == (1 << 2))
        {
            adxl375_interface_debug_print("adxl375: detect action.\n");
           
            break;
        }
        if (a_adxl375_test_server() != 0)
        {
            return 1;
        }
        adxl375_interface_delay_ms(500);
    }
    
    /* inaction */
    adxl375_interface_debug_print("adxl375: please stop action.\n");
    gs_shock_action_flag = 0;
    while (1)
    {
        if ((gs_shock_action_flag & (1 << 3)) == (1 << 3))
        {
            adxl375_interface_debug_print("adxl375: detect stop.\n");
           
            break;
        }
        if (a_adxl375_test_server() != 0)
        {
            return 1;
        }
        adxl375_interface_delay_ms(500);
    }
    
    adxl375_interface_debug_print("adxl375: finish shock action test.\n");
    (void)adxl375_deinit(&gs_handle);
    
    return 0;
}
