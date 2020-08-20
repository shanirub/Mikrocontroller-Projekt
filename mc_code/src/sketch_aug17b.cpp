/*
 * Code for the ESP12-E microcontroller
 * 
 *  1. (establishes an access point.) <-- wifi temporarily for testing
 *  2. starts a webserver.
 *  3. waits on http requests and starts motors accordingly.
 * 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// motors pins
#define DRIVE_MOTOR_POWER D2 // Rear motor
#define DRIVE_MOTOR_DIRECTION D4
#define STEER_MOTOR_POWER D1 // Steer motor
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
/*
  // establishes an access point TEMP REMOVED for testing
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);

  // starts a webserver
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
*/


/*
TEMP wifi for easier debugging
*/

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.begin(115200);
  WiFi.begin("Kaisernet", "yuvalshani");
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("WifiCar")) {
    Serial.println("MDNS Responder Started");
  }

/*
TEMP END
*/

  // define request handlers
  server.on("/", []() {
    Serial.println("root request");
  });

  server.on("/back", []() {
    Serial.println("back");
    analogWrite(DRIVE_MOTOR_POWER, drivePower);
    digitalWrite(DRIVE_MOTOR_DIRECTION, !driveDirection);
    server.send(200, "text/plain", "Drive backwards");
  });

  server.on("/forward", []() {
    Serial.println("forward");
    analogWrite(DRIVE_MOTOR_POWER, drivePower);
    digitalWrite(DRIVE_MOTOR_DIRECTION, driveDirection);
    server.send(200, "text/plain", "Drive forward");
  });

  server.on("/right", []() {
    Serial.println("right");
    analogWrite(STEER_MOTOR_POWER, steeringPower);
    digitalWrite(STEER_MOTOR_DIRECTION, steerDirection);
    server.send(200, "text/plain", "Turn right");  
  });

  server.on("/left", []() {
    Serial.println("left");
    analogWrite(STEER_MOTOR_POWER, steeringPower);
    digitalWrite(STEER_MOTOR_DIRECTION, !steerDirection);
    server.send(200, "text/plain", "Turn left");
  });

  server.on("/stopDrive", []() { 
    Serial.println("driveStop");
    analogWrite(DRIVE_MOTOR_POWER, 0);
    server.send(200, "text/plain", "Drive motor stopped");
  });

  server.on("/stopTurning", []() {
    Serial.println("steerStop");
    analogWrite(STEER_MOTOR_POWER, 0);
    server.send(200, "text/plain", "Steer motor stopped");
  });

  server.begin();
  Serial.println("HTTP server started");

  // defines outputs (2 motors x 2 pins)
  pinMode(DRIVE_MOTOR_POWER, OUTPUT);
  pinMode(DRIVE_MOTOR_DIRECTION, OUTPUT);
  pinMode(STEER_MOTOR_POWER, OUTPUT);
  pinMode(STEER_MOTOR_DIRECTION, OUTPUT);

}

void loop() {
  server.handleClient();
}
