#include <Fishino.h>
#include <SD.h>
#include <SPI.h>
#include <FishinoUdp.h>
#include <ArduinoJson.h>

#define FILE "SETTIN~1.JSO"

const int chipSelect = 4;

static char *IP_HUB_POLIS_NETWORK;
static char *SSID, *PASSW, *THINGTAG, *METRICTAG;
static int Analog_Input = A7;
FishinoClient client;

void setup()
{
    Serial.begin(115200);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    
    pinMode(Analog_Input, INPUT);
    
    LoadSettings();
    delay(2000);
    WifiConnection();
}

void loop()
{
    double temperature = LeggiTemperatura();

    if(client.connect(IP_HUB_POLIS_NETWORK, 80))
    {
        //Serial.println(temperature);
        String request = "GET /?thingTag=";
        request.concat(THINGTAG);
        request.concat("&metricTag=");
        request.concat(METRICTAG);
        request.concat("&value=");
        request.concat(String(temperature));
        client.print(request);
        client.flush();
        client.stop();
    }

    delay(120000);
}

void LoadSettings()
{
    Serial.println("Initializing Card...");

    while(!SD.begin(chipSelect));
    delay(2000);
    Serial.println("Card Initialized...");
    
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
    Fishino.setMode(STATION_MODE);
    
    while(!Fishino.begin(SSID, PASSW))
    {
        Serial.println("Ciao");
    }
    ;
    Serial.println("Fishino Connected...");
    
    delay(10000);
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