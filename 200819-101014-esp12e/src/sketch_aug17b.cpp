/*
 * Code for the ESP12-E microcontroller.
 * 
 *  1. establishes an access point.
 *  2. starts a webserver.
 *  3. waits on http requests and starts motors accordingly.
 * 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// motors pins
#define DRIVE_MOTOR_POWER D2 // Motor B
#define DRIVE_MOTOR_DIRECTION D4
#define STEER_MOTOR_POWER D1 // Motor A
#define STEER_MOTOR_DIRECTION D3
#define ROOT_REQUEST 0
#define BACK_REQUEST 1
#define FORWARD_REQUEST 2
#define RIGHT_REQUEST 3
#define LEFT_REQUEST 4
#define STOP_DRIVE_REQUEST 5
#define STOP_TURN_REQUEST 6

// drivePower sets how fast the car goes
// Can be set between 0 and 1023 (although car probably wont move if values are too low)
int drivePower = 1023; // todo: make changeable

// driveDirection sets what direction the car drives
// If the car is moving backwards when you press the forward button, change this to LOW
uint8_t driveDirection = HIGH;

// steeringPower sets how fast the car turns
// Can be set between 0 and 1023 (again, car probably won't steer if the value is too low)
int steeringPower = 1023;

// steerDirection sets what direction the car steers
// If the car is steering right when you press the left button, change this to LOW
uint8_t steerDirection = HIGH;

#ifndef APSSID
#define APSSID "car_wifi"
#define APPSK  "car"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

// method for handling various requests
void handleRequest(int requestCode) {
  Serial.print(" request arrived: ");
  Serial.println(requestCode);
}

void setup() {
  // establishes an access point
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  // starts a webserver
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // define request handlers
  server.on("/", []() {
    handleRequest(ROOT_REQUEST);
  });

  server.on("/back", []() {
    handleRequest(BACK_REQUEST);
  });

  server.on("/forward", []() {
    handleRequest(FORWARD_REQUEST);
  });

  server.on("/right", []() {
    handleRequest(RIGHT_REQUEST);
  });

  server.on("/left", []() {
    handleRequest(LEFT_REQUEST);
  });

  server.on("/stopDrive", []() { 
    handleRequest(STOP_DRIVE_REQUEST);
  });

  server.on("/stopTurning", []() {
    handleRequest(STOP_TURN_REQUEST);
  });

  server.begin();
  Serial.println("HTTP server started");

  // defines outputs (2 motors x 2 pins + 1 LED)
  pinMode(DRIVE_MOTOR_POWER, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(DRIVE_MOTOR_DIRECTION, OUTPUT);
  pinMode(STEER_MOTOR_POWER, OUTPUT);
  pinMode(STEER_MOTOR_DIRECTION, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level)

  Serial.println("blink on");
  // drive_right_backwards();

  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("blink off");
  // stop_driving();

  delay(2000);

  // access point handling
  server.handleClient();
}
