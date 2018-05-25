#include <SPI.h>
#include <Ethernet.h>

#define SRV_PORT 32819

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
      Serial.println(message);
    }
    else
    {
      Serial.println("Failed to read the message");
    }
  }

  delay(1);

}

bool GetClient(EthernetClient &client)
{
  EthernetClient acceptedClient = server.available();

  //Check if we indeed accepted the client and got a client object back.
  if(!acceptedClient)
  {
    return false;
  }

  Serial.println("Accepted a new client...");
  
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
      Serial.println(buffer);

      message = String(buffer);
      len = counter;
      return true;
    }

    message = String("");
    len = 0;
    return false;
  }
  
}

