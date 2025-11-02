#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

const char* wifiSSID = "VM1094316"; //make sure to remove and put placeholders!!!
const char* wifiPassword = "c8bxuz5xghpLvacd";   //make sure to remove and put placeholders!!!
const char* destinationIp = "192.168.0.23";   //make sure to remove and put placeholders!!!
const unsigned int destinationPort = 4210;   //make sure to remove and put placeholders!!!???

Adafruit_BME280 bme;
BH1750 lightMeter;
WiFiUDP Udp;
unsigned long lastSend = 0;
const unsigned long sendIntervalMs = 1000;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("System starting");

  Wire.begin();
  WiFi.begin(wifiSSID, wifiPassword);
  Serial.println("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (millis() - start > 5000) {
      Serial.println("WiFi connection failed - restarting in 3s");
      delay(3000);
      ESP.restart();
    }
  }
  Serial.println("WiFi connected");

  bme.begin(0x76);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  Serial.println("Sensors initialized");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSend >= sendIntervalMs) {
    lastSend = now;
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F;
    float humidity = bme.readHumidity();
    float lightLevel = lightMeter.readLightLevel();
    
    String json = "{";
    json += "\"Temperature\": " + String(temperature, 2) + ", ";
    json += "\"Pressure\": " + String(pressure, 2) + ", ";
    json += "\"Humidity\": " + String(humidity, 2) + ", ";
    json += "\"Lightlevel\": " + String(lightLevel, 2);
    json += "}";

    Udp.beginPacket(destinationIp, destinationPort);
    Udp.print(json);
    Udp.endPacket();

    Serial.println(json);
  }

  //Testing
  //Serial.print("Temp: "); Serial.print(temperature); Serial.print(" °C | ");
  //Serial.print("Humidity: "); Serial.print(humidity); Serial.print("% | ");
  //Serial.print("Pressure: "); Serial.print(pressure); Serial.print(" hPa | ");
  //Serial.print("Light: "); Serial.print(lightLevel); Serial.println("%");

  delay(1000);
}
