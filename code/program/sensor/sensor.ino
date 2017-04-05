//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
  #include <Wire.h>           // To Be Able To Use Sensors Like IMU9050
  #include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
  #include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
  #define       LED0      2         // WIFI Module LED
  const String  Devicename        = "Rainbow1";
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// WIFI Authentication Variables
//------------------------------------------------------------------------------------
/* This Client Is Going To Connect To A WIFI Server Access Point
 * So You Have To Specify Server WIFI SSID & Password In The Code Not Here
 * Please See The Function Below Name (WiFi.Begin)
 * If WIFI dont need Any password Then WiFi.begin("SSIDNAME")
 * If WIFI needs a Password Then WiFi.begin("SSIDNAME", "PASSWORD")
 */
  char*         ssid;            // Wifi Name
  char*         password;        // Wifi Password
//------------------------------------------------------------------------------------
// WIFI Module Role & Port
//------------------------------------------------------------------------------------
/* This WIFI Module Code Works As A Client
 * That Will Connect To A Server WIFI Modul With (IP ADDress 192.168.4.1)
 */
  int             TCP_ServerPort  = 9001;
  IPAddress       TCP_Server(192,168,4,1);
  WiFiClient      TCP_Client;
//====================================================================================

  void setup()
  {
    // Setting The Serial Port ----------------------------------------------
    Serial.begin(115200);           // Computer Communication
   
    // Setting The Mode Of Pins ---------------------------------------------
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    digitalWrite(LED0, !LOW);       // Turn WiFi LED Off
   
    // Print Message Of I/O Setting Progress --------------------------------
    Serial.println("\nI/O Pins Modes Set .... Done");

    // WiFi Connect ----------------------------------------------------
    Check_WiFi_and_Connect_or_Reconnect();        // Checking For Connection
  }

//====================================================================================
 
  void loop()
  {
    Send_Request_To_Server();
  }

//====================================================================================

  void Send_Request_To_Server() {
       TCP_Client.println("Here my message!"); 

       if(TCP_Client.available()) {   
      String Response = TCP_Client.readStringUntil('\r');
       Serial.println(Response);
      }
       TCP_Client.flush();  // Bufffer löschen
       Check_WiFi_and_Connect_or_Reconnect();
  }

//====================================================================================

  void Check_WiFi_and_Connect_or_Reconnect()
  {
    if (WiFi.status() != WL_CONNECTED){
 
    TCP_Client.stop();  //Erstmal sicher gehen dass alles resettet wurde
    WiFi.disconnect();
    Serial.println("Not Connected...trying to connect...");
    //Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");   
    delay(5000);
    WiFi.mode(WIFI_STA);               // To Avoid Broadcasting An SSID
    WiFi.begin("TCP_SERVER");          // The SSID That We Want To Connect To
   
    while(WiFi.status() != WL_CONNECTED)   
    {
      for(int i=0; i < 10; i++)
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

//====================================================================================

  void I2C_Begin()
  {
    /* ----------------------------------------------------------------------
     * Setting The I2C Pins SDA, SCL
     * Because We Didnt Specify Any Pins The Defult
     * SDA = D4 <GPIO2>, SCL = D5 <GPIO14> For ESP8266 Dev Kit Node MCU v3
     --------------------------------------------------------------------- */
    // Frequency Of The I2C
    #define TWI_FREQ  400000L       // I2C Frequency Setting To 400KHZ
   
    // If Ever You Needed To Use Wire I2C
    Wire.begin();                   // Begginning The I2C
   
    // Setting Up The I2C Of The MPU9250 ------------------------------------
    Wire.setClock(TWI_FREQ);        // Setting The Frequency MPU9250 Require
  }
       
//====================================================================================

  void Tell_Server_we_are_there()
  {
    // First Make Sure You Got Disconnected
    TCP_Client.stop();

    // If Sucessfully Connected Send Connection Message
    if(TCP_Client.connect(TCP_Server, TCP_ServerPort))
    {
      Serial.println    ("<" + Devicename + "-CONNECTED>");
      TCP_Client.println ("<" + Devicename + "-CONNECTED>");
    }
  }


