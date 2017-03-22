#include             <ESP8266WiFi.h>
#include            <WiFiClient.h>

const int  ledPin  = 0;

// ---------------------------------------------------------------------------
void setup() {

WiFiClient            client;
int i;
char pass[]                = "1234";
char ssid[]= "gonosz2";
char s[100];
int status;
char testStr[] = "The Quick Brown Fox Jumps Over the Lazy Dog";

Serial.begin(9600);
pinMode(ledPin, OUTPUT);

Serial.print("Tryingonnect to ");
Serial.println(ssid);

// attempt to connect to Wifi network:
status = WiFi.begin(ssid, pass);

status = WiFi.waitForConnectResult();
if (status != WL_CONNECTED) {
    Serial.println("Connection Failed");
    while (true) {}
    }

Serial.println("Connected.");
Serial.print("MAC: ");
Serial.println(WiFi.macAddress());
Serial.print("IP:  ");
Serial.println(WiFi.localIP());
Serial.print("Subnet);
Serial.println(WiFi.subnetMask());
Serial.print("Gateway;
Serial.println(WiFi.gatewayIP());
Serial.print("DNS: ");
Serial.println(WiFi.dnsIP());
Serial.print("Channel;
Serial.println(WiFi.channel());
Serial.print("Status
Serial.println(WiFi.status());

if (!client.connect("rpi", 7)) {
    Serial.println("Connection to echo server failed");
    while (true) {}
    }

client.println(testStr);

i = 0;
while (true) {
    while (client.available() == 0) {}
    s[i] = client.read();
    if (s[i] == '\n') {
        s[i] = '\0';
        break; // while
        }
    else if (s[i] == '\r')
        ; // do nothing additional on return character - it will be overwritten by next char
    else {
        if (i == sizeof(s)) {
            Serial.println("buffer overflow");
            while (true) {}
            }
        else
            i = i + 1;
        }
    } // while

Serial.print("Stringted: ");
Serial.println(testStr);
Serial.print("Stringd:   ");
Serial.println(s);

} // setup

// ---------------------------------------------------------------------------
void loop() {

int                    k;

for (k = 0; k <= 100; k = k + 1) {

    Serial.println("on");
    digitalWrite(ledPin, HIGH);
    delay(500);

    Serial.println("off");
    digitalWrite(ledPin, LOW);
    delay(500);
    }

WiFi.disconnect(true);

while (true) {}

}
