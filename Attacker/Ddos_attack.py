import threading
import paho.mqtt.client as mqtt
import time
import random

def publish_messages(thread_id):
    client = mqtt.Client(f"Publisher-{thread_id}")
    client.connect('localhost', 1883, 60)
    try:
        while True:
            message = f"Thread {thread_id} Data: {random.random()}"
            client.publish("environment/stress_test", message)
    except KeyboardInterrupt:
        print(f"Thread {thread_id} stopped by user.")
    except Exception as e:
        print(f"Error in thread {thread_id}: {e}")
    finally:
        client.disconnect()

if __name__ == '__main__':
    num_threads = 500

    threads = []
    for i in range(num_threads):
        t = threading.Thread(target=publish_messages, args=(i,))
        t.start()
        threads.append(t)
        
    for t in threads:
        t.join()
