const float pi = 3.14159265f;

void setup()
{
  Serial.begin(9600);
  Serial.print("Area of a circle with radius = 4 in m2 is: ");
  Serial.println(FindCircleArea(4));

  for (int n = 1; n <= 10; n++)
  {
    Serial.print("Via a normal loop");
    Serial.print("Area of a circle with radius = ");
    Serial.print(n);
    Serial.print(" in m2 is: ");
    Serial.println(FindCircleArea(n));
  }
}

int a = 1;
void loop()
{
  if(a <= 10)
  {
    Serial.print("Via loop method...");
    Serial.print("Area of a circle with radius = ");
    Serial.print(a);
    Serial.print(" in m2 is: ");
    Serial.println(FindCircleArea(a));
    a++;
  }
  else if(a == 11)
  {
    float r = FindRadius(3, 4, 2, 6, 8, 4);
    Serial.print("The radius is: ");
    Serial.println(r);
    a++;
  }
}

float FindCircleArea(int radius)
{
  return pi * pow(radius, 2);
}

float FindRadius(int px, int py, int pz, int mx, int my, int mz)
{
  return sqrt(pow(px - mx, 2) +  pow(py - my, 2) + pow(pz - mz, 2));
}

