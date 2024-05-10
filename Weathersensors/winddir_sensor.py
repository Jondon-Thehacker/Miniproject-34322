import paho.mqtt.client as mqtt
import time
import random

broker = 'localhost'    
port = 1883
topic_wind_direction = "environment/wind_direction" 

client = mqtt.Client()
client.connect(broker, port, 60)

while True:
    wind_direction = random.randint(0, 359) 

    client.publish(topic_wind_direction, f"{wind_direction}")

    print(f"Wind Direction: {wind_direction} degrees")

    time.sleep(30)
