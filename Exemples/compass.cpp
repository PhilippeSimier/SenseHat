/**********************************************************************
    \file   : compass.cpp
    \author : Philippe SIMIER (Touchard Washington Le Mans)
    \date   : 01/03/2018
    \brief  : Programme démonstration de la class SenseHat
    \details: Affiche les coordonnées sphérique du vecteur champ magnétique
	      si teta = 0° alors l'axe X est aligné avec la direction du Nord
	      si teta = -90°   l'axe X est aligné avec l'Est
	      si teta = +90°   l'axe X est aligné avec l'Ouest
              si teta = +180 ou -180 l'axe X est aligné avec le Sud

    \Compilation : g++ compass.cpp -l SenseHat -l RTIMULib -o compass
***********************************************************************/

#include <SenseHat.h>
#include <iostream>


#define PI 3.14159265

int main(){

    float x,y,z;
    float ro,teta,delta;

    SenseHat carte;

    while(1){
	carte.ObtenirMagnetismeSpherique(ro, teta, delta);
        system("clear");
    	std::cout << "θ : " << teta  << " °" << std::endl;
	std::cout << "δ : " << delta << " °" << std::endl;
	std::cout << "ρ : " << ro    << " µT"<< std::endl;

    	usleep(500*1000);
    }
    return 0;
}

