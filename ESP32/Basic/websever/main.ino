/**
 * @author Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]
 * @date 17 - 05 -2024
 * *This is a basic server & show is connected client . THIS WILL BE UPDATE , I'M LEARNING THIS STUFFS....!!!
 * TODO: Function {with have something use in this project} => mywebserver(), root() and loop()
 * *WEBSITE PART => I have a basic website and here i have written that html part and *.css & *.js has been take from my website 
*/

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);
bool newClientAvailable = false;

void root() {
  String html;
  html = "<!DOCTYPE html> <head> <title>ESP32 Number counter </title> <link rel=\"stylesheet\" href=\"https://karnikhil90.w3spaces.com/style.css\"></head>";
  html += "<body><h1>Welcome To Counter</h1><div id=\"theNumContainer\"> <label id=\"theNum\">0</label> <div id=\"contaner\">";
  html += "<button id=\"Decr\" class=\"btn\">Decrease</button><button id=\"Re\" class=\"btn\">Reset</button> <button id=\"Incr\" class=\"btn\">Increase</button>";
  html +=" </div> <script src=\"https://karnikhil90.w3spaces.com/script.js\"></script> </body>";
  server.send(200, "text/html", html.c_str()); // Convert String to const char* using c_str()
}

void mywebserver() {
  server.on("/", root);

  server.onNotFound([]() { // Handle 404 Not Found
    String message = "File Not Found\n\n";
    server.send(404, "text/plain", message);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void wifi() {
  const char* SSID = "YourWifi";
  const char* PASS = "TheBestWifi";

  Serial.println("Connecting to WiFi...");
  WiFi.begin(SSID, PASS);
  WiFi.enableIpV6();
  int8_t counter = 0;
  while (WiFi.status() != WL_CONNECTED && counter <= 30) {
    delay(1000);
    Serial.print(".");
    counter++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected.");
    Serial.print("IPv4 ADDRESS : ");
    Serial.println(WiFi.localIP());
    Serial.print("IPv6 ADDRESS : ");
    Serial.println(WiFi.localIPv6());
  } else {
    Serial.println("Failed to connect to WiFi.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  wifi();
  delay(1000);
  mywebserver();
}

void loop() {
 WiFiClient client = server.client();
 if (client) newClientAvailable= true;

  if (newClientAvailable){
    if (client) {
      Serial.println("New client connected.");
      Serial.print("Client IP address: ");
      Serial.println(client.remoteIP());
      newClientAvailable = false;
    }
  }
  server.handleClient();
}