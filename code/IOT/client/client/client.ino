//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
#include <Wire.h>           // To Be Able To Use Sensors Like IMU9050
#include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
#include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
#include <ArduinoJson.h>


#define       LED0      2         // WIFI Module LED
#define BUTTON_PIN 13
#define DEVICE_NAME "ESP_client_01"


const String  Devicename        = "Rainbow1";
int             TCP_ServerPort  = 9001;
IPAddress       TCP_Server(192, 168, 4, 1);
WiFiClient      TCP_Client;
int sample;
struct SensorData {
  char  device_name[20];
  char sensor_name[20];
  int value;
  int timestamp;
};

void setup() {
  // Setting The Serial Port ----------------------------------------------
  Serial.begin(115200);           // Computer Communication
  setupButton();
  Check_WiFi_and_Connect_or_Reconnect();        // Checking For Connection
}


void loop() {
    Send_Request_To_Server();

  sample = analogRead(A0);
  Serial.print("Sample: ");
  Serial.println(sample);
  delay(5000);

}



void setupButton() {
  Send_Request_To_Server();

  
}

void Send_Request_To_Server() {
  StaticJsonBuffer<200> jsonBuffer;
    sample = analogRead(A0);
  Serial.print("Sample: ");
  Serial.println(sample);

  JsonObject& root = jsonBuffer.createObject();
  root["device"] = Devicename;
  root["sensor"] = "temperature";
  root["value"] = sample;
  root["time"] = millis();

  root.printTo(TCP_Client);


  if (TCP_Client.available()) {
    String Response = TCP_Client.readStringUntil('\r');
    Serial.println(Response);
  }
  TCP_Client.flush();  // Bufffer löschen
  Check_WiFi_and_Connect_or_Reconnect();
}

void Check_WiFi_and_Connect_or_Reconnect()
{
  if (WiFi.status() != WL_CONNECTED) {

    TCP_Client.stop();  //Erstmal sicher gehen dass alles resettet wurde
    WiFi.disconnect();
    Serial.println("Not Connected...trying to connect...");
    //Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");
    delay(5000);
    WiFi.mode(WIFI_STA);               // To Avoid Broadcasting An SSID
    WiFi.begin("TCP_SERVER");          // The SSID That We Want To Connect To

    while (WiFi.status() != WL_CONNECTED)
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }


    // Stop Blinking To Indicate Connected ----------------------------------
    digitalWrite(LED0, !HIGH);
    Serial.println("!-- Client Device Connected --!");

    // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TCP_Server);
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());

    // Conecting The Device As A Client -------------------------------------
    Tell_Server_we_are_there();
  }
}


void Tell_Server_we_are_there()
{
  // First Make Sure You Got Disconnected
  TCP_Client.stop();

  // If Sucessfully Connected Send Connection Message
  if (TCP_Client.connect(TCP_Server, TCP_ServerPort))
  {
    Serial.println    ("<" + Devicename + "-CONNECTED>");
    TCP_Client.println ("<" + Devicename + "-CONNECTED>");
  }
}
void highInterrupt() {
}

