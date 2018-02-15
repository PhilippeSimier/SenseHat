# SenseHat

# Description
« **Sense-Hat** » est une carte d'extension pour le Raspberry-Pi. Elle lui
permet d'être sensible à un certain nombre d'éléments et d'afficher des
informations en utilisant sa matrice d'affichage LED 8x8.

Ce dépôt propose une classe C++ pour le SenseHat sur Raspberry pi.

<img src="https://github.com/PhilippeSimier/SenseHat/blob/master/Documentation/Sense-HAT.png">

# Installation


**Prérequis** : 
Cloner le dépot

    pi@raspberry:~ $ git clone https://github.com/PhilippeSimier/SenseHat.git
    pi@raspberry:~ $ cd SenseHat/
    pi@raspberry:~ $ ./install.sh


Exécution des programmes de démonstration:
 
    pi@raspberry:~ $ cd /Exemples
    pi@PI3002:~/SenseHat/Exemples $ ./exemple
    pi@raspberry:~/SenseHat/Exemples $ ./niveau

Vous devriez voir sur votre carte Sense-hat, les trois premières colonnes de leds s'allumer  progressivement en rouge bleu et vert.

 Puis sur le terminal affichage de la valeur de la température, de la pression en hPa
et de l'humidité relative en %.

les coordonnées x,y,z du vecteur accélération en g (z=1g si la carte est en position horizontale
 
les coordonnées de l'orientation en radian/s

les coordonnées du vecteur champs magnétique terreste en µT 

Le programme niveau simule un niveau à bulle.

# Changelog

**30/06/2016 : ** Distribution et organisation originale du repository **SenseHat**. 

**10/02/2018 : ** Ajout de la documentation.

**15/02/2018 : ** Ajout de la procédure de calibration. 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


