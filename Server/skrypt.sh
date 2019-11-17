#!/bin/bash
g++ -c -Wall -pthread threadedServer.cpp room.cpp user.cpp message_handling.cpp
g++ -o server.exe room.o user.o threadedServer.o message_handling.o -pthread
echo "Compilation ended"
