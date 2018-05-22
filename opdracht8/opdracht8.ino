//Assignment 8 - Using PORTB and DDRB.

/*
Q: Tot slot van deze opgave nog even een detailvraagje over de timing. Waarom zie je, tenzij
je bionische ogen hebt, bij de uitvoer van onderstaande statements de LED’s niet aan- en
uitgaan?

A: Because it refeshes at a 20MHz interval, which is too fast for the human eye to preceive. One way to solve it is using a delay between the two statements.
*/

//I did not have a led module, So i used other pins :)
int pins[] = { 3, 5, 6, 9, 10, 11 };
#define digital true

void setup() 
{
  //Set the pin modes for all the pins.
  for(int pin : pins)
  {
    pinMode(pin, OUTPUT);
  }
}

void loop() 
{
 #if digital
  digitalWritePattern();
 #else
  PortBPattern();
 #endif
}


void digitalWritePattern()
{
  //NOTE: We can use patterns here also, we just need to map the bits pins we defined.
  
  //Alternating pattern 1
  for(int i = 0; i < 6; i++)
  {
    if(i % 2 == 0)
    {
      digitalWrite(pins[i], HIGH); 
    }
    else
    {
      digitalWrite(pins[i], LOW); 
    }
  }

  delay(200);

  //Alternating pattern 2
  for(int i = 0; i < 6; i++)
  {
    if(i % 2 != 0)
    {
      digitalWrite(pins[i], HIGH); 
    }
    else
    {
      digitalWrite(pins[i], LOW); 
    }
  }

  delay(200);

  //Turn all off.
  for(int i = 0; i < 6; i++)
  {
    digitalWrite(pins[i], LOW); 
  }

  delay(200);
  
  //Turn all on.
  for(int i = 0; i < 6; i++)
  {
    digitalWrite(pins[i], HIGH); 
  }

  delay(200);
}

void PortBPattern()
{
  //Alternating patterns and on/off patten.
  int patterns[] = { 0x2A, 0x15, 0x0, 0x3F};

  for(int pattern : patterns)
  {
    PORTB = pattern;
    delay(200);
  }

  delay(200);
}


