# IoT LED Control System with SD Card Logging

This Arduino code is designed for an ESP8266-based IoT system that controls LEDs and logs user actions to an SD card.

## Features

- Connects to a specified WiFi network using provided credentials.
- Sets up a web server on port 80 to handle user requests.
- Responds to requests to toggle LEDs (white or red) via a simple web interface.
- Logs user actions and system events, including timestamps and IP addresses, to an SD card.
- Provides a default web page ("/website/index.html") for user interaction.

## Hardware Requirements

- ESP8266 board
- SD card module
- LEDs (checking LED, White LED, Red LED)
- Resistors (as needed for LEDs)
- Wiring based on pin connections specified in the code

## Usage

1. Ensure correct pin connections for LEDs and the SD card module.
2. Set WiFi credentials (SSID and PASS) to connect to the desired network.
3. Customize the "/website/index.html" file for the web interface if needed.

## Getting Started

1. Upload the code to your ESP8266 board.
2. Power up the system and monitor the Serial Monitor for connection status and logs.
3. Open a web browser and navigate to the ESP8266's IP address to access the web interface.

## Customize

- Modify the code and HTML file to suit your specific IoT application.
- Extend the functionality by adding more features or sensors.

## Contact Information

For inquiries, please contact us via:
- Email: [![Email](https://img.shields.io/badge/Email-nikhilbroo%40hotmail.com-blue)](mailto:nikhilbroo@hotmail.com)
- Twitter: [![Twitter](https://img.shields.io/badge/Twitter-%40karnikhil-blue)](https://twitter.com/karnikhil90)
- GitHub: [![GitHub](https://img.shields.io/badge/GitHub-%40Karnikhil90-green)](https://github.com/Karnikhil90)
- LinkedIn: [![LinkedIn](https://img.shields.io/badge/LinkedIn-Nikhil%20Karmakar-blue)](https://www.linkedin.com/in/NikhilKarmakar/)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
