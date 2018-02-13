# SenseHat

# Description
« **Sense-Hat** » est une carte d'extension pour le Raspberry-Pi. Elle lui
permet d'être sensible à un certain nombre d'éléments et d'afficher des
informations en utilisant sa matrice d'affichage LED 8x8.

Ce dépôt propose une classe C++ pour le SenseHat sur Raspberry pi.

<img src="https://github.com/PhilippeSimier/SenseHat/blob/master/Documentation/Sense-HAT.jpg">

# Installation


**Prérequis** : 
Cloner le dépot

    pi@raspberry:~ $ git clone https://github.com/PhilippeSimier/SenseHat.git
    pi@raspberry:~ $ cd SenseHat/

Compiler la classe

    pi@raspberry:~/SenseHat $ gcc -c sensehat.cpp
    pi@raspberry:~/SenseHat $ ar cr libSenseHat.a sensehat.o
    pi@raspberry:~/SenseHat $ ar s libSenseHat.a

copier **libSenseHat.a**  dans le répertoire /usr/lib/

    pi@raspberry:~/SenseHat $ sudo cp libSenseHat.a /usr/lib/libSenseHat.a
     

puis copier **SenseHat.h**      dans le répertoire /usr/include/
	
    pi@raspberry:~/SenseHat $ sudo cp sensehat.h    /usr/include/SenseHat.h

**exemple.cpp**  est un programme  de démonstration de  la classe **SenseHat**
Compilation de exemple.cpp : 

    pi@raspberry:~/SenseHat $ g++ exemple.cpp -l SenseHat -o exemple

Exécution   du programme de démonstration: 

    ./exemple

Vous devriez voir sur votre carte Sense-hat, les deux premières colonnes de leds s'allumer  progressivement en rouge et bleu.

 Puis sur le terminal affichage de la valeur de la température, de la pression en hPa
et de l'humidité relative en %.
les coordonnées x,y,z de l'accélération en g 
les coordonnées de l'orientation en radian
les coordonnées du champs magnétiques en microTesla

# Changelog

**30/06/2016 : ** Distribution et organisation originale du repository **SenseHat**. 

**10/02/2018 : ** Ajout de la documentation . 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


