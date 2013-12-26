var http = require('http'),
    fs = require('fs'),
    // NEVER use a Sync function except at start-up!
    index = fs.readFileSync(__dirname + '/client/index.html');

var url = require("url");
var path = require('path');
var mime = require('mime');

// Send index.html to all requests
var app = http.createServer(function(req, res) {
    var dir = "/client";
    var uri = url.parse(req.url).pathname;
    if (uri == "/")
    {
        uri = "index.html";
    }
    var filename = path.join(dir, uri);
    console.log(filename);
    console.log(mime.lookup(filename));
    fs.readFile(__dirname + filename,
        function (err, data)
        {
            if (err)
            {
                res.writeHead(500);
                return res.end('Error loading index.html');
            }
            console.log(data);
            console.log(filename + " has read");
            res.setHeader('content-type', mime.lookup(filename));
            res.writeHead(200);
            res.end(data);
        });
});

// Socket.io server listens to our app
var io = require('socket.io').listen(app);

// Send current time to all connected clients
function sendTime() {
    io.sockets.emit('time', { time: new Date().toJSON() });
}

function action(a) {
	console.log(a);
	io.sockets.emit('python', a);
}

// Send current time every 10 secs
setInterval(sendTime, 10000);

// Emit welcome message on connection
io.sockets.on('connection', function(socket) {
    socket.emit('welcome', { message: 'Welcome!' });

    socket.on('i am client', console.log);
    socket.on('action', action)
});

app.listen(3000);