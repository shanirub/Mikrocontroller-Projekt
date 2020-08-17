/*
 * Code for the ESP12-E microcontroller.
 * 
 */


/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "car_wifi"
#define APPSK  "car"
#endif


int PWMA=5;//Right side 
int PWMB=4;//Left side 
int DA=0;//Right reverse 
int DB=2;//Left reverse 

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  // establish access point
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  // define outputs
  pinMode(LED_BUILTIN, OUTPUT); // for testing
  pinMode(PWMA, OUTPUT); // front motor
  pinMode(PWMB, OUTPUT); // back motor
  pinMode(DA, OUTPUT); 
  pinMode(DB, OUTPUT);
  
}

void loop() {
  // led blink testing
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level)
  digitalWrite(PWMA, LOW); 
  digitalWrite(DA, LOW);  
  digitalWrite(PWMB, HIGH); 
  digitalWrite(DB, HIGH); 
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(PWMA, HIGH); 
  digitalWrite(DA, HIGH);  
  digitalWrite(PWMB, LOW); 
  digitalWrite(DB, LOW); 
  delay(2000);

  // access point handling
  server.handleClient();
  
}
