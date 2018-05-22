#define digital true

void setup() 
{
  Serial.begin(9600);
  
  String s1 = "0123456789"; // a constant String
  String c1 = "a"; // a constant String
  //Pssst, the constant is a lie, the compiler doesn't know it's a constant!
  //It's only a constant string if you declare it like `const String hw = "Hello Wolrd";` :)

  String s2 = String("bcdefghij");
  String c2 = String(c1);

  Serial.println(s1);
  Serial.println(s2);
  Serial.println(c1);
  Serial.println(c2);

  s2 = c2 + s2;
  Serial.println(s2); 
  
  Serial.println(s1[0]);
  
  for(int i = 0; i < s1.length(); i++)
  {
     Serial.println(s1[i]);
  }
  
  Serial.println(s1[s1.length() - 1]);

  Serial.println(s2[0]);
  
  for(int i = 0; i < s2.length(); i++)
  {
     Serial.println(s2[i]);
  }

  Serial.println(s2[s2.length() - 1]);

  for(int i = 0; i < s2.length(); i++)
  {
     Serial.print(s2[i]);
     Serial.print(" ");
     Serial.print(s2[i], DEC);
     Serial.print(" ");
     Serial.print(s2[i], HEX);
     Serial.print(" ");
     Serial.println(s2[i], BIN);
  }

  #if digital
    PrintDigital(s2);
  #else
    PrintPortB(s2)
  #endif

  
}

void PrintDigital(String s)
{
  int pins[] = { 3, 5, 6, 9, 10, 11 };
  byte mask = 1;
  int pin = 0;

  for(int a = 0; a < s.length(); a++)
  {
    byte pattern = s[a];
    for (mask = 000001; mask > 0; mask <<= 1) 
    {
      Serial.print(s[a]);
      Serial.print(" - ");
      Serial.print(mask, BIN);
      Serial.print(" & ");
      Serial.print(pattern, BIN);
      Serial.print(" = ");
      Serial.println(pattern & mask);
  
      if(pattern & mask)
      {
        analogWrite(pins[pin], 255);
      }
      else
      {
        analogWrite(pins[pin], 0);
      }
  
      if(pin > 5)
      {
        continue;
      }
  
      pin++;
    }
        
    pin = 0;
    delay(1000);
  }
}

void PrintPortB(String s)
{
  for(int a = 0; a < s.length(); a++)
  {
    PORTB = s[a];
  }
}


void loop() 
{
 
}
