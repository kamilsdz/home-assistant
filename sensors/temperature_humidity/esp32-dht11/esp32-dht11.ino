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

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

#define DHTPIN 27
#define DHTTYPE DHT11
#define LED 2
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  600

RTC_DATA_ATTR int bootCount = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  dht.begin();
  pinMode(LED,OUTPUT);
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

void go_sleep(){
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  Serial.println("Turning ESP in deep sleep mode");
  delay(1000);
  Serial.flush();

  esp_deep_sleep_start();
}

void loop(){
  Serial.println("Starting measurements..");
  digitalWrite(LED,HIGH);

  if (!mqttClient.connected()) {
    connect_to_mqtt_broker();
  }

  float humidity = dht.readHumidity();
  float temp_c = dht.readTemperature();

  if (isnan(humidity) || isnan(temp_c)) {
    Serial.println("Unable to read measurements from sensor");
    return;
  } else {
    // Use this to compute capacity: https://arduinojson.org/v6/assistant/
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root  = jsonBuffer.createObject();

    root["humidity"] = humidity;
    root["temp"] = temp_c;

    char jsonChar[100];
    root.printTo(jsonChar);
    mqttClient.publish(mqtt_topic, jsonChar);
    mqttClient.disconnect();

    digitalWrite(LED,LOW);

    go_sleep();
  }
}
