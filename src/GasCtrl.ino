#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "Control.h"
#include "Led.h"
#include "Inp.h"

const uint8_t arPins[]={ 12, 13, 14, 15, 16 };  //порты управляющие инжеторами
const uint8_t arPush[]={ 17, 18,19, 4, 23 };   //порты опрашивающие датчики давления
const int nControl = 5;                 //максимально возможное число газовых балонов
Control* ctrl;
Led* led;
Inp* inp;

 
const char* ssid     = "ivanych";
const char* password = "stroykomitet";
 
// Set web server port number to 80
// WiFiServer server(80);
AsyncWebServer server(80);
 
// Variable to store the HTTP request
String header;


//******************************************************
void attention(int pin, int err){
    ctrl->stop();
    char strOut[20];
    int nCh = 0;
    for(;nCh < nControl; ++nCh)
      if(pin == arPins[nCh])break;
    sprintf(strOut, "Error k %i pin %i", nCh, pin);
    // led->outError(strOut);
    ctrl->addError(strOut);
}
//******************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  led = new Led();
  inp = new Inp();
  ctrl = new Control(nControl, arPins, arPush, led, inp);
  Serial.println("end setup");

  if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
  }
  led->outValue(WiFi.localIP().toString());
  Serial.println(WiFi.localIP());
  // WiFi.softAP(ssid, password);
  // IPAddress IP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(IP);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/app.91fc97eb.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/app.91fc97eb.css", "text/css");
  });
  server.on("/app.1cc39d02.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/app.1cc39d02.js", "text/javascript");
  });
  server.on("/chunk-vendors.473ee555.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/chunk-vendors.473ee555.js", "text/javascript");
  });
  server.on("/send_array", HTTP_GET, [](AsyncWebServerRequest *request){
    int params = request->params();
    AsyncWebParameter* p = request->getParam(0);
    int nCh = (int)p->value().toInt();
    int* arTemp = new int(nCh);
    for(int i = 1; i < params; ++i){
      p = request->getParam(i);
      arTemp[i-1] = (int)p->value().toInt();
    }
    request->send(200, "text/plain", "send_array");
    ctrl->init(arTemp, nCh);
  });
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "STOP");
    ctrl->stop();
  });
  server.begin();
}
//******************************
void loop() {
  ctrl->cycle();
}
