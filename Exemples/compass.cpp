/**********************************************************************
    \file   : compass.cpp
    \author : Philippe SIMIER (Touchard Washington Le Mans)
    \date   : 12/02/2018
    \brief  : Programme démonstration de la class SenseHat
    \Compilation : g++ compass.cpp -l SenseHat -l RTIMULib -o compass
***********************************************************************/

#include <SenseHat.h>
#include <iostream>
#include <math.h>    /* atan2 */

#define PI 3.14159265

int main(){

    float x,y,z;
    float ro,teta,delta;

    SenseHat carte;

    while(1){
	carte.ObtenirMagnetismeSpherique(ro, teta, delta);
        system("clear");
    	std::cout << "θ : " << teta << " °"<< std::endl;
	std::cout << "δ : " << delta << " °"<< std::endl;
	std::cout << "ρ : " << ro << " µT"<< std::endl;
	x = ro * cos(delta) * cos (teta);
        y = ro * cos(delta) * sin (teta);
        z = ro * sin(delta);
        std::cout << "conv x : " << x << "(µT) y : " << y << "(µT) z : " << z << "(µT)" << std::endl;
    	usleep(500*1000);
    }
    return 0;
}

