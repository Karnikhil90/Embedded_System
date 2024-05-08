/*
Based on the provided .ino file, it seems you're attempting to build a basic 
computer terminal using an ESP8266 microcontroller and an SD card module. Here's a
 breakdown of what you're trying to accomplish and an explanation of the functions included:

1. Objective: Build a computer terminal capable of interacting with an SD card, allowing
   sers to perform basic file system operations like navigating directories, creating files,
   reading files, and deleting files.

2. Included Functions:

Key functions include:

*   setup(): Initializes serial communication and SD card setup, displaying root directory contents.
*   sdCardSetup(): Checks if the SD card is connected.
        Utility functions for string manipulation.
*   File system operations: ShowRootDir(), openDir(), readFile(), createFile(), revmdir().
*   cmd(): Parses and executes user commands received via serial input.
*   loop(): Continuously processes user input to maintain interactivity.
*   WifiHostpot() & Wifi() : Is there to connect to wifi.

Overall, this code provides a foundation for building a simple computer terminal interface 
with file system capabilities using an ESP8266 microcontroller and an SD card module. With 
further development, additional features and functionalities can be added to enhance its usability
 and versatility.
 
        Created by: Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]
  Today I did start this! :) {07-05-2024}
  Today I did 2nd commit this! :) {08-05-2024} : "add a lot of command features "


*/

#include <SD.h>
#include <ESP8266WiFi.h> // Include the ESP8266WiFi library for ESP8266
#include <NTPClient.h>
#include <WiFiUdp.h>

// System Files
const char* IGNORED_FOLDERS[] = {".delete", ".bin", "System Volume Information", ".boot"};

bool wifiEnabled = false; // Initially, WiFi is disabled
bool haveInternet = false; // Checking if internet is available

const char* SSID = "*"; // Default values
const char* PASS = "*"; // Default values

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // Universal Time -> IST(5:30)

void setup() {
  Serial.begin(9600);
  Serial.println("ESP is started....");
  delay(1000);
  sdCardSetup();
  delay(1000);
  ShowRootDir();
  time();
}

void wifi(const char* name, const char* pass) {
  if (strcmp(SSID, "*") != 0 && strcmp(PASS, "*") != 0) {
    Serial.printf("Connecting to %s...\n", name);
    WiFi.begin(name, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Check network connectivity
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Device is connected to the network");
      haveInternet = true;
    } else {
      Serial.println("Device is not connected to the network");
    }
    
  }
  else 
    Serial.println("ErrorType[V001]: The ssid and pass are unavailable");
} 

void WifiHostpot(const char* ssid, const char* password) {
  Serial.printf("Creating Wi-Fi hotspot: %s\n", ssid);
  WiFi.softAP(ssid, password);
  Serial.println("Wi-Fi hotspot created");
  Serial.print("IP address of the hotspot: ");
  Serial.println(WiFi.softAPIP());
  if (strcmp(password, "") == 0)
    Serial.println("No Security On The Network");
}

void sdCardSetup() {
  if (SD.begin(D8)) {
    Serial.println("Card Connected Successfully");
  } else {
    Serial.println("ErrorType: SD card or SD card module not found");
  }
}

String removeQuotes(const String& str) {
  String result;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '"') {
      result += str[i];
    }
  }
  return result;
}

String* split(const String& str, char delimiter) {
  int length = str.length();
  int count = 0;
  int startIndex = -1;
  bool inQuotes = false; // Flag to track whether inside quotes or not

  // Count the number of substrings (parts)
  for (int i = 0; i < length; i++) {
    if (str.charAt(i) == '"') {
      inQuotes = !inQuotes; // Toggle inQuotes flag when encountering a quotation mark
    } else if (!inQuotes && str.charAt(i) == delimiter) {
      count++;
    }
  }

  // Allocate memory for the array of substrings
  String* parts = new String[count + 1]; // Plus one for the last part

  int partIndex = 0;
  inQuotes = false; // Reset inQuotes flag for splitting

  // Find the delimiter and split the string
  for (int i = 0; i < length; i++) {
    if (str.charAt(i) == '"') {
      inQuotes = !inQuotes; // Toggle inQuotes flag when encountering a quotation mark
    } else if (!inQuotes && str.charAt(i) == delimiter) {
      parts[partIndex++] = removeQuotes(str.substring(startIndex + 1, i)); // Remove quotes from substring
      startIndex = i;
    }
  }

  // Add the last part
  if (startIndex != length - 1) {
    parts[partIndex++] = removeQuotes(str.substring(startIndex + 1)); // Remove quotes from substring
  }

  // Return the array of substrings
  return parts;
}

