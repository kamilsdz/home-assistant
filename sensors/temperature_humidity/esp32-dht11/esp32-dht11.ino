#include "config.h"
#include <Adafruit_Sensor.h>
#include <CommonSetup.h>
#include <DHT.h>
#include <time.h>

#define DHTPIN 27
#define DHTTYPE DHT11

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
