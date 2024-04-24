/*
    This code is a foundation for creating web-connected projects using an ESP8266 microcontroller, 
    commonly used in IoT (Internet of Things) applications. 
    Here's a simple breakdown of what it does:

    How it Works:

*   Connecting to Wi-Fi: The device joins your home Wi-Fi network so it can talk to other 
    devices and services online.
*    Handling WebSockets: It sets up a system for real-time chats between the device and a web browser,
    making updates and interactions happen instantly without refreshing the page.
   
*    SD Card Operations: It reads and serves files from an SD card, like hosting web pages or sharing resources.

*    NTP Time Synchronization: It keeps the device's clock accurate by syncing it with an online clock, ensuring
    events and data arelogged with the correct time.
*   ESP8266 Web Server: It acts as a mini website, serving pages and responding to requests from browsers 
    or other devices.

     Created by: Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]
  
  Note: This code is designed for educational purposes and can be extended for various IoT applications.
!  Problems : Getting Stack Overflow Errors while handling multiple(specificly more than 2 users) on my esp8266 web server.  

Pin Connections:
    - D1: checking LED
    - D2: White LED
    - D3: Red LED
    - D8: SD card module


*/




#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <SD.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

WiFiUDP ntpUDP; // Declare WiFiUDP object
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // Universal Time -> IST(5:30)

const char* ssid = "Airtel_Zeus";
const char* password = "TheBestWifi";

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

  void Wifi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nWiFi connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.println("");
    }
  }

