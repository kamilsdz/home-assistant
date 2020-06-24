# Home Assistant for Raspberry Pi

## Instalation (MacOS):
* `brew install http://git.io/sshpass.rb`
* `brew install ansible`

## Deploy Home Assistant server to Pi:
* Connect your Raspberry Pi to the same network
* `cd server/`
* `vim hosts` - provide raspberry's IP, `:w`
* run deploy: `ansible-playbook -i hosts site.yml`

App should be available at: <raspberry_IP>

## Debug:

### Using mosquitto:
You can use mosquitto to publish new message:

`mosquitto_pub -h 192.168.0.178 -p 1883 -u root -P buenosdias -t "home/bedroom/sensor1" -m '{"temp": 23.20,"humidity": 43.70}'`

You can also subscribe to all topics:

`mosquitto_sub -h 192.168.0.178 -p 1883 -u root -P buenosdias -t '#' -v`

### Logs:
* Connect to your Raspberry Pi using ssh
* `cd homeassistant/`
* `docker-compose logs`

# Sensors

## ESP32 + DHT11
(temperature + humidity)

It will produce MQTT message each 30 seconds:
```js
{
  "humidity": 20.0,
  "temp": 30.0
}
```
