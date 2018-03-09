/**********************************************************************
Author : Philippe SIMIER
Date   : 20/02/2018
Sujet  : Programme clock pour afficher l'heure
Compilation : g++ clock.cpp -l SenseHat -l RTIMULib -o clock

Documentation : http://en.cppreference.com/w/cpp/io/manip/put_time
***********************************************************************/

#include <SenseHat.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>   //put_time()

using namespace std;


string ObtenirHeure()
{
    /* L'heure courante */
    time_t  t = time(nullptr);

    /* Creer une chaine  HH:MM */
    stringstream ss;
    ss  <<  put_time( localtime(&t), "  %H:%M" );

    /* retourner la chaine */
    return ss.str();
}

int main(){

   SenseHat carte;
   float x,y,z;

   carte << setcouleur(ORANGE);

   while(1){
	carte.ObtenirAcceleration(x,y,z);
       	if (x < -0.8)
          carte.FixerRotation(270);
        if (x > +0.8)
          carte.FixerRotation(+90);
        if (y < -0.8)
          carte.FixerRotation(180);
	if (y > 0.8)
	  carte.FixerRotation(0);

	carte << ObtenirHeure() << endl;

	sleep(1);
   }
}



