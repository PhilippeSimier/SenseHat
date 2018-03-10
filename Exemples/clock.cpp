/**
 * @file    clock.cpp
 * @author  Philippe SIMIER
 * @date    20/02/2018
 * @brief   Programme clock pour afficher l'heure
 * @detail  Compilation : g++ clock.cpp -l SenseHat -l RTIMULib -o clock

*/

#include <SenseHat.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>   //put_time()

using namespace std;

/**
 * @brief ObtenirHeure
 * @return std::string
 * @details retourne une chaine de caratères représentant l'heure courante
 *          au fornat hh:mm
 */
string ObtenirHeure()
{
    /* L'heure courante au format time_t*/
    time_t  t = time(nullptr);

    /* Cration d'une chaine  HH:MM à l'aide des flux sur chaînes*/
    stringstream ss;
    ss  <<  put_time( localtime(&t), "  %H:%M" );

    /* retourne le flux converti en string */
    return ss.str();
}

/**
 * @brief main
 * @return int 0
 * @details Affiche sur l'écran de la sense-hat l'heure courante en boucle.
 *          La rotation de l'affichage s'adapte à la position du raspberry.
 */

int main(){

   SenseHat carte;
   float x,y,z;
   int angle;

   carte << setcouleur(carte.ConvertirRGB565(64,84,0));

   do{
	carte.ObtenirAcceleration(x,y,z);
       	if (x < -0.8)
            angle = 270;
        if (x > +0.8)
            angle = 90;
        if (y < -0.8)
            angle = 180;
	if (y > 0.8)
	    angle = 0;

	carte << setrotation(angle) << ObtenirHeure() << endl;

	sleep(1);

   }while(z > -0.5);

   return 0;
}



