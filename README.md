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

