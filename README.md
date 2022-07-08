# astroids-copilot
astroids multiplayer over network

Compile the code like this (tested on ubuntu):
  * make build && make cli srv

Running the programs:
  * fristly the host needs to start the server: ./server
  * now the clients can connect: ./client
  * when starting the client it will ask for a number to identify each ship. Two different clients could contorll the same ship.
  * When all ships are "red" either you could spawn in more ships to clear level or restart server.
The server_ip variable probably will need to change if you host server.
The server_ip variable can be found both in the cli.cpp and srv.cpp files.

Required stuff:
  * g++
  * make
  * sfml
  
On ubuntu using apt the requierments can be installed like this:
  * sudo apt install g++;
  * sudo apt install make;
  * sudo apt install libsfml-dev;
