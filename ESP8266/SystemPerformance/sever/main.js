/*
* This server code hosts the webpage located at "website/index.html" using sockets.
* It establishes a websocket connection with the ESP8266 to receive system data,
        including heap (available memory), stack usage, and CPU frequency.
* The received data is converted into JSON format and sent to the webpage for real-time display
        and visualization, allowing users to monitor the ESP8266's performance remotely.
* Additionally, the server handles HTTP requests to serve static files and communicates with the
        webpage using websockets for dynamic updates without page reloading.

        Created by: Nikhil Karmakar , Contacts = @Karnikhil90 [Github, Twitter, LinkedIn]

 */

const http = require('http');
const url = require('url');
const fs = require('fs'); // Import the fs module
const WebSocket = require('ws');

const HOST_HTTP = '192.168.1.2'; // Localhost for HTTP server
const PORT_HTTP = 8080; // Port for HTTP server

const HOST_WS = HOST_HTTP; // Localhost for WebSocket server
const PORT_WS = 8081; // Port for WebSocket server

// Create a WebSocket server
const wss = new WebSocket.Server({ host: HOST_WS, port: PORT_WS });

// Function to broadcast data to all connected WebSocket clients
function broadcastData(data) {
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(data));
        }
    });
}

// Event listener for WebSocket server connection
wss.on('connection', function connection(ws) {
    console.log('Client connected');

    // Event listener for receiving messages from clients (optional)
    ws.on('message', function incoming(message) {
        console.log('Received from client:', message);
    });
});

// Create an HTTP server
const server = http.createServer((req, res) => {
    const { pathname, query } = url.parse(req.url, true);
    console.log('Request received:', pathname);
    console.log('Request received:', query);

    if (req.url === '/' || req.url === 'index.html') {
        // Read the index.html file and send it as the response
        fs.readFile('./website/index.html', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Internal Server Error');
            } else {
                res.writeHead(200, { 'Content-Type': 'text/html' });
                res.end(data);
            }
        });
    } else if (req.url === '/favicon.ico') {
        // Ignore requests for favicon.ico
        return;
    } else if (req.method === 'GET') {
        console.log('='.repeat(30));
        console.log('Request received:', pathname);
        console.log('Request received:', query);
        const queryString = JSON.stringify(query);

        // Check if the system query is present
        if (pathname === "/system" &&
            (queryString.search('cpu') !== -1 &&
                queryString.search('heap') !== -1 &&
                queryString.search('stack') !== -1)) {
            // Broadcast the query JSON data to WebSocket clients
            const jsonData = JSON.stringify(query);
            const json = JSON.parse(jsonData);
            broadcastData(json);
            console.log(json);
        }
        // Send back a 200 response
        res.writeHead(200, { 'Content-Type': 'text/plain' });
        res.end('HTTP server response');
    } else {
        // For other HTTP methods, send a 405 Method Not Allowed response
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Method not allowed.');
    }
});

// Start the HTTP server
server.listen(PORT_HTTP, HOST_HTTP, () => {
    console.log(`HTTP server listening on ${HOST_HTTP}:${PORT_HTTP}`);
});

console.log(`WebSocket server listening on ws://${HOST_WS}:${PORT_WS}`);