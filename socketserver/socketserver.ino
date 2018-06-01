#include <SPI.h>
#include <Ethernet.h>

#define SRV_PORT 43748

//Server configuration
IPAddress ip = IPAddress(192, 168, 1, 16);
byte macAddr[] = { 0x27, 0xB6, 0xCB, 0x31, 0x10, 0xB9 };

//Server variables.
EthernetServer server(SRV_PORT);
bool isConnected = false;

void setup() 
{
  Serial.begin(9600);

  //Bind IP.
  Ethernet.begin(macAddr, ip);

  Serial.print("Bound server to this endpoint: ");
  Serial.print(ip);
  Serial.print(":");
  Serial.println(SRV_PORT);

  //Begin listening on the given port.
  server.begin();
  isConnected = true;
  
  Serial.print("Listening for incoming requests on IP ");
  Serial.println(Ethernet.localIP());
}

void loop() 
{
  //We are not connected, we don't want to check for network stuff.
  if(!isConnected)
  {
    return;
  }

  //Check if we have a client available...
  EthernetClient client;
  if(GetClient(client))
  {
    //Read out the message we receive from the client.
    String message;
    int len;
    
    if(ReadClient(client, message, len))
    {
      int type = -1;
      String arg0, arg1, arg2, ret;
      
      handleIncomingMessage(message, len, type, arg0, arg1, arg2);

      switch(type)
      {
        case -1:
          Serial.println("Failed to parse message.");
          break;
        case 0:
          ret = doHandshake();
          break;
        case 2:
          //doSetDigitalPin();
          break;
        case 4:
          //doSetAnalogPin();
          break;
        case 8:
          ret = readDigitalPin(arg0);
          break;
        case 32:
          ret = readAnalogPin(arg0);
          break;
        case 128:
          ret = doGetPinList();
          break;
        default:
          Serial.print("Received unknown message:");
          Serial.println(type);
          break;
      }

      if(ret != "")
      {
        //Serial.print("Returned data to client...");
        client.print(ret);
      }
    }
    else
    {
      Serial.println("Failed to read the message");
    }
  }

  delay(1);
}

String readAnalogPin(String arg0)
{
  int pin = arg0.toInt();
  int value = analogRead(pin);
  
  return "64;" + String(value);
}

String readDigitalPin(String arg0)
{
  int pin = arg0.toInt();
  int value = digitalRead(pin);
  
  return "16;" + String(value);
}

String doHandshake()
{
  Serial.println("Received handshake from client...");
  return "1;ok";
}

String doGetPinList()
{
  String result = "";
  
  for(int i = 0; i <= 13; i++)
  {
    result += "D";
    result += String(i);
    result += ":";
    result += String(digitalRead(i));
    result += "-";
  }

  for(int n = 0; n <= 5; n++)
  {
    result += "A";
    result += String(n);
    result += ":";
    result += String(analogRead(n));

    //Don't include last one...
    if(n != 5)
    {
      result += "-";
    }
  }

  return "256;" + result;
}

void handleIncomingMessage(String message, int len, int &type, String &a0, String &a1, String &a2)
{
  //Convert a string object to a C string.
  char buf[256];
  message.toCharArray(buf, sizeof(buf));

  //Split by semicolon
  int counter = 0;
  char *p = buf;
  char *str;
  while ((str = strtok_r(p, ";", &p)) != NULL)
  {
    String value = String(str);
    
    if(counter == 0) type = value.toInt();
    if(counter == 1) a0 = value;
    if(counter == 2) a1 = value;
    if(counter == 3) a2 = value;
    if(counter > 3) break;

    counter++;
  }
}
bool GetClient(EthernetClient &client)
{
  EthernetClient acceptedClient = server.available();

  //Check if we indeed accepted the client and got a client object back.
  if(!acceptedClient)
  {
    return false;
  }

  Serial.println("New data received...");
  
  client = acceptedClient;
  return true;
}

bool ReadClient(EthernetClient client, String &message, int &len)
{
  char buffer[128];

  //Check if the client and server are available.
  //Otherwise we want to stop reading since it's pretty pointless.
  while(client.connected() && isConnected)
  {
    int counter = 0;
    
    //As long as there is data available to be read...
    while(client.available())
    {
      //Read the incoming character and push it to the buffer.
      buffer[counter++] = client.read();
    }

    //Null terminate the character array..
    buffer[counter] = '\0';
    
    //Ensure we actually read something...
    if(counter > 0)
    {
      message = String(buffer);
      len = counter;
      return true;
    }

    message = String("");
    len = 0;
    return false;
  }
}

