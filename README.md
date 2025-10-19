# ESP32 WiFi Sensor Project

This project uses an ESP32 to collect temperature, pressure, humidity, and light data and send it via UDP over Wi-Fi to a Python script that visualises the results in real time. 

## Components
- ESP32 Dev Module
- Soon to be added(currently randomly generating data):
- BME280 (Temperature, Humidity, Pressure)
- BH1750 (Light Level)

## Next steps
- Fully implement the sensors to get real data
- Adapt this setup into a fully controllable robot car, the ESP32 will interface with a motor controller and receive commands from a Bluetooth connected PS4 controller allowing real-time manual control and data streaming to the dashboard
