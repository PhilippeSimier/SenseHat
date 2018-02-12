/**********************************************************************
Author : Philippe SIMIER
Date   : 30/06/2016
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


carte.Effacer();
sleep(1);
for (i=0;i<8;i++){
   carte.AllumerPixel(1,i,BLEU);
   carte.AllumerPixel(0,i,ROUGE);
   carte.AllumerPixel(2,i,VERT);
   sleep(1);
}
pression = carte.ObtenirPression();
std::cout << "pression : " << pression << " hPa"<< std::endl;

temperature = carte.ObtenirTemperature();
std::cout << "Température : " << temperature << " °C" << std::endl;

humidite = carte.ObtenirHumidite();
std::cout << "Humidité : " << humidite << " %" << std::endl;

carte.InitialiserAcceleration();
carte.ObtenirAcceleration(x,y,z);
std::cout << "accélération x : " << x << " y : " << y << " z : " << z << std::endl;


}
