#define PRESSURE_INPUT_PIN  0 // set the input for the input to analog pin 0

void setup(){
    Serial.begin(115200);
}

void loop(){
  int pressure_min = analogRead(PRESSURE_INPUT_PIN); // value is 0-1023
  Serial.print("Pressure value: ");
  Serial.println(pressure_min);

  delay(100);


}

