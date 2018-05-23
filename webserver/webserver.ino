//Static content.
const char cs0[] PROGMEM = "<STRONG>Opdracht 17 van het vak embedded systems 1</STRONG>"; 
const char cs1[] PROGMEM = "Dit voorbeeld is gebaseerd op het script in Voorbeelden->Ethernet->Webserver";
const char cs2[] PROGMEM = "De website is dynamische gemaakt door sensorwaarden van kanaal 0 toe te voegen.";
const char cs3[] PROGMEM = "<B>Breid het programma uit</B> met de mogelijkheid om variabelen mee te geven.";
const char cs4[] PROGMEM = "Dit kan o.a. door GET-variabelen, via de URL (192.168.1.3/?p8=1).";
const char cs5[] PROGMEM = "Gebruik de functie <STRONG style='color:Black'>parseHeader(httpHeader, arg, val))</STRONG>";
const char* const string_table[] PROGMEM = {cs0, cs1, cs2, cs3, cs4, cs5};

#define maxLength     50  // header length, don't make it to long; Arduino doesn't have much memory
#define sensorPin     0   // sensor on channel A0 
#define ledPin        8
#define infoPin       9  

#include <SPI.h>
#include <Ethernet.h>
#include <ctype.h>

// Initialize the Ethernet server library (port 80 is default for HTTP):
EthernetServer server(80);

//Used for storing the query paramter
int arg = 0, val = 0; 

//Check if a dhcp is available in the network
//Failover when the timeout is expired (in seconds).
void checkDhcp(int timeout, byte mac[], IPAddress ip)
{
  Serial.println("Checking if DHCP is available in the network...");
  int state = Ethernet.begin(mac, timeout);

  if(state == 0)
  {
    Ethernet.begin(mac, ip);
    Serial.println("DHCP server was not available, using manual IP");
  }
  else
  {
    Serial.println("A DHCP server has been found");
  }
}

void setup() 
{
  Serial.begin(9600);
  
  Serial.println("Starting server...");
  Serial.println("Setting up server I/O...");
  
  pinMode(ledPin, OUTPUT);
  pinMode(infoPin, OUTPUT);
  
  digitalWrite(ledPin, LOW);
  digitalWrite(infoPin, LOW);

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress ip(192, 168, 1, 16);
 
  checkDhcp(10, mac, ip);

  server.begin();
  Serial.println(" ");
  Serial.println("---- BEGIN SERVER INFORMATION ----");
  Serial.println("Embedded Webserver with I/O-control v1.4");
  Serial.println("Ethernetboard connected (pins 10, 11, 12, 13 and SPI)");
  Serial.print("Server is at "); 
  Serial.println(Ethernet.localIP());
  Serial.print("ledpin at pin "); 
  Serial.println(ledPin);
  Serial.print("infoPin at pin "); 
  Serial.println(ledPin);
  Serial.println("---- END SERVER INFORMATION ----");
  Serial.println(" ");
}

void loop() 
{
  //Listen for incoming requests (aka clients).
  EthernetClient client = server.available(); 
  
  //Check if the client is not null.
  if (client) 
  {
    //Variables to parse the header.
    boolean processedHeader = false;
    String header = "";
    
    while (client.connected()) 
    {
      //Exit if the client is not available.
      if(!client.available()) break;

      //Read out the response character by character.
      char c = client.read();

      //If we received a vertical tab or line feed, we want to truncate the rest of the header.
      if(c == 0x0B || c == 0x0A)
      {
        processedHeader = true;
      }
      //Append the character to the header.
      else
      {
        //Check if it does not exceed the max length, else tell we are done with the header.
        if(header.length() < maxLength)
        {
          header += c;
        }
        else
        {
          processedHeader = true;
        }
      }

      if(processedHeader)
      {
        //Clean up the header, remove the http part and get part.
        header.replace(" HTTP/1.1", "");
        header.trim();

        Serial.print("Request received with following query paramters: ");
        Serial.println(header);

        //Write the response header and content.
        writeResponseHeader(client);

        //Begin writing the default content
        beginWriteContent(client);

        //Send the content we defined.
        writeUserContent(client, header);

        //End writing the the default content.
        endWriteContent(client);

        //Reset the heade variables
        processedHeader = false;
        header = "";

        //Tell the while loop we are done processing.
        break;
      }
    }
    
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
  }
}

