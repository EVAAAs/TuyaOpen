/**
 * @file board_bmi270_api.h
 * @author Tuya Inc.
 * @brief BMI270 sensor driver API for TUYA_T5AI_POCKET board
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */

#ifndef __BOARD_BMI270_API_H__
#define __BOARD_BMI270_API_H__

#include "tuya_cloud_types.h"
#include "tkl_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/

/* BMI270 I2C Configuration */
#define BMI270_I2C_PORT              TUYA_I2C_NUM_0
#define BMI270_I2C_ADDR              0x68  /* BMI270 I2C address (ADDR pin = 0) */
#define BMI270_I2C_ADDR_ALT          0x69  /* BMI270 I2C address (ADDR pin = 1) */

/* BMI270 Register Addresses */
#define BMI270_REG_CHIP_ID           0x00
#define BMI270_REG_ERR_REG           0x02
#define BMI270_REG_STATUS            0x1E
#define BMI270_REG_AUX_X_LSB        0x0C
#define BMI270_REG_AUX_X_MSB        0x0D
#define BMI270_REG_AUX_Y_LSB        0x0E
#define BMI270_REG_AUX_Y_MSB        0x0F
#define BMI270_REG_AUX_Z_LSB        0x10
#define BMI270_REG_AUX_Z_MSB        0x11
#define BMI270_REG_GYR_X_LSB        0x12
#define BMI270_REG_GYR_X_MSB        0x13
#define BMI270_REG_GYR_Y_LSB        0x14
#define BMI270_REG_GYR_Y_MSB        0x15
#define BMI270_REG_GYR_Z_LSB        0x16
#define BMI270_REG_GYR_Z_MSB        0x17
#define BMI270_REG_ACC_X_LSB        0x18
#define BMI270_REG_ACC_X_MSB        0x19
#define BMI270_REG_ACC_Y_LSB        0x1A
#define BMI270_REG_ACC_Y_MSB        0x1B
#define BMI270_REG_ACC_Z_LSB        0x1C
#define BMI270_REG_ACC_Z_MSB        0x1D
#define BMI270_REG_TEMP_MSB         0x20
#define BMI270_REG_TEMP_LSB         0x21
#define BMI270_REG_AUX_IF_0         0x22
#define BMI270_REG_AUX_IF_1         0x23
#define BMI270_REG_AUX_IF_2         0x24
#define BMI270_REG_AUX_IF_3          0x25
#define BMI270_REG_AUX_IF_4         0x26
#define BMI270_REG_AUX_IF_5         0x27
#define BMI270_REG_AUX_IF_6         0x28
#define BMI270_REG_AUX_IF_7         0x29
#define BMI270_REG_AUX_IF_8         0x2A
#define BMI270_REG_AUX_IF_9         0x2B
#define BMI270_REG_AUX_IF_10        0x2C
#define BMI270_REG_AUX_IF_11        0x2D
#define BMI270_REG_AUX_IF_12        0x2E
#define BMI270_REG_AUX_IF_13        0x2F
#define BMI270_REG_AUX_IF_14        0x30
#define BMI270_REG_AUX_IF_15        0x31
#define BMI270_REG_AUX_IF_16        0x32
#define BMI270_REG_AUX_IF_17        0x33
#define BMI270_REG_AUX_IF_18        0x34
#define BMI270_REG_AUX_IF_19        0x35
#define BMI270_REG_AUX_IF_20        0x36
#define BMI270_REG_AUX_IF_21        0x37
#define BMI270_REG_AUX_IF_22        0x38
#define BMI270_REG_AUX_IF_23        0x39
#define BMI270_REG_AUX_IF_24        0x3A
#define BMI270_REG_AUX_IF_25        0x3B
#define BMI270_REG_AUX_IF_26        0x3C
#define BMI270_REG_AUX_IF_27        0x3D
#define BMI270_REG_AUX_IF_28        0x3E
#define BMI270_REG_AUX_IF_29        0x3F
#define BMI270_REG_AUX_IF_30        0x40
#define BMI270_REG_AUX_IF_31        0x41
#define BMI270_REG_AUX_IF_32        0x42
#define BMI270_REG_AUX_IF_33        0x43
#define BMI270_REG_AUX_IF_34        0x44
#define BMI270_REG_AUX_IF_35        0x45
#define BMI270_REG_AUX_IF_36        0x46
#define BMI270_REG_AUX_IF_37        0x47
#define BMI270_REG_AUX_IF_38        0x48
#define BMI270_REG_AUX_IF_39        0x49
#define BMI270_REG_AUX_IF_40        0x4A
#define BMI270_REG_AUX_IF_41        0x4B
#define BMI270_REG_AUX_IF_42        0x4C
#define BMI270_REG_AUX_IF_43        0x4D
#define BMI270_REG_AUX_IF_44        0x4E
#define BMI270_REG_AUX_IF_45        0x4F
#define BMI270_REG_AUX_IF_46        0x50
#define BMI270_REG_AUX_IF_47        0x51
#define BMI270_REG_AUX_IF_48        0x52
#define BMI270_REG_AUX_IF_49        0x53
#define BMI270_REG_AUX_IF_50        0x54
#define BMI270_REG_AUX_IF_51        0x55
#define BMI270_REG_AUX_IF_52        0x56
#define BMI270_REG_AUX_IF_53        0x57
#define BMI270_REG_AUX_IF_54        0x58
#define BMI270_REG_AUX_IF_55        0x59
#define BMI270_REG_AUX_IF_56        0x5A
#define BMI270_REG_AUX_IF_57        0x5B
#define BMI270_REG_AUX_IF_58        0x5C
#define BMI270_REG_AUX_IF_59        0x5D
#define BMI270_REG_AUX_IF_60        0x5E
#define BMI270_REG_AUX_IF_61        0x5F
#define BMI270_REG_AUX_IF_62        0x60
#define BMI270_REG_AUX_IF_63        0x61
#define BMI270_REG_AUX_IF_64        0x62
#define BMI270_REG_AUX_IF_65        0x63
#define BMI270_REG_AUX_IF_66        0x64
#define BMI270_REG_AUX_IF_67        0x65
#define BMI270_REG_AUX_IF_68        0x66
#define BMI270_REG_AUX_IF_69        0x67
#define BMI270_REG_AUX_IF_70        0x68
#define BMI270_REG_AUX_IF_71        0x69
#define BMI270_REG_AUX_IF_72        0x6A
#define BMI270_REG_AUX_IF_73        0x6B
#define BMI270_REG_AUX_IF_74        0x6C
#define BMI270_REG_AUX_IF_75        0x6D
#define BMI270_REG_AUX_IF_76        0x6E
#define BMI270_REG_AUX_IF_77        0x6F
#define BMI270_REG_AUX_IF_78        0x70
#define BMI270_REG_AUX_IF_79        0x71
#define BMI270_REG_AUX_IF_80        0x72
#define BMI270_REG_AUX_IF_81        0x73
#define BMI270_REG_AUX_IF_82        0x74
#define BMI270_REG_AUX_IF_83        0x75
#define BMI270_REG_AUX_IF_84        0x76
#define BMI270_REG_AUX_IF_85        0x77
#define BMI270_REG_AUX_IF_86        0x78
#define BMI270_REG_AUX_IF_87        0x79
#define BMI270_REG_AUX_IF_88        0x7A
#define BMI270_REG_AUX_IF_89        0x7B
#define BMI270_REG_AUX_IF_90        0x7C
#define BMI270_REG_AUX_IF_91        0x7D
#define BMI270_REG_AUX_IF_92        0x7E
#define BMI270_REG_AUX_IF_93        0x7F
#define BMI270_REG_AUX_IF_94        0x80
#define BMI270_REG_AUX_IF_95        0x81
#define BMI270_REG_AUX_IF_96        0x82
#define BMI270_REG_AUX_IF_97        0x83
#define BMI270_REG_AUX_IF_98        0x84
#define BMI270_REG_AUX_IF_99        0x85
#define BMI270_REG_AUX_IF_100       0x86
#define BMI270_REG_AUX_IF_101       0x87
#define BMI270_REG_AUX_IF_102       0x88
#define BMI270_REG_AUX_IF_103       0x89
#define BMI270_REG_AUX_IF_104       0x8A
#define BMI270_REG_AUX_IF_105       0x8B
#define BMI270_REG_AUX_IF_106       0x8C
#define BMI270_REG_AUX_IF_107       0x8D
#define BMI270_REG_AUX_IF_108       0x8E
#define BMI270_REG_AUX_IF_109       0x8F
#define BMI270_REG_AUX_IF_110       0x90
#define BMI270_REG_AUX_IF_111       0x91
#define BMI270_REG_AUX_IF_112       0x92
#define BMI270_REG_AUX_IF_113       0x93
#define BMI270_REG_AUX_IF_114       0x94
#define BMI270_REG_AUX_IF_115       0x95
#define BMI270_REG_AUX_IF_116       0x96
#define BMI270_REG_AUX_IF_117       0x97
#define BMI270_REG_AUX_IF_118       0x98
#define BMI270_REG_AUX_IF_119       0x99
#define BMI270_REG_AUX_IF_120       0x9A
#define BMI270_REG_AUX_IF_121       0x9B
#define BMI270_REG_AUX_IF_122       0x9C
#define BMI270_REG_AUX_IF_123       0x9D
#define BMI270_REG_AUX_IF_124       0x9E
#define BMI270_REG_AUX_IF_125       0x9F
#define BMI270_REG_AUX_IF_126       0xA0
#define BMI270_REG_AUX_IF_127       0xA1
#define BMI270_REG_AUX_IF_128       0xA2
#define BMI270_REG_AUX_IF_129       0xA3
#define BMI270_REG_AUX_IF_130       0xA4
#define BMI270_REG_AUX_IF_131       0xA5
#define BMI270_REG_AUX_IF_132       0xA6
#define BMI270_REG_AUX_IF_133       0xA7
#define BMI270_REG_AUX_IF_134       0xA8
#define BMI270_REG_AUX_IF_135       0xA9
#define BMI270_REG_AUX_IF_136       0xAA
#define BMI270_REG_AUX_IF_137       0xAB
#define BMI270_REG_AUX_IF_138       0xAC
#define BMI270_REG_AUX_IF_139       0xAD
#define BMI270_REG_AUX_IF_140       0xAE
#define BMI270_REG_AUX_IF_141       0xAF
#define BMI270_REG_AUX_IF_142       0xB0
#define BMI270_REG_AUX_IF_143       0xB1
#define BMI270_REG_AUX_IF_144       0xB2
#define BMI270_REG_AUX_IF_145       0xB3
#define BMI270_REG_AUX_IF_146       0xB4
#define BMI270_REG_AUX_IF_147       0xB5
#define BMI270_REG_AUX_IF_148       0xB6
#define BMI270_REG_AUX_IF_149       0xB7
#define BMI270_REG_AUX_IF_150       0xB8
#define BMI270_REG_AUX_IF_151       0xB9
#define BMI270_REG_AUX_IF_152       0xBA
#define BMI270_REG_AUX_IF_153       0xBB
#define BMI270_REG_AUX_IF_154       0xBC
#define BMI270_REG_AUX_IF_155       0xBD
#define BMI270_REG_AUX_IF_156       0xBE
#define BMI270_REG_AUX_IF_157       0xBF
#define BMI270_REG_AUX_IF_158       0xC0
#define BMI270_REG_AUX_IF_159       0xC1
#define BMI270_REG_AUX_IF_160       0xC2
#define BMI270_REG_AUX_IF_161       0xC3
#define BMI270_REG_AUX_IF_162       0xC4
#define BMI270_REG_AUX_IF_163       0xC5
#define BMI270_REG_AUX_IF_164       0xC6
#define BMI270_REG_AUX_IF_165       0xC7
#define BMI270_REG_AUX_IF_166       0xC8
#define BMI270_REG_AUX_IF_167       0xC9
#define BMI270_REG_AUX_IF_168       0xCA
#define BMI270_REG_AUX_IF_169       0xCB
#define BMI270_REG_AUX_IF_170       0xCC
#define BMI270_REG_AUX_IF_171       0xCD
#define BMI270_REG_AUX_IF_172       0xCE
#define BMI270_REG_AUX_IF_173       0xCF
#define BMI270_REG_AUX_IF_174       0xD0
#define BMI270_REG_AUX_IF_175       0xD1
#define BMI270_REG_AUX_IF_176       0xD2
#define BMI270_REG_AUX_IF_177       0xD3
#define BMI270_REG_AUX_IF_178       0xD4
#define BMI270_REG_AUX_IF_179       0xD5
#define BMI270_REG_AUX_IF_180       0xD6
#define BMI270_REG_AUX_IF_181       0xD7
#define BMI270_REG_AUX_IF_182       0xD8
#define BMI270_REG_AUX_IF_183       0xD9
#define BMI270_REG_AUX_IF_184       0xDA
#define BMI270_REG_AUX_IF_185       0xDB
#define BMI270_REG_AUX_IF_186       0xDC
#define BMI270_REG_AUX_IF_187       0xDD
#define BMI270_REG_AUX_IF_188       0xDE
#define BMI270_REG_AUX_IF_189       0xDF
#define BMI270_REG_AUX_IF_190       0xE0
#define BMI270_REG_AUX_IF_191       0xE1
#define BMI270_REG_AUX_IF_192       0xE2
#define BMI270_REG_AUX_IF_193       0xE3
#define BMI270_REG_AUX_IF_194       0xE4
#define BMI270_REG_AUX_IF_195       0xE5
#define BMI270_REG_AUX_IF_196       0xE6
#define BMI270_REG_AUX_IF_197       0xE7
#define BMI270_REG_AUX_IF_198       0xE8
#define BMI270_REG_AUX_IF_199       0xE9
#define BMI270_REG_AUX_IF_200       0xEA
#define BMI270_REG_AUX_IF_201       0xEB
#define BMI270_REG_AUX_IF_202       0xEC
#define BMI270_REG_AUX_IF_203       0xED
#define BMI270_REG_AUX_IF_204       0xEE
#define BMI270_REG_AUX_IF_205       0xEF
#define BMI270_REG_AUX_IF_206       0xF0
#define BMI270_REG_AUX_IF_207       0xF1
#define BMI270_REG_AUX_IF_208       0xF2
#define BMI270_REG_AUX_IF_209       0xF3
#define BMI270_REG_AUX_IF_210       0xF4
#define BMI270_REG_AUX_IF_211       0xF5
#define BMI270_REG_AUX_IF_212       0xF6
#define BMI270_REG_AUX_IF_213       0xF7
#define BMI270_REG_AUX_IF_214       0xF8
#define BMI270_REG_AUX_IF_215       0xF9
#define BMI270_REG_AUX_IF_216       0xFA
#define BMI270_REG_AUX_IF_217       0xFB
#define BMI270_REG_AUX_IF_218       0xFC
#define BMI270_REG_AUX_IF_219       0xFD
#define BMI270_REG_AUX_IF_220       0xFE
#define BMI270_REG_AUX_IF_221       0xFF

