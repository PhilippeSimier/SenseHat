/**********************************************************************
    \file   : exemple.cpp
    \author : Philippe SIMIER (Touchard Washington Le Mans)
    \date   : 12/02/2018
    \brief  : Programme démonstration de la class SenseHat


    \Compilation : g++ exemple.cpp -l SenseHat -l RTIMULib -o exemple

***********************************************************************/

#include <SenseHat.h>
#include <iostream>



int main(){

    SenseHat carte;

    float pression;
    float temperature;
    float humidite;
    float xa,ya,za,xm,ym,zm;
    float pitch,roll,yaw;

    carte.Effacer();
    COULEUR vert  = carte.ConvertirRGB565("#009900");
    COULEUR rouge = carte.ConvertirRGB565(255,0,0);

    for (int i=0; i<8; i++){
   	carte.AllumerPixel(1, i, BLEU);
   	carte.AllumerPixel(0, i, rouge);
   	carte.AllumerPixel(2, i, vert);
   	sleep(1);
    }

    while(1){
    	pression    = carte.ObtenirPression();
	temperature = carte.ObtenirTemperature();
	humidite    = carte.ObtenirHumidite();

	usleep(20*1000);
	carte.ObtenirAcceleration(xa,ya,za);

	usleep(20*1000);
	carte.ObtenirOrientation(pitch,roll,yaw);

	usleep(20*1000);
	carte.ObtenirMagnetisme(xm,ym,zm);

	system("clear");
	std::cout << "pression : " << pression << " hPa"<< std::endl;
	std::cout << "Température : " << temperature << " °C" << std::endl;
	std::cout << "Humidité : " << humidite << " %" << std::endl;
	std::cout << "accélération x : " << xa << "(g) y : " << ya << "(g) z : " << za << "(g)" << std::endl;
        std::cout << "orientation pitch : " << pitch << " roll : " << roll << " yaw : " << yaw << std::endl;
	std::cout << "magnétisme x : " << xm << "(µT) y : " << ym << "(µT) z : " << zm << "(µT)" << std::endl;

    	usleep(500*1000); 
    }
}
