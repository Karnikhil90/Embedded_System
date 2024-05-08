
---

# ESP8266 Command Line Interface (CLI) with SD Card Support

This project enables you to build a basic command line interface (CLI) using an ESP8266 microcontroller and an SD card module. With this CLI, you can interact with the SD card, performing file system operations such as navigating directories, creating, reading, and deleting files.

## Features

- **File System Operations**: Perform basic file system tasks like navigating directories, creating, reading, and deleting files.
- **Interactive Command Line Interface**: Use a serial terminal to input commands and receive feedback from the ESP8266.
- **SD Card Support**: Interact with an SD card connected to the ESP8266 for storing and retrieving files.

Certainly! Here's the updated section with the title "Internal Functions" and the provided information under it:

---

### Internal Functions

#### Deleting Files/Directories

Use the `rmdir` command followed by a directory name to delete a directory and its contents. Note: Deleted directories are moved to a `.delete` folder on the SD card.

#### Creating Files

Use the `mkfl` command followed by the file name to create a new file. Optionally, provide content using the syntax `mkfl filename "file content"`.


---

Let me know if this looks good or if there's anything else you'd like to adjust!

## How to Use

### Hardware Requirements

- ESP8266 microcontroller (e.g., NodeMCU)
- SD card module
- USB cable for programming and power supply

### Software Setup

1. **Install Arduino IDE**: Download and install the Arduino IDE from [Arduino's official website](https://www.arduino.cc/en/software).
   
2. **Install ESP8266 Board Manager**: Follow the instructions [here](https://github.com/esp8266/Arduino#installing-with-boards-manager) to install the ESP8266 board manager in Arduino IDE.

3. **Install SD Card Library**: In Arduino IDE, go to `Sketch -> Include Library -> Manage Libraries...`. Search for "SD" and install the "SD" library by Adafruit.

4. **Upload Code to ESP8266**: Open the provided `.ino` file in Arduino IDE. Connect your ESP8266 board to your computer via USB, select the appropriate board and port from the `Tools` menu, and click the "Upload" button to upload the code to the board.

   - Alternatively, you can clone the repository from [GitHub](https://github.com/Karnikhil90/Embedded-System-.git) by running the following command in your terminal or command prompt:
     ```
     git clone https://github.com/Karnikhil90/Embedded-System-.git
     ```
     Then, navigate to the "ESP8266/TermComp" directory to find the `main.ino` file.

Certainly! Here's the updated "Using the Command Line Interface" section without the "example" directory:

---
Certainly! Here's the updated section with a division for WiFi-related commands and system data display commands:

---

### Using the Command Line Interface

1. **Connecting to Serial Terminal**: After uploading the code, open the serial monitor in Arduino IDE (`Tools -> Serial Monitor`). Ensure the baud rate is set to `9600`.

2. **Command List**:
   - **cd**: Change directory. Syntax: `cd <directory_name>`.
   - **mkdir**: Create a new directory. Syntax: `mkdir <directory_name>`.
   - **open**: Open a file. Syntax: `open <file_name>`.
   - **root**: Display contents of the root directory.
   - **mkfl**: Create a new file. Syntax: `mkfl <file_name> `.
   - **wrt**: Write content to a file. Syntax: `wrt <file_name> [content]`.
   - **rmdir**: Delete a directory and its contents. Syntax: `rmdir <directory_name>`.
   
#### WiFi Commands:
   - **wifi \<on/off>**: Enable or disable WiFi.
   - **wifi show**: Show current WiFi status.
   - **wifi connect \<ssid> \<password>**: Connect to a WiFi network with the specified SSID and password.
   - **wifi scan**: Scan for available WiFi networks.
   - **wifi host \<on/off>**: Enable or disable WiFi hotspot mode.
   - **wifi rssi**: Display WiFi signal strength (RSSI).
#### System Commands:
   - **reboot**: Reboot the ESP8266.
   - **show heap**: Show heap memory usage.
   - **show time**: Display current time.
   - **set <GPIO_pin_name> <high/low/toggle>**: Set GPIO pin to high, low, or toggle its state.
---

Let me know if this meets your requirements or if you need further adjustments!
---

Feel free to let me know if you need further adjustments!

## Additional Notes

- Ensure the connections between the ESP8266, SD card module, and any other components are correct before uploading the code.
- Be cautious when performing file system operations, as they may have permanent effects on the SD card's contents.


## Author

This code was created by Nikhil Karmakar. You can find him on:

[![Email](https://img.shields.io/badge/Email-nikhilbroo%40hotmail.com-red)](mailto:nikhilbroo@hotmail.com)
[![Twitter](https://img.shields.io/badge/Twitter-%40karnikhil90-43A047 )](https://twitter.com/karnikhil90)
<br>
[![GitHub](https://img.shields.io/badge/GitHub-%40Karnikhil90-A322D7)](https://github.com/Karnikhil90)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Nikhil%20Karmakar-03A9F4 )](https://www.linkedin.com/in/karnikhil90/)
