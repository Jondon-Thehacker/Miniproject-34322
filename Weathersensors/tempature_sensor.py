import paho.mqtt.client as mqtt
import time
import random

broker = 'localhost'    
port = 1883
topic_temperature = "environment/temperature"

client = mqtt.Client()
client.connect(broker, port, 61)

while True:
    temperature = random.uniform(15, 20) 

    client.publish(topic_temperature, f"{temperature:.2f}")

    print(f"Published Temperature: {temperature:.2f} C")

    time.sleep(30)