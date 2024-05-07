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

Overall, this code provides a foundation for building a simple computer terminal interface 
with file system capabilities using an ESP8266 microcontroller and an SD card module. With 
further development, additional features and functionalities can be added to enhance its usability
 and versatility.
 
        Created by: Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]
  Today I did this! :) {07-05-2024}


*/


#include <SD.h>

void setup() {
  Serial.begin(9600);
  Serial.println("\nESP is started....");
  delay(1000);
  sdCardSetup();
  delay(1000);
  ShowRootDir();
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

void ShowRootDir() {
  Serial.printf("===================================================================\n");
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    else if (strcmp(entry.name(),"System Volume Information") == 0 || strcmp(entry.name(),".delete") == 0) {
      continue;
    }

    if (entry.isDirectory()) {
      char path[50];
      sprintf(path, "/%s", entry.name());
      Serial.println(path);
      
      File underRoot = SD.open(path);
      File underentry = underRoot.openNextFile();
      while (true) {
        if (!underentry)
          break;
        if (underentry.isDirectory()) {
          sprintf(path, "   /%s - /%s", entry.name(), underentry.name());
          Serial.println(path);
        } else {
          sprintf(path, "   /%s - %s", entry.name(), underentry.name());
          Serial.println(path);
        }
        underentry = underRoot.openNextFile();
      }
    } else {
      Serial.print("  FILE: ");
      Serial.println(entry.name());
    }
    
    entry.close();
  }
  root.close();
}
void openDir(const char* path) {
  const char* cheak[] = {".delete", ".bin","System Volume Information"};
  Serial.printf("===================================================================\n");
  File root = SD.open(path);
  while (true) {
    File entry = root.openNextFile();
    if (!entry) 
      break;

    bool skip = false;
    for (const char* keyword : cheak) {
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
int8_t createFile(const char* filename){
  return createFile(filename," ");
}
int8_t  createFile(const char* filename, const char* content) {
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
void cmd(const char* recived) {
  String* parts = split(recived, ' '); // Use single quotes for character literals
  const char* command = "*";
  const char* command1 = "*";
  const char* command2 = "*";

  if (parts->length() > 0) {
      command = parts[0].c_str(); // Convert String to const char*
  }
  if (parts->length() > 1) {
      command1 = parts[1].c_str(); // Convert String to const char*
  }
  if (parts->length() > 2) {
      command2 = parts[2].c_str(); // Convert String to const char*
  }
  Serial.printf("DeBug %s ? %s  ? %s\n", command, command1,command2);

  if (String(command).startsWith("cd")) { // Convert const char* to String for startsWith()
    openDir(command1);
  } else if (String(command).startsWith("open")) { // Convert const char* to String for startsWith()
    readFile(command1);
  } 
  else if (String(command).startsWith("mkdir")) { // Convert const char* to String for startsWith()
    if (SD.mkdir(command1)) {
      Serial.println("'" + extractFromEnd(command1, '/') + "' Created Successfully");
    } else {
      Serial.println("'" + extractFromEnd(command1, '/') + "' Creating Failed");
    }
  } 
  else if(String(command).startsWith("root")){
    ShowRootDir();
  } 
  else if(String(command).startsWith("mkfl")){
    uint8_t temp = -1;
    if(strcmp(command2," ")== 0|| strcmp(command2,"")==0 ||strcmp(command2,"*")==0)
      temp = createFile(command1);
    else
      createFile(command1,command2);
    
    if(temp == 0)
      Serial.println("File has been created");
    else 
      Serial.println("File Failed to created"); 
  }
  else if(String(command).startsWith("wrt")){
   if(createFile(command1,command2)==1)
    Serial.println(" Failed  to write  ");
  }
  else if(String(command).startsWith("rmdir")){}
   if(revmdir(command1))
    Serial.println("File had been deleted");
  else {
    Serial.println("ErrorType : command doesn't exist");
  }
  command = "*";
  command1 = "*";
  command2 = "*";
}

void loop() {
  if (Serial.available() > 0) {
      String userInput = Serial.readStringUntil('\n');  // Read until newline
      userInput.trim();  // Remove leading and trailing whitespaces

      if (userInput.length() > 0) {
        cmd(userInput.c_str());
      }
    }
}
