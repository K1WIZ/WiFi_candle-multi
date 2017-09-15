/*
 * This is a simple flickering candle that can be controlled by WiFi.  With EEPROM 
 * support, it is possible for the candle to remember its state (on or off) if power is 
 * cycled.
 * 
 * Written by: John Rogers
 * Date: 9/11/17
 * 
 * License: GPL v3
 */

#include <ESP8266TrueRandom.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
/*
IPAddress ip(10, 2, 50, 100);
IPAddress dns(10, 0, 1, 10);
IPAddress gateway(10, 2, 50, 1);
IPAddress subnet(255, 255, 255, 0);
*/
const char* ssid = "SSID";
const char* password = "P@ssw0rd";
MDNSResponder mdns;
ESP8266WebServer server(80);

int state;
int lastState;
String response;  // we build the string that creates the web UI based on selection

void handleRoot() {
  server.send(200, "text/html", response);
}

#define fPin 12
#define fPin2 13

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void){
  pinMode(fPin, OUTPUT);
  pinMode(fPin2, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //WiFi.config(ip, dns, gateway, subnet);  // This section if you want hard-coded IP
  WiFi.begin(ssid, password);
  EEPROM.begin(8);
  
  server.on("/", handleRoot);
  
  server.on("/on", [](){
  server.send(200, "text/html", response);
  state = 1;
  setting();
  });
  
  server.on("/off", [](){
  server.send(200, "text/html", response);
  state = 0;
  setting();
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}


void setting(){
  if( EEPROM.read(0) != state ){
    EEPROM.write(0, state);
    EEPROM.commit();
  }
}

void showstate(){
  if( EEPROM.read(0) == 1 ){
    response = "<h1>Wireless Candle</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a><br><br>CANDLE IS ON</p>";
  } else if ( EEPROM.read(0) == 0 ){
    response = "<h1>Wireless Candle</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a><br><br>CANDLE IS OFF</p>";;
  }
}

 
void loop(void){
showstate();
  lastState = EEPROM.read(0);
  
  if (lastState == 1) {
  candle();
  } else if (lastState == 0) {
    analogWrite(fPin, 0);
    analogWrite(fPin2, 0);
    }
  server.handleClient();
}
 
void candle() {

analogWrite(fPin, ESP8266TrueRandom.random(30,255)); //Use the same pins used above
analogWrite(fPin2, ESP8266TrueRandom.random(30,255));
delay(ESP8266TrueRandom.random(40,70)); //Limits the speed.
}
