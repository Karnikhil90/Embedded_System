R"(
  <html>

<head>
    <title>bitluni's VGA Text Terminal</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script>
        function load() {
            document.querySelector('#in').focus();
        }

        function send() {
            var e = document.querySelector('#in');
            var xhr = new XMLHttpRequest();
            xhr.open('POST', '/command');
            xhr.send(e.value);
            document.querySelector("#log").innerHTML = e.value + "\n" + document.querySelector("#log").innerHTML;
            e.value = '';
        }
    </script>
    <style>
        h1 {
            color: white;
            background: #800000;
            padding: 10px;
            border-radius: 5px;
            text-align: center;
        }

        body {
            font-family: monospace;
            padding: 0;
            margin: 0;
        }

        input,
        textarea,
        button {
            border-style: solid;
            border-radius: 3px;
            padding: 10px;
            font-size: 1em;
        }

        #in {
            width: 100%;
            margin-bottom: 10px;
            box-sizing: border-box;
        }

        #send {
            width: 100%;
            padding: 10px;
            margin-bottom: 10px;
            font-size: 1em;
        }

        #log {
            resize: vertical;
            height: 250px;
            width: 100%;
            box-sizing: border-box;
            font-size: 0.9em;
        }

        #area {
            margin-top: 20px;
        }

        table {
            width: 100%;
        }

        @media (max-width: 600px) {
            #send {
                margin-top: 10px;
                width: 100%;
            }
        }
    </style>
</head>

<body onload='load()'>
    <h1>bitluni's VGA Text Terminal</h1>
    <table>
        <tr>
            <td><input id='in' type='text' tabindex="1" onchange='send()' autocomplete='off' placeholder="Type your command..."></td>
        </tr>
        <tr>
            <td><button id='send' onclick='send()'>Send</button></td>
        </tr>
        <tr>
            <td colspan="2">
                <textarea id='log' readonly placeholder="Command log..."></textarea>
            </td>
        </tr>
    </table>
</body>

</html>

)"