// Test temperature sensor 0.1
// KY-013 Temperature Sensor Module
// http://sensorkit.en.joy-it.net/index.php?title=KY-013_Temperature-Sensor_module
#include <Arduino.h>

static int Analog_Input = A7;

void setup()
{
    Serial.begin (9600);
    pinMode(Analog_Input, INPUT);
}

void loop()
{
    int Valore = analogRead(Analog_Input);
    Serial.println(Temperatura(Valore));
    delay(2000);
}

double Temperatura(int Valore)
{
    double Temperatura;
    Temperatura = (1024 - Valore);
    Temperatura /= 6.6;
    Temperatura -= 55;
    return Temperatura;
}