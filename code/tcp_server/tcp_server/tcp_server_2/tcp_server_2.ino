//-- Libraries Included --------------------------------------------------------------
#include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
// Define I/O Pins
#define     LED0        2         // WIFI Module LED
#define     analogPin   A0        // ADC pin lichtorgel 
//------------------------------------------------------------------------------------
// Authentication Variables
char*       ssid;              // SERVER WIFI NAME
char*       password;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
#define     MAXSC     6           // MAXIMUM NUMBER OF CLIENTS

WiFiServer  TCP_SERVER(9001);      // THE SERVER AND THE PORT NUMBER
WiFiClient  TCP_Client[MAXSC];     // THE SERVER CLIENTS Maximum number
//====================================================================================

void setup()
{
  // Setting The Serial Port
  Serial.begin(115200);           // Computer Communication
  // Setting The Mode Of Pins
  pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
  pinMode(analogPin, INPUT);      // ADC PIN
  // Setting Up A Wifi Access Point
  SetWifi("TCP_SERVER", "");
}

//====================================================================================
struct SensorData {
  char  device_name[20];
  char sensor_name[20];
  int value;
  int timestamp;
};


void loop()
{
  HandleClients();
}

//====================================================================================

void SetWifi(char* Name, char* Password)
{
  // Stop Any Previous WIFI
  WiFi.disconnect();

  // Setting The Wifi Mode
  WiFi.mode(WIFI_AP_STA);
  Serial.println("WIFI Mode : AccessPoint Station");

  // Setting The AccessPoint Name & Password
  ssid      = Name;
  password  = Password;

  // Starting The Access Point
  WiFi.softAP(ssid, password);
  Serial.println("WIFI < " + String(ssid) + " > ... Started");

  // Wait For Few Seconds
  delay(1000);

  // Getting Server IP
  IPAddress IP = WiFi.softAPIP();

  // Printing The Server IP Address
  Serial.print("AccessPoint IP : ");
  Serial.println(IP);

  // Starting Server
  TCP_SERVER.begin();
  Serial.println("Server Started");
}

//====================================================================================

void HandleClients()
{
  if (TCP_SERVER.hasClient())
  {
    WiFiClient Client = TCP_SERVER.available();
    while (1)
    {
      //---------------------------------------------------------------
      // If Clients Are Connected
      //---------------------------------------------------------------
      if (Client.available())
      {
        // Here We Read The Message
        String Message = Client.readStringUntil('\r');



        StaticJsonBuffer<200> jsonBuffer;

        JsonObject& root = jsonBuffer.parseObject(Message);

        SensorData.device_name = root["device"];
        SensorData.sensor_name = root["sensor"];
        SensorData.value    = root["value"];
        SensorData.timestamp   = root["time"];
        // Here We Print The Message On The Screen
        Serial.println(Message);
        // Here We Reply To The Client With A Message
        Client.print("Sensor Data OK");
        Client.flush();
      }

      //---------------------------------------------------------------
      // If Clients Are Disconnected
      //---------------------------------------------------------------
      /* if(!TCP_Client || !TCP_Client.connected())
        {
         // Here We Turn Off The LED To Indicated The Its Disconnectted
         //digitalWrite(LED1, LOW);
         // Here We Jump Out Of The While Loop
         break;
        }

        //----------------------------------------------------------------
      */
    }
  }
  else
  {
    // This LED Blinks If No Clients Where Available
    digitalWrite(LED0, HIGH);
    delay(250);
    digitalWrite(LED0, LOW);
    delay(250);
  }
}
