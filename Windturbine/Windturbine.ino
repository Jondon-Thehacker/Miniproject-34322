#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";     
const char* password = "";   

const int mosfetGatePin = D1;  

// MQTT Broker settings
const char* mqtt_broker = "192.168.1.116";  // MQTT broker IP address
const char* mqtt_topic = "environment/wind_speed";        // MQTT topic for wind speed
const char* mqtt_topic2 = "environment/temperature";      // MQTT topic for temperature (subscribed but not used)
const char* mqtt_topic3 = "environment/wind_direction";   // MQTT topic for wind direction (subscribed but not used)
const char* mqtt_username = "username";  // Username for MQTT broker
const char* mqtt_password = "password";  // Password for MQTT broker
const int mqtt_port = 1883;              // MQTT port

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;

void setup_wifi() {
    delay(10);
    pinMode(mosfetGatePin, OUTPUT);  
    digitalWrite(mosfetGatePin, HIGH); 
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}

void callback(char* topic, byte* payload, unsigned int length) {
    unsigned long now = millis();
    unsigned long timeSinceLastMsg = now - lastMsgTime;
    lastMsgTime = now;
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Time since last message: ");
    Serial.print(timeSinceLastMsg / 1000);
    Serial.println(" seconds");

    if (String(topic) == mqtt_topic) {
        float wind_speed = message.toFloat();
        if (wind_speed <= 15.0 ) {
            digitalWrite(LED_BUILTIN, HIGH);  
            digitalWrite(mosfetGatePin, HIGH);
            Serial.println("Wind speed under 15 m/s: Windturbine ON");
        } else {
            digitalWrite(LED_BUILTIN, LOW);  
            digitalWrite(mosfetGatePin, LOW);
            Serial.println("Wind speed over 15 m/s: Windturbine OFF");
        }
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("NodeMCUClient", mqtt_username, mqtt_password)) {
            Serial.println("connected");
            client.subscribe(mqtt_topic);
            client.subscribe(mqtt_topic2);
            client.subscribe(mqtt_topic3);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