//Write a new response header.
void writeResponseHeader(EthernetClient client)
{
  //Default response header.
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  //client.println("Refresh: 3");               // refresh the page automatically every 3 sec
}

//Write the content we want to send back to the browser.
void beginWriteContent(EthernetClient client)
{
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<HTML>");
  client.println("<HEAD><TITLE>Embedded I/O-Webserver</TITLE></HEAD>");
  client.println("<STYLE> body{width:800px;font-family:verdana;background-color:LightBlue;} ");
  client.println("</STYLE>");
  client.println("<BODY>");
  client.println("<H4 style='color:DarkBlue'>Embedded I/O-Webserver</H4>"); 
  writeStaticContent(client);
}

//Begin writing user-defined content,
void writeUserContent(EthernetClient client, String header)
{
  //Read out the sensor pin. (see defines).
  int sensorValue = analogRead(sensorPin);
  client.println("<P style='color:DarkBlue'>");      
  client.print("Analog sensor, channel "); client.print(sensorPin); client.print(": ");
  client.print(sensorValue);
  client.println("</P>");

  //Read out the query parameter string.
  client.println("<P>");
  
  if (parseHeader(header, arg, val)) 
  { 
    digitalWrite(arg, val); 
    client.print("Pin ");
    client.print(arg); 
    client.print(" = "); 
    client.println(val);
  }
  else 
  {
    client.println("No IO-pins to control");
  }
  
  client.println("</P>");
}

//Write the end part of the default content.
void endWriteContent(EthernetClient client)
{
  client.println("</BODY>");
  client.println("</HTML>");
}

//Print the static content from the progtam memory.
void writeStaticContent(EthernetClient client)
{
  char buffer[100]; 
  client.println("<P style='font-size:80%; color:Gray'>");

  //Read static content from the program memory.
  for (int i = 0; i <= 5; i++) 
  {
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i]))); 
    
    client.println(buffer); 
    client.println("<br>");
  }
  
  client.println("</P>");
}

//Parse the query string parameters.
bool parseHeader(String header, int &a, int &v)
{
  //Store the string we build in the loop.
   String tempArg = String("");
   String tempValue = String("");
  
  //Subdivide the header into arg and value (a & v by reference).
  bool writeState = false;
  bool isAtQueryPart = false;
  for(int i = 0; i < header.length(); i++)
  {
    //Flip the bit, to signal that we are writing the value and not the arg.
    //Continue, we don't want to write = to the arg nor the value.
    if(header[i] == '=')
    {
      writeState = true;
      continue;
    }
    //We reached the part that contains the query information.
    else if(header[i] == '?')
    {
      isAtQueryPart = true;
      continue;
    }
    //There are more than one query parameter, lets ignore this part.
    else if(header[i] == '&')
    {
      Serial.println("The user specified multiple query parameters, we can only account for one.");
      break;
    }

    //Don't write if we haven't reached the ? character yet.
    if(!isAtQueryPart)
    {
      continue;
    }

    //Decide where to write to 
    if(!writeState)
    {
      tempArg += header[i];
    }
    else
    {
      tempValue += header[i];
    }
  }

  //Parse the pin numbers.
  int pin = tempArg.substring(1, tempArg.length()).toInt();
  if(pin <= 2 || pin >= 10)
  {
    return false;
  }

  //Compute the value state.
  bool state = false;
  if(tempValue == "0" || tempValue == "uit" || tempValue == "off" || tempValue == "false")
  {
    state = false;
  }
  else if(tempValue == "1" || tempValue == "aan" || tempValue == "on" || tempValue == "true")
  {
    state = true;
  }
  else
  {
    return false;
  }

  //Return the results.
  a = pin;
  v = state;
  return true;
}



