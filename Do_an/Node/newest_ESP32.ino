#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "T3zT";
const char* password = "emvaytrang123";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "testingESP32";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
float pHValue = 0.0;

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT message received (if needed)
}

void connectToMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
  
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe(mqttTopic);
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void reconnectToMQTT() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Reconnected to MQTT broker");
      mqttClient.subscribe(mqttTopic);
    } else {
      Serial.print("MQTT reconnection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600); // Initialize ESP32 serial monitor
  Serial2.begin(9600); // Initialize UART communication with Arduino Uno

  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectToMQTT();
  }

  if (Serial2.available()) {
    // Read the pH value received from Arduino Uno
    String receivedData = Serial2.readStringUntil('\n');
    receivedData.trim();

    // Check if the received data starts with "pH value: "
    if (receivedData.startsWith("pH value: ")) {
      // Extract the pH value substring
      String pHValueString = receivedData.substring(10);
      pHValue = pHValueString.toFloat();

      // Display the received pH value in the ESP32 serial monitor
      Serial.print("Received pH value: ");
      Serial.println(pHValue, 2);

      // Publish the pH value to the MQTT topic
      mqttClient.publish(mqttTopic, pHValueString.c_str());
    }
  }

  mqttClient.loop();

  // Rest of your code...
}
