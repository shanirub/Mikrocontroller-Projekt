#include <Arduino.h>
#include <driver/adc.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define j_sw 33   // ADC1 5
#define j_y 32    // ADC1 4
#define j_x 35    // ADC1 7
#define led 22

const char* ssid     = "Olafchen";
const char* password = "lalalalala";

String serverName = "http://192.168.4.100";
String serverPath = "";

int x_value = 0;
int y_value = 0;
int sw_value = 0;


void setup() {

  Serial.begin(9600);

  // piModes
  pinMode(led, OUTPUT);
  pinMode(j_sw, INPUT_PULLUP);
  pinMode(j_y, ANALOG);
  pinMode(j_x, ANALOG);

  // establish access point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // wait for auto (esp12) to connect
  while (WiFi.softAPgetStationNum() == 0) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Stations connected: ");
  Serial.println(WiFi.softAPgetStationNum());
  // auto ip was set to 192.168.4.100
  
  
}

void loop() {
  x_value = analogRead(j_x);
  y_value = analogRead(j_y);
  sw_value = digitalRead(j_sw);

  Serial.print("X ");
  Serial.print(x_value);
  Serial.print("| Y ");
  Serial.print(y_value);
  Serial.print("| SW ");
  Serial.print(sw_value);
  Serial.println("");

  serverPath = "";

  if (x_value < 500) {
    // left
    serverPath = serverName + "/left";
  }

  if (x_value > 3500) {
    // right
    serverPath = serverName + "/right";
  }

  if (y_value < 500) {
    // forward
    serverPath = serverName + "/forward";
  }

  if (y_value > 3500) {
    // backwards
    serverPath = serverName + "/back";
  }

  if (sw_value == 0) {
    // stop
    serverPath = serverName + "/driveStop";
  }

  if (serverPath.length() > 0) {
    HTTPClient http;
    http.begin(serverPath);
    int httpResponseCode = http.GET();
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  
  delay(500);
}