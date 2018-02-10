# SenseHat

# Description
« **Sense-Hat** » est une carte d'extension pour le Raspberry-Pi. Elle lui
permet d'être sensible à un certain nombre d'éléments et d'afficher des
informations en utilisant sa matrice d'affichage LED 8x8.

Ce dépôt propose une classe C++ pour le SenseHat sur Raspberry pi.


# Installation


**Prérequis** : copier **libSenseHat.a**  dans le répertoire /usr/lib/

    cp libSenseHat.a /usr/lib/libSenseHat.a
     

puis copier **SenseHat.h**      dans le répertoire /usr/include/
	

    cp SenseHat.h  /usr/include/SenseHat.h

**main.cpp**  est un programme  de démonstration de  la classe **SenseHat**
Compilation de main.pp : 

    g++ main.cpp -l SenseHat -o main

Exécution   du programme de démonstration: 

    ./main

Vous devriez voir sur votre carte Sense-hat, les deux premières colonnes de leds s'allumer  progressivement en rouge et bleu.

 Puis sur le terminal affichage de la valeur de la température, de la pression en hPa
et de l'humidité relative en %.

# Changelog

**30/06/2016 : ** Distribution et organisation originale du repository **SenseHat**. 

**10/02/2017 : ** Ajout de la documentation . 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


