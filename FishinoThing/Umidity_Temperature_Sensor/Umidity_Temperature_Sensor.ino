// Umidity and Temperature Sensor with DHT11 Sensor
#include <Fishino.h>
#include <SD.h>
#include <SPI.h>
#include <FishinoUdp.h>
#include <ArduinoJson.h>

#define FILE "SETTIN~1.JSO"

#define DHTPIN 7
#define DHTTYPE DHT11
#define REPORT_INTERVAL 5000

unsigned long startMills;
float Humidity;
float Temperature;
int acquireresult;
int acquirestatus;

void dht_wrapper();

PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);
bool bDHTstarted;

void dht_wrapper()
{
    DHT.isrCallback();
}

const int chipSelect = 4;

static char *IP_HUB_POLIS_NETWORK;
static char *SSID, *PASSW, *THINGTAG, *METRICTAG_01, *METRICTAG_02;
FishinoClient client;

void setup()
{
    Serial.begin(115200);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV4);

    dht.begin();

    LoadSettings();

    delay(2000);

    WiFiConnection();
}

#pragma region LoadSettings from SD

void LoadSettings()
{
    Serial.println("Initializing Card...");

    while (!SD.begin(chipSelect))
        delay(1000);

    Serial.println("Card Initialized...");

    char json[220];
    File fileSettings = SD.open(FILE);
    fileSettings.readBytes(json, sizeof(json));
    fileSettings.close();

    StaticJsonBuffer<JSON_OBJECT_SIZE(6)> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(json);

    SSID = root["ssid"];
    PASSW = root["passw"];
    IP_HUB_POLIS_NETWORK = root["ipHub"];
    THINGTAG = root["thingTag"];
    METRICTAG_01 = root["metricTag_01"];
    METRICTAG_02 = root["metricTag_02"];
}
#pragma endregion

#pragma region WifiConnection to Wi - Fi

void WiFiConnection()
{
    Fishino.setMode(STATION_MODE);

    while (!Fishino.begin(SSID, PASSW))
        delay(5000);

    Serial.println("Fishino Connected...");

    delay(10000);
    Serial.println("Fishino IP:");
    Serial.println(Fishino.localIP());
}

#pragma endregion

void loop()
{

    if (bDHTstarted)
    {
        acquirestatus = DHT.acquiring();
        if (!acquirestatus)
        {
            acquireresult = DHT.getStatus();
            if (acquireresult == 0)
            {
                t = DHT.getCelsius();
                h = DHT.getHumidity();
                d = DHT.getDewPoint();
            }
            bDHTstarted = false;
        }
    }

    if ((millis() - startMills) > REPORT_INTERVAL)
    {
        if (acquireresult == 0)
        {
            Serial.println("");

            Serial.print("Humidity (%): ");
            Serial.println(Humidity);

            Serial.print("Temperature (oC): ");
            Serial.println(Temperature);
        }
        else
        {
            Serial.println("Is DHT11 connected");
        }
        startMills = millis();

        // to remove lock
        if (acquirestatus == 1)
        {
            DHT.reset();
        }

        if (!bDHTstarted)
        {
            // non blocking method
            DHT.acquire();
            bDHTstarted = true;
        }
    }

    /*if (client.connect(IP_HUB_POLIS_NETWORK, 80))
    {
        LetturaValori();

        client.println(ConcatRequest(Humidity, 1));
        client.flush();
        client.stop();
        //client.println(ConcatRequest(Temperature, 2));
    }
    delay(30000);*/
}

void LetturaValori()
{
    Humidity = dht.readHumidity();
    //Temperature = dht.readTemperature();
}

String ConcatRequest(float RelevationAcquired, int MetricTagSelector)
{
    String request = "GET /?thingTag=";
    request.concat("uuuuuuuuuuuu");
    request.concat("&metricTag=");

    //if (MetricTagSelector == 1)
    request.concat("uuuuuuuuuuuu");

    request.concat("&value=");
    request.concat(String(RelevationAcquired));
    Serial.println(request);
    return request;
}