/*!
 *  @file         temperature.cpp
 *  @author    	  Philippe SIMIER (Touchard Wahington le Mans)
 *  @license      GPL2 (see license.txt)
 *  @date         25 février 2018
 *  @brief        Programme exemple pour l'affichage d'un message
 *  @details      Mise en oeuvre de l'opérateur de flux <<
		  pour afficher la valeur de la température qu'il fait
                  à l'intérieur du boîtier.

        compilation: g++ temperature.cpp -l SenseHat -l RTIMULib -o temperature
 *  @version      v1.0
 */

#include <SenseHat.h>
#include <iostream>
#include <string>


using namespace std;

int main() {

    SenseHat carte;
    float x,y,z;
    int angle;

    carte.Effacer();
    carte << setcouleur(carte.ConvertirRGB565(64,84,0));

    while(1){
	carte.ObtenirAcceleration(x,y,z);
       	if (x < -0.8)
            angle = 270;
        if (x > +0.8)
            angle = 90;
        if (y < -0.8)
            angle = 180;
	if (y > 0.8)
	    angle = 0;

	carte << setrotation(angle) << "Température: " << carte.ObtenirTemperature() << "'" << flush;
	sleep(1);
    }
    return 0;
}

