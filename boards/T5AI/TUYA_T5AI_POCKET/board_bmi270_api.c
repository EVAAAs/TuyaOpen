/**
 * @file board_bmi270_api.c
 * @author Tuya Inc.
 * @brief BMI270 sensor driver implementation for TUYA_T5AI_POCKET board
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */

#include "board_bmi270_api.h"
#include "tkl_pinmux.h"
#include "tal_log.h"
#include "tkl_system.h"

/***********************************************************
***********************variable define**********************
***********************************************************/

/* Global BMI270 device instance */
static bmi270_dev_t g_bmi270_dev = {0};

/* I2C configuration for BMI270 */
static TUYA_IIC_BASE_CFG_T g_bmi270_i2c_cfg = {
    .role = TUYA_IIC_MODE_MASTER,
    .speed = TUYA_IIC_BUS_SPEED_100K,
    .addr_width = TUYA_IIC_ADDRESS_7BIT
};

/***********************************************************
***********************function define**********************
***********************************************************/

/**
 * @brief Write data to BMI270 register
 * @param dev Pointer to BMI270 device structure
 * @param reg Register address
 * @param data Data to write
 * @return OPERATE_RET_OK on success, error code on failure
 */
static OPERATE_RET bmi270_write_reg(bmi270_dev_t *dev, uint8_t reg, uint8_t data)
{
    OPERATE_RET ret;
    uint8_t buf[2];
    
    if (!dev) {
        return OPRT_INVALID_PARM;
    }
    
    buf[0] = reg;
    buf[1] = data;
    
    ret = tkl_i2c_master_send(dev->i2c_port, dev->i2c_addr, buf, 2, 3000);
    if (ret < 0) {
        return ret;
    }
    
    return OPRT_OK;
}

/**
 * @brief Read multiple bytes from BMI270 registers
 * @param dev Pointer to BMI270 device structure
 * @param reg Starting register address
 * @param data Pointer to store read data
 * @param len Number of bytes to read
 * @return OPERATE_RET_OK on success, error code on failure
 */
static OPERATE_RET bmi270_read_regs(bmi270_dev_t *dev, uint8_t reg, uint8_t *data, uint8_t len)
{
    OPERATE_RET ret;
    
    if (!dev || !data || len == 0) {
        return OPRT_INVALID_PARM;
    }
    
    ret = tkl_i2c_master_send(dev->i2c_port, dev->i2c_addr, &reg, 1, 3000);
    if (ret < 0) {
        return ret;
    }
    
    ret = tkl_i2c_master_receive(dev->i2c_port, dev->i2c_addr, data, len, 3000);
    if (ret < 0) {
        return ret;
    }
    
    return OPRT_OK;
}

// /**
//  * @brief Scan I2C bus for devices
//  * @param port I2C port number
//  * @return OPERATE_RET_OK on success, error code on failure
//  */


/**
 * @brief Test if device responds at given address
 * @param dev Pointer to BMI270 device structure
 * @return true if device responds, false otherwise
 */
static bool bmi270_test_device_response(bmi270_dev_t *dev)
{
    OPERATE_RET ret;
    uint8_t dummy_data = 0;
    
    ret = tkl_i2c_master_send(dev->i2c_port, dev->i2c_addr, &dummy_data, 1, 3000);
    return (ret == OPRT_OK);
}

/**
 * @brief Check if BMI270 chip is present
 * @param dev Pointer to BMI270 device structure
 * @return true if chip is present, false otherwise
 */
