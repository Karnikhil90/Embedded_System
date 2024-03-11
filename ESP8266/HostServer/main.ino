
/*
  IoT LED Control System with SD Card Logging

  This Arduino code is designed for an ESP8266-based IoT system that controls LEDs and logs user actions to an SD card.
  
  Pin Connections:
  - D1: checking LED
  - D2: White LED
  - D3: Red LED
  - D8: SD card module

  Functionality:
  - Connects to a specified WiFi network using provided credentials.
  - Sets up a web server on port 80 to handle user requests.
  - Responds to requests to toggle LEDs (white or red) via a simple web interface.
  - Logs user actions and system events, including timestamps and IP addresses, to an SD card.
  - Provides a default web page ("/website/index.html") for user interaction.

  Usage:
  - Ensure correct pin connections for LEDs and the SD card module.
  - Set WiFi credentials (SSID and PASS) to connect to the desired network.
  - Customize the "/website/index.html" file for the web interface if needed.
  
  Created by: @Karnikhil90 [Github, Twitter, LinkedIn]
  
  Note: This code is designed for educational purposes and can be extended for various IoT applications.
*/

#include <SD.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // Universal Time -> IST(5:30)

// Global Variables (Wifi Credentials)
const char* SSID = "yourWifi";
const char* PASS = "TheBestWifi";

// Declare variables for LEDs
const int checkingLED = D1; // checking LED 
const int WhiteLED = D2;    // White LED
const int RedLED = D3;

// Checking variable
bool isEverythingOk = true;

ESP8266WebServer server(80);

// Function declarations
void SDcard();
void Wifi();
void handleRoot();
void logToFile(IPAddress ip, String action);
void AllOffLED();
void OnLed(String color);

void setup() {
  Serial.begin(9600);
  Serial.println("\n\tStarted.....");
  delay(1500);
  SDcard();
  Wifi();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/button", HTTP_GET, []() {
    String color = server.arg("color");
    if (color == "white" || color == "Lightoff" || color == "red") {
      logToFile(server.client().remoteIP(), "Button Pressed: " + color);
      OnLed(color);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Invalid color");
    }
  });

  server.begin();
  pinMode(checkingLED, OUTPUT);
  pinMode(WhiteLED, OUTPUT);
  pinMode();
  AllOffLED();
  digitalWrite(WhiteLED, HIGH);
}

void SDcard() {
  Serial.println("\n\tTrying To Connect with SD card(module)\n");
  if (SD.begin(D8)) {
    Serial.println("SD Card Just Connected....\n");
  } else {
    Serial.println("Connection Failed\n");
    isEverythingOk = false;
  }
}

void Wifi() {
  Serial.printf("\n\tStarting Wifi to ");
  WiFi.begin(SSID, PASS);
  delay(2000);
  Serial.printf("Connecting to the network '%s' ", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.print("\nIP Address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Connected to '%s' ", SSID);
}

void handleRoot() {
  Serial.println(server.client().remoteIP());
  String filePath = "/website/index.html";

  if (!SD.exists(filePath)) {
    server.send(404, "text/plain", "404: File not found");
    logToFile(server.client().remoteIP(), "File not found");
    isEverythingOk = false;
    return;
  }

  File file = SD.open(filePath, "r");

  if (file) {
    logToFile(server.client().remoteIP(), "Client requested root");
    server.send(200, "text/html", file);
    while (file.available()) {
      server.sendContent(String((char)file.read()));
    }
    file.close();
  } else {
    server.send(500, "text/plain", "500: Internal Server Error");
    logToFile(server.client().remoteIP(), "Internal Server Error");
  }
}

void logToFile(IPAddress ip, String action) {
  timeClient.update(); // Update the timeClient

  File historyFile = SD.open("/LOG/serverlog.txt", FILE_WRITE);

  if (historyFile) {
    historyFile.printf("[%02d:%02d:%02d] %d.%d.%d.%d %s\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), ip[0], ip[1], ip[2], ip[3], action.c_str());
    historyFile.close();
  } else {
    Serial.println("Error opening serverlog.txt for writing");
  }
}

void AllOffLED() {
  digitalWrite(checkingLED, LOW);
  digitalWrite(WhiteLED, LOW);
}

void OnLed(String color) {
  if (color == "white") {
    digitalWrite(WhiteLED, !digitalRead(WhiteLED));  // Toggle the state of the white LED
  } else if (color == "red") {
    digitalWrite(RedLED, !digitalRead(RedLED));  // Toggle the state of the white LED
  } else if (color == "LightOFF") {
    AllOffLED();
  } 
}

void loop() {
  server.handleClient();
}

/*
    Build by @Karnikhil90 [Github,Twitter and linkedin]
    Date : 11 March , 2024

*/