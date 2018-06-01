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
A: Volgens mijn eigen metingen ligt het bereik tussen 5 cm en 200cm.

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

Q: Hoe stel je in je programma wanneer de sensor geen bereik heeft?
A: We zetten een timeout op de pulse in, wanneer de timeout verstreken is sturen we -1 terug.
   De maximale afstand (op basis van de specificatie is 500 cm. Geluid beweegt 343 m/s voort.
   Maar wij meten in cm, dus 343 * 100 = 34300 cm/s. Maar wij meten niet in seconden maar microseconden (pulseIn).
   In 1 second zit 1000000 msec. Om van cm/s naar cm/us kunnen we zeggen 34300 / 1000000 = 0.343 cm/msec.

   We weten dan de sensor een maximaal bereik heeft van 500 cm, om het maximale aantal msec dat we willen wachten op de pulse(In) is als volgt:
   500 / 0,0343 = 14577 msec. Omdat de uiterste van het bereik vaak onstabel, kunnen we die cijfer mooi afronden zodat we niet in ide
   instabiele gebieden kom. Laten we 14000 msec zeggen. Omdat we heen en weer gaan, moeten we de timeout met twee vermenigvuldigen. Dus 28000 msec
*/

//Min and max distance in centimeters
#define MIN_DIST 8
#define MAX_DIST 200

#define INT_MAX 32767
#define INT_MIN -32767

int ledPinCount = 0;
int ledPins[] = { 5, 4, 3 };
int ranges[] = { MIN_DIST, 125, MAX_DIST - 5 };

//0 = 


void setup() 
{
  ledPinCount = sizeof(ledPins) / sizeof(int);
  Serial.begin(9600);
}

int i = -10;
void loop() 
{
  int dist = 0;
#if 0
  dist = i++;
#else if
  dist = distanceAv(12, 11);
#endif

  Serial.println(dist);
  
  // We got a reasonable reading.
  if(dist > 0)
  {
    writeValueToLeds(dist);
  }
  //We are out of range or too close.
  else
  {
    writeOutOfRangeToLeds();
  }
}
int distance(int trig, int echo)
{
  //Setup pins if we already have set it. ( required for multiple sensors :) )
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  delay(1);
  
  //Clear the trigger pin.
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  //Send a 10 msec pulse
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long timeout = (MAX_DIST / 0.0343f) * 2.2f; //(0.2f for 20% margin).

  //Returns the time in microseconds.
  int pulse = pulseIn(echo, HIGH, timeout);
  
  if(pulse == 0)
  {
    //We did not find a distance, return -1;
    return -1;
  }
  else
  {
    //Return in centimeters
    return pulse * 0.0343f / 2;
  }
}

int distanceAv(int trig, int echo)
{
  int minValue = INT_MAX;
  int maxValue = INT_MIN;
  int count = 0;

  //Take 6 samples, store min max.
  for(int i = 0; i < 6; i++)
  {
    int dist = distance(trig, echo);

    //Store lowest value.
    if(dist < minValue)
    {
      minValue = dist;
    }

    //Store max value.
    if(dist > maxValue)
    {
      maxValue = dist;
    }

    count += dist;
  }
  
  //Remove the most significat and insignificant value and divide by 4 samples 
  //(because we removed 2).
  return (count - (minValue + maxValue)) / 4;
}

void writeValueToLeds(int cm)
{
  int ledCount = 0;
  for(int range : ranges)
  {
    if(cm >= range)
    {
      ledCount++;
    }
  }

  for(int i = 0; i < ledPinCount; i++)
  {
    digitalWrite(ledPins[i], i < ledCount);
  }

  delay(100);
}

void writeOutOfRangeToLeds()
{
  for(int b = 0; b < 10; b++)
  {
    if(b % 2 == 0)
    {
      for(int i = 0; i < ledPinCount; i++)
      {
          digitalWrite(ledPins[i], i % 2 == 0);
      }
    }
    else
    {
      for(int i = 0; i < ledPinCount; i++)
      {
         digitalWrite(ledPins[i], i % 2 != 0);
      }
    }

    delay(100);
  }
}



