int sensorPin = 36;
float max_to = 4096;
float low_from = 5.0;
 
void setup() {
  Serial.begin(115200);
}
 
void loop() {
  delay(1000);

  int input = analogRead(sensorPin);
  input = map(input, low_from, max_to, max_to, low_from);
  
  Serial.println(convert_to_lux(input));
}
 
float convert_to_lux(int input) {
  float value = input * low_from / max_to;
  return pow(10, value);
}
