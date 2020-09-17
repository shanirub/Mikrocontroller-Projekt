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
  delay(500);
}