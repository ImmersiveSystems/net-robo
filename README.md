Immersive Systems: net-robo
===========================

Working prototype of internet controlled robots.

Arduino
-------
Arduino code for robots

Node
----
Node.js server that opens sockets to the Python client. Please run 'npm install socket.io' and 'npm install mime' after cloneing to grab the respective node_modules. This was only used for proof-of-concept, will eventually be phased out by our MEAN server.

Python
------
Python code and scripts for the Raspberry Pi to interface with the webcam and open a socket connection to our servers.


MEAN Sever
----------
Server repository is located at: https://github.com/gmxu/mean
