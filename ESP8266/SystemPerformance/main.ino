/*
* This code is designed to run on an ESP8266 microcontroller board, commonly used for IoT projects. 
* Its purpose is to gather system information such as heap (available memory), stack usage,
and frequency (clock speed) of the microcontroller.

* The ESP8266 sends this information to a server running on a computer. 
* The server then hosts a webpage and communicates with the ESP8266 using websockets.

Here's a breakdown of the process:

1. The ESP8266 gathers system information periodically.
2. It establishes a connection with the server using websockets.
3. It sends the system information to the server.
4. The server receives the data and updates the webpage accordingly.

Overall, this setup allows remote monitoring and visualization of system information from the ESP8266 on a webpage hosted by the server.

    Created by: Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]

*/


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>


const char* ssid = "YourWifi";
const char* password = "TheBestWifi";
String serverAddress = "192.168.1.99"; 


IPAddress staticIP(192, 168, 1, 100); // Define your desired static IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8); // DNS serverc
const char* DeviceName = "TestModulte";


void setup() {
  Serial.begin(9600);
  delay(100);

  // Connect to WiFi
   WiFi.config(staticIP, gateway, subnet, dns); 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to WiFi");
   MDNS.begin(DeviceName);
}

void loop(){
  requests();
}

void requests() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    
    // Prepare the URL with heap, CPU, and stack values as parameters
    String getRequest = "/system?heap=";
    getRequest += ESP.getFreeHeap();
    getRequest += "&cpu=";
    getRequest += ESP.getCycleCount() / (ESP.getCpuFreqMHz() * 1000000.0) * 100;
    getRequest += "&stack=";
    getRequest += ESP.getFreeContStack();

    // Construct the full URL
    String fullURL = "http://" + String(serverAddress) +":8080" + getRequest;

    // Send GET request
    http.begin(client, fullURL); // Specify request destination with updated syntax
    int httpResponseCode = http.GET(); // Send the request

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = http.getString(); // Get the response payload
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Close connection
  } else {
    Serial.println("Not connected to WiFi");
  }

  delay(3000); // Wait before making the next request
}
