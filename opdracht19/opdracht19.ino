/*
Q: Omschrijf de werking van de ultrasone sensor in eigen woorden, inclusief het natuurkundig 
principe waarop het gebaseerd is.

A: De ultrasone sensor is meet de afstand tussen zichzelf en objecten.
   Hierbij wordt gebruik gemaakt van sonar (sound, navigation and ranging) om de afstand te meten.
   De sensor stuurt een geluid pulse uit (die kegel vormig) is, wanneer het een object raakt, dan zal de geluiden weerkaatst worden.
   Dit pikt de sensor weer op en door middel van een meeting van de verstreken tijden kan het bepalen hoe ver een object is/was.

Q: Wat is de betekenis van het getal 340 in de formule voor de tijdsbepaling en waarom moet er door 2 worden gedeeld?
A: 340 m/s is de snelheid van het geluiden, dit is cruciaal voor de berekening m.b.t. het meten van de afstand.
   Het wordt door tweeen gedeeld om dat we hier te maken hebben met een "round trip", wat betekent dat het geluid 2 keer weerkaatst wordt.

Q: Wat zijn (bij benadering) de minimale en de maximale meetafstand?
A: Volgens mijn eigen metingen ligt het bereik tussen 5 cm en 250cm.

Q: Wat is de nauwkeurigheid (in cm.)?
A: Ik heb een meeting verricht van de nauwkeurigheid, ik heb de sensor geplaatst op de 0 cm punt van een lineaal en een object op 10cm.
   Geduurende 30 seconden heb ik elke 10 msec een sample opgevraagt van de sensor. Het resultaat werdt in de serial monitor uitgeprint en toegevoegt aan het gemiddelde.
   Na 3000 samples bereken ik het gemiddelde. Daarnaast wordt de output van de serial monitor door de een occurence counter gehaald, waarbij het aantal % aangegeven wordt.
   De test wordt 3x herhaalt en een gemiddelde daarvan getrokken.

   Resultaat:
   Gemiddelde: 
   1) The average is 9.94 centimeters. 
   2) The average is 9.90 centimeters. 
   3) The average is 9.86 centimeters.
   average) 9.90cm
   
   Meest frequente waardes over 9000 samples: 
   23.17% - 10.06 cm
   15.96% - 9.66 cm
   12.84% - 9.76 cm
   11.28% - 10.17 cm
   9.28% - 9.61 cm
   6.68% - 9.71 cm
   6.41% - 10.18cm

   De sensor heeft ongeveer ~0.1cm afwijking op een afstand van 10 cm.
*/

#include <math.h>
#define TRIGGER_PIN 9
#define ECHO_PIN 10

void setup() 
{
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() 
{
  ClearTrigger();
  SetTrigger();

  int t = ReadEcho();
  
  if(t > 0)
  {
    float cm = ToCentimeters(t);
    Serial.print(cm);
    Serial.println(" centimeters");
  }
  else
  {
    Serial.println("Out of range.");
  }
  
  delay(250);
}

void ClearTrigger()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
}

void SetTrigger()
{
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}

int ReadEcho()
{
  int timePulse = pulseIn(ECHO_PIN, HIGH);
  Serial.println(timePulse);
  return timePulse;
}

float ToCentimeters(int t)
{
  return t * 0.034f / 2;
}



