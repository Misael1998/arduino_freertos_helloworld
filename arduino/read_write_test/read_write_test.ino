#include <string.h>

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  String buffer = "demo";
  if (Serial.available() > 0) {
    buffer = Serial.readString();
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(buffer);
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    //Serial.println("misa");
  }
}