/* BMI270 Chip ID */
#define BMI270_CHIP_ID               0x24

/* BMI270 Power Modes */
#define BMI270_POWER_MODE_SUSPEND    0x00
#define BMI270_POWER_MODE_CONFIG     0x01
#define BMI270_POWER_MODE_LOW_POWER  0x02
#define BMI270_POWER_MODE_NORMAL     0x03

/* BMI270 Sensor Configuration */
#define BMI270_ACC_RANGE_2G          0x00
#define BMI270_ACC_RANGE_4G          0x01
#define BMI270_ACC_RANGE_8G          0x02
#define BMI270_ACC_RANGE_16G         0x03

#define BMI270_GYR_RANGE_2000DPS     0x00
#define BMI270_GYR_RANGE_1000DPS     0x01
#define BMI270_GYR_RANGE_500DPS      0x02
#define BMI270_GYR_RANGE_250DPS      0x03
#define BMI270_GYR_RANGE_125DPS      0x04

/* BMI270 Output Data Rate (ODR) */
#define BMI270_ODR_0_78HZ            0x01
#define BMI270_ODR_1_56HZ            0x02
#define BMI270_ODR_3_12HZ            0x03
#define BMI270_ODR_6_25HZ            0x04
#define BMI270_ODR_12_5HZ            0x05
#define BMI270_ODR_25HZ              0x06
#define BMI270_ODR_50HZ              0x07
#define BMI270_ODR_100HZ             0x08
#define BMI270_ODR_200HZ             0x09
#define BMI270_ODR_400HZ             0x0A
#define BMI270_ODR_800HZ             0x0B
#define BMI270_ODR_1600HZ            0x0C
#define BMI270_ODR_3200HZ            0x0D

