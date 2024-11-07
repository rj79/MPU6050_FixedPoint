#include <Arduino.h>
#include "MPU6050_FixedPoint.h"

MPU6050_FixedPoint MPU;
unsigned long NextRead;
Vector3Fix GyroVector;
Vector3Fix AccVector;
fixed Temperature;
char Message[128];

String FixToString(fixed value)
{
    char buffer[32];
    snprintfix(buffer, sizeof(buffer), value);
    return String(buffer);
}

String VectorToString(const Vector3Fix& vector) 
{
    return String("(") + 
        FixToString(vector.X) + ", " +
        FixToString(vector.Y) + ", " +
        FixToString(vector.Z) + ")";
/*    char buffer[32];
    snprintfix(buffer, sizeof(buffer), vector.X);    
    String result = "(" + String(buffer);
    snprintfix(buffer, sizeof(buffer), vector.Y);
    result += ", " + String(buffer);
    snprintfix(buffer, sizeof(buffer), vector.Z);
    result += ", " + String(buffer) + ")";
    return result;*/
}

void fail()
{
    while (true) {};
}

void setup()
{
    Serial.begin(9600);
    delay(1000);

    if (!MPU.begin()) {
        Serial.println("Could not initialize MPU6050");
        fail();
    }

    NextRead = millis() + 100;
}

void loop()
{
    if (millis() > NextRead) {
        MPU.getGyroAccTemp(GyroVector, AccVector, Temperature);
        Serial.print("Gyro=");
        Serial.print(VectorToString(GyroVector));
        Serial.print(" Acc=");
        Serial.print(VectorToString(AccVector));
        Serial.print(" Temp=");
        Serial.println(FixToString(Temperature));
        NextRead += 100;
    }
}