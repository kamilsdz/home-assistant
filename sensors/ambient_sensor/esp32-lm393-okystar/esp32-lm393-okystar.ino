#include "config.h"
#include <CommonSetup.h>

int sensorPin = 36;
float max_to = 4096;
float low_from = 5.0;

void setup() {
  common_setup();
  mqttClient.setServer(mqtt_broker, 1883);

  connect_to_wifi();
  connect_to_mqtt_broker();
}

void loop() {
  Serial.println("Starting measurements..");

  int input = analogRead(sensorPin);
  input = map(input, low_from, max_to, max_to, low_from);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root  = jsonBuffer.createObject();

  root["lux"] = convert_to_lux(input);
  Serial.println(convert_to_lux(input));

  char jsonChar[100];
  root.printTo(jsonChar);
  mqttClient.publish(mqtt_topic, jsonChar);
  mqttClient.disconnect();

  go_sleep();
}

float convert_to_lux(int input) {
  float value = input * low_from / max_to;
  return pow(10, value);
}
