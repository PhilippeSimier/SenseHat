/**********************************************************************
Author : Philippe SIMIER
Date   : 16/02/2018
Sujet  : Programme secouer moi
Compilation : g++ secouer.cpp -l SenseHat -l RTIMULib -o secouer
***********************************************************************/

#include <SenseHat.h>
#include <iostream>
#include <cmath>



int main(){

    SenseHat carte;
    float x,y,z;

    while(true)
    {
	usleep(50*1000);
	carte.ObtenirAcceleration(x,y,z);
	x = abs(x);
	y = abs(y);
	z = abs(z);
	if (x>0.2 || y>0.2 || z>1.2){
	    carte.Effacer(BLEU);
	    sleep(3);
	}

	carte.Effacer(NOIR);
    }
}
