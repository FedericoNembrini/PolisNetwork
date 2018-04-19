#include <SPI.h>
#include <Fishino.h>
#include <SD.h>
#include <FishinoUdp.h>
#include <ArduinoJson.h>

#define FILE "settings.json"

const int chipSelect = 4;

static char *SSID, *PASSW, *THINGTAG, *METRICTAG;
static int IP_HUB_POLIS_NETWORK;
static int Analog_Input = A7;
FishinoClient client;

void setup()
{
    Serial.begin(250000);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    
    pinMode(Analog_Input, INPUT);
    
    LoadSettings();
    delay(1000);
    WifiConnection();
}

void loop()
{
    /*double temperature = LeggiTemperatura();

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

    delay(120000);*/
}

void LoadSettings()
{
    Serial.println("Initializing Card...");

    while(!SD.begin(chipSelect));
    
    Serial.println("Card Initialized.");
    
    char json[160];
    File fileSettings = SD.open(FILE);
    fileSettings.readBytes(json, sizeof(json));
    fileSettings.close();

    StaticJsonBuffer<JSON_OBJECT_SIZE(5)> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    SSID = root["ssid"];
    PASSW = root["passw"];
    IP_HUB_POLIS_NETWORK = root["ipHub"];
    THINGTAG = root["thingTag"];
    METRICTAG = root["metricTag"];

}

void WifiConnection()
{
    while(!Fishino.reset());
    Serial.println("Fishino Reeboted Correctly");
        
    Fishino.setMode(STATION_MODE);
    
    while(!Fishino.begin(SSID, PASSW));
    Serial.println("Fishino Connected");
    
    delay(8000);
    Serial.println("Fishino IP:");
    Serial.println(Fishino.localIP());
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