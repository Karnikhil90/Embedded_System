#include <SPI.h>
#include <SD.h>

#define CS_PIN 5
#define SCK_PIN 18
#define MOSI_PIN 23
#define MISO_PIN 19

File myFile;

void setup() {
  Serial.begin(115200);

  // Initialize SPI with custom pins
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  
  // Initialize SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Write to file
  myFile = SD.open("/example.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("Hello from ESP32!");
    myFile.close();
    Serial.println("Data written to example.txt");
  } else {
    Serial.println("Failed to open file for writing.");
  }

}

void loop() {
  // Do nothing here
}