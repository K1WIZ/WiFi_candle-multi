#include <ESP8266TrueRandom.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

IPAddress ip(10, 2, 50, 100);
IPAddress dns(10, 0, 1, 10);
IPAddress gateway(10, 2, 50, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "SSID";
const char* password = "P@ssw0rd";
MDNSResponder mdns;
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Wireless Candle</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>");
}

#define fPin 12
#define fPin2 13

int state = 0;

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
  SPIFFS.begin();
  WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(ssid, password);
  
  server.on("/", handleRoot);
  
  server.on("/on", [](){
  server.send(200, "text/plain", "Okay -- Candle is lit!");
  state = 1;
  File f = SPIFFS.open("/state.txt", "w"); 

    if (!f) {
      Serial.println("file open failed on update.");
    } else {
      f.println(state); 
      f.close();
    }
  
  });
  
  server.on("/off", [](){
  server.send(200, "text/plain", "Okay -- Candle is off!");
  //digitalWrite(fPin, 0);
  state = 0;
  File f = SPIFFS.open("/state.txt", "w"); 

    if (!f) {
      Serial.println("file open failed on update.");
    } else {
      f.println(state); 
      f.close();
    }
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){

  File f = SPIFFS.open("/state.txt", "r");
  if (!f) {
      Serial.println("File open failed on read.");
    } else {
      while(f.available()) {
        //Lets read line by line from the file
        String line = f.readStringUntil('\n');
        state = line.toInt();
        break; //if left in, we'll just read the first line then break out of the while.
      } 
      f.close();
    }

  
  if (state == 1) {
  candle();
  }
  else {
    if (state == 0) {
    analogWrite(fPin, 0);
    analogWrite(fPin2, 0);
    }
  }
  server.handleClient();
}
 
void candle() {

analogWrite(fPin, ESP8266TrueRandom.random(30,255)); //Use the same pins used above
analogWrite(fPin2, ESP8266TrueRandom.random(30,255));
delay(ESP8266TrueRandom.random(40,70)); //Limits the speed.
}
