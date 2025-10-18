#include <WiFi.h>
#include <WiFiUdp.h>

const char* wifiSSID = "SSID"; //make sure to remove and put placeholders!!!
const char* wifiPassword = "Password";   //make sure to remove and put placeholders!!!

const char* destinationIp = "IP";   //make sure to remove and put placeholders!!!
const unsigned int destinationPort = 4210;   //make sure to remove and put placeholders!!!???

WiFiUDP Udp;
unsigned long lastSend = 0;
const unsigned long sendIntervalMs = 1000;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("System starting");

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
}

void loop() {
  //Simulating data as no real sensors as of yet
  unsigned long now = millis();
  float temperature = random(200, 350) / 10.0;
  float pressure = random(9800, 10300) / 10.0;
  float humidity = random(400, 800) / 10.0;
  float lightLevel = random(0, 100);
  if (now - lastSend >= sendIntervalMs) {
    lastSend = now;
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
