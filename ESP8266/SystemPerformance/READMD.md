# ESP8266 System Monitor

This project is designed for monitoring system information from an ESP8266 microcontroller board and displaying it on a webpage hosted by a server running on your computer. The system information includes heap (available memory), stack usage, and frequency (clock speed) of the microcontroller.

## Project Structure

The project structure includes the following components:

- **ESP8266 Code:** The code running on the ESP8266 microcontroller board is responsible for gathering system information and transmitting it to the server.

- **Server Code:** The server code is responsible for hosting a webpage and communicating with the ESP8266 using websockets.

- **Website:** The `website` folder contains the files for the webpage hosted by the server. The `index.html` file is the main webpage, and `main.js` contains the server code for hosting the webpage and handling websocket communication.

## How it Works

1. **ESP8266 Code:** The ESP8266 gathers system information periodically.
2. **Server Setup:** Run the server code on your computer. It hosts the webpage and listens for incoming connections from the ESP8266.
3. **Webpage Display:** Access the webpage hosted by the server from any browser. The webpage dynamically updates with the system information received from the ESP8266.
4. **Websocket Communication:** The ESP8266 and server communicate using websockets, allowing real-time transmission of data between them.

## Getting Started

1. **Hardware Setup:** Connect the ESP8266 microcontroller board to your computer and ensure it's connected to your local Wi-Fi network.
2. **ESP8266 Code:** Upload the provided code to the ESP8266 board.
3. **Server Setup:** Navigate to the `server` folder and run the server code.
4. **Access the Webpage:** Open a web browser and navigate to the server's IP address. You should see the webpage displaying the system information from the ESP8266.

## Dependencies

- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino): Required for programming the ESP8266 microcontroller.
- [Socket.IO](https://socket.io/): Used for real-time communication between the ESP8266 and the server.
- [Express.js](https://expressjs.com/): Used for serving the webpage and handling HTTP requests on the server.


## Author

This code was created by Nikhil Karmakar. You can find him on:

[![Email](https://img.shields.io/badge/Email-nikhilbroo%40hotmail.com-red)](mailto:nikhilbroo@hotmail.com)
[![Twitter](https://img.shields.io/badge/Twitter-%40karnikhil-blue)](https://twitter.com/karnikhil90)
<br>
[![GitHub](https://img.shields.io/badge/GitHub-%40Karnikhil90-green)](https://github.com/Karnikhil90)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Nikhil%20Karmakar-blue)](https://www.linkedin.com/in/karnikhil90/)
