import socket
import json
import matplotlib.pyplot as plt
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("192.168.0.23", 4210))

timer, temperature, pressure, humidity, lightLevel = [], [], [], [], []

plt.ion()
fig, ax = plt.subplots(4, 1, figsize=(7, 6))
plt.pause(0.1)
start = time.time()

print(f"Listening for data...")

while True:
    try:
        data, addr = sock.recvfrom(1024)
        decodedData = data.decode('utf-8').strip()
        
        #test
        #print("Received raw:", decodedData)

        try:
            graphData = json.loads(decodedData)
        except json.JSONDecodeError:
            print("Received invalid data", decodedData)
            continue

        timer.append(time.time() - start)
        temperature.append(graphData.get("Temperature", 0))
        pressure.append(graphData.get("Pressure", 0))
        humidity.append(graphData.get("Humidity", 0))
        lightLevel.append(graphData.get("Lightlevel", 0))

        if len(timer) > 100:
            timer.pop(0)
            temperature.pop(0)
            pressure.pop(0)
            humidity.pop(0)
            lightLevel.pop(0)
        
        #test received data
        print(f"t={time.time() - start:.1f}s | Temp={graphData.get("Temperature", 0):.2f}°C | Pres={graphData.get("Pressure", 0):.2f}hPa | Hum={graphData.get("Humidity", 0):.2f}% | Light={graphData.get("Lightlevel", 0):.2f}%")

        for a in ax:
            a.clear()
        
        ax[0].plot(timer, temperature, color="red")
        ax[0].set_ylabel("Temp (°C)")
        ax[1].plot(timer, pressure, color="blue")
        ax[1].set_ylabel("Pressure (hPa)")
        ax[2].plot(timer, humidity, color="green")
        ax[2].set_ylabel("Humidity (%)")
        ax[3].plot(timer, lightLevel, color="yellow")
        ax[3].set_ylabel("LightLevel (%)")
        ax[3].set_xlabel("Time (s)")
        plt.pause(0.2)

    except KeyboardInterrupt:
        print("\nConnection stopped by user")
        break