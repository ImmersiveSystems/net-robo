Immersive Systems: net-robo
===========================

Working prototype of internet controlled robots. Please visit the wiki for more information: https://github.com/ImmersiveSystems/net-robo/wiki

Arduino
-------
Arduino code for robots to control them and interface with serial input.

Node
----
Node.js server that opens sockets to the Python client. Please run 'npm install socket.io' and 'npm install mime' after cloneing to grab the respective node_modules. This was only used for proof-of-concept, will eventually be phased out by our MEAN server.

Pi
------
Python code and unix scripts for the Raspberry Pi to interface with the webcam and open a socket connection to our servers.


MEAN Server
----------
Server repository is located at: https://github.com/gmxu/mean