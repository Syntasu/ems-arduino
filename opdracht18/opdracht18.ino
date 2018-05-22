#include <Servo.h>

Servo servo;

void setup() 
{
  servo.attach(3);

  Serial.begin(9600);
}

void loop() 
{
  int pot = analogRead(0);
  int mapped = map(pot, 0, 1023, 0, 180);

  servo.write(mapped);
  delay(20);
}
