//This example acts like a websever. It can create an access point or join an existing WiFI network.
//All text that's entered in the served page will bis displayed on the connected VGA screen.
//You need to connect a VGA screen cable to the pins specified below.
//cc by-sa 4.0 license
//bitluni

#include <stdio.h>
#include <WiFi.h>
#include <WebServer.h>
//ESP32Lib video headers
#include <ESP32Video.h>
#include <Ressources/Font6x8.h>

// my changes for Future changes 
#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


#define DEVICE_NAME "ESP32 Generic Dev Module"

char USER_DEVICE_NAME* = "Nikhil'S_ESP32";


bool isBluetoothOn = false;
//true: creates an access point, false: connects to an existing wifi
const bool AccessPointMode = false;
//wifi credentials (enter yours if you arne not using the AccessPointMode)
const char *ssid = "Airtel_Zeus";
const char *password = "TheBestWifi";

//pin configuration, change if you need to
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

//the webserver at pot 80
WebServer server(80);

//The VGA Device
VGA3Bit videodisplay;

//include html page
const char *page =
#include "page.h"
	;

///Html page is sent on root
void sendPage()
{
	server.send(200, "text/html", page);
}

///Received text will be displayed on the screen
void text()
{
	server.send(200, "text/plain", "ok");
	videodisplay.println(server.arg(0).c_str());
}

void clearScreen(){
  videodisplay.clear(videodisplay.RGBA(0, 0, 255));
	videodisplay.backColor = videodisplay.RGB(0, 0, 255);
  videodisplay.clear(videodisplay.RGBA(0, 0, 255));
  videodisplay.setCursor(0, 0);
}

void bluetoothOn() {
    if (!isBluetoothOn) {
        SerialBT.begin(USER_DEVICE_NAME);
        BLEDevice::init("");
        pBLEScan = BLEDevice::getScan(); // create a scanner
        pBLEScan->setActiveScan(true);   // active scan uses more power, but gets results faster
        isBluetoothOn = true;
        Serial.println("Bluetooth is ON");
    } else {
        Serial.println("Bluetooth is already ON");
    }
}

void bluetoothOff() {
    if (isBluetoothOn) {
        SerialBT.end();
        BLEDevice::deinit();
        isBluetoothOn = false;
        Serial.println("Bluetooth is OFF");
    } else {
        Serial.println("Bluetooth is already OFF");
    }
}

String bluetoothScan() {
    String scanResults = "";

    if (isBluetoothOn) {
        scanResults += "Scanning for nearby Bluetooth devices...\n";
        BLEScanResults foundDevices = pBLEScan->start(5); // scan for 5 seconds
        scanResults += "Devices found: " + String(foundDevices.getCount()) + "\n";
        
        for (int i = 0; i < foundDevices.getCount(); i++) {
            BLEAdvertisedDevice device = foundDevices.getDevice(i);
            scanResults += "Device " + String(i + 1) + ": ";
            scanResults += device.toString().c_str();
            scanResults += "\n";
        }
        pBLEScan->clearResults(); // clear the results to free memory
    } else {
        scanResults = "Please turn on Bluetooth first.\n";
    }

    return scanResults;
}

// Function to split a string by a specified delimiter
void split(String data, char delimiter, String* resultArray, int maxParts) {
    int index = 0;
    int start = 0;
    int end = data.indexOf(delimiter);
  
    while (end != -1 && index < maxParts - 1) {
        resultArray[index++] = data.substring(start, end);
        start = end + 1;
        end = data.indexOf(delimiter, start);
    }
    resultArray[index] = data.substring(start);
}

void handleCommand() {
    String cmd = server.arg("cmd"); // Get the 'cmd' parameter from the URL

    // Display the command on the screen
    videodisplay.print("> ");
    videodisplay.println(cmd.c_str());
    delay(300);

    // Define an array to store command parts (assuming max 5 parts)
    const int maxParts = 5;
    String cmd_array[maxParts];

    // Split the command string into parts
    split(cmd, ' ', cmd_array, maxParts);

    // Print the breakdown of the command parts
    videodisplay.println("Command breakdown:");
    for (int i = 0; i < maxParts; i++) {
        if (cmd_array[i].length() > 0) {  // Check if the part is not empty
            videodisplay.print("Part ");
            videodisplay.print(i);
            videodisplay.print(": ");
            videodisplay.println(cmd_array[i].c_str());
        }
    }

    // Process commands based on cmd_array contents
    if (cmd_array[0] == "clear") {
        clearScreen();
        server.send(200, "text/plain", "Screen cleared.");
    } 
    else if (cmd_array[0] == "bluetooth") {
        if (cmd_array[1] == "on") {
            bluetoothOn();
        } else if (cmd_array[1] == "off") {
            bluetoothOff();
        } else if (cmd_array[1] == "scan") {
            String result = bluetoothScan();
            videodisplay.println(result.c_str());
        }
    } 
    else if (cmd_array[0] == "reboot") {
        videodisplay.println("ESP32 is rebooting.....");
        for (int i = 5; i >= 0; i--) {
            videodisplay.println(String(i).c_str());
        }
        delay(100);
        ESP.restart();
    }
    else if (cmd_array[0] == "set") {
        // This handles the "set" command with further breakdown
        if (cmd_array[1] == "background") {
            // Check for RGB values in the command
            if (cmd_array[2].length() > 0 && cmd_array[3].length() > 0 && cmd_array[4].length() > 0) {
                int red = cmd_array[2].toInt();
                int green = cmd_array[3].toInt();
                int blue = cmd_array[4].toInt();
                // Set background color with the RGB values
                setBackgroundColor(red, green, blue);
                server.send(200, "text/plain", "Background color set.");
            } else {
                server.send(400, "text/plain", "Invalid color values.");
            }
        } else {
            // Handle other "set" subcommands if needed
            server.send(200, "text/plain", "Unknown 'set' command option.");
        }
    } 
    else {
        server.send(200, "text/plain", "Unknown command.");
    }
}


void setBG(int R = 255 , G = 255 , B = 255){
  videodisplay.clear(videodisplay.RGBA(R, G, B));
	videodisplay.backColor = videodisplay.RGB(R, G, B);
}


///initialization
void setup()
{
	Serial.begin(115200);
	//Handle the WiFi AP or STA mode and display results on the screen
	if (AccessPointMode)
	{
		Serial.println("Creating access point...");
		WiFi.softAP(ssid, password, 6, 0);
	}
	else
	{
		Serial.print("Connecting to SSID ");
		Serial.println(ssid);
		WiFi.begin(ssid, password);
		while (WiFi.status() != WL_CONNECTED)
		{
			delay(500);
			videodisplay.print(".");
		}
	}
	//start vga on the specified pins
	videodisplay.init(VGAMode::MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	//make the background blue
	videodisplay.clear(videodisplay.RGBA(0, 0, 255));
	videodisplay.backColor = videodisplay.RGB(0, 0, 255);
	//select the font
	videodisplay.setFont(Font6x8);

	//send page on http://<ip>/
	server.on("/", sendPage);
	//receive text on http://<ip>/text
	server.on("/text", text);
  server.on("/command", handleCommand);
	//start the server
	server.begin();

	//display some text header on the screen including the ip
	videodisplay.clear(videodisplay.RGBA(0, 0, 255));
	videodisplay.setCursor(0, 0);
	videodisplay.println("----------------------");
  videodisplay.println("Nikhil's OS");
	videodisplay.print("http://");
	videodisplay.println(WiFi.localIP().toString().c_str());
  videodisplay.println("----------------------");
}

void loop()
{
	//process the server stuff
	server.handleClient();
	delay(10);
}
