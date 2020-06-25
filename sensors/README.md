# Sensors

## ESP32 + DHT11
**(temperature + humidity)**

It will produce MQTT message each X seconds:
```js
{
  "humidity": 20.0,
  "temp": 30.0
}
```
### Pinout:
- ESP32 3v3 - pin 1
- ESP32 GND - pin 4
- ESP32 D27 - pin 2
- ESP32 3v3 - pin 2 with 4.7k ohm resistor


## ESP32 + ambient sensor on LM393
**(ambient sensor)**

It will produce MQTT message each X seconds:
```js
{
  "lux": 500.0,
}
```
### Pinout:
- ESP32 3v3 - VCC
- ESP32 GND - GND
- ESP32 VP (pin 36) - AO
- ESP 3v3 - AO with 1.2k ohm resistor

