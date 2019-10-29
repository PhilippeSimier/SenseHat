# SenseHat

# Description
« **Sense-Hat** » is an expansion card for the Raspberry-Pi. It allows it to be sensitive to a number of elements and display information using its 8x8 LED display matrix.

This repository offers a C ++ class for the SenseHat on Raspberry pi.

<img src="https://github.com/PhilippeSimier/SenseHat/blob/master/Documentation/Sense-HAT.png">

# Installation

 ![description](https://img.shields.io/badge/build-passing-green.svg) the installation has been tested on  **Raspbian Stretch**  version Novembre 2017

**Prerequisites** : Clone the Repository
```bash
    pi@raspberry:~ $ git clone https://github.com/PhilippeSimier/SenseHat.git
    pi@raspberry:~ $ cd SenseHat/
```
**Compilation & installation** : SenseHat Library
```bash
    pi@raspberry:~/SenseHat $ make
    pi@raspberry:~/SenseHat $ sudo make install
    pi@raspberry:~/SenseHat $ make clean
```
**Compilation & execution** : Demonstration Programs:
```bash
    pi@raspberry:~/SenseHat $ cd /Exemples
    pi@raspberry:~/SenseHat/Exemples $ make
    pi@raspberry:~/SenseHat/Exemples $ ./exemple
```
You should see on your Sense-hat card, the first three lines of leds gradually light up in red blue and green.

Then the display shows the value of the temperature, the pressure in hPa and the relative humidity in%.

the x, y, z coordinates of the acceleration vector in g (z = 1g if the map is in horizontal position

the coordinates of the orientation in radian / s

the coordinates of the terrestrial magnetic field vector in μT

# Sample programs

 **niveau** level simulates a spirit level, tilt your raspberry to observe its movement.

 **text**  displays a message that scrolls in a loop.

 **clock** clock displays the time (The rotation of the display is automatic).  

 **secouer** : the screen turns blue when the raspberry is moved or shaken.

# SenseHat and the output stream :

The output stream of the SenseHat class makes it possible to perform display operations on the LED matrix in a simplified manner. The **<<**  operator has been overloaded for the following data types:

 **string**  **double**  **int**  **char** et **bool**

 In addition, SenseHat defines flow manipulators to perform simple operations.

 **endl** **flush** **setcouleur** **setrotation**

 The use of the << operator on this stream is therefore summarized in the following syntax:

```cpp
    SenseHat carte;
    carte << setcouleur(ORANGE) << setrotation(90) << "message " << 3.1415 << 58 << 'E' << endl;
```
As can be seen, it is possible to make several outputs successively on the same stream.



The **endl**  or **flush** manipulator must be inserted at the end of the flow to get the output. The class stores what is passed to it in a buffer and actually writes only during a call to flush or endl.

The **setcouleur** manipulator is used to set the color of the text.

The **setrotation** manipulator is used to rotate the display at 90 °, 180 ° or 270 °.

# Changelog

 **30/06/2016 : ** Distribution et organisation originale du repository **SenseHat**.

 **10/02/2018 : ** Ajout de la documentation.

 **15/02/2018 : ** Ajout de la procédure de calibration.

 **19/02/2018 : ** Ajout de la méthode AfficherMessage() proposée par Christophe GRILO (Merci à lui pour sa contribution)

 **09/03/2018 : ** Ajout de la gestion du flux de sortie  via l'opérateur <<

 **30/07/2019 (June 30): ** Translation to English and bug fixes started by Jon Dellaria <<

 **31/07/2019 (June 31): ** Adding Projects folder including measurements database, pixelBlink and SparkelContainer by Jon Dellaria <<
 
> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
<!-- TOOLBOX

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#
