/**
 * @author Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]
 * @date 17 - 05 -2024
 * *This is a basic wifi setup & with internet connectivity testing
 * TODO: Function => request(website, request) and wifi()
 * 
 * @bug "the ping is not working properly"
*/


#include "WiFi.h"
#include <HTTPClient.h>

void setup() {
  Serial.begin(115200);
  Serial.printf("ESP32 STARTED..................\n");
  delay(500);
  wifi();
  delay(500);
  request("www.google.com","");
}
void wifi(){
  const char* SSID = "YourWifi";
  const char* PASS = "TheBestWifi";

  Serial.printf("Conneting to wifi....");
  WiFi.begin(SSID,PASS);
  WiFi.enableIpV6();
  int8_t counter = 0;
  while(WiFi.status() != WL_CONNECTED && counter <= 30){
    delay(1000);
    Serial.print(".");
    counter++;
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.printf("\n WIFI CONNECTED TO %s ",SSID);
    Serial.print("IPv4 ADDRESS : ");
    Serial.println(WiFi.localIP());
    Serial.print("IPv6 ADDRESS : ");
    Serial.println(WiFi.localIPv6());
  // PING TEST 
  }else 
    Serial.println("FAILED TO CONNECT TO WIFI.........");

}


void request(String serverAddress ,String getRequest){
  HTTPClient http;
  WiFiClient client;

  String fullURL = "http://" + String(serverAddress) + getRequest;
  
  unsigned long startTime = millis();

  // Send GET request
  http.begin(client, fullURL); // Specify request destination with updated syntax
  int httpResponseCode = http.GET(); // Send the request

  unsigned long requestTime = millis() - startTime;  //Count the time in 'ms'

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Time taken for HTTP request: ");
    Serial.print(requestTime);
    Serial.println(" ms");

    String response = http.getString(); // Get the response payload
    Serial.println("Response:");
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end(); // Close connection
  delay(1000);
}

void loop() {} //Nothing needs here...