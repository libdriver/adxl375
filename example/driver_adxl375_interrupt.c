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
 * @file      driver_adxl375_interrupt.c
 * @brief     driver adxl375 interrupt source file
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

#include "driver_adxl375_interrupt.h"

static adxl375_handle_t gs_handle;                     /**< adxl375 handle */
static void (*a_callback)(uint8_t type) = NULL;        /**< irq callback */

/**
 * @brief  interrupt irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl375_interrupt_irq_handler(void)
{
    if (adxl375_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     interrupt receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_adxl375_interrupt_receive_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL375_INTERRUPT_DATA_READY :
        {
            if (a_callback != NULL)
            {
                a_callback(ADXL375_INTERRUPT_DATA_READY);
            }
            
            break;
        }
        case ADXL375_INTERRUPT_SINGLE_SHOCK :
        {
            if (a_callback != NULL)
            {
                a_callback(ADXL375_INTERRUPT_SINGLE_SHOCK);
            }
            
            break;
        }
        case ADXL375_INTERRUPT_DOUBLE_SHOCK :
        {
            if (a_callback != NULL)
            {
                a_callback(ADXL375_INTERRUPT_DOUBLE_SHOCK);
            }
            
            break;
        }
        case ADXL375_INTERRUPT_ACTIVITY :
        {
            if (a_callback != NULL)
            {
                a_callback(ADXL375_INTERRUPT_ACTIVITY);
            }
            
            break;
        }
        case ADXL375_INTERRUPT_INACTIVITY :
        {
            if (a_callback != NULL)
            {
                a_callback(ADXL375_INTERRUPT_INACTIVITY);
            }
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     interrupt example init
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @param[in] *callback pointer to an irq callback address
 * @param[in] shock_enable bool value
 * @param[in] action_enable bool value
 * @param[in] inaction_enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t adxl375_interrupt_init(adxl375_interface_t interface, adxl375_address_t addr_pin,
                               void (*callback)(uint8_t type),
                               adxl375_bool_t shock_enable, adxl375_bool_t action_enable, 
                               adxl375_bool_t inaction_enable)
{
    uint8_t res;
    uint8_t source;
    int8_t reg;

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
    DRIVER_ADXL375_LINK_RECEIVE_CALLBACK(&gs_handle, a_adxl375_interrupt_receive_callback);
    
    /* set the interface */
    res = adxl375_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interface failed.\n");
       
        return 1;
    }
    
    /* set addr pin */
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
    
    /* set default rate */
    res = adxl375_set_rate(&gs_handle, ADXL375_INTERRUPT_DEFAULT_RATE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set rate failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default spi wire */
    res = adxl375_set_spi_wire(&gs_handle, ADXL375_INTERRUPT_DEFAULT_SPI_WIRE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set spi wire failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt pin */
    res = adxl375_set_interrupt_active_level(&gs_handle, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_ACTIVE_LEVEL);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set adxl375 set interrupt active level failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set auto sleep */
    res = adxl375_set_auto_sleep(&gs_handle, ADXL375_INTERRUPT_DEFAULT_AUTO_SLEEP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set auto sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep */
    res = adxl375_set_sleep(&gs_handle, ADXL375_INTERRUPT_DEFAULT_SLEEP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sleep frequency */
    res = adxl375_set_sleep_frequency(&gs_handle, ADXL375_INTERRUPT_DEFAULT_SLEEP_FREQUENCY);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set sleep frequency failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set justify */
    res = adxl375_set_justify(&gs_handle, ADXL375_INTERRUPT_DEFAULT_JUSTIFY);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set justify failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set mode */
    res = adxl375_set_mode(&gs_handle, ADXL375_INTERRUPT_DEFAULT_MODE);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set mode failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set trigger pin */
    res = adxl375_set_trigger_pin(&gs_handle, ADXL375_INTERRUPT_DEFAULT_TRIGGER_PIN);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set trigger pin failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set watermark level */
    res = adxl375_set_watermark(&gs_handle, ADXL375_INTERRUPT_DEFAULT_WATERMARK);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set watermark failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default offset */
    res = adxl375_offset_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_OFFSET, (int8_t *)&reg);
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
    
    /* set default shock threshold */
    res = adxl375_shock_threshold_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_SHOCK_THRESHOLD, (uint8_t *)&reg);
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
    
    /* set default duration */
    res = adxl375_duration_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_DURATION, (uint8_t *)&reg);
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
    res = adxl375_latent_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_LATENT, (uint8_t *)&reg);
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
    
    /* set window time */
    res = adxl375_window_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_WINDOW, (uint8_t *)&reg);
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
    
    /* set shock */
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_X, shock_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Y, shock_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_shock_axis(&gs_handle, ADXL375_SHOCK_AXIS_Z, shock_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock axis failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default suppress */
    res = adxl375_set_shock_suppress(&gs_handle, ADXL375_INTERRUPT_DEFAULT_SHOCK_SUPPRESS);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set shock suppress failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default single shock map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_SINGLE_SHOCK_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable single shock */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_SINGLE_SHOCK, shock_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default double shock map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_DOUBLE_SHOCK_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable single shock */
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DOUBLE_SHOCK, shock_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /** set default linking activity and inactivity */
    res = adxl375_set_link_activity_inactivity(&gs_handle, ADXL375_INTERRUPT_DEFAULT_LINK_ACTIVITY_INACTIVITY);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set link activity inactivity failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default action threshold */
    res = adxl375_action_threshold_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_ACTION_THRESHOLD, (uint8_t *)&reg);
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
    
    /* set default inaction threshold */
    res = adxl375_inaction_threshold_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_INACTION_THRESHOLD, (uint8_t *)&reg);
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
    
    /* set default inaction time */
    res = adxl375_inaction_time_convert_to_register(&gs_handle, ADXL375_INTERRUPT_DEFAULT_INACTION_TIME, (uint8_t *)&reg);
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
    
    /* set action */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_X, action_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Y, action_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_ACTION_Z, action_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inaction */
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_X, inaction_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Y, inaction_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_action_inaction(&gs_handle, ADXL375_INACTION_Z, inaction_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action inaction failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default action coupled */
    res = adxl375_set_action_coupled(&gs_handle, ADXL375_INTERRUPT_DEFAULT_ACTION_COUPLED);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set action coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inaction coupled */
    res = adxl375_set_inaction_coupled(&gs_handle, ADXL375_INTERRUPT_DEFAULT_INACTION_COUPLED);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set inaction coupled failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default activity map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_ACTIVITY_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_ACTIVITY, action_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inactivity map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_INACTIVITY_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_INACTIVITY, inaction_enable);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default data ready map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_DATA_READY_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_DATA_READY, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_DATA_READY);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default watermark map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_WATERMARK_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_WATERMARK, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_WATERMARK);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default overrun map */
    res = adxl375_set_interrupt_map(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_OVERRUN_MAP);
    if (res != 0)
    {
        adxl375_interface_debug_print("adxl375: set interrupt map failed.\n");
        (void)adxl375_deinit(&gs_handle);
        
        return 1;
    }
    res = adxl375_set_interrupt(&gs_handle, ADXL375_INTERRUPT_OVERRUN, ADXL375_INTERRUPT_DEFAULT_INTERRUPT_OVERRUN);
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
    a_callback = callback;
    
    return 0;
}

/**
 * @brief  interrupt server
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl375_interrupt_server(void)
{
    int16_t raw[3];
    float g_out[3];
    uint16_t len;
    
    len = 1;
    
    /* read data */
    if (adxl375_read(&gs_handle, (int16_t (*)[3])&raw, (float (*)[3])&g_out, (uint16_t *)&len) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adxl375_interrupt_deinit(void)
{
    if (adxl375_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
