# SenseHat

# Description
« **Sense-Hat** » est une carte d'extension pour le Raspberry-Pi. Elle lui
permet d'être sensible à un certain nombre d'éléments et d'afficher des
informations en utilisant sa matrice d'affichage LED 8x8.

Ce dépôt propose une classe C++ pour le SenseHat sur Raspberry pi.

<img src="https://github.com/PhilippeSimier/SenseHat/blob/master/Documentation/Sense-HAT.png">

# Installation 

 ![description](https://img.shields.io/badge/build-passing-green.svg) l'installation a été testée sur **Raspbian Stretch**  version Novembre 2017 

**Prérequis** : Cloner le dépôt

    pi@raspberry:~ $ git clone https://github.com/PhilippeSimier/SenseHat.git
    pi@raspberry:~ $ cd SenseHat/

**Compilation & installation** : de la bibliothèque SenseHat

    pi@raspberry:~/SenseHat $ make
    pi@raspberry:~/SenseHat $ sudo make install
    pi@raspberry:~/SenseHat $ make clean

**Compilation & exécution** : des programmes de démonstration:
 
    pi@raspberry:~/SenseHat $ cd /Exemples
    pi@raspberry:~/SenseHat/Exemples $ make
    pi@raspberry:~/SenseHat/Exemples $ ./exemple

Vous devriez voir sur votre carte Sense-hat, les trois premières lignes de leds s'allumer  progressivement en rouge bleu et vert.

Puis sur le terminal affichage de la valeur de la température, de la pression en hPa
et de l'humidité relative en %.

les coordonnées x,y,z du vecteur accélération en g (z=1g si la carte est en position horizontale
 
les coordonnées de l'orientation en radian/s

les coordonnées du vecteur champs magnétique terreste en µT 

# Les programmes exemples

 **niveau** simule un niveau à bulle, inclinez votre raspberry pour observer son déplacement.

 **text**  affiche un message qui défile en boucle.

 **clock** affiche l'heure

 **secouer** : l'écran devient bleu lorsque la raspberry est déplacée ou secouée. 

**SenseHat et le flux de sortie** :

Le flux de sortie de la classe SenseHat permet d'effectuer des opération d'affichage sur la matrice de leds de manière simplifiée.
L'opérateur << a été surchargé pour les types de données suivantes : 
 
 **string** pour les chaînes de caratères

 **double** pour les nombres réels

 **int** pour les nombres entiers

 **char** pour les caractères

Exemple d'utilisation :

    SenseHat carte;
    carte << "message " << 3.1415 << 58 << 'E' << endl;

Le manipulateur **endl** ou **flush** doit être insérer en fin de ligne.  


# Changelog

**30/06/2016 : ** Distribution et organisation originale du repository **SenseHat**. 

**10/02/2018 : ** Ajout de la documentation.

**15/02/2018 : ** Ajout de la procédure de calibration.

**19/02/2018 : ** Ajout de la méthode AfficherMessage() proposée par Christophe GRILO (Merci à lui pour sa contribution) 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


