#include <SD.h>
#include <SPI.h>

#define SD_CS 5    // Chip Select pin
#define SPI_SCK 18 // SPI Clock (SCK)
#define SPI_MOSI 23 // Master Out Slave In (MOSI)
#define SPI_MISO 19 // Master In Slave Out (MISO)

// Function to display SD card details
void SD_info() {
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.println("SD Card Info:");

  // Card Type
  Serial.print("Card Type: ");
  switch (cardType) {
    case CARD_MMC:
      Serial.println("MMC");
      break;
    case CARD_SD:
      Serial.println("SDSC (Standard Capacity)");
      break;
    case CARD_SDHC:
      Serial.println("SDHC (High Capacity)");
      break;
    default:
      Serial.println("Unknown");
      break;
  }

  // Card Size
  uint64_t cardSizeMB = SD.cardSize() / (1024 * 1024); // Size in MB
  float cardSizeGB = (float)cardSizeMB / 1024;         // Size in GB

  Serial.print("Card Size: ");
  if (cardSizeMB > 10 * 1024) { // If size is greater than 10 GB
    Serial.print(cardSizeGB, 2); // Print in GB with 2 decimal points
    Serial.println(" GB");
  } else {
    Serial.print(cardSizeMB);
    Serial.println(" MB");
  }

  // Card Volume
  uint64_t totalBytesMB = SD.totalBytes() / (1024 * 1024); // Size in MB
  float totalBytesGB = (float)totalBytesMB / 1024;         // Size in GB

  uint64_t usedBytesMB = SD.usedBytes() / (1024 * 1024);   // Size in MB
  float usedBytesGB = (float)usedBytesMB / 1024;           // Size in GB

  Serial.print("Total Space: ");
  if (totalBytesMB > 10 * 1024) {
    Serial.print(totalBytesGB, 2);
    Serial.println(" GB");
  } else {
    Serial.print(totalBytesMB);
    Serial.println(" MB");
  }

  Serial.print("Used Space: ");
  if (usedBytesMB > 10 * 1024) {
    Serial.print(usedBytesGB, 2);
    Serial.println(" GB");
  } else {
    Serial.print(usedBytesMB);
    Serial.println(" MB");
  }

  Serial.print("Free Space: ");
  if (totalBytesMB > 10 * 1024) {
    Serial.print(totalBytesGB - usedBytesGB, 2);
    Serial.println(" GB");
  } else {
    Serial.print(totalBytesMB - usedBytesMB);
    Serial.println(" MB");
  }

  // Card Name and File Structure
  Serial.println("Files on SD Card:");
  ls("/");
}


void ls(const char *path) {
  File entry = SD.open(path);

  if (!entry) {
    Serial.println("Failed to open path");
    return;
  }

  if (entry.isDirectory()) {
   Serial.printf("/");
   Serial.printf(entry.name());
  } else {
    // If it’s a file, print its name, extension, and size
    Serial.print(entry.name());
    Serial.print("\t");  // Tab for spacing
    Serial.print(entry.size());
    Serial.println(" bytes");
  }

  entry.close();
}

void setup() {
  Serial.begin(115200);

  // Display SD card info
  SD_info();
}

void loop() {
  // Nothing to do here
}
