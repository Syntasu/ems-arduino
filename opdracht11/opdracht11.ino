const int potmeterPin = A0;

void setup() 
{
  Serial.begin(9600);  
}

void loop() 
{
  /*
    anlogRead reads an analog value from a pin.

    analogRead has the following code signature:
    analogRead([int]pin);
    
    The pin parameter is the pin that should be read.

    It returns a value between 0 and 1023.
    The 1023 orginates from the AD converter. Which useds 10 bits to map the voltage to an value between 0 and 1023. 
    ADC reads in as binary, so with 2^10-1 we get 1023. minus one because we start at zero.  

    It directly maps to a voltage, so we can say that the resolition is 5v / (2^10-1).
    Which correlates to 0.049mV per unit.

    If the analogue pin is not connected, it's called a floating pin.
    This will pick a value between 0-1023 which is influenced by the environment around it.
  */

  
  int potmeter = analogRead(potmeterPin);
  Serial.println(potmeter);

  delay(100);

}
