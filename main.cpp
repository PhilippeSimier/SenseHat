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
float x,y,z;
float pitch,roll,yaw;


    carte.Effacer();

    for (i=0;i<8;i++){
   	carte.AllumerPixel(1,i,BLEU);
   	carte.AllumerPixel(0,i,ROUGE);
   	carte.AllumerPixel(2,i,VERT);
   	sleep(1);
    }
    while(1){
    	pression = carte.ObtenirPression();
    	std::cout << "pression : " << pression << " hPa"<< std::endl;

	temperature = carte.ObtenirTemperature();
	std::cout << "Température : " << temperature << " °C" << std::endl;

	humidite = carte.ObtenirHumidite();
	std::cout << "Humidité : " << humidite << " %" << std::endl;

	carte.ObtenirAcceleration(x,y,z);
	std::cout << "accélération x : " << x << "(g) y : " << y << "(g) z : " << z << "(g)" << std::endl;

	carte.ObtenirOrientation(pitch,roll,yaw);
	std::cout << "orientation pitch : " << pitch << " roll : " << roll << " yaw : " << yaw << std::endl;
    	sleep(1);
	system("clear"); 
    }
}
