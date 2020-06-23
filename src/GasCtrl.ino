#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "Control.h"
#include "Led.h"
#include "Inp.h"

const uint8_t arPins[]={ 12, 13, 14, 15, 16 };  //порты управляющие инжеторами
const uint8_t arPush[]={ 17, 18,19, 4, 23 };   //порты опрашивающие датчики давления
const int nControl = 5;                 //максимально возможное число газовых балонов
Control* ctrl;
Led* led;
Inp* inp;

 
const char* ssid     = "ESP32-Gaz";
const char* password = "iot3iot3iot3";
 
// Set web server port number to 80
WiFiServer server(80);
 
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

  WiFi.softAP(ssid, password);
 
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();

}
//******************************
void loop() {
    WiFiClient client = server.available();   // Listen for incoming clients
 
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            // if (header.indexOf("GET /26/on") >= 0) {
            //   Serial.println("GPIO 26 on");
            //   output26State = "on";
            //   digitalWrite(output26, HIGH);
            // } else if (header.indexOf("GET /26/off") >= 0) {
            //   Serial.println("GPIO 26 off");
            //   output26State = "off";
            //   digitalWrite(output26, LOW);
            // } else if (header.indexOf("GET /27/on") >= 0) {
            //   Serial.println("GPIO 27 on");
            //   output27State = "on";
            //   digitalWrite(output27, HIGH);
            // } else if (header.indexOf("GET /27/off") >= 0) {
            //   Serial.println("GPIO 27 off");
            //   output27State = "off";
            //   digitalWrite(output27, LOW);
            // }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<title>gasVue</title>");
            client.println("<link href=/app.91fc97eb.css rel=preload as=style>");
            client.println("<link href=/app.e045f4ff.js rel=preload as=script>");
            client.println("<link href=/chunk-vendors.b0f460c7.js rel=preload as=script>");
            client.println("<link href=/app.91fc97eb.css rel=stylesheet>");
            // client.println("");
            // client.println("");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            // client.println("<style>.buttons_1[data-v-18a0b573]{display:grid;grid-template-areas:\"but_left but_right\" \"but_next but_next\";grid-gap:1rem}.buttons_1[data-v-6adf7416]{display:grid;grid-template-areas:\"but_left but_right\" \"slider slider\" \"but_next but_next\";grid-gap:1rem}#slider[data-v-6adf7416]{grid-area:slider}#app{font-family:Avenir,Helvetica,Arial,sans-serif;-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale;text-align:center;color:#2c3e50;margin-top:5px;font-size:2.5rem}.nchanel{display:grid;grid-template-rows:24rem 1fr;grid-gap:2rem}.mt10{margin-top:10rem;display:block;font-weight:700}.fs_big{font-size:2.5rem}.buttons_1 div{border:1px solid #ccc;border-radius:5px;color:#fff;font-weight:600;padding:.5rem}#but_left{grid-area:but_left}#but_left,#but_right{background-color:#999}#but_right{grid-area:but_right}#but_next{grid-area:but_next;background-color:#006400}.but_stop{background-color:#de1000;color:#fff;border-radius:5px;padding:2.5rem 0;font-weight:700;font-size:5rem}.second li{display:grid;grid-template-columns:1fr 1fr;font-size:2.5rem;align-items:center}.active{background-color:#006400;color:#fff}.second{margin:0;padding:.5rem 1rem;display:grid}</style>");
            // client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            // client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            // client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // client.println(".button2 {background-color: #555555;}</style>");
            client.println("</head>");
            
            // Web Page Heading
            client.println("<body><div id=app>***</div><script src=chunk-vendors.b0f460c7.js></script><script src=app.e045f4ff.js></script>");
            // client.println("<script src=/js2.js></script>");
            // client.println("<script src=/js1.js></script>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  //+++++++++++++++++++++++++++++++
  ctrl->cycle();
}
