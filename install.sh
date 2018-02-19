#!/bin/bash
echo Compilation de SenseHat
gcc -c SenseHat.cpp
ar cr libSenseHat.a SenseHat.o
ar s libSenseHat.a
echo Installation de Sensehat
sudo cp libSenseHat.a /usr/lib/libSenseHat.a
sudo cp SenseHat.h    /usr/include/SenseHat.h
echo Compilation des exemples
cd Exemples
g++ exemple.cpp -l SenseHat -l RTIMULib -o exemple
g++ niveau.cpp  -l SenseHat -l RTIMULib -o niveau
g++ text.cpp    -l SenseHat -l RTIMULib -o text
g++ caractere.cpp -l SenseHat -l RTIMULib -o caractere
