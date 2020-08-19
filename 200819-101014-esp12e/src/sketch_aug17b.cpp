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
    Serial.println("root request");
  });

  // driving forward, then backwards repeat
  server.on("/back", []() {
    Serial.println("back");
    analogWrite(DRIVE_MOTOR_POWER, drivePower);
    digitalWrite(DRIVE_MOTOR_DIRECTION, !driveDirection);
    server.send(200, "text/plain", "back");
  });

  // driving forward, then backwards repeat
  server.on("/forward", []() {
    Serial.println("forward");
    analogWrite(DRIVE_MOTOR_POWER, drivePower);
    digitalWrite(DRIVE_MOTOR_DIRECTION, driveDirection);
    server.send(200, "text/plain", "forward");
  });

  // WORKING
  server.on("/right", []() {
    Serial.println("right");
    analogWrite(STEER_MOTOR_POWER, steeringPower);
    digitalWrite(STEER_MOTOR_DIRECTION, steerDirection);
    server.send(200, "text/plain", "right");  
  });

  // WORKING
  server.on("/left", []() {
    Serial.println("left");
    analogWrite(STEER_MOTOR_POWER, steeringPower);
    digitalWrite(STEER_MOTOR_DIRECTION, !steerDirection);
    server.send(200, "text/plain", "left");
  });

  // WORKING
  server.on("/stopDrive", []() { 
    Serial.println("driveStop");
    analogWrite(DRIVE_MOTOR_POWER, 0);
    server.send(200, "text/plain", "driveStop");
  });

  // WORKING
  server.on("/stopTurning", []() {
    Serial.println("steerStop");
    analogWrite(STEER_MOTOR_POWER, 0);
    server.send(200, "text/plain", "steerStop");
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
