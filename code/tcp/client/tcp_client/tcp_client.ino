
extern "C" {
#include "user_interface.h"

}
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include<WiFiUdp.h> // libary for UDp connections

const char* host = "192.168.4.1";
const int port = 23;


const char *ssid = "SOFTAP";
const char *password = "123456789";
int status = 6;
WiFiClient client;

void fpm_wakup_cb_func1(void)  {
  wifi_fpm_close(); // disable force sleep function
  //wifi_set_opmode(STATION_MODE); // set station mode
  //wifi_station_connect();
  // Serial.println("Woken up...");
}

// LIGHT SLEEP FUNCTION
void sleepNow(int MILI) {         
  // Serial.println("Going to sleep...");
  float sleepTime = millis();
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T); // light sleep
  wifi_fpm_open(); // enable force sleep
  wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1); // Set wakeup callback
  wifi_fpm_do_sleep(MILI * 1000);
  delay(MILI +1);
 
}
void setup() {
  Serial.begin(9600);

  WiFi.disconnect();
  delay(10);
  WiFi.mode(WIFI_STA);

   WiFi.begin(ssid, password);

   if (!client.connect(host, port))
      {
        Serial.println("Failed Connection to Server");
        status = 1;
      }

}

void loop()
{

  status = WiFi.status();
  if (status == 3 && client.available() < 1)   // if there is no data ready for Module go to sleep
  {
    Serial.println("GO To SLEEP");
    sleepNow(10);
    Serial.println("WAKING UP");
  }
  else
  {
     // function to reconnect
  }

  if (!client.connect(host, port))
      {
        Serial.println("Failed Connection to Server");
        status = 1;
      }
  else if (client.available() >0)
  {
    String TEMP_Message = "";
  while (client.available() > 0)
  {
    TEMP_Message += char(client.read());
  }
  Serial.println("GOT message = " + TEMP_Message);
  client.flush();
  }
 
}
