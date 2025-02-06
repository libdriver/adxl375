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
 * @file      driver_adxl375.c
 * @brief     driver adxl375 source file
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

#include "driver_adxl375.h"

/**
 * @brief chip register definition
 */
#define CHIP_NAME                 "Analog Devices ADXL375"        /**< chip name */
#define MANUFACTURER_NAME         "Analog Devices"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.0f                            /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                            /**< chip max supply voltage */
#define MAX_CURRENT               0.145f                          /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                          /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                           /**< chip max operating temperature */
#define DRIVER_VERSION            1000                            /**< driver version */

/**
 * @brief chip register definition
 */
#define ADXL375_REG_DEVID                 0x00        /**< device id register */
#define ADXL375_REG_THRESH_SHOCK          0x1D        /**< thresh shock register */
#define ADXL375_REG_OFSX                  0x1E        /**< x offset register */
#define ADXL375_REG_OFSY                  0x1F        /**< y offset register */
#define ADXL375_REG_OFSZ                  0x20        /**< z offset register */
#define ADXL375_REG_DUR                   0x21        /**< duration time register */
#define ADXL375_REG_LATENT                0x22        /**< latent register */
#define ADXL375_REG_WINDOW                0x23        /**< window register */
#define ADXL375_REG_THRESH_ACT            0x24        /**< act thresh register */
#define ADXL375_REG_THRESH_INACT          0x25        /**< inact thresh register */
#define ADXL375_REG_TIME_INACT            0x26        /**< inact time register */
#define ADXL375_REG_ACT_INACT_CTL         0x27        /**< act inact ctl register */
#define ADXL375_REG_SHOCK_AXES            0x2A        /**< shock axes register */
#define ADXL375_REG_ACT_SHOCK_STATUS      0x2B        /**< shock status register */
#define ADXL375_REG_BW_RATE               0x2C        /**< bandwidth rate register */
#define ADXL375_REG_POWER_CTL             0x2D        /**< power control register */
#define ADXL375_REG_INT_ENABLE            0x2E        /**< interrupt enable register */
#define ADXL375_REG_INT_MAP               0x2F        /**< interrupt map register */
#define ADXL375_REG_INT_SOURCE            0x30        /**< interrupt source register */
#define ADXL375_REG_DATA_FORMAT           0x31        /**< data format register */
#define ADXL375_REG_DATAX0                0x32        /**< data X0 register */
#define ADXL375_REG_DATAX1                0x33        /**< data X1 register */
#define ADXL375_REG_DATAY0                0x34        /**< data Y0 register */
#define ADXL375_REG_DATAY1                0x35        /**< data Y1 register */
#define ADXL375_REG_DATAZ0                0x36        /**< data Z0 register */
#define ADXL375_REG_DATAZ1                0x37        /**< data Z1 register */
#define ADXL375_REG_FIFO_CTL              0x38        /**< fifo control register */
#define ADXL375_REG_FIFO_STATUS           0x39        /**< fifo status register */

/**
 * @brief      iic or spi interface read bytes
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_adxl375_iic_spi_read(adxl375_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == ADXL375_INTERFACE_IIC)                        /* iic interface */
    {
        if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)      /* read data */
        {
            return 1;                                                    /* return error */
        }
        else
        {
            return 0;                                                    /* success return 0 */
        }
    }
    else                                                                 /* spi interface */
    {
        if (len > 1)                                                     /* if length > 1 */
        {
            reg |= 1 << 6;                                               /* flag length > 1 */
        }
        reg |= 1 << 7;                                                   /* flag read */
        
        if (handle->spi_read(reg, buf, len) != 0)                        /* read data */
        {
            return 1;                                                    /* return error */
        }
        else
        {
            return 0;                                                    /* success return 0 */
        }
    }
}

/**
 * @brief     iic or spi interface write bytes
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_adxl375_iic_spi_write(adxl375_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == ADXL375_INTERFACE_IIC)                         /* iic interface */
    {
        if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)      /* write data */
        {
            return 1;                                                     /* return error */
        }
        else
        {
            return 0;                                                     /* success return 0 */
        }
    }
    else                                                                  /* spi interface */
    {
        if (len > 1)                                                      /* if length > 1 */
        {
            reg |= 1 << 6;                                                /* flag length > 1 */
        }
        
        if (handle->spi_write(reg, buf, len) != 0)                        /* write data */
        {
            return 1;                                                     /* return error */
        }
        else
        {
            return 0;                                                     /* success return 0 */
        }
    }
}

