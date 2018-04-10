// Test temperature sensor 0.1
// KY-028 Temperature Sensor Module
#include <Arduino.h>

static int Analog_Input = 22;

void setup()
{
    Serial.begin (9600);
    pinMode(Analog_Input, INPUT);
}

void loop()
{
    float Analog_Value;
    Analog_Value = analogRead (Analog_Input) * (5.0 / 1023.0);
    
    Serial.print ("Temperature:"); Serial.print (Analog_Value, 4); Serial.print("\n");
}