#include "config.h"
#include <CommonSetup.h>

int sensorPin = 36;
float dry_value = 4096;
float wet_value = 0;

void setup() {
  common_setup();
  mqttClient.setServer(mqtt_broker, 1883);

  connect_to_wifi();
  connect_to_mqtt_broker();
}

void loop() {
  Serial.println("Starting measurements..");

  int input = analogRead(sensorPin);
  input = map(input, dry_value, wet_value, 0, 100);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root  = jsonBuffer.createObject();

  root["moisture"] = input;

  char jsonChar[100];
  root.printTo(jsonChar);
  mqttClient.publish(mqtt_topic, jsonChar);
  mqttClient.disconnect();

  go_sleep();
}
