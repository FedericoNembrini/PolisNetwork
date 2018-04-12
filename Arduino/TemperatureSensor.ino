#include <Arduino.h>
#include <SPI.h>
#include <Fishino.h>

#define SSID "Nembrini_Casa"
#define PASSW "GGWPAT20"
#define IP_HUB_POLIS_NETWORK "192.168.1.30"
#define THINGTAG "cccccccccccc"
#define METRICTAG "daaaaaaaaaaa"

static int Analog_Input = A7;
FishinoClient client;

void setup()
{
    Serial.begin(9600);

    pinMode(Analog_Input, INPUT);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    while(!Fishino.reset());
    Serial.println("Fishino Reeboted Correctly");

    Fishino.setMode(STATION_MODE);

    while(!Fishino.begin(SSID, PASSW));
    Serial.println("Fishino Connected");
    
    delay(8000);
    Serial.println("Fishino IP:");
    Serial.println(Fishino.localIP());
}

void loop()
{
    double temperature = LeggiTemperatura();

    if(client.connect(IP_HUB_POLIS_NETWORK, 80))
    {
        Serial.println(temperature);
        String request = "GET /?thingTag=";
        request.concat(THINGTAG);
        request.concat("&thingMetric=");
        request.concat(METRICTAG);
        request.concat("&value=");
        request.concat(temperature);
        
        client.println(request);
    }

    delay(120000);
}

double LeggiTemperatura()
{
    int Valore = analogRead(Analog_Input);
    return Temperatura(Valore);
}

double Temperatura(int Valore)
{
    double Temperatura;
    Temperatura = (1024 - Valore);
    Temperatura /= 6.1;
    Temperatura -= 55;
    return Temperatura;
}