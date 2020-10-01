/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  for (pushButton=2; pushButton< 13; pushButton++)
    pinMode(pushButton, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  for (pushButton=2; pushButton< 9; pushButton++) {
    if (digitalRead(pushButton) == LOW) {   
      // print out the state of the button:
      Serial.print(pushButton);
      Serial.println(" is LOW");
    }
  }
  if (digitalRead(11) == LOW) {   
    // print out the state of the button:
    Serial.println("11 is LOW");
  }
}