void setup() {
  Serial.begin(9600);
  delay(100);
  Wifi();
  if (!SD.begin(D8)) {
    Serial.println("==========SD CARD INITIALIZING FAILED==========");
    return;
  }
  Serial.println("==========SD CARD INITIALIZED SUCCESSFULLY==========");

  // Start WebSocket server
  webSocket.begin(); // Start the WebSocket server
  delay(100);
  webSocket.onEvent(handleWebSocketEvent); // Set up the WebSocket event handler

  // Serve the index.html file for root path
  server.on("/", HTTP_GET, []() {
    File file = SD.open("/website/index.html");
    delay(1000);
    if (!file) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    size_t sent = server.streamFile(file, "text/html");
    file.close();
  });

 // Serve all files in the /data directory
  server.on("/data", HTTP_GET, [](){
    if (!SD.exists("/data")) {
      server.send(404, "text/plain", "Directory not found");
      return;
    }
    File dir = SD.open("/data");
    if (!dir || !dir.isDirectory()) {
      server.send(500, "text/plain", "Failed to open directory");
      return;
    }

    server.setContentLength(CONTENT_LENGTH_UNKNOWN); // Required for chunked transfer encoding

    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/html", ""); // Start chunked transfer

    File entry;
    while (entry = dir.openNextFile()) {
      if (entry.isDirectory()) {
        continue;
      }
      server.streamFile(entry, getContentType(entry.name()));
      entry.close();
    }
    dir.close();
  });

   server.on("/client?online", HTTP_GET, []() {
    String clientList = "";
    for (int i = 0; i < webSocket.connectedClients(); i++) {
      clientList += String(i) + ": " + getClientIdentifier(i) + "\n";
    }
    // Convert clientList to hexadecimal
    char hexBuffer[clientList.length() * 2 + 1];
    char hexChars[] = "0123456789ABCDEF";
    for (size_t i = 0; i < clientList.length(); ++i) {
      hexBuffer[i * 2] = hexChars[clientList.charAt(i) >> 4];
      hexBuffer[i * 2 + 1] = hexChars[clientList.charAt(i) & 0xF];
    }
    hexBuffer[clientList.length() * 2] = '\0'; // Null-terminate the string
    server.send(200, "text/plain", hexBuffer);
  });
  server.on("/command", HTTP_GET, []() {
    if (server.args() > 0) {
      String cmd;
      for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i) == "cmd") {
          cmd = server.arg(i);
          break;
        }
      }
      if (!cmd.isEmpty()) {
        if (cmd.startsWith("D")) {
           Serial.println(cmd);
          int pin = cmd.substring(1).toInt();
          if (pin >= 0 && pin <= 4) {
            // Toggle the specified pin
            pinToggle(cmd.substring(0,2));
            Serial.print(cmd.substring(0,2));
            server.send(200, "text/plain", "Pin toggled successfully");
          } else {
            server.send(400, "text/plain", "Invalid pin number");
          }
        } else if (cmd == "reboot") {
          // Delay the restart for 2 seconds
          delay(2000);
          // Restart the ESP
          ESP.restart();
          server.send(200, "text/plain", "System is rebooting");
        } else {
          server.send(400, "text/plain", "Invalid command");
        }
      } else {
        server.send(400, "text/plain", "Missing cmd parameter");
      }
    } else {
      server.send(400, "text/plain", "Missing cmd parameter");
    }
  });

  // Start the ESP8266WebServer
  server.begin();
  delay(1000);
  Serial.println("WebSocket server & WebServer Both are online ");

  // Log server online status
  log("=================================SERVER IS ONLINE===============================\n\n\n");
  delay(100);
  // Just to control few leds with it. You can remove it if ypur are't using it. 
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
}
  void pinToggle(String pin){
    if(pin=="D0")
      digitalWrite(D0, !digitalRead(D0));
    else if(pin=="D1")
    digitalWrite(D1, !digitalRead(D1));
    else if(pin=="D2")
    digitalWrite(D2, !digitalRead(D2));
    else if(pin=="D3")
    digitalWrite(D3, !digitalRead(D3));
    else if(pin=="D4")
    digitalWrite(D4, !digitalRead(D4));
    else Serial.println("INVALID PIN");
    delay(1200);
  }

  void loop() {
    delay(100);
    webSocket.loop(); // Handle WebSocket events
    delay(100);
    server.handleClient(); // Handle HTTP requests
  }

  void logToFile(IPAddress ip, String action) {
  delay(100);
    timeClient.update(); // Update the timeClient

    File historyFile = SD.open("/log/log.txt", "a"); // Open file for appending

    if (historyFile) {
      historyFile.printf("[%02d:%02d:%02d] %d.%d.%d.%d %s\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), ip[0], ip[1], ip[2], ip[3], action.c_str());
      historyFile.close();
    } else {
      Serial.println("Error opening serverlog.txt for writing");
    }
}

 void log(const char* action) {
  delay(100);
    timeClient.update(); // Update the timeClient
    Serial.println(action);
    File historyFile = SD.open("/log/log.txt", "r"); // Open file for appending

    if (historyFile) {
      historyFile.printf("[%02d:%02d:%02d]=> %s\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), action);
      historyFile.close();
    } else {
      Serial.println("Error opening log file for writing");
    }
}

void handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  log(((char*)payload));
  delay(100);
  if (type == WStype_TEXT) {
    delay(100);
    String message = String((char*)payload);

    if (message.startsWith("/join?username=")) {
      String username = message.substring(15); // Length of "/join?username=" is 15
      addClient(username);
      Serial.print("New client joined - Username: ");
      Serial.print(username);
      Serial.print(", IP address: ");
      Serial.println(webSocket.remoteIP(num));
      String welcomeMessage = String(num) + " Welcome, " + username + "!";
      webSocket.sendTXT(num, welcomeMessage);
    }
    else if (message.startsWith("/sendfile?")) {
      // Handle sendfile command
      // Extract recipient and file path from the message
      delay(100);
      int8_t recipientStart = message.indexOf("to=") + 3;
      int8_t recipientEnd = message.indexOf("&", recipientStart);
      String recipient = message.substring(recipientStart, recipientEnd);

      int filePathStart = message.indexOf("from=") + 5;
      String filePath = message.substring(filePathStart);

      // Check if recipient is connected
      bool recipientFound = false;
      int8_t recipientConnectionIndex = -1;
      for (int8_t i = 0; i < webSocket.connectedClients(); i++) {
        String clientIdentifier = getClientIdentifier(i); // Get the identifier of the connected client
        if (clientIdentifier == recipient) {
          // Recipient found
          recipientFound = true;
          recipientConnectionIndex = i;
          break;
        }
      }
      // Define a buffer size for reading file contents
    #define BUFFER_SIZE 512

    // Read the file into a buffer and send it over WebSocket
    if (recipientFound) {
        // Open the file for reading
        File fileToSend = SD.open(filePath, FILE_READ);
        if (fileToSend) {
            // Create a buffer to store file contents
            uint8_t buffer[BUFFER_SIZE];
            size_t bytesRead;

            // Get the content type based on file extension
            String contentType = getContentType(filePath);

            // Send the content type as a text message first
            webSocket.sendTXT(recipientConnectionIndex, contentType);

            // Read file contents into buffer and send them in chunks
            while ((bytesRead = fileToSend.read(buffer, BUFFER_SIZE)) > 0) {
                // Send the buffer over WebSocket
                webSocket.sendBIN(recipientConnectionIndex, buffer, bytesRead);
            }

            // Close the file
            fileToSend.close();
        } else {
            // File not found, send error message to sender
            String errorMessage = "ERROR:File not found: " + filePath;
            webSocket.sendTXT(num, errorMessage);
        }
    }

        } else if (message.startsWith("/message?")) {
          // Handle message command
          // Extract recipient from message
          int recipientStart = message.indexOf("to=") + 3;
          int recipientEnd = message.indexOf("&", recipientStart);
          String recipient = message.substring(recipientStart, recipientEnd);
          
          // Check if recipient is connected
          bool recipientFound = false;
          for (int i = 0; i < webSocket.connectedClients(); i++) {
            IPAddress clientIP = webSocket.remoteIP(i);
            String clientIdentifier = getClientIdentifier(i); // Get the identifier of the connected client
            Serial.printf("DEBUG: client ID=%s\n", clientIdentifier);
            if (clientIdentifier == recipient) {
              // Recipient found, send the message
              webSocket.sendTXT(i, message);
              recipientFound = true;
              break;
            }
            delay(100);
          }
          
          // If recipient not found, send a message back to sender
          if (!recipientFound) {
            String errorMessage = "ERROR:Recipient " + recipient + " not found";
            webSocket.sendTXT(num, errorMessage);
          }
        }
      } else if (type == WStype_DISCONNECTED) {
        // Handle disconnection
        // Find the username of the disconnected client and remove it from the list
        String disconnectedClientIdentifier = getClientIdentifier(num);
        removeClient(disconnectedClientIdentifier);
        
        // Print a message indicating the client has disconnected
        Serial.print("Client disconnected - Username: ");
        Serial.println(disconnectedClientIdentifier);
      }
      delay(500);
  }


#define MAX_CLIENTS 10

  String connectedClients[MAX_CLIENTS];
  int8_t numConnectedClients = 0;

void addClient(String username) {
    // log("DEBUG: ADD USER %s \n",username);
    if (numConnectedClients < MAX_CLIENTS) {
      connectedClients[numConnectedClients++] = username;
    }
  }

void removeClient(String username) {  
    for (int i = 0; i < numConnectedClients; i++) {
    if (connectedClients[i] == username) {
        for (int j = i; j < numConnectedClients - 1; j++) {
        connectedClients[j] = connectedClients[j + 1];
        }
        numConnectedClients--;
        break;
    }
    }
}

String getClientIdentifier(int clientIndex) {
  Serial.printf("DEBUG: CHEAKING FOR AN USER \n");
  if (clientIndex >= 0 && clientIndex < numConnectedClients) {
    return connectedClients[clientIndex];
  } else {
    return ""; // Return an empty string if client index is out of bounds
  }
}
void sendFile(uint8_t num, String filePath) {
  // Open the file from SD card
  File file = SD.open(filePath, FILE_READ);

  if (file) {
    // Read file content and send it over WebSocket
    while (file.available()) {
      String fileContent = file.readStringUntil('\n');
      webSocket.sendTXT(num, fileContent);
      delay(100); // Add a small delay to avoid overloading WebSocket
    }
    file.close();
  } else {
    // File not found, send error message
    String errorMessage = "File not found: " + filePath;
    webSocket.sendTXT(num, errorMessage);
  }
}
String getContentType(String filename) {
  if (filename.endsWith(".htm") || filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/pdf";
  else if (filename.endsWith(".zip")) return "application/zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "application/octet-stream";
}