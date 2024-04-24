# ESP8266 Web Server with WebSocket

This repository contains code for setting up a web-connected project using an ESP8266 microcontroller. The project includes functionalities commonly used in IoT (Internet of Things) applications, such as connecting to Wi-Fi, handling WebSocket communication, serving files from an SD card, synchronizing time via NTP, and acting as a web server.

## How it Works

### Connecting to Wi-Fi

The device joins a specified Wi-Fi network, enabling communication with other devices and services online.

### Handling WebSockets

The system establishes real-time communication channels between the device and web browsers, allowing updates and interactions to occur instantly without requiring page refreshes.

### SD Card Operations

Files can be read from and served from an SD card, facilitating tasks like hosting web pages or sharing resources.

### NTP Time Synchronization

The device's clock remains accurate by synchronizing with an online time server, ensuring that events and data are timestamped correctly.

### ESP8266 Web Server

The device acts as a mini website, serving web pages and responding to requests from browsers or other devices.

## Author

This code was created by Nikhil Karmakar. You can find him on:

[![Email](https://img.shields.io/badge/Email-nikhilbroo%40hotmail.com-red)](mailto:nikhilbroo@hotmail.com)
[![Twitter](https://img.shields.io/badge/Twitter-%40karnikhil-blue)](https://twitter.com/karnikhil90)
<br>
[![GitHub](https://img.shields.io/badge/GitHub-%40Karnikhil90-green)](https://github.com/Karnikhil90)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Nikhil%20Karmakar-blue)](https://www.linkedin.com/in/karnikhil90/)

## Note

This code is designed for educational purposes and can be extended for various IoT applications.

## Known Issues

The current implementation may encounter stack overflow errors when handling multiple users, especially more than two, on the ESP8266 web server. Efforts are ongoing to address this issue.