/***********************************************************
***********************typedef define***********************
***********************************************************/

/* BMI270 Sensor Data Structure */
typedef struct {
    int16_t acc_x;    /* Accelerometer X-axis data */
    int16_t acc_y;    /* Accelerometer Y-axis data */
    int16_t acc_z;    /* Accelerometer Z-axis data */
    int16_t gyr_x;    /* Gyroscope X-axis data */
    int16_t gyr_y;    /* Gyroscope Y-axis data */
    int16_t gyr_z;    /* Gyroscope Z-axis data */
    int16_t temp;     /* Temperature data */
} bmi270_sensor_data_t;

/* BMI270 Configuration Structure */
typedef struct {
    uint8_t acc_range;    /* Accelerometer range */
    uint8_t gyr_range;    /* Gyroscope range */
    uint8_t acc_odr;      /* Accelerometer output data rate */
    uint8_t gyr_odr;      /* Gyroscope output data rate */
    uint8_t power_mode;   /* Power mode */
} bmi270_config_t;

/* BMI270 Device Structure */
typedef struct {
    TUYA_I2C_NUM_E i2c_port;     /* I2C port number */
    uint8_t i2c_addr;             /* I2C device address */
    bmi270_config_t config;       /* Sensor configuration */
    bool initialized;              /* Initialization status */
} bmi270_dev_t;

