extern "C" {
#include "user_interface.h"
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <string.h>
#include <SoftwareSerial.h>


#define MAX_SRV_CLIENTS 6
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];
WiFiClient client;


char Name[32] = "HELLOWORLD"; // Standard name part
char ID[8] ; // buffer of ID of the ESP8266 ID

IPAddress myIP;
int connected = 0 ;

const char *ssid = "SOFTAP";
const char *password = "123456789";

void setup() {
  Serial.begin(9600);

  itoa(ESP.getChipId(), ID, 10); // get ID of ESP
  // Convert ID to constant Char
  for (int i = 0; i < 8; i ++)
  {
    Name[i + 12] = ID[i];

  }

  wifi_station_disconnect();
  wifi_set_opmode(0x02); // SETTING IT TO AP MODE

  WiFi.softAP(ssid, password, 8, 0);
  myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  server.setNoDelay(false);
  Serial.println("TCP Server Setup done");
  delay(5000);

}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    connected = 0;
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.println(i);
        serverClients[i].print("00000010");
        continue;
      }
      connected ++;
    }
    Serial.print("Connected clients = ");
    Serial.println(connected);
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  // Check for Data each time only 1 byte is expected and needed to be send
  if (Serial.available() > 0)
  {
    byte temp_input = Serial.read();
    Serial.flush();

    String byte_message = "";
      byte_message += bitRead(temp_input, 7);
      byte_message += bitRead(temp_input, 6);
      byte_message += bitRead(temp_input, 5);
      byte_message += bitRead(temp_input, 4);
      byte_message += bitRead(temp_input, 3);
      byte_message += bitRead(temp_input, 2);
      byte_message += bitRead(temp_input, 1);
      byte_message += bitRead(temp_input, 0);          // TAKES THE 10001011 version of the Byte
     
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
       
        serverClients[i].print(byte_message);
        delay(1);
      }
    }
  }
}
