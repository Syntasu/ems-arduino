//Assignment 16 - A led light pattern inspired by the night rider, the kitt car.

//All the direcitons we can move in.
enum Direction
{
  Left, 
  Right
};

//The current index that is being lit.
int index = 0;

//The length of the pin array.
int pinsLen = 0;

//Array of the pins that are connected to the leds.
const int pins[] = { 3, 5, 6, 9, 10, 11};

//How much the leds drop in intensity from the index.
const float fadeDropoff = 3.5f;

Direction dir = Direction::Left;

void setup()
{
  //Calculate the length of the pins array by getting the
  //size in memory and dividing this with the size of an int.
  pinsLen = sizeof(pins) / sizeof(int);
  
  Serial.begin(9600);
}

void loop() 
{
  //Process the direction and the state of the leds.
  ProcessDirections();
  ProcessFadedLeds();

  int potmeter = analogRead(0);
  float factor = (float)potmeter / 1023.0f;
  int maxDelay = 300;
  
  delay(20 + maxDelay * factor);
}

//Calculate in which direction we are moving.
void ProcessDirections()
{
  if(dir == Direction::Left)
  {
    index += 1;

    if(index >= pinsLen - 1)
    {
      dir = Direction::Right;
    }
  }
  else
  {
    index -= 1;
    
    if(index <= 0)
    {
      dir = Direction::Left;
    }
  }
}

// Calculate which leds should fade and by how much. 
void ProcessFadedLeds()
{
  byte intensity = 0;
  
  for(int i = 0; i < pinsLen; i++)
  {
    //Grab the distance from the led.
    int distance = index - i;

    //Absolute the value, so we don't get negatives.
    int delta = abs(distance);

    //Convert it to a 0.0f to 1.0f scale.
    float factor = (float)delta / (float)pinsLen;

    //Get the inverse factor.
    float invFactor = 1.0f - factor;

    //If the inverse factor is one, give it full intensity.
    //Since this is the led where the index is at.
    //We branch to prevent calculating the range dropoff for the main led.
    if(invFactor >= 1.0f)
    {
      analogWrite(pins[i], 255);
    }
    //Else we calculate how far the led
    else
    {   
      //Scale the intensity factor by the given delta (+ predefined dropoff).
      float w = factor / pow(delta, fadeDropoff);
      byte intensity = 255 * w;

      //Check on which side the led is.
      //If it's behind the direction we are moving in, then we want to light it up.
      //Else we're going to disable the led, because it's infront of use.
      //Previous if statement filters out the main led, so it doesn't get affected.
      if(distance > 0)
      {
        analogWrite(pins[i], (dir == Direction::Left) ? intensity : 0);
      }
      else
      {
        analogWrite(pins[i], (dir == Direction::Right) ? intensity : 0);
      }
    }   
  }
}

