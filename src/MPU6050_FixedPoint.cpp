/**
 * Implementation of MPU6050 which returns gyroscope, accelerometer and 
 * temperature readings as 16.16 fixed-point numbers for faster calculations on 
 * systems which don't have a Floating Point Unit (FPU).
 * 
 * External references:
 * MPU-6000/MPU-6050 Product Specification, PS-MPU-6000A-00, rev 3.4
 * MPU-6000/MPU-6050 Register Map and Descriptions, RM-MPU-6000A-00, rev. 4.2
 */
#include "MPU6050_FixedPoint.h"
#include <Wire.h>

struct RawVector 
{
    int16_t X;
    int16_t Y;
    int16_t Z;
};

struct RawReading 
{
    RawVector AccVector;
    int16_t Temp;
    RawVector GyroVector;
};

MPU6050_FixedPoint::MPU6050_FixedPoint(uint8_t address) :
    Address(address),
    GyroRange(MPU6050_GYRO_RANGE_250_DPS),
    AccelRange(MPU6050_ACCEL_RANGE_2_G),
    GyroFactor(1),
    AccelFactor(1)
{
    // Empty
}

MPU6050_FixedPoint::~MPU6050_FixedPoint()
{
    // Empty
}

bool MPU6050_FixedPoint::begin()
{
    Wire.begin();
    if (!reset()) {
        return false;
    }
    setGyroRange(MPU6050_GYRO_RANGE_250_DPS);
    setAccelRange(MPU6050_ACCEL_RANGE_2_G);
    setLowPass(MPU6050_LOW_PASS_44_HZ);
    return true;
}

void MPU6050_FixedPoint::getGyroAccTemp(Vector3Fix& gyroVector, Vector3Fix& accVector, fixed& temp)
{
    RawReading reading;
    burstRead(MPU6050_REG_ACCEL_XOUT_H, sizeof(reading), (uint8_t*)&reading);

    // Convert from raw reading to 16.16 fixed point
    gyroVector.X = reading.GyroVector.X * GyroFactor;
    gyroVector.Y = reading.GyroVector.Y * GyroFactor;
    gyroVector.Z = reading.GyroVector.Z * GyroFactor;

    // Convert from raw reading to 16.16 fixed point
    accVector.X = reading.AccVector.X * AccelFactor;
    accVector.Y = reading.AccVector.Y * AccelFactor;
    accVector.Z = reading.AccVector.Z * AccelFactor;

    // Convert temp from raw reading to 16.16 fixed point.
    // See section 4.18 in the MPU6050 Register Map.
    int32_t temp_whole = (int32_t)(reading.Temp / 340) + 36;
    int32_t temp_frac = reading.Temp % 340 + 0.53 * 340;
    temp = (fixed)(temp_whole * FIX_SCALE + temp_frac * FIX_SCALE / 340);
}

bool MPU6050_FixedPoint::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(Address);
    Wire.write(reg);
    Wire.write(value);
    uint8_t ret = Wire.endTransmission(true);
    return ret == 0;
}

bool MPU6050_FixedPoint::burstRead(uint8_t reg, uint8_t size, uint8_t* buffer) 
{
    Wire.beginTransmission(Address);
    Wire.write(reg);
    uint8_t ret = Wire.endTransmission(false);
    if (ret != 0) {
        return false;
    }
    uint8_t bytes = Wire.requestFrom(Address, (uint8_t)6, (uint8_t)true);
    for (uint8_t i = 0; i < size; ++i) {
        buffer[i] = Wire.read();
    }
    return bytes == size;
}

bool MPU6050_FixedPoint::reset()
{
    return writeRegister(MPU6050_REG_PWR_MGMGT_1, 0b10000000);
}

bool MPU6050_FixedPoint::setLowPass(MPU6050_LowPassConfig lowPassConfig)
{
    return writeRegister(MPU6050_REG_CONFIG, lowPassConfig);
}

bool MPU6050_FixedPoint::setGyroRange(MPU6050_GyroRange gyroRange)
{
    bool ret = writeRegister(MPU6050_REG_GYRO_CONFIG, gyroRange << 3);
    if (ret) {
        // See section 4.19 in the MPU6050 Register Map. 
        GyroRange = gyroRange;
        if (GyroRange == MPU6050_GYRO_RANGE_250_DPS) {
            GyroFactor = 500; // 500 ~ 65536 / 131 
        }
        else if (GyroRange == MPU6050_GYRO_RANGE_500_DPS) {
            GyroFactor = 1001; // 1001 ~ 65536 / 65.5
        }
        else if (GyroRange == MPU6050_GYRO_RANGE_1000_DPS) {
            GyroFactor = 1998; // 1998 ~ 65536 / 32.8
        }
        else if (GyroRange == MPU6050_GYRO_RANGE_2000_DPS) {
            GyroFactor = 3996; // 3996 ~ 65536 / 16.4
        }
    }
    return ret;
}

bool MPU6050_FixedPoint::setAccelRange(MPU6050_AccelRange accelRange)
{
    uint8_t ret = writeRegister(MPU6050_REG_ACCEL_CONFIG, accelRange << 3);
    if (ret) {
        // See section 4.17 in the MPU6050 Register Map. 
        AccelRange = accelRange;
        if (AccelRange == MPU6050_ACCEL_RANGE_2_G) {
            AccelFactor = 4; // 4 = 65546 / 16384
        }
        else if (AccelRange == MPU6050_ACCEL_RANGE_4_G) {
            AccelFactor = 8; // 8 = 65546 / 8192
        }
        else if (AccelRange == MPU6050_ACCEL_RANGE_8_G) {
            AccelFactor = 16; // 16 = 65546 / 4192
        }
        else if (AccelRange == MPU6050_ACCEL_RANGE_16_G) {
            AccelFactor = 32; // 32 = 65546 / 2048
        }
    }
    return ret;
}
