import paho.mqtt.client as mqtt
import time
import random

broker = 'localhost'    
port = 1883
topic_wind_speed = "environment/wind_speed"

client = mqtt.Client()
client.connect(broker, port, 60)

while True:
    wind_speed = random.uniform(8, 20)   

    client.publish(topic_wind_speed, f"{wind_speed:.2f}")

    print(f"Wind Speed: {wind_speed:.2f} m/s")

    time.sleep(20)