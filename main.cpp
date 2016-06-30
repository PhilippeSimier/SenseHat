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

carte.Effacer();
sleep(1);
for (i=0;i<8;i++){
   carte.AllumerPixel(1,i,BLEU);
   carte.AllumerPixel(0,i,ROUGE);
   sleep(1);
}
pression = carte.ObtenirPression();
std::cout << "pression : " << pression << " hPa"<< std::endl;

}
