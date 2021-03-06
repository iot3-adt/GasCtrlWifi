#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
// #include "AsyncJson.h"
// #include "ArduinoJson.h"
#include "Control.h"
#include "Led.h"
#include "Inp.h"

const uint8_t arPins[]={ 1, 3, 5, 7 };  //порты удержания управляющие инжеторами
const uint8_t arPinU[]={ 0, 2, 4, 6 };  //порты открывающие (силовые) управляющие инжеторами
const uint8_t arPush[]={ 17, 18, 19, 4};   //порты опрашивающие датчики давления
const uint8_t pinPause = 27;  //обратная связь от ресивера. Пауза в работе.
const int nControl = 3;       //максимально возможное число газовых балонов
int arTemp[nControl];
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
  pinMode(pinPause, INPUT_PULLDOWN);
  ctrl = new Control(nControl, arPins, arPinU, arPush, led, inp, pinPause, 0x20);
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
  led->outError(WiFi.localIP().toString());
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
    int nCh = (int)(p->value().toInt());
    if(nCh > nControl)nCh = nControl;
    for(unsigned int i = 0; (i < nControl) && (i < params - 1); ++i){
      p = request->getParam(i+1);
      arTemp[i] = (int)(p->value().toInt());
    Serial.println(arTemp[i]);
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
