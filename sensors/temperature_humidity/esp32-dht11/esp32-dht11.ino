#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "config.h"

char* ssid = (char*)WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_broker = MQTT_BROKER;
const char* mqtt_topic = MQTT_TOPIC;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;

const int measurement_interval = 30*1000;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

#define DHTPIN 27
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  dht.begin();
  mqttClient.setServer(mqtt_broker, 1883);

  connect_to_wifi();
  connect_to_mqtt_broker();
  loop();
}

void connect_to_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conecting to wifi..");
    delay(1000);
  }

  Serial.println(WiFi.localIP());
}

void connect_to_mqtt_broker() {
  while (!mqttClient.connected()) {
    delay(1000);
    mqttClient.connect("ESP32Client", mqtt_user, mqtt_password);
    Serial.println("Conecting to mqtt..");
  }
}

void loop(){
  delay(measurement_interval);
  Serial.println("Starting measurements..");

  if (!mqttClient.connected()) {
    connect_to_mqtt_broker();
  }

  unsigned long int epoch = time(NULL);
  float humidity = dht.readHumidity();
  float temp_c = dht.readTemperature();

  if (isnan(humidity) || isnan(temp_c)) {
    Serial.println("Unable to read measurements from sensor");
    return;
  } else {
    // Use this to compute capacity: https://arduinojson.org/v6/assistant/
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root  = jsonBuffer.createObject();

    root["epoch"] = epoch;
    root["humidity"] = humidity;
    root["temp"] = temp_c;

    char jsonChar[100];
    root.printTo(jsonChar);
    mqttClient.publish(mqtt_topic, jsonChar);
  }
}
