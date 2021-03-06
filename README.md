# Qushie ( MUD Telnet Client)

This application is still in early Alpha release. Some settings
are still semi hardcoded. It may not entirely work out of the box.

I have only tested compiling on Windows 7, should work with Linux
and MAC with little modification. (Maybe a couple of ifdefs and
change to the .pro file.)

This application was specifically designed to connect to the
Aardwolf MUD. It is a C++ Thin Client with the UI and Application
Logic written in Javascript. The UI is furnished by QWebView, and
JSCore is connected to exposed QObjects (QushieFile, QTelnet, QushieDatabase).

### Components

Qushie depends on the project https://github.com/jasonknight/qushie-app to
actually run. You need to have both for it to actually work.

Qushie is just a thin client, qushie-app is the code that actually
does stuff.

The main JS Space has the QRuntimeObject Qushie.

### To create a Socket/Telnet connection

```javascript
	var connection = Qushie.getTelnetObjectById(Qushie.aardwolf_socket);

	connection.SignalSocketReadyRead.connect(function () {
		var txt = connection.readAll();
		parseServerOutput(txt); // you define this elsewhere

	});

	connection.SignalConnected.connect(function (server,port) {
		console.log("Connected to: ", server,port);
	});

	connection.SignalSocketConnectionClosed.connect(function () {
		console.log("Connection has been closed");
	});

	connection.SignalSocketError.connect(function (err) {
		console.log("Error: ", err);
	});

	connection.SignalSocketException.connect(function (code) {
		console.log("Exception: ", code);
	});
```

### To create a file

```javascript
	var file = Qushie.createFileObject();
	var contents = file.getContents('C:\\my\\path\\file.txt');
	// You also have file.putContents(path,content);
	// an file.append(path,content);
```

### To creat a database:

```javascript
var db = Qushie.createDatabaseObject();
if ( db.open('mydb.db') ) {
	console.log("Opened DB");
	if ( db.create("SELECT * FROM sqlite_master WHERE type = :type") ) {
		console.log("Statement Prepared");
		db.bind({ type: "table" });
		if ( db.run() ) {
			console.log("Statement Executed");
			var result = db.next();
			while ( ! $.isEmptyObject(result) ) {
				console.log("Result is: ", result);
				result = db.next();
			}
			db.finish();
		} else {
			console.log("Statement could not be executed");
		}
	} else {
		console.log("Failed to prepare statement", db.lastError());
	}
} else {
	console.log("Failed to open", db.lastError());
}
```

Copyright (c) 2013-2014 Red(E) Tools LTD

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.