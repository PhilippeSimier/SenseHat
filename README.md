# SenseHat
Class C++ pour SenseHat sur Raspberry

Sujet  : Programme démo  la class SenseHat
Prérequis : copier libSenseHat.a  dans le répertoire /usr/lib/
          : et     SensHat.h      dans le répertoire /usr/include/

Compilation : g++ main.cpp -l SenseHat -o main
Exécution   : ./main

Vous devriez voir sur votre carte Sense Hat, les deux premières colonnes de leds s'allumer
 progressivement en rouge et bleu.

Ensuite affichage sur le terminal de la valeur de la température, 

de la pression en hPa

et l'humidité relative en %.



