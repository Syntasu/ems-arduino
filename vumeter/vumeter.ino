//Assignement 15 - VU-meter by using a LDR (light diode resistor).
//Success! Your submission appears on this page. The submission confirmation number is 58661c37-9245-4c13-8a76-34c244a26bdb. Copy and save this number as proof of your submission.
//The length of the pin array.
int pinsLen = 0;

//Array of the pins that are connected to the leds.
const int pins[] = { 3, 5, 6, 9, 10, 11};

void setup() 
{
  //Calculate the length of the pins array by getting the
  //size in memory and dividing this with the size of an int.
  pinsLen = sizeof(pins) / sizeof(int);
  
  Serial.begin(9600);
}

void loop()
{
  //Read out the light diode.
  int ldr = analogRead(1);

  //Inverse the value, 
  int lightLevel = 1023 - ldr;

  //Count iterations.
  int accumulator = 1;

  //Calculate which value it atleast needs to have to turn on the led.
  float steps = 1023.0f / pinsLen;

  //Loop over pins.
  for(int pin : pins)
  {
    //For each step, make the nex step bigger by multiplying by acummulator.
    float s = steps * (float)accumulator;

    //If step divided by the light level is great than 1.0f, then turn on the led.
    //If not, turn it off. We could do some interpolation when s ≥ lightlevel and |lightlevel - s| ≤ steps
    if((lightLevel / s) > 1.0f)
    {
      analogWrite(pin, 255);
    }
    else
    {
      analogWrite(pin, 0);
    }
    
    accumulator++;
  }

  delay(10);
}
