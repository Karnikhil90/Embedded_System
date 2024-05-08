<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP8266 Command Line Interface (CLI) with SD Card Support</title>
    <style>
         body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            margin: 20px;
            padding: 20px;
            background-color: #616161; /* Changed background color */
            color: #ffffff; /* Changed text color to white */
        }
        h1 {
            color: #ffffff; /* Changed heading color to white */
        }
        h2 {
            color: #ffffff; /* Changed heading color to white */
        }
        p, ul, ol {
            color: #ffffff; /* Changed text color to white */
        }
        hr {
            border: none;
            border-top: 1px solid #ffffff; /* Changed border color to white */
            margin: 20px 0;
        }
        dl {
            margin-bottom: 20px;
            background-color: black; /* Changed background color of dl element */
            padding: 10px;
            border-radius: 5px;
        }
        dt {
            font-weight: bold;
            color: #ffffff; /* Changed text color to white */
        }
        dd {
            margin-left: 20px;
            color: #ffffff; /* Changed text color to white */
        }
        ul, ol {
             margin-bottom: 20px;
            background-color: black; /* Changed background color of dl element */
            padding: 10px;
            border-radius: 5px;
            list-style-type: disc;
            padding-left: 20px;
            border: 1px solid #ffffff; /* Added border */
            border-radius: 5px;
            margin-bottom: 20px; /* Added margin */
        }
        a {
            color: #ffffff; /* Changed link color to white */
            text-decoration: none;
        }
        a:hover {
            text-decoration: underline;
        }
        .author {
            color: #90CAF9; /* Changed author text color */
        }

    </style>
</head>
<body>

<h1>ESP8266 Command Line Interface (CLI) with SD Card Support</h1>

<p>This project enables you to build a basic command line interface (CLI) using an ESP8266 microcontroller and an SD card module. With this CLI, you can interact with the SD card, performing file system operations such as navigating directories, creating, reading, and deleting files.</p>

<h2>Features</h2>

<ul>
    <li><strong>File System Operations:</strong> Perform basic file system tasks like navigating directories, creating, reading, and deleting files.</li>
    <li><strong>Interactive Command Line Interface:</strong> Use a serial terminal to input commands and receive feedback from the ESP8266.</li>
    <li><strong>SD Card Support:</strong> Interact with an SD card connected to the ESP8266 for storing and retrieving files.</li>
</ul>

<hr>

<h2>Internal Functions</h2>

<p>Here are some internal functions provided by the CLI:</p>

<dl>
    <dt>Deleting Files/Directories</dt>
    <dd>Use the <code>rmdir</code> command followed by a directory name to delete a directory and its contents. Note: Deleted directories are moved to a <code>.delete</code> folder on the SD card.</dd>
    <dt>Creating Files</dt>
    <dd>Use the <code>mkfl</code> command followed by the file name to create a new file. Optionally, provide content using the syntax <code>mkfl filename "file content"</code>.</dd>
</dl>

<hr>

<h2>How to Use</h2>

<h3>Hardware Requirements</h3>

<ul>
    <li>ESP8266 microcontroller (e.g., NodeMCU)</li>
    <li>SD card module</li>
    <li>USB cable for programming and power supply</li>
</ul>

<h3>Software Setup</h3>

<ol>
    <li><strong>Install Arduino IDE:</strong> Download and install the Arduino IDE from <a href="https://www.arduino.cc/en/software">Arduino's official website</a>.</li>
    <li><strong>Install ESP8266 Board Manager:</strong> Follow the instructions <a href="https://github.com/esp8266/Arduino#installing-with-boards-manager">here</a> to install the ESP8266 board manager in Arduino IDE.</li>
    <li><strong>Install SD Card Library:</strong> In Arduino IDE, go to <strong>Sketch &rarr; Include Library &rarr; Manage Libraries...</strong>. Search for "SD" and install the "SD" library by Adafruit.</li>
    <li><strong>Upload Code to ESP8266:</strong> Open the provided <code>.ino</code> file in Arduino IDE. Connect your ESP8266 board to your computer via USB, select the appropriate board and port from the <strong>Tools</strong> menu, and click the "Upload" button to upload the code to the board.</li>
</ol>

<h3>Using the Command Line Interface</h3>

<ol>
    <li><strong>Connecting to Serial Terminal:</strong> After uploading the code, open the serial monitor in Arduino IDE (<strong>Tools &rarr; Serial Monitor</strong>). Ensure the baud rate is set to <code>9600</code>.</li>
    <li><strong>Command List:</strong>
        <ul>
            <li><strong>cd:</strong> Change directory. Syntax: <code>cd &lt;directory_name&gt;</code>.</li>
            <li><strong>mkdir:</strong> Create a new directory. Syntax: <code>mkdir &lt;directory_name&gt;</code>.</li>
            <li><strong>open:</strong> Open a file. Syntax: <code>open &lt;file_name&gt;</code>.</li>
            <li><strong>root:</strong> Display contents of the root directory.</li>
            <li><strong>mkfl:</strong> Create a new file. Syntax: <code>mkfl &lt;file_name&gt; </code>.</li>
            <li><strong>wrt:</strong> Write content to a file. Syntax: <code>wrt &lt;file_name&gt; [content]</code>.</li>
            <li><strong>rmdir:</strong> Delete a directory and its contents. Syntax: <code>rmdir &lt;directory_name&gt;</code>.</li>
            <li><strong>reboot:</strong> Reboot the ESP8266.</li>
        </ul>
    </li>
</ol>

<hr>

<h2>Additional Notes</h2>

<ul>
    <li>Ensure the connections between the ESP8266, SD card module, and any other components are correct before uploading the code.</li>
    <li>Be cautious when performing file system operations, as they may have permanent effects on the SD card's contents.</li>
</ul>

<hr>

<h2>Author</h2>

<p>This code was created by Nikhil Karmakar.</p>

<ul >
    <li><a href="mailto:nikhilbroo@hotmail.com">Email</a>: nikhilbroo@hotmail.com</li>
    <li><a href="https://twitter.com/karnikhil90">Twitter</a>: @karnikhil90</li>
    <li><a href="https://github.com/Karnikhil90">GitHub</a>: @Karnikhil90</li>
    <li><a href="https://www.linkedin.com/in/karnikhil90/">LinkedIn</a>: Nikhil Karmakar</li>
</ul>

</body>
</html>
