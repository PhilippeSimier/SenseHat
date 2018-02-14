#!/bin/bash
echo Compilation de SenseHat
gcc -c SenseHat.cpp
ar cr libSenseHat.a SenseHat.o
ar s libSenseHat.a
echo Installation de Sensehat
sudo cp libSenseHat.a /usr/lib/libSenseHat.a
sudo cp SenseHat.h    /usr/include/SenseHat.h
echo Compilation des exemples
g++ exemple.cpp -l SenseHat -o exemple
g++ niveau.cpp -l SenseHat -o niveau