/**
 * @brief     set the chip interface
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] interface chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t adxl375_set_interface(adxl375_handle_t *handle, adxl375_interface_t interface) 
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;        /* set interface */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *interface pointer to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl375_get_interface(adxl375_handle_t *handle, adxl375_interface_t *interface) 
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    
    *interface = (adxl375_interface_t)(handle->iic_spi);        /* get interface */
    
    return 0;                                                   /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an adxl375 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t adxl375_set_addr_pin(adxl375_handle_t *handle, adxl375_address_t addr_pin)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;      /* set pin */
    
    return 0;                                  /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an adxl375 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl375_get_addr_pin(adxl375_handle_t *handle, adxl375_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (adxl375_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t adxl375_set_shock_threshold(adxl375_handle_t *handle, uint8_t threshold)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_THRESH_SHOCK, &threshold, 1);     /* write config */
}

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
uint8_t adxl375_get_shock_threshold(adxl375_handle_t *handle, uint8_t *threshold)
{
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_THRESH_SHOCK, threshold, 1);     /* read config */
}

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
uint8_t adxl375_shock_threshold_convert_to_register(adxl375_handle_t *handle, float g, uint8_t *reg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *reg = (uint8_t)(g / 0.780f);         /* convert real data to register data */
    
    return 0;                             /* success return 0 */
}

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
uint8_t adxl375_shock_threshold_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *g)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *g = (float)(reg) * 0.780f;         /* convert raw data to real data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t adxl375_set_offset(adxl375_handle_t *handle, int8_t x, int8_t y, int8_t z)
{
    uint8_t res;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_adxl375_iic_spi_write(handle, ADXL375_REG_OFSX, (uint8_t *)&x, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("adxl375: write failed.\n");                             /* write failed */
       
        return 1;                                                                    /* return error */
    }
    res = a_adxl375_iic_spi_write(handle, ADXL375_REG_OFSY, (uint8_t *)&y, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("adxl375: write failed.\n");                             /* write failed */
       
        return 1;                                                                    /* return error */
    }
    res = a_adxl375_iic_spi_write(handle, ADXL375_REG_OFSZ, (uint8_t *)&z, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("adxl375: write failed.\n");                             /* write failed */
       
        return 1;                                                                    /* return error */
    }

    return 0;                                                                        /* success return 0 */ 
}

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
uint8_t adxl375_get_offset(adxl375_handle_t *handle, int8_t *x, int8_t *y, int8_t *z)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_OFSX, (uint8_t *)x, 1);       /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                            /* read failed */
       
        return 1;                                                                  /* return error */
    }
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_OFSY, (uint8_t *)y, 1);       /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                            /* read failed */
       
        return 1;                                                                  /* return error */
    }
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_OFSZ, (uint8_t *)z, 1);       /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                            /* read failed */
       
        return 1;                                                                  /* return error */
    }

    return 0;                                                                      /* success return 0 */
}

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
uint8_t adxl375_offset_convert_to_register(adxl375_handle_t *handle, float g, int8_t *reg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *reg = (int8_t)(g / 0.196f);        /* convert real data to register data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t adxl375_offset_convert_to_data(adxl375_handle_t *handle, int8_t reg, float *g)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *g = (float)(reg) * 0.196f;        /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t adxl375_set_duration(adxl375_handle_t *handle, uint8_t t)
{
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_DUR, &t, 1);       /* write config */
}

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
uint8_t adxl375_get_duration(adxl375_handle_t *handle, uint8_t *t)
{
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_DUR, t, 1);       /* read config */
}

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
uint8_t adxl375_duration_convert_to_register(adxl375_handle_t *handle, uint32_t us, uint8_t *reg)
{
    if (handle == NULL)                /* check handle */
    { 
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *reg = (uint8_t)(us / 625);        /* convert real data to register data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t adxl375_duration_convert_to_data(adxl375_handle_t *handle, uint8_t reg, uint32_t *us)
{
    if (handle == NULL)                             /* check handle */
    {
        return 2;                                   /* return error */
    }
    if (handle->inited != 1)                        /* check handle initialization */
    {
        return 3;                                   /* return error */
    }
    
    *us = (uint32_t)((float)(reg) * 625.0f);        /* convert raw data to real data */
    
    return 0;                                       /* success return 0 */
}

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
uint8_t adxl375_set_latent(adxl375_handle_t *handle, uint8_t t)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_LATENT, &t, 1);       /* write config */
}

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
uint8_t adxl375_get_latent(adxl375_handle_t *handle, uint8_t *t)
{
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_LATENT, t, 1);       /* read config */
}

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
uint8_t adxl375_latent_convert_to_register(adxl375_handle_t *handle, float ms, uint8_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (uint8_t)(ms / 1.25f);        /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t adxl375_latent_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *ms)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *ms = (float)(reg) * 1.25f;        /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t adxl375_set_window(adxl375_handle_t *handle, uint8_t t)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_WINDOW, &t, 1);       /* write config */
}

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
uint8_t adxl375_get_window(adxl375_handle_t *handle, uint8_t *t)
{
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_WINDOW, t, 1);       /* read config */
}

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
uint8_t adxl375_window_convert_to_register(adxl375_handle_t *handle, float ms, uint8_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (uint8_t)(ms / 1.25f);        /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t adxl375_window_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *ms)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *ms = (float)(reg) * 1.25f;        /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t adxl375_set_action_threshold(adxl375_handle_t *handle, uint8_t threshold)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_THRESH_ACT, &threshold, 1);       /* write config */
}

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
uint8_t adxl375_get_action_threshold(adxl375_handle_t *handle, uint8_t *threshold)
{
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_THRESH_ACT, threshold, 1);       /* read config */
}

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
uint8_t adxl375_action_threshold_convert_to_register(adxl375_handle_t *handle, float g, uint8_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (uint8_t)(g / 0.780f);        /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t adxl375_action_threshold_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *g)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *g = (float)(reg) * 0.780f;        /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t adxl375_set_inaction_threshold(adxl375_handle_t *handle, uint8_t threshold)
{
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_THRESH_INACT, &threshold, 1);       /* write config */
}

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
uint8_t adxl375_get_inaction_threshold(adxl375_handle_t *handle, uint8_t *threshold)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_THRESH_INACT, threshold, 1);       /* read config */
}

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
uint8_t adxl375_inaction_threshold_convert_to_register(adxl375_handle_t *handle, float g, uint8_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (uint8_t)(g / 0.780f);        /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t adxl375_inaction_threshold_convert_to_data(adxl375_handle_t *handle, uint8_t reg, float *g)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *g = (float)(reg) * 0.780f;        /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t adxl375_set_inaction_time(adxl375_handle_t *handle, uint8_t t)
{
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_TIME_INACT, &t, 1);       /* write config */
}

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
uint8_t adxl375_get_inaction_time(adxl375_handle_t *handle, uint8_t *t)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_TIME_INACT, t, 1);       /* read config */
}

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
uint8_t adxl375_inaction_time_convert_to_register(adxl375_handle_t *handle, uint8_t s, uint8_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = s;                       /* convert real data to register data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t adxl375_inaction_time_convert_to_data(adxl375_handle_t *handle, uint8_t reg, uint8_t *s)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *s = reg;                       /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t adxl375_set_action_inaction(adxl375_handle_t *handle, adxl375_action_inaction_t type, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                           /* read failed */
        
        return 1;                                                                                 /* return error */
    }
    prev &= ~(1 << type);                                                                         /* clear type */
    prev |= (enable << type);                                                                     /* set type */
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_action_inaction(adxl375_handle_t *handle, adxl375_action_inaction_t type, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
       
        return 1;                                                                               /* return error */
    }
    prev &= (1 << type);                                                                        /* get type */
    *enable = (adxl375_bool_t)(prev >> type);                                                   /* set type */

    return 0;                                                                                   /* success return 0 */
}

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
uint8_t adxl375_set_action_coupled(adxl375_handle_t *handle, adxl375_coupled_t coupled)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                           /* read failed */
       
        return 1;                                                                                 /* return error */
    }
    prev &= ~(1 << 7);                                                                            /* clear coupled */
    prev |= (coupled << 7);                                                                       /* set coupled */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_action_coupled(adxl375_handle_t *handle, adxl375_coupled_t *coupled)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
       
        return 1;                                                                               /* return error */
    }
    prev &= (1 << 7);                                                                           /* clear config */
    *coupled = (adxl375_coupled_t)(prev >> 7);                                                  /* set config */

    return 0;                                                                                   /* success return 0 */
}

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
uint8_t adxl375_set_inaction_coupled(adxl375_handle_t *handle, adxl375_coupled_t coupled)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
       
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 3);                                                                          /* clear config */
    prev |= (coupled << 3);                                                                     /* set inaction coupled */
   
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);     /* write config */
}

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
uint8_t adxl375_get_inaction_coupled(adxl375_handle_t *handle, adxl375_coupled_t *coupled)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_INACT_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
       
        return 1;                                                                               /* return error */
    }
    prev &= (1 << 3);                                                                           /* clear config */
    *coupled = (adxl375_coupled_t)(prev >> 3);                                                  /* get inaction coupled */

    return 0;                                                                                   /* success return 0 */
}

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
uint8_t adxl375_set_shock_axis(adxl375_handle_t *handle, adxl375_shock_axis_t axis, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_SHOCK_AXES, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << axis);                                                                    /* clear axis */
    prev |= enable << axis;                                                                  /* set axis */
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_SHOCK_AXES, (uint8_t *)&prev, 1);     /* write config */
}

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
uint8_t adxl375_get_shock_axis(adxl375_handle_t *handle, adxl375_shock_axis_t axis, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_SHOCK_AXES, (uint8_t *)&prev, 1);     /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                    /* read failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= (1 << axis);                                                                   /* clear axis */
    *enable = (adxl375_bool_t)(prev >> axis);                                              /* set axis */

    return 0;                                                                              /* success return 0 */
}

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
uint8_t adxl375_set_shock_suppress(adxl375_handle_t *handle, adxl375_bool_t enable)
{
    uint8_t res, prev;
   
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_SHOCK_AXES, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 3);                                                                       /* clear suppress */
    prev |= enable << 3;                                                                     /* set suppress */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_SHOCK_AXES, (uint8_t *)&prev, 1);     /* write config */
}

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
uint8_t adxl375_get_shock_suppress(adxl375_handle_t *handle, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_SHOCK_AXES, (uint8_t *)&prev, 1);     /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                    /* read failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= (1 << 3);                                                                      /* clear config */
    *enable = (adxl375_bool_t)(prev >> 3);                                                 /* set config */

    return 0;                                                                              /* success return 0 */
}

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
uint8_t adxl375_get_shock_status(adxl375_handle_t *handle, uint8_t *status)
{
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_ACT_SHOCK_STATUS, (uint8_t *)status, 1);     /* read config */
}

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
uint8_t adxl375_set_rate(adxl375_handle_t *handle, adxl375_rate_t rate)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_BW_RATE, (uint8_t *)&prev, 1);          /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 1;                                                                            /* return error */
    }
    prev &= ~(0x1F);                                                                         /* clear rate */
    prev |= rate;                                                                            /* set rate */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_BW_RATE, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t adxl375_get_rate(adxl375_handle_t *handle, adxl375_rate_t *rate)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_BW_RATE, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                   /* read failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= 0x1F;                                                                         /* clear config */
    *rate = (adxl375_rate_t)(prev);                                                       /* get rate */

    return 0;                                                                             /* success return 0 */
}

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
uint8_t adxl375_set_interrupt(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_INT_ENABLE, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                        /* read failed */
       
        return 1;                                                                              /* return error */
    }
    prev &= ~(1 << type);                                                                      /* clear interrupt */
    prev |= enable << type;                                                                    /* set interrupt */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_INT_ENABLE, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_interrupt(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_INT_ENABLE, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= (1 << type);                                                                     /* clear config */
    *enable = (adxl375_bool_t)(prev >> type);                                                /* set config */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t adxl375_set_interrupt_map(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_interrupt_pin_t pin)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_INT_MAP, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << type);                                                                   /* clear type */
    prev |= pin << type;                                                                    /* set interrupt map */
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_INT_MAP, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_interrupt_map(adxl375_handle_t *handle, adxl375_interrupt_t type, adxl375_interrupt_pin_t *pin)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_INT_MAP, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                   /* read failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= (1 << type);                                                                  /* clear config */
    *pin = (adxl375_interrupt_pin_t)(prev >> type);                                       /* set interrupt map */

    return 0;                                                                             /* success return 0 */
}

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
uint8_t adxl375_get_interrupt_source(adxl375_handle_t *handle, uint8_t *source)
{
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, ADXL375_REG_INT_SOURCE, (uint8_t *)source, 1);       /* read config */
}

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
uint8_t adxl375_set_self_test(adxl375_handle_t *handle, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
       
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 7);                                                                          /* clear config */
    prev |= (enable << 7);                                                                      /* set self test */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_self_test(adxl375_handle_t *handle, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
        
        return 1;                                                                             /* return error */
    }
    prev &= (1 << 7);                                                                         /* clear config */
    *enable = (adxl375_bool_t)(prev >> 7);                                                    /* set self test */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t adxl375_set_spi_wire(adxl375_handle_t *handle, adxl375_spi_wire_t wire)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
       
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 6);                                                                          /* clear config */
    prev |= (wire << 6);                                                                        /* set wire */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_spi_wire(adxl375_handle_t *handle, adxl375_spi_wire_t *wire)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= (1 << 6);                                                                         /* clear config */
    *wire = (adxl375_spi_wire_t)(prev >> 6);                                                  /* set wire */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t adxl375_set_interrupt_active_level(adxl375_handle_t *handle, adxl375_interrupt_active_level_t active_level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
        
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 5);                                                                          /* clear config */
    prev |= (active_level << 5);                                                                /* set active level */
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_interrupt_active_level(adxl375_handle_t *handle, adxl375_interrupt_active_level_t *active_level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);     /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
        
        return 1;                                                                           /* return error */
    }
    prev &= (1 << 5);                                                                       /* get config */
    *active_level = (adxl375_interrupt_active_level_t)(prev >> 5);                          /* get active level */
    
    return 0;                                                                               /* success return 0 */ 
}

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
uint8_t adxl375_set_justify(adxl375_handle_t *handle, adxl375_justify_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                         /* read failed */
        
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 2);                                                                          /* clear config */
    prev |= (enable << 2);                                                                      /* set justify */
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_justify(adxl375_handle_t *handle, adxl375_justify_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
        
        return 1;                                                                             /* return error */
    }
    prev &= (1 << 2);                                                                         /* get config */
    *enable = (adxl375_justify_t)(prev >> 2);                                                 /* get justify */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t adxl375_set_mode(adxl375_handle_t *handle, adxl375_mode_t mode)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 1;                                                                            /* return error */
    }
    prev &= ~(3 << 6);                                                                       /* clear config */
    prev |= (mode << 6);                                                                     /* set mode */
    
    return a_adxl375_iic_spi_write(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_mode(adxl375_handle_t *handle, adxl375_mode_t *mode)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                    /* read failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= (3 << 6);                                                                      /* get config */
    *mode = (adxl375_mode_t)(prev >> 6);                                                   /* set mode */

    return 0;                                                                              /* success return 0 */
}

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
uint8_t adxl375_set_trigger_pin(adxl375_handle_t *handle, adxl375_interrupt_pin_t pin)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 5);                                                                       /* clear config */
    prev |= (pin << 5);                                                                      /* set pin */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_trigger_pin(adxl375_handle_t *handle, adxl375_interrupt_pin_t *pin)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                    /* read failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= (1 << 5);                                                                      /* clear config */
    *pin = (adxl375_interrupt_pin_t)(prev >> 5);                                           /* set pin */

    return 0;                                                                              /* success return 0 */
}

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
uint8_t adxl375_set_watermark(adxl375_handle_t *handle, uint8_t level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 1;                                                                            /* return error */
    }
    prev &= ~(0x1F);                                                                         /* clear config */
    prev |= (level & 0x1F);                                                                  /* set watermark */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_watermark(adxl375_handle_t *handle, uint8_t *level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                    /* read failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= (0x1F);                                                                        /* get config */
    *level = prev & 0x1F;                                                                  /* get watermark */

    return 0;                                                                              /* success return 0 */
}

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
uint8_t adxl375_get_watermark_level(adxl375_handle_t *handle, uint8_t *level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_STATUS, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
        
        return 1;                                                                             /* return error */
    }
    prev &= (0x3F);                                                                           /* get config */
    *level = prev & 0x3F;                                                                     /* get level */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t adxl375_get_trigger_status(adxl375_handle_t *handle, adxl375_trigger_status_t *status)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_STATUS, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= (1 << 7);                                                                         /* get config */
    *status = (adxl375_trigger_status_t)((prev >> 7) & 0x01);                                 /* get status */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t adxl375_set_link_activity_inactivity(adxl375_handle_t *handle, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << 5);                                                                        /* clear config */
    prev |= enable << 5;                                                                      /* set enable */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_link_activity_inactivity(adxl375_handle_t *handle, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= 1 << 5;                                                                         /* get config */
    *enable = (adxl375_bool_t)(prev >> 5);                                                  /* get enable */

    return 0;                                                                               /* success return 0 */
}

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
uint8_t adxl375_set_auto_sleep(adxl375_handle_t *handle, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << 4);                                                                        /* get config */
    prev |= enable << 4;                                                                      /* set enable */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_auto_sleep(adxl375_handle_t *handle, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
        
        return 1;                                                                           /* return error */
    }
    prev &= 1 << 4;                                                                         /* get config */
    *enable = (adxl375_bool_t)(prev >> 4);                                                  /* get auto sleep */
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t adxl375_set_measure(adxl375_handle_t *handle, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << 3);                                                                        /* clear config */
    prev |= enable << 3;                                                                      /* set measure */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_measure(adxl375_handle_t *handle, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= 1 << 3;                                                                         /* get config */
    *enable = (adxl375_bool_t)(prev >> 3);                                                  /* get measure */

    return 0;                                                                               /* success return 0 */
}

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
uint8_t adxl375_set_sleep(adxl375_handle_t *handle, adxl375_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << 2);                                                                        /* clear config */
    prev |= enable << 2;                                                                      /* set sleep */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_sleep(adxl375_handle_t *handle, adxl375_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= 1 << 2;                                                                         /* get config */
    *enable = (adxl375_bool_t)(prev >> 2);                                                  /* get sleep */
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t adxl375_set_sleep_frequency(adxl375_handle_t *handle, adxl375_sleep_frequency_t sleep_frequency)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~0x03;                                                                            /* clear config */
    prev |= sleep_frequency;                                                                  /* set frequency */

    return a_adxl375_iic_spi_write(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t adxl375_get_sleep_frequency(adxl375_handle_t *handle, adxl375_sleep_frequency_t *sleep_frequency)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                     /* read failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= 0x03;                                                                           /* get config */
    *sleep_frequency = (adxl375_sleep_frequency_t)(prev & 0x03);                            /* get frequency */
   
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an adxl375 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 close failed
 * @note      none
 */
static uint8_t a_adxl375_close(adxl375_handle_t *handle)
{
    if (handle->iic_spi == ADXL375_INTERFACE_IIC)                        /* iic interface */
    {
        if (handle->iic_deinit() != 0)                                   /* iic deinit */
        {
            handle->debug_print("adxl375: iic deinit failed.\n");        /* iic deinit failed */
       
            return 1;                                                    /* return error */
        }
        else
        {
            return 0;                                                    /* success return 0 */
        }
    }
    else                                                                 /* spi interface */
    {
        if (handle->spi_deinit() != 0)                                   /* spi deinit */
        {
            handle->debug_print("adxl375: spi deinit failed.\n");        /* spi deinit failed */
       
            return 1;                                                    /* return error */
        }
        else
        {
            return 0;                                                    /* success return 0 */
        }
    }
}

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
uint8_t adxl375_init(adxl375_handle_t *handle)
{
    uint8_t id;
  
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                                /* check debug_print */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->iic_init == NULL)                                                   /* check iic_init */
    {
        handle->debug_print("adxl375: iic_init is null.\n");                        /* iic_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_deinit == NULL)                                                 /* check iic_deinit */
    {
        handle->debug_print("adxl375: iic_deinit is null.\n");                      /* iic_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_read == NULL)                                                   /* check iic_read */
    {
        handle->debug_print("adxl375: iic_read is null.\n");                        /* iic_read is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_write == NULL)                                                  /* check iic_write */
    {
        handle->debug_print("adxl375: iic_write is null.\n");                       /* iic_write is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_init == NULL)                                                   /* check spi_init */
    {
        handle->debug_print("adxl375: spi_init is null.\n");                        /* spi_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_deinit == NULL)                                                 /* check spi_deinit */
    {
        handle->debug_print("adxl375: spi_deinit is null.\n");                      /* spi_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_read == NULL)                                                   /* check spi_read */
    {
        handle->debug_print("adxl375: spi_read is null.\n");                        /* spi_read is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_write == NULL)                                                  /* check spi_write */
    {
        handle->debug_print("adxl375: spi_write is null.\n");                       /* spi_write is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                                   /* check delay_ms */
    {
        handle->debug_print("adxl375: delay_ms is null.\n");                        /* delay_ms is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->receive_callback == NULL)                                           /* check receive_callback */
    {
        handle->debug_print("adxl375: receive_callback is null.\n");                /* receive_callback is null */
       
        return 3;                                                                   /* return error */
    }
    
    if (handle->iic_spi == ADXL375_INTERFACE_IIC)                                   /* iic interface */
    {
        if (handle->iic_init() != 0)                                                /* initialize iic bus */
        {
            handle->debug_print("adxl375: iic init failed.\n");                     /* iic init failed */
            
            return 1;                                                               /* return error */
        }
    }
    else                                                                            /* spi interface */
    {
        if (handle->spi_init() != 0)                                                /* initialize spi bus */
        {
            handle->debug_print("adxl375: spi init failed.\n");                     /* spi init failed */
           
            return 1;                                                               /* return error */
        }
    }
    if (a_adxl375_iic_spi_read(handle, ADXL375_REG_DEVID, (uint8_t *)&id, 1) != 0)  /* read id */
    {
        handle->debug_print("adxl375: read failed.\n");                             /* read failed */
        (void)a_adxl375_close(handle);                                              /* close */
        
        return 4;                                                                   /* return error */
    }
    if (id != 0xE5)                                                                 /* check id */
    {
        handle->debug_print("adxl375: id is invalid.\n");                           /* id is invalid */
        (void)a_adxl375_close(handle);                                              /* close */
       
        return 4;                                                                   /* return error */
    }
    handle->inited = 1;                                                             /* flag finish initialization */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t adxl375_deinit(adxl375_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
       
        return 4;                                                                            /* return error */
    }
    prev &= ~(1 << 3);                                                                       /* stop measure */
    prev |= 1 << 2;                                                                          /* sleep */
    res = a_adxl375_iic_spi_write(handle, ADXL375_REG_POWER_CTL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: write failed.\n");                                     /* write failed */
       
        return 4;                                                                            /* return error */
    }
    else
    {
        res = a_adxl375_close(handle);                                                       /* close */
        if (res != 0)                                                                        /* check result */
        {
            return 1;                                                                        /* return error */
        }
        else
        {
            handle->inited = 0;                                                              /* flag close */
    
            return 0;                                                                        /* success return 0 */
        }
    }
}

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
uint8_t adxl375_read(adxl375_handle_t *handle, int16_t (*raw)[3], float (*g)[3], uint16_t *len)
{
    uint8_t res, prev;
    uint8_t justify;
    uint8_t mode, cnt, i;
    uint8_t buf[32 * 6];
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    if ((*len) == 0)                                                                              /* check length */
    {
        handle->debug_print("adxl375: length is zero.\n");                                        /* length is zero */
       
        return 1;                                                                                 /* return error */
    }
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_CTL, (uint8_t *)&prev, 1);              /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                           /* read failed */
       
        return 1;                                                                                 /* return error */
    }
    mode = prev >> 6;                                                                             /* get mode */
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATA_FORMAT, (uint8_t *)&prev, 1);           /* read config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                           /* read failed */
       
        return 1;                                                                                 /* return error */
    }
    justify = (prev >> 2) & 0x01;                                                                 /* get justify */
    if (mode == ADXL375_MODE_BYPASS)                                                              /* bypass */
    {
        *len = 1;                                                                                 /* set length 1 */
        res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATAX0, (uint8_t *)buf, 6);              /* read data */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
           
            return 1;                                                                             /* return error */
        }
        raw[0][0] = (int16_t)(buf[1] << 8) | buf[0];                                              /* set raw z */
        raw[0][1] = (int16_t)(buf[3] << 8) | buf[2];                                              /* set raw y */
        raw[0][2] = (int16_t)(buf[5] << 8) | buf[4];                                              /* set raw z */
        if (justify == 1)                                                                         /* if justify */
        {
            if ((raw[0][0] & (1 << 15)) != 0)                                                     /* check signed bit */
            {
                raw[0][0] = ((uint16_t)0xFC << 8) | ((raw[0][0] >> 6) & 0x3FF);                   /* negative */
            }
            else
            {
                raw[0][0] = (raw[0][0] >> 6) & 0x3FF;                                             /* positive */
            }
            if ((raw[0][1] & (1 << 15)) != 0)                                                     /* check signed bit */
            {
                raw[0][1] = ((uint16_t)0xFC << 8) | ((raw[0][1] >> 6) & 0x3FF);                   /* negative */
            }
            else
            {
                raw[0][1] = (raw[0][1] >> 6) & 0x3FF;                                             /* positive */
            }
            if ((raw[0][2] & (1 << 15)) != 0)                                                     /* check signed bit */
            {
                raw[0][2] = ((uint16_t)0xFC << 8) | ((raw[0][2] >> 6) & 0x3FF);                   /* negative */
            }
            else
            {
                raw[0][2] = (raw[0][2] >> 6) & 0x3FF;                                             /* positive */
            }
        }
        g[0][0] = (float)(raw[0][0]) * 0.049f;                                                    /* convert x */
        g[0][1] = (float)(raw[0][1]) * 0.049f;                                                    /* convert y */
        g[0][2] = (float)(raw[0][2]) * 0.049f;                                                    /* convert z */
    }
    else                                                                                          /* fifo mode */
    {
        res = a_adxl375_iic_spi_read(handle, ADXL375_REG_FIFO_STATUS, (uint8_t *)&prev, 1);       /* read fifo status */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
            
            return 1;                                                                             /* return error */
        }
        cnt = prev & 0x3F;                                                                        /* get cnt */
        *len = ((*len) < cnt) ? (*len) : cnt;                                                     /* get min cnt */
        res = a_adxl375_iic_spi_read(handle, ADXL375_REG_DATAX0, (uint8_t *)buf, 6*(*len));       /* read data */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("adxl375: read failed.\n");                                       /* read failed */
           
            return 1;                                                                             /* return error */
        }
        for (i = 0; i < (*len); i++)                                                              /* read length */
        {
            raw[i][0] = (int16_t)(buf[1 + i * 6] << 8) | buf[0 + i * 6];                          /* set raw x */
            raw[i][1] = (int16_t)(buf[3 + i * 6] << 8) | buf[2 + i * 6];                          /* set raw y */
            raw[i][2] = (int16_t)(buf[5 + i * 6] << 8) | buf[4 + i * 6];                          /* set raw z */
            if (justify == 1)                                                                     /* if justify */
            {
                if ((raw[i][0] & (1 << 15)) != 0)                                                 /* check signed bit */
                {
                    raw[i][0] = ((uint16_t)0xFC << 8) | ((raw[i][0] >> 6) & 0x3FF);               /* negative */
                }
                else
                {
                    raw[i][0] = (raw[i][0] >> 6) & 0x3FF;                                         /* positive */
                }
                if ((raw[i][1] & (1 << 15)) != 0)                                                 /* check signed bit */
                {
                    raw[i][1] = ((uint16_t)0xFC << 8) | ((raw[i][1] >> 6) & 0x3FF);               /* negative */
                }
                else
                {
                    raw[i][1] = (raw[i][1] >> 6) & 0x3FF;                                         /* positive */
                }
                if ((raw[i][2] & (1 << 15)) != 0)                                                 /* check signed bit */
                {
                    raw[i][2] = ((uint16_t)0xFC << 8) | ((raw[i][2] >> 6) & 0x3FF);               /* negative */
                }
                else
                {
                    raw[i][2] = (raw[i][2] >> 6) & 0x3FF;                                         /* positive */
                }
            }
            g[i][0] = (float)(raw[i][0])*0.049f;                                                  /* convert x */
            g[i][1] = (float)(raw[i][1])*0.049f;                                                  /* convert y */
            g[i][2] = (float)(raw[i][2])*0.049f;                                                  /* convert z */
        }
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t adxl375_irq_handler(adxl375_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_adxl375_iic_spi_read(handle, ADXL375_REG_INT_SOURCE, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("adxl375: read failed.\n");                                      /* read failed */
        
        return 1;                                                                            /* return error */
    }
    if ((prev & (1 << ADXL375_INTERRUPT_DATA_READY)) != 0)                                   /* if data ready */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_DATA_READY);                          /* run callback */
        }
    }
    if ((prev & (1 << ADXL375_INTERRUPT_SINGLE_SHOCK)) != 0)                                 /* if single shock */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_SINGLE_SHOCK);                        /* run callback */
        }
    }
    if ((prev & (1 << ADXL375_INTERRUPT_DOUBLE_SHOCK)) != 0)                                 /* if double shock */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_DOUBLE_SHOCK);                        /* run callback */
        }
    }
    if ((prev & (1 << ADXL375_INTERRUPT_ACTIVITY)) != 0)                                     /* if activity */
    {
        if (handle->receive_callback != NULL)                                                /*if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_ACTIVITY);                            /* run callback */
        }
    }
    if ((prev & (1 << ADXL375_INTERRUPT_INACTIVITY)) != 0)                                   /* if inactivity */
    {
        if (handle->receive_callback != NULL)                                                /*if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_INACTIVITY);                          /* run callback */
        }
    }
    if ((prev & (1 << ADXL375_INTERRUPT_WATERMARK)) != 0)                                    /* if watermark */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_WATERMARK);                           /* run callback */
        }
    }
    if ((prev & (1 << ADXL375_INTERRUPT_OVERRUN)) != 0)                                      /* if overrun */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(ADXL375_INTERRUPT_OVERRUN);                             /* run callback */
        }
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t adxl375_set_reg(adxl375_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    return a_adxl375_iic_spi_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t adxl375_get_reg(adxl375_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
    
    return a_adxl375_iic_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an adxl375 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl375_info(adxl375_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(adxl375_info_t));                        /* initialize adxl375 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
