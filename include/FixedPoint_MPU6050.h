/**
 * Implementation of MPU6050 which returns gyroscope, accelerometer and 
 * temperature readings as 16.16 fixed-point numbers for faster calculations on 
 * systems which don't have a Floating Point Unit (FPU).
 * 
 * External references:
 * MPU-6000/MPU-6050 Product Specification, PS-MPU-6000A-00, rev 3.4
 * MPU-6000/MPU-6050 Register Map and Descriptions, RM-MPU-6000A-00, rev. 4.2
 */
#ifndef FIXEDPOINT_MPU6050_H
#define FIXEDPOINT_MPU6050_H

#include "fixedpoint.h"
#include "Vector3Fix.h"

enum MPU6050_LowPassConfig 
{
    MPU6050_LOW_PASS_260_HZ = 0,
    MPU6050_LOW_PASS_184_HZ = 1,
    MPU6050_LOW_PASS_94_HZ = 2,
    MPU6050_LOW_PASS_44_HZ = 3,
    MPU6050_LOW_PASS_21_HZ = 4,
    MPU6050_LOW_PASS_10_HZ = 5,
    MPU6050_LOW_PASS_5_HZ = 6,
};

enum MPU6050_GyroRange 
{
    MPU6050_GYRO_RANGE_250_DPS = 0,
    MPU6050_GYRO_RANGE_500_DPS = 1,
    MPU6050_GYRO_RANGE_1000_DPS = 2,
    MPU6050_GYRO_RANGE_2000_DPS = 3,
};

enum MPU6050_AccelRange
{
    MPU6050_ACCEL_RANGE_2_G = 0,
    MPU6050_ACCEL_RANGE_4_G = 1,
    MPU6050_ACCEL_RANGE_8_G = 3,
    MPU6050_ACCEL_RANGE_16_G = 4
};

enum MPU6050_Register
{
    MPU6050_REG_CONFIG = 0x1a,
    MPU6050_REG_GYRO_CONFIG = 0x1b,
    MPU6050_REG_ACCEL_CONFIG = 0x1c,
    MPU6050_REG_GYRO_XOUT_H = 0x3b,
    MPU6050_REG_GYRO_XOUT_L = 0x3c,
    MPU6050_REG_GYRO_YOUT_H = 0x3d,
    MPU6050_REG_GYRO_YOUT_L = 0x3e,
    MPU6050_REG_GYRO_ZOUT_H = 0x3f,
    MPU6050_REG_GYRO_ZOUT_L = 0x40,

    MPU6050_REG_ACCEL_XOUT_H = 0x43,
    MPU6050_REG_ACCEL_XOUT_L = 0x44,
    MPU6050_REG_ACCEL_YOUT_H = 0x45,
    MPU6050_REG_ACCEL_YOUT_L = 0x46,
    MPU6050_REG_ACCEL_ZOUT_H = 0x47,
    MPU6050_REG_ACCEL_ZOUT_L = 0x48,

    MPU6050_REG_PWR_MGMGT_1 = 0x6b
};

class FixedPoint_MPU6050
{
public:
    FixedPoint_MPU6050(uint8_t address=0x68);
    ~FixedPoint_MPU6050();

    bool begin();
    void getGyroAccTemp(Vector3Fix& gyroVector, Vector3Fix& accVector, fixed& temp);

private:
    uint8_t Address;
    MPU6050_GyroRange GyroRange;
    MPU6050_AccelRange AccelRange;
    // Factor to convert from raw reading to 16.16 fixed point
    fixed GyroFactor;
    // Factor to convert from raw reading to 16.16 fixed point
    fixed AccelFactor;
    bool writeRegister(uint8_t reg, uint8_t value);
    bool burstRead(uint8_t reg, uint8_t size, uint8_t* buffer);
    bool reset();
    bool setLowPass(MPU6050_LowPassConfig lowPassConfig);
    bool setGyroRange(MPU6050_GyroRange gyroRange);
    bool setAccelRange(MPU6050_AccelRange accelRange);
};

#endif 