static bool bmi270_check_chip_id(bmi270_dev_t *dev)
{
    OPERATE_RET ret;
    uint8_t chip_id;
    uint8_t chip_id_reg = BMI270_REG_CHIP_ID;
    
    ret = tkl_i2c_master_send(dev->i2c_port, dev->i2c_addr, &chip_id_reg, 1, 3000);
    if (ret == OPRT_OK) {
        ret = tkl_i2c_master_receive(dev->i2c_port, dev->i2c_addr, &chip_id, 1, 3000);
        if (ret == OPRT_OK && chip_id == BMI270_CHIP_ID) {
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Configure BMI270 sensor
 * @param dev Pointer to BMI270 device structure
 * @param config Pointer to configuration structure
 * @return OPERATE_RET_OK on success, error code on failure
 */
static OPERATE_RET bmi270_configure_sensor(bmi270_dev_t *dev, const bmi270_config_t *config)
{
    OPERATE_RET ret;
    uint8_t reg_val;
    
    if (!dev || !config) {
        return OPRT_INVALID_PARM;
    }
    
    /* Configure accelerometer range and ODR */
    reg_val = (config->acc_range << 1) | config->acc_odr;
    ret = bmi270_write_reg(dev, 0x40, reg_val);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Configure gyroscope range and ODR */
    reg_val = (config->gyr_range << 1) | config->gyr_odr;
    ret = bmi270_write_reg(dev, 0x42, reg_val);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Set power mode */
    ret = bmi270_write_reg(dev, 0x7C, config->power_mode);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Store configuration */
    dev->config = *config;
    
    return OPRT_OK;
}

OPERATE_RET board_bmi270_init(bmi270_dev_t *dev)
{
    OPERATE_RET ret;
    
    if (!dev) {
        return OPRT_INVALID_PARM;
    }
    
    /* Initialize I2C */
    ret = tkl_i2c_init(BMI270_I2C_PORT, &g_bmi270_i2c_cfg);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Configure I2C pins */
    tkl_io_pinmux_config(TUYA_GPIO_NUM_20, TUYA_IIC0_SCL);
    tkl_io_pinmux_config(TUYA_GPIO_NUM_21, TUYA_IIC0_SDA);
    
    /* Initialize device structure */
    dev->i2c_port = BMI270_I2C_PORT;
    dev->i2c_addr = BMI270_I2C_ADDR;
    dev->initialized = false;
    
    /* Check if device responds at primary address */
    if (!bmi270_test_device_response(dev)) {
        dev->i2c_addr = BMI270_I2C_ADDR_ALT;
        if (!bmi270_test_device_response(dev)) {
            return OPRT_COM_ERROR;
        }
    }
    
    /* Soft reset */
    bmi270_write_reg(dev, 0x7E, 0xB6);
    tkl_system_sleep(50);
    
    /* Check chip ID */
    if (!bmi270_check_chip_id(dev)) {
        dev->i2c_addr = BMI270_I2C_ADDR_ALT;
        if (!bmi270_check_chip_id(dev)) {
            return OPRT_COM_ERROR;
        }
    }
    
    /* Set default configuration */
    bmi270_config_t default_config = {
        .acc_range = BMI270_ACC_RANGE_2G,
        .gyr_range = BMI270_GYR_RANGE_2000DPS,
        .acc_odr = BMI270_ODR_100HZ,
        .gyr_odr = BMI270_ODR_100HZ,
        .power_mode = BMI270_POWER_MODE_NORMAL
    };
    
    /* Wake up sensor */
    bmi270_write_reg(dev, 0x7C, BMI270_POWER_MODE_NORMAL);
    tkl_system_sleep(10);
    
    /* Configure sensor */
    ret = bmi270_configure_sensor(dev, &default_config);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    dev->initialized = true;
    return OPRT_OK;
}

OPERATE_RET board_bmi270_deinit(bmi270_dev_t *dev)
{
    if (!dev || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    bmi270_write_reg(dev, 0x7C, BMI270_POWER_MODE_SUSPEND);
    dev->initialized = false;
    
    return OPRT_OK;
}

OPERATE_RET board_bmi270_config(bmi270_dev_t *dev, const bmi270_config_t *config)
{
    if (!dev || !config || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    return bmi270_configure_sensor(dev, config);
}

OPERATE_RET board_bmi270_read_data(bmi270_dev_t *dev, bmi270_sensor_data_t *data)
{
    OPERATE_RET ret;
    uint8_t buf[12];
    
    if (!dev || !data || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    /* Read sensor data (6 bytes for accel + 6 bytes for gyro) */
    ret = bmi270_read_regs(dev, BMI270_REG_ACC_X_LSB, buf, 12);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Parse accelerometer data */
    data->acc_x = (int16_t)((buf[1] << 8) | buf[0]);
    data->acc_y = (int16_t)((buf[3] << 8) | buf[2]);
    data->acc_z = (int16_t)((buf[5] << 8) | buf[4]);
    
    /* Parse gyroscope data */
    data->gyr_x = (int16_t)((buf[7] << 8) | buf[6]);
    data->gyr_y = (int16_t)((buf[9] << 8) | buf[8]);
    data->gyr_z = (int16_t)((buf[11] << 8) | buf[10]);
    
    /* Read temperature data */
    ret = bmi270_read_regs(dev, BMI270_REG_TEMP_LSB, buf, 2);
    if (ret == OPRT_OK) {
        data->temp = (int16_t)((buf[1] << 8) | buf[0]);
    } else {
        data->temp = 0;
    }
    
    return OPRT_OK;
}

OPERATE_RET board_bmi270_read_accel(bmi270_dev_t *dev, int16_t *acc_x, int16_t *acc_y, int16_t *acc_z)
{
    OPERATE_RET ret;
    uint8_t buf[6];
    
    if (!dev || !acc_x || !acc_y || !acc_z || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    /* Read accelerometer data */
    ret = bmi270_read_regs(dev, BMI270_REG_ACC_X_LSB, buf, 6);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Parse accelerometer data */
    *acc_x = (int16_t)((buf[1] << 8) | buf[0]);
    *acc_y = (int16_t)((buf[3] << 8) | buf[2]);
    *acc_z = (int16_t)((buf[5] << 8) | buf[4]);
    
    return OPRT_OK;
}

OPERATE_RET board_bmi270_read_gyro(bmi270_dev_t *dev, int16_t *gyr_x, int16_t *gyr_y, int16_t *gyr_z)
{
    OPERATE_RET ret;
    uint8_t buf[6];
    
    if (!dev || !gyr_x || !gyr_y || !gyr_z || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    /* Read gyroscope data */
    ret = bmi270_read_regs(dev, BMI270_REG_GYR_X_LSB, buf, 6);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Parse gyroscope data */
    *gyr_x = (int16_t)((buf[1] << 8) | buf[0]);
    *gyr_y = (int16_t)((buf[3] << 8) | buf[2]);
    *gyr_z = (int16_t)((buf[5] << 8) | buf[4]);
    
    return OPRT_OK;
}

OPERATE_RET board_bmi270_read_temp(bmi270_dev_t *dev, int16_t *temp)
{
    OPERATE_RET ret;
    uint8_t buf[2];
    
    if (!dev || !temp || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    /* Read temperature data */
    ret = bmi270_read_regs(dev, BMI270_REG_TEMP_LSB, buf, 2);
    if (ret != OPRT_OK) {
        return ret;
    }
    
    /* Parse temperature data */
    *temp = (int16_t)((buf[1] << 8) | buf[0]);
    
    return OPRT_OK;
}

OPERATE_RET board_bmi270_set_power_mode(bmi270_dev_t *dev, uint8_t power_mode)
{
    if (!dev || !dev->initialized) {
        return OPRT_INVALID_PARM;
    }
    
    return bmi270_write_reg(dev, 0x7C, power_mode);
}

bool board_bmi270_is_ready(bmi270_dev_t *dev)
{
    return (dev && dev->initialized);
}

OPERATE_RET board_bmi270_register(void)
{
    return board_bmi270_init(&g_bmi270_dev);
}
