 /***********************************************************************************
/*!
    \file         temperature.cpp
    \author    	  Philippe SIMIER (Touchard Wahington le Mans)
    \license      GPL2 (see license.txt)
    \date         25 février 2018
    \brief        Programme exemple pour l'affichage d'un message
    \details      Mise en oeuvre de l'opérateur de flux <<
		  pour afficher des messages sur une carte SenseHat

    \compilation: g++ temperature.cpp -l SenseHat -l RTIMULib -o temperature
    \version      v1.0
/***********************************************************************************/

#include <SenseHat.h>
#include <iostream>
#include <string>


using namespace std;

int main() {

    SenseHat carte;
    double valeur;

    carte.Effacer();
    carte << setcouleur(carte.ConvertirRGB565(64,84,0)) << setrotation(270);

    while(1){
        valeur = carte.ObtenirTemperature();
	carte << "Temp inside : " << valeur << "'" << flush;
	sleep(1);
    }
    return 0;
}