bool isIgnoredFolder(const char* folderName) {
  for (int i = 0; i < sizeof(IGNORED_FOLDERS) / sizeof(IGNORED_FOLDERS[0]); ++i) {
    if (strcmp(folderName, IGNORED_FOLDERS[i]) == 0) return true;
  }
  return false;
}

void listFiles(const char* path) {
  File dir = SD.open(path);
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    Serial.print("    ");
    if (entry.isDirectory()) {
      Serial.print("/ ");
    }
    Serial.println(entry.name());
    entry.close();
  }
  dir.close();
}

void ShowRootDir() {
  Serial.println("===================================================================");
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    if (isIgnoredFolder(entry.name())) continue;
    if (entry.isDirectory()) {
      Serial.print("/");
      Serial.println(entry.name());
      listFiles(entry.name());
    } else {
      Serial.print("-- ");
      Serial.println(entry.name());
    }
    entry.close();
  }
  root.close();
}

void openDir(const char* path) {
  Serial.printf("===================================================================\n");
  File root = SD.open(path);
  while (true) {
    File entry = root.openNextFile();
    if (!entry)
      break;

    bool skip = false;
    for (const char* keyword : IGNORED_FOLDERS) {
      if (strstr(entry.name(), keyword)) {
        skip = true;
        break;
      }
    }
    
    if (skip){
      skip = false;
      continue;
    }

    if (entry.isDirectory()) {
      Serial.print("/");
      Serial.println(entry.name());
    } else {
      Serial.print("  FILE: ");
      Serial.println(entry.name());
    }
    
    entry.close();
  }
  root.close();
}

String extractFromEnd(const String& str, char delimiter) {
    int lastIndex = str.lastIndexOf(delimiter);
    if (lastIndex != -1) { // If delimiter is found
        return str.substring(lastIndex); // Extract substring from lastIndex to the end
    } else {
        return ""; // Return empty string if delimiter is not found
    }
}

void readFile(const char* filename) {
  // Open the file in read mode
  File fileObj = SD.open(filename, FILE_READ);
  
  // Check if the file is opened successfully
  if (fileObj) {
    Serial.printf("\nContents of '%s':\n", filename);
    Serial.println("==============================================================================================\n\n");

    // Read and print the contents of the file
    while (fileObj.available()) {
      Serial.write(fileObj.read());
    }

    // Close the file
    fileObj.close();

    Serial.println("\n\n==============================================================================================");
  } else {
    // Print an error message if the file doesn't exist
    Serial.printf("\nThe '%s' doesn't exist.\n", filename);
  }
}

int8_t createFile(const char* filename, const char* content = "") {
  File file = SD.open(filename, FILE_WRITE); // Open the file in write mode

  if (file) {
    file.println(content); // Write content to the file
    file.close(); // Close the file
    return 0;
  } else {
    return 1;
  }
}

bool revmdir(const char* path) {
  if (!SD.exists(path)) {
    Serial.println("Directory does not exist.");
    return false;
  }

  // Create the .delete folder if it doesn't exist
  if (!SD.exists("/.delete")) {
    SD.mkdir("/.delete");
  }

  // Generate a unique file name within the .delete folder
  String newDirName = "/.delete/" + String(path) + "_ESPDELETE";

  // Rename the directory to move it to .delete folder
  if (SD.rename(path, newDirName.c_str())) {
    return true;
  } else {
    Serial.println("Failed to move directory to .delete folder.");
    return false;
  }
}

