#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

char* ssid = (char*)WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_broker = MQTT_BROKER;
const char* mqtt_topic = MQTT_TOPIC;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;

#define LED 2
#define uS_TO_S_FACTOR 1000000

RTC_DATA_ATTR int bootCount = 0;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

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

  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * uS_TO_S_FACTOR);

  Serial.println("Turning ESP in deep sleep mode");
  delay(1000);
  Serial.flush();

  esp_deep_sleep_start();
}