/***********************************************************
********************function declaration********************
***********************************************************/

/**
 * @brief Initialize BMI270 sensor
 * @param dev Pointer to BMI270 device structure
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_init(bmi270_dev_t *dev);

/**
 * @brief Deinitialize BMI270 sensor
 * @param dev Pointer to BMI270 device structure
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_deinit(bmi270_dev_t *dev);

/**
 * @brief Configure BMI270 sensor
 * @param dev Pointer to BMI270 device structure
 * @param config Pointer to configuration structure
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_config(bmi270_dev_t *dev, const bmi270_config_t *config);

/**
 * @brief Read sensor data from BMI270
 * @param dev Pointer to BMI270 device structure
 * @param data Pointer to sensor data structure
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_read_data(bmi270_dev_t *dev, bmi270_sensor_data_t *data);

/**
 * @brief Read accelerometer data from BMI270
 * @param dev Pointer to BMI270 device structure
 * @param acc_x Pointer to X-axis accelerometer data
 * @param acc_y Pointer to Y-axis accelerometer data
 * @param acc_z Pointer to Z-axis accelerometer data
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_read_accel(bmi270_dev_t *dev, int16_t *acc_x, int16_t *acc_y, int16_t *acc_z);

/**
 * @brief Read gyroscope data from BMI270
 * @param dev Pointer to BMI270 device structure
 * @param gyr_x Pointer to X-axis gyroscope data
 * @param gyr_y Pointer to Y-axis gyroscope data
 * @param gyr_z Pointer to Z-axis gyroscope data
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_read_gyro(bmi270_dev_t *dev, int16_t *gyr_x, int16_t *gyr_y, int16_t *gyr_z);

/**
 * @brief Read temperature data from BMI270
 * @param dev Pointer to BMI270 device structure
 * @param temp Pointer to temperature data
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_read_temp(bmi270_dev_t *dev, int16_t *temp);

/**
 * @brief Set power mode of BMI270
 * @param dev Pointer to BMI270 device structure
 * @param power_mode Power mode to set
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_set_power_mode(bmi270_dev_t *dev, uint8_t power_mode);

/**
 * @brief Check if BMI270 is ready
 * @param dev Pointer to BMI270 device structure
 * @return true if ready, false otherwise
 */
bool board_bmi270_is_ready(bmi270_dev_t *dev);

/**
 * @brief Register BMI270 sensor to hardware registration process
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_register(void);

/**
 * @brief Scan I2C bus for devices
 * @param port I2C port number
 * @return OPERATE_RET_OK on success, error code on failure
 */
OPERATE_RET board_bmi270_scan_i2c(TUYA_I2C_NUM_E port);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_BMI270_API_H__ */
