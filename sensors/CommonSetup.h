#include <ArduinoJson.h>
#include "driver/adc.h"
#include <esp_bt.h>
#include <esp_wifi.h>
#include <PubSubClient.h>
#include <WiFi.h>

char* ssid = (char*)WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_broker = MQTT_BROKER;
const char* mqtt_topic = MQTT_TOPIC;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;
const uint64_t us_to_s_factor = 1000000LL;

#define LED 2
#define uS_TO_S_FACTOR 1000000

RTC_DATA_ATTR int bootCount = 0;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void go_sleep(){
  ++bootCount;

  Serial.println("Turning ESP in deep sleep mode");
  Serial.println("Boot number: " + String(bootCount));

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_wifi_stop();
  adc_power_off();

  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * us_to_s_factor);

  Serial.flush();

  esp_deep_sleep_start();
}

void common_setup() {
  setCpuFrequencyMhz(120);

  mqttClient.setServer(mqtt_broker, 1883);
  adc_power_on();
  btStop();
}

void connect_to_wifi() {
  int wifi_attempts = 0;

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    ++wifi_attempts;
    if (wifi_attempts < 10) {
      delay(1000);
      Serial.println("Conecting to wifi..");
    } else {
      go_sleep();
    }
  }

  Serial.println(WiFi.localIP());
}

void connect_to_mqtt_broker() {
  int mqtt_attempts = 0;

  while (!mqttClient.connected()) {
    ++mqtt_attempts;

    if (mqtt_attempts < 10) {
      delay(1000);
      Serial.println("Conecting to mqtt..");
      mqttClient.connect("ESP32Client", mqtt_user, mqtt_password);
    } else {
      go_sleep();
    }
  }
}
