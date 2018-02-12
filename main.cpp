/**********************************************************************
Author : Philippe SIMIER
Date   : 12/02/2018
Sujet  : Programme démo  la class SenseHat
Prérequis : copier libSenseHat.a  dans /usr/lib/
          : et     SensHat.h      dans /usr/include/

Compilation : g++ main.cpp -l SenseHat -o main

***********************************************************************/

#include <SenseHat.h>
#include <iostream>



int main(){

SenseHat carte;
int i;
float pression;
float temperature;
float humidite;
float xa,ya,za,xm,ym,zm;
float pitch,roll,yaw;


    carte.Effacer();

    for (i=0;i<8;i++){
   	carte.AllumerPixel(1,i,BLEU);
   	carte.AllumerPixel(0,i,ROUGE);
   	carte.AllumerPixel(2,i,VERT);
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
	std::cout << "magnétisme x : " << xm << "(microT) y : " << ym << "(microT) z : " << zm << "(microT)" << std::endl;

    	usleep(500*1000); 
    }
}
