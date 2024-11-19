#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
 
const char*   ssid = "Airtel_Zeus";
const char*   password = "TheBestWifi";
AsyncWebServer server(80);

#include "html.h"

// --------------------- root page ---------------------
void handleRootPage(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", index_html);
}

// --------------------- canvas management ---------------------
void handleCanvasBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {

  if (index == 0) Serial.printf("\nIncoming PNG: %u bytes\n", total); // FIRST CALL

  // step 2 the canvas body has been converted to PNG and is received as a binary byte stream,
  // multiple calls possible
  // (use memcpy into a global buffer if you want to save it, using index and len to write the bytes in the right place)
  // here we just dump the hex representation to the serial monitor which you could copy & paste into https://tomeko.net/online_tools/hex_to_file.php to get the file
  for (size_t i = 0; i < len; i++) Serial.printf("%02x", data[i]);

  if (index + len == total) Serial.printf("\nPNG transfer complete (%u bytes)\n", total); // END CALL, WE ARE DONE
}

void handleCanvasResponse(AsyncWebServerRequest * request) {
  Serial.println("End of PNG request.");
  request->send(200);
}

// --------------------- file upload management ---------------------


void handleFileUpload(AsyncWebServerRequest * request, String filename, size_t index, uint8_t *data, size_t len, bool final) {

  if (index == 0) Serial.printf("\nReceiving file %s\n", filename.c_str()); // FIRST CALL

  // step 2 the file is getting received as a raw byte stream,
  // multiple calls possible
  // (use memcpy into a global buffer if you want to save it, using index and len to write the bytes in the right place)
  // here we just dump the hex representation to the serial monitor which you could copy & paste into https://tomeko.net/online_tools/hex_to_file.php to get the file
  // for (size_t i = 0; i < len; i++) Serial.printf("%02x", data[i]);

  //alternatively if the file is a text file, you can just dump the bytes and they will be interpretted by the Serial monitor
  for (size_t i = 0; i < len; i++) Serial.write(data[i]);

  if (final) Serial.printf("\nFile %s, transfer complete (%u bytes)\n", filename.c_str(), index + len); // END CALL, WE ARE DONE
}

void handleFileResponse(AsyncWebServerRequest * request) {
  Serial.println("End of File request.");
  request->send(200);
}

// --------------------- Text upload management ---------------------

void handleTextBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {

  if (index == 0) Serial.printf("\nText Body: %u bytes\n", total); // FIRST CALL

  // step 2 the textfield content is received as a binary byte stream,
  // multiple calls possible
  // (use memcpy into a global buffer if you want to save it, using index and len to write the bytes in the right place)
  // here we just dump the binary representation to the serial monitor. The serial monitors handle UTF8 so all should be fine
  for (size_t i = 0; i < len; i++) Serial.write(data[i]);

  if (index + len == total) Serial.printf("\nText transfer complete (%u bytes)\n", total); // END CALL, WE ARE DONE
}

void handleTextResponse(AsyncWebServerRequest * request) {
  Serial.println("End of Text request.");
  request->send(200);
}

// ---------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    while (true) yield();
  }
  Serial.println("Connected to WiFi");

  Serial.print("Open URL http://"); Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRootPage);
  server.on("/uploadFile",   HTTP_POST, handleFileResponse, handleFileUpload);
  server.on("/uploadCanvas", HTTP_POST, handleCanvasResponse, nullptr, handleCanvasBody);
  server.on("/uploadText",   HTTP_POST, handleTextResponse, nullptr, handleTextBody);

  server.begin();
}

void loop() {}