void cmd(const char* received) {
  String command = received;
  command.trim(); // Remove leading and trailing whitespaces
  
  // Split the command into parts
  String* parts = split(command, ' '); 

  // Extract individual commands
  String primaryCommand = parts[0];
  String secondaryCommand = (parts->length() > 1) ? parts[1] : "";
  String tertiaryCommand = (parts->length() > 2) ? parts[2] : "";
  String quaternaryCommand = (parts->length() > 3) ? parts[3] : "";

  Serial.printf("Debug: Primary Command: %s, Secondary Command: %s, Tertiary Command: %s\n", 
                 primaryCommand.c_str(), secondaryCommand.c_str(), tertiaryCommand.c_str());

  if (primaryCommand.startsWith("cd")) {
    openDir(secondaryCommand.c_str());
  } else if (primaryCommand.startsWith("open")) {
    readFile(secondaryCommand.c_str());
  } else if (primaryCommand.startsWith("mkdir")) {
    if (SD.mkdir(secondaryCommand.c_str())) {
      Serial.printf("'%s' Created Successfully\n", extractFromEnd(secondaryCommand, '/').c_str());
    } else {
      Serial.printf("'%s' Creation Failed\n", extractFromEnd(secondaryCommand, '/').c_str());
    }
  } else if (primaryCommand.startsWith("root")) {
    ShowRootDir();
  } else if (primaryCommand.startsWith("mkfl")) {
      int8_t result = -1;
      if (tertiaryCommand.isEmpty() || tertiaryCommand == " " || tertiaryCommand == "*") {
        result = createFile(secondaryCommand.c_str());
      } else {
        result = createFile(secondaryCommand.c_str(), tertiaryCommand.c_str());
      }
      if (result == 0) {
        Serial.println("File has been created");
      } else {
        Serial.println("File creation failed");
      }
  } else if (primaryCommand.startsWith("wrt")) {
    if (createFile(secondaryCommand.c_str(), tertiaryCommand.c_str()) != 0) {
      Serial.println("Failed to write to file");
    }
  } else if (primaryCommand.startsWith("rmdir")) {
    if (revmdir(secondaryCommand.c_str())) {
      Serial.println("Directory has been deleted");
    } else {
      Serial.println("Failed to delete directory");
    }
  } else if (primaryCommand.startsWith("set")) {
    if (primaryCommand.equals("D0")) {
      if (tertiaryCommand.equalsIgnoreCase("high")) 
        digitalWrite(D0, HIGH);
      else if (tertiaryCommand.equalsIgnoreCase("low"))
        digitalWrite(D0, LOW);
      else if (tertiaryCommand.equalsIgnoreCase("toggle"))
        digitalWrite(D0, !digitalRead(D0));
      else 
        Serial.println("ErrorType : Invalid position for the GPIO PIN");
    } else if (primaryCommand.equals("D1")) {
      if (tertiaryCommand.equalsIgnoreCase("high")) 
        digitalWrite(D1, HIGH);
      else if (tertiaryCommand.equalsIgnoreCase("low"))
        digitalWrite(D1, LOW);
      else if (tertiaryCommand.equalsIgnoreCase("toggle"))
        digitalWrite(D1, !digitalRead(D1));
      else 
        Serial.println("ErrorType : Invalid position for the GPIO PIN");
    } else if (primaryCommand.equals("D2")) {
      if (tertiaryCommand.equalsIgnoreCase("high")) 
        digitalWrite(D2, HIGH);
      else if (tertiaryCommand.equalsIgnoreCase("low"))
        digitalWrite(D2, LOW);
      else if (tertiaryCommand.equalsIgnoreCase("toggle"))
        digitalWrite(D2, !digitalRead(D2));
      else 
        Serial.println("ErrorType : Invalid position for the GPIO PIN");
    } else if (primaryCommand.equals("D3")) {
      if (tertiaryCommand.equalsIgnoreCase("high")) 
        digitalWrite(D3, HIGH);
      else if (tertiaryCommand.equalsIgnoreCase("low"))
        digitalWrite(D3, LOW);
      else if (tertiaryCommand.equalsIgnoreCase("toggle"))
        digitalWrite(D3, !digitalRead(D3));
      else 
        Serial.println("ErrorType : Invalid position for the GPIO PIN");
    } else {
      Serial.println("ErrorType : Invalid GPIO PIN. Only these pins are allowed to be changed: \n\t- D0 \n\t- D1 \n\t- D2 \n\t- D3");
    }
  } else if (primaryCommand == "wifi") {
    if (secondaryCommand == "on") {
      if (!wifiEnabled) {
        wifiEnabled = true;
        Serial.println("Wi-Fi turned on");
      } else {
        Serial.println("Wi-Fi is already on");
      }
    } else if (secondaryCommand == "off") {
      if (wifiEnabled) {
        WiFi.disconnect();
        wifiEnabled = false;
        haveInternet = false;
        Serial.println("Wi-Fi turned off");
      } else {
        Serial.println("Wi-Fi is already off");
      }
    } else if (secondaryCommand.equals("scan")) {
      int networkCount = WiFi.scanNetworks();
      Serial.printf("%d networks found:\n", networkCount);
      for (int i = 0; i < networkCount; ++i) {
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID(i));
        Serial.print("Signal strength (RSSI): ");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm");
        Serial.print("Requires password: ");
        if (WiFi.encryptionType(i) == AUTH_OPEN) {
          Serial.println("No"); // Network does not require authentication
        } else {
          Serial.println("Yes"); // Network requires authentication
        }
        Serial.println();
      }
    } else if (secondaryCommand == "connect" && tertiaryCommand != "" && quaternaryCommand != "") {
      if (!wifiEnabled) {
        Serial.println("Please turn on Wi-Fi before connecting to a network");
      } else {
        SSID = tertiaryCommand.c_str();
        PASS = quaternaryCommand.c_str();
        wifi(SSID, PASS); // Pass both name and password
      }
    } else if (secondaryCommand.equals("rssi")) {
      int32_t rssi = WiFi.RSSI();
      Serial.printf("Signal strength (RSSI): %d dBm\n", rssi);
    } else if (secondaryCommand.equals("host")) {
      if (tertiaryCommand.equals("off")) {
        if (WiFi.softAPgetStationNum() > 0) {
          WiFi.softAPdisconnect();
          Serial.println("Wi-Fi hotspot turned off");
        } else {
          Serial.println("Wi-Fi hotspot is already off");
        }
      } else if (tertiaryCommand.equals("on")) {
        WifiHostpot("ESP8266_Network", "");
      } else {
        Serial.println("ErrorType: Invalid Command for 'On' or 'Off' position");
      }
    } else {
      Serial.println("ErrorType: Invalid or incomplete For 'wifi' command");
    }
  } else {
    Serial.println("ErrorType: Command not recognized");
  }

  // Clean up memory
  delete[] parts;
}

void time() {
  timeClient.update(); // Update the timeClient
  Serial.printf("-- Now Time > [%02d:%02d:%02d] \n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
}

void loop() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n'); // Read until newline
    userInput.trim(); // Remove leading and trailing whitespaces

    if (userInput.length() > 0) {
      if (userInput.equals("reboot")) { // Use equals() to compare strings
        Serial.println("Rebooting..........");
        delay(1000);
        ESP.restart();
      } else if (userInput.equals("show heap")) {
        size_t freeHeap = ESP.getFreeHeap();
        Serial.print("Free Heap Memory: ");
        Serial.print(freeHeap);
        Serial.println(" bytes");
      } else if (userInput.equals("show time")) {
        if(haveInternet) time(); 
        else Serial.println("ErrorType[N001] : No Internet ");
      } else {
        cmd(userInput.c_str());
      }
    }
  }
}
