//Include all the required Libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include "Error_404.h"
#include "Error_405.h"
#include "index.h"
#include "login.h"
#include "control_panel.h"

#define RXp2 16
#define TXp2 17

const char* ssid = "Trung Hieu";//Đặt tên wifi cần kết nối
const char* password = "23011992";//mật khẩu wifi cần kết nối

int IRSensor = 9; 
bool state = 0;

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);


String btos(bool x) 
{ 
    if (x) 
        return "True"; 
    return "False"; 
} 
// Replaces placeholder with button section in your web page
String processor(const String& var){
  if(var == "PLACEHOLDER")
    return "<h4><tr><th>esp1</th><th>no ida</th><th>" +btos(state)+ "</th></tr></h4>";
  else return "";
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);

   pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  if(!MDNS.begin("smarttrash"))
  {
   Serial.print("Error setting up MDNS responder!"); 
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/login.html", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/html", login_html);
  });
  server.on("/ControlPanel.html", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", Control_panel_html, processor);
  });


  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET) {
      request->send(404, "text/html", HTML_CONTENT_404);
      Serial.println("not found");
    } else {
      request->send(405, "text/html", HTML_CONTENT_405);
    }
  });
  server.begin();
}

void loop() {
  state = digitalRead(IRSensor); // Set the GPIO as Input